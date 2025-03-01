#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

// Function to parse command line into separate commands split by '|'
int build_cmd_list(char *cmd_line, command_list_t *clist) {
    clist->num = 0;
    char *token = strtok(cmd_line, PIPE_STRING);

    while (token) {
        if (clist->num >= CMD_MAX) {
            printf("CMD_ERR_PIPE_LIMIT\n");  // Print error message
            return ERR_TOO_MANY_COMMANDS;   // Return error
        }
        cmd_buff_t *cmd = &clist->commands[clist->num++];
        cmd->argc = 0;
        bool in_quote = false;
        char *arg_start = token;
        
        for (char *p = token; *p; p++) {
            if (*p == '"') { 
                in_quote = !in_quote;
                memmove(p, p + 1, strlen(p)); // Remove quote but keep parsing
                p--; // Stay on the same character
            } else if (*p == ' ' && !in_quote) { 
                *p = '\0'; // Terminate current argument
                if (arg_start != p) { // Avoid empty args
                    cmd->argv[cmd->argc++] = arg_start;
                }
                arg_start = p + 1;
            }
        }

        if (*arg_start) { // Add last argument if not empty
            cmd->argv[cmd->argc++] = arg_start;
        }
        cmd->argv[cmd->argc] = NULL; // Null-terminate

        token = strtok(NULL, PIPE_STRING); // Get next piped command
    }

    if (clist->num == 0) {
        return WARN_NO_CMDS;
    }

    if (clist->num > CMD_MAX) {
        return ERR_TOO_MANY_COMMANDS;
    }

    return OK;
}

// Function to handle built-in commands like 'cd' and 'exit'
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argc > 1) {
            if (chdir(cmd->argv[1]) != 0) {
                perror("cd failed");
            } else {
                char cwd[1024];
                getcwd(cwd, sizeof(cwd));
                printf("%s\n", cwd); // Print new directory
            }
        } else {
            // If no argument, do nothing (stay in the same directory)
        }
        return BI_EXECUTED;
    }
    return BI_NOT_BI;
}

// Function to execute multiple piped commands
int execute_pipeline(command_list_t *clist) {
    int pipes[CMD_MAX][2];
    pid_t pids[CMD_MAX];

    // Create pipes
    for (int i = 0; i < clist->num - 1; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe failed");
            return ERR_EXEC_CMD;
        }
    }

    for (int i = 0; i < clist->num; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork failed");
            return ERR_EXEC_CMD;
        }

        if (pids[i] == 0) { // Child process
            if (i > 0) { // Not the first command
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < clist->num - 1) { // Not the last command
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors in the child
            for (int j = 0; j < clist->num - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("CMD_ERR_EXECUTE");
            exit(ERR_EXEC_CMD);
        }
    }

    // Close all pipes in the parent process
    for (int i = 0; i < clist->num - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes
    for (int i = 0; i < clist->num; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return OK;
}


int exec_local_cmd_loop() {
    char *cmd_buff = malloc(SH_CMD_MAX);
    if (!cmd_buff) {
        perror("Failed to allocate memory for command buffer");
        return ERR_MEMORY;
    }

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (strcmp(cmd_buff, EXIT_CMD) == 0) {
            break;
        }

        command_list_t cmd_list;
        if (build_cmd_list(cmd_buff, &cmd_list) != OK) {
            printf("CMD_WARN_NO_CMD\n");
            continue;
        }

        if (cmd_list.num == 0) {
            printf("CMD_WARN_NO_CMD\n");
            continue;
        }

        if (cmd_list.num > CMD_MAX) {
            printf("CMD_ERR_PIPE_LIMIT\n");
            continue;
        }

        if (cmd_list.num == 1 && strcmp(cmd_list.commands[0].argv[0], "cd") == 0) {
            exec_built_in_cmd(&cmd_list.commands[0]);
        } else {
            execute_pipeline(&cmd_list);
        }
    }

    free(cmd_buff);
    return OK;
}