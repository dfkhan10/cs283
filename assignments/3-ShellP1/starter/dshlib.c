#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    // Check for a NULL pointer or an empty (or all-whitespace) input.
    if (!cmd_line || !clist)
        return EXIT_NOT_IMPL;
    
    // Check if cmd_line is all whitespace.
    char *p = cmd_line;
    while (*p) {
        if (!isspace((unsigned char)*p))
            break;
        p++;
    }
    if (*p == '\0')  // The line is empty or only whitespace.
        return WARN_NO_CMDS;

    // Reset the command list structure.
    memset(clist, 0, sizeof(command_list_t));

    // Make a copy of cmd_line so we don't modify the original.
    char cmd_copy[SH_CMD_MAX];
    strncpy(cmd_copy, cmd_line, SH_CMD_MAX);
    cmd_copy[SH_CMD_MAX - 1] = '\0';  // Ensure null termination

    // Use strtok_r for outer tokenization by the pipe character.
    char *outer_saveptr;
    char *cmd_token = strtok_r(cmd_copy, PIPE_STRING, &outer_saveptr);
    int cmd_count = 0;

    while (cmd_token != NULL)
    {
        if (cmd_count >= CMD_MAX)
            return ERR_TOO_MANY_COMMANDS;

        // Trim leading spaces.
        while (*cmd_token == ' ')
            cmd_token++;

        // Trim trailing spaces.
        char *end = cmd_token + strlen(cmd_token) - 1;
        while (end > cmd_token && *end == ' ')
            *end-- = '\0';

        // If after trimming the token is empty, treat it as no command provided.
        if (strlen(cmd_token) == 0)
            return WARN_NO_CMDS;

        // Copy the token into a temporary buffer for inner tokenization.
        char token_copy[SH_CMD_MAX];
        strncpy(token_copy, cmd_token, SH_CMD_MAX);
        token_copy[SH_CMD_MAX - 1] = '\0';

        command_t *cmd = &clist->commands[cmd_count];

        // Use strtok (non-reentrant is fine for inner loop) to split by space.
        char *inner_token = strtok(token_copy, " ");
        if (!inner_token || strlen(inner_token) >= EXE_MAX)
            return ERR_CMD_OR_ARGS_TOO_BIG;

        // Set the command executable name.
        strcpy(cmd->exe, inner_token);

        // Reset the args string.
        cmd->args[0] = '\0';

        // Append remaining tokens as arguments.
        int first_arg = 1;
        while ((inner_token = strtok(NULL, " ")) != NULL)
        {
            if (!first_arg)
                strcat(cmd->args, " ");
            else
                first_arg = 0;

            strcat(cmd->args, inner_token);
        }

        cmd_count++;
        cmd_token = strtok_r(NULL, PIPE_STRING, &outer_saveptr);
    }

    if (cmd_count == 0)
        return WARN_NO_CMDS;

    clist->num = cmd_count;
    return OK;
}