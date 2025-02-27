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

void parse_command(char *input, cmd_buff_t *cmd) {
    cmd->argc = 0;
    bool in_quote = false;
    char *arg_start = input;

    for (char *p = input; *p; p++) {
        if (*p == '\"') {
            in_quote = !in_quote;
            memmove(p, p + 1, strlen(p)); // Remove the quote but keep parsing
            p--; // Stay on the current character
        } else if (*p == ' ' && !in_quote) {
            *p = '\0'; // Terminate current arg
            if (arg_start != p) { // Avoid empty arguments
                cmd->argv[cmd->argc++] = arg_start;
            }
            arg_start = p + 1; // Start new arg
        }
    }
    if (arg_start[0] != '\0') { // Add last argument if not empty
        cmd->argv[cmd->argc++] = arg_start;
    }
    cmd->argv[cmd->argc] = NULL; // NULL terminate the argv
}

void handle_cd_command(cmd_buff_t *cmd) {
    if (cmd->argc == 1) {
        // Do nothing if no arguments are provided
    } else if (cmd->argc == 2) {
        if (chdir(cmd->argv[1]) != 0) {
            perror("cd failed");
        }
    } else {
        printf("Usage: cd <directory>\n");
    }
}

void execute_external_command(cmd_buff_t *cmd) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return;
    }
    if (pid == 0) { // Child process
        execvp(cmd->argv[0], cmd->argv);
        fprintf(stderr, "CMD_ERR_EXECUTE\n");
        exit(127); 
    } else { // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            const int es = WEXITSTATUS(status);
        }
    }
}

int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(SH_CMD_MAX);
    if (!cmd_buff) {
        perror("Failed to allocate memory for command buffer");
        return ERR_MEMORY;
    }
    int rc = 0;
    cmd_buff_t cmd;

    // TODO IMPLEMENT MAIN LOOP

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

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

        parse_command(cmd_buff, &cmd);
        if (strcmp(cmd.argv[0], "cd") == 0) {
            handle_cd_command(&cmd);
        } else {
            execute_external_command(&cmd);
        }

        // Clear the buffer for the next command
        memset(cmd_buff, 0, SH_CMD_MAX);
    }

    free(cmd_buff);  // Ensure memory is freed once we are done
    return OK;
}