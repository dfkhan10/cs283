#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
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
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

static const unsigned char compressed_dragon[] = {
    72, ' ', 
    1, '@', 
    4, '%', 
    23, ' ', 
    1, '\n', 
    69, ' ', 
    6, '%', 
    25, ' ', 
    1, '\n', 
    68, ' ', 
    6, '%', 
    26, ' ', 
    1, '\n', 
    65, ' ', 
    1, '%', 
    1, ' ', 
    7, '%', 
    11, ' ', 
    1, '@', 
    14, ' ', 
    1, '\n', 
    64, ' ', 
    10, '%', 
    8, ' ', 
    7, '%', 
    11, ' ', 
    1, '\n', 
    39, ' ', 
    7, '%', 
    2, ' ', 
    4, '%', 
    1, '@', 
    9, ' ', 
    12, '%', 
    1, '@', 
    4, ' ', 
    6, '%', 
    2, ' ', 
    1, '@', 
    4, '%', 
    8, ' ', 
    1, '\n', 
    34, ' ', 
    22, '%', 
    6, ' ', 
    28, '%', 
    10, ' ', 
    1, '\n', 
    32, ' ', 
    26, '%', 
    3, ' ', 
    12, '%', 
    1, ' ', 
    15, '%', 
    11, ' ', 
    1, '\n', 
    31, ' ', 
    29, '%', 
    1, ' ', 
    19, '%', 
    5, ' ', 
    3, '%', 
    12, ' ', 
    1, '\n', 
    29, ' ', 
    28, '%', 
    1, '@', 
    1, ' ', 
    1, '@', 
    18, '%', 
    8, ' ', 
    2, '%', 
    12, ' ', 
    1, '\n', 
    28, ' ', 
    33, '%', 
    1, ' ', 
    22, '%', 
    16, ' ', 
    1, '\n', 
    28, ' ', 
    58, '%', 
    14, ' ', 
    1, '\n', 
    28, ' ', 
    50, '%', 
    1, '@', 
    6, '%', 
    1, '@', 
    14, ' ', 
    1, '\n', 
    6, ' ', 
    8, '%', 
    1, '@', 
    11, ' ', 
    16, '%', 
    8, ' ', 
    26, '%', 
    6, ' ', 
    2, '%', 
    16, ' ', 
    1, '\n', 
    4, ' ', 
    13, '%', 
    9, ' ', 
    2, '%', 
    1, '@', 
    12, '%', 
    11, ' ', 
    11, '%', 
    1, ' ', 
    12, '%', 
    6, ' ', 
    1, '@', 
    1, '%', 
    16, ' ', 
    1, '\n', 
    2, ' ', 
    10, '%', 
    3, ' ', 
    3, '%', 
    8, ' ', 
    14, '%', 
    12, ' ', 
    24, '%', 
    24, ' ', 
    1, '\n', 
    1, ' ', 
    9, '%', 
    7, ' ', 
    1, '%', 
    9, ' ', 
    13, '%', 
    13, ' ', 
    12, '%', 
    1, '@', 
    11, '%', 
    23, ' ', 
    1, '\n', 
    9, '%', 
    1, '@', 
    16, ' ', 
    1, '%', 
    1, ' ', 
    13, '%', 
    12, ' ', 
    1, '@', 
    25, '%', 
    21, ' ', 
    1, '\n', 
    8, '%', 
    1, '@', 
    17, ' ', 
    2, '%', 
    1, '@', 
    12, '%', 
    12, ' ', 
    1, '@', 
    28, '%', 
    18, ' ', 
    1, '\n', 
    7, '%', 
    1, '@', 
    19, ' ', 
    15, '%', 
    11, ' ', 
    33, '%', 
    14, ' ', 
    1, '\n', 
    10, '%', 
    18, ' ', 
    15, '%', 
    10, ' ', 
    35, '%', 
    6, ' ', 
    4, '%', 
    2, ' ', 
    1, '\n', 
    9, '%', 
    1, '@', 
    19, ' ', 
    1, '@', 
    14, '%', 
    9, ' ', 
    12, '%', 
    1, '@', 
    1, ' ', 
    4, '%', 
    1, ' ', 
    17, '%', 
    3, ' ', 
    8, '%', 
    1, '\n', 
    10, '%', 
    18, ' ', 
    17, '%', 
    8, ' ', 
    13, '%', 
    6, ' ', 
    18, '%', 
    1, ' ', 
    9, '%', 
    1, '\n', 
    9, '%', 
    1, '@', 
    2, '%', 
    1, '@', 
    16, ' ', 
    16, '%', 
    1, '@', 
    7, ' ', 
    14, '%', 
    5, ' ', 
    24, '%', 
    2, ' ', 
    2, '%', 
    1, '\n', 
    1, ' ', 
    10, '%', 
    18, ' ', 
    1, '%', 
    1, ' ', 
    14, '%', 
    1, '@', 
    8, ' ', 
    14, '%', 
    3, ' ', 
    26, '%', 
    1, ' ', 
    2, '%', 
    1, '\n', 
    2, ' ', 
    12, '%', 
    2, ' ', 
    1, '@', 
    11, ' ', 
    18, '%', 
    8, ' ', 
    40, '%', 
    2, ' ', 
    3, '%', 
    1, ' ', 
    1, '\n', 
    3, ' ', 
    13, '%', 
    1, ' ', 
    2, '%', 
    2, ' ', 
    1, '%', 
    2, ' ', 
    1, '%', 
    1, '@', 
    1, ' ', 
    18, '%', 
    10, ' ', 
    37, '%', 
    4, ' ', 
    3, '%', 
    1, ' ', 
    1, '\n', 
    4, ' ', 
    18, '%', 
    1, ' ', 
    22, '%', 
    11, ' ', 
    1, '@', 
    31, '%', 
    4, ' ', 
    7, '%', 
    1, ' ', 
    1, '\n', 
    5, ' ', 
    39, '%', 
    14, ' ', 
    28, '%', 
    8, ' ', 
    3, '%', 
    3, ' ', 
    1, '\n', 
    6, ' ', 
    1, '@', 
    35, '%', 
    18, ' ', 
    25, '%', 
    15, ' ', 
    1, '\n', 
    8, ' ', 
    32, '%', 
    22, ' ', 
    19, '%', 
    2, ' ', 
    7, '%', 
    10, ' ', 
    1, '\n', 
    11, ' ', 
    26, '%', 
    27, ' ', 
    15, '%', 
    2, ' ', 
    1, '@', 
    9, '%', 
    9, ' ', 
    1, '\n', 
    14, ' ', 
    20, '%', 
    11, ' ', 
    1, '@', 
    1, '%', 
    1, '@', 
    1, '%', 
    18, ' ', 
    1, '@', 
    18, '%', 
    3, ' ', 
    3, '%', 
    8, ' ', 
    1, '\n', 
    18, ' ', 
    15, '%', 
    8, ' ', 
    10, '%', 
    20, ' ', 
    15, '%', 
    4, ' ', 
    1, '%', 
    9, ' ', 
    1, '\n', 
    16, ' ', 
    36, '%', 
    22, ' ', 
    14, '%', 
    12, ' ', 
    1, '\n', 
    16, ' ', 
    26, '%', 
    2, ' ', 
    4, '%', 
    1, ' ', 
    3, '%', 
    22, ' ', 
    10, '%', 
    2, ' ', 
    3, '%', 
    1, '@', 
    10, ' ', 
    1, '\n', 
    21, ' ', 
    19, '%', 
    1, ' ', 
    6, '%', 
    1, ' ', 
    2, '%', 
    26, ' ', 
    13, '%', 
    1, '@', 
    10, ' ', 
    1, '\n', 
    81, ' ', 
    7, '%', 
    1, '@', 
    7, ' ', 
    0
};

// Function to decompress and print the Drexel dragon from the compressed array
void print_dragon_compressed(void) {
    const unsigned char *p = compressed_dragon;
    while (*p != 0) {  // Continue until we reach the termination marker
        int count = *p++;  // Number of times to print the character
        char ch = *p++;    // Character to print
        for (int i = 0; i < count; i++) {
            putchar(ch);
        }
    }
}

int main()
{
    char *cmd_buff = malloc(SH_CMD_MAX);  // Allocate memory for user input
    int rc = 0;
    command_list_t clist;

    if (!cmd_buff) {
        perror("Failed to allocate memory for command buffer");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Always print the prompt.
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // Remove trailing newline.
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // Check for exit command.
        if (strcmp(cmd_buff, EXIT_CMD) == 0)
        {
            break;
        }

        // EXTRA CREDIT: If the command is "dragon", print the Drexel dragon.
        if (strcmp(cmd_buff, "dragon") == 0) {
            // Uncomment one of the following lines depending on which version you wish to use:
            
            // Version 1: Simple uncompressed version.
            // print_dragon_simple();
            
            // Version 2: Compressed (binary data) version.
            print_dragon_compressed();
            
            continue;
        }


        // Clear and parse the command.
        memset(&clist, 0, sizeof(clist));
        rc = build_cmd_list(cmd_buff, &clist);

        if (rc == WARN_NO_CMDS)
        {
            printf(CMD_WARN_NO_CMD);
            continue;
        }
        else if (rc == ERR_TOO_MANY_COMMANDS)
        {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        }

        printf(CMD_OK_HEADER, clist.num);
        for (int i = 0; i < clist.num; i++)
        {
            printf("<%d> %s", i + 1, clist.commands[i].exe);
            if (strlen(clist.commands[i].args) > 0)
                printf(" [%s]", clist.commands[i].args);
            printf("\n");
        }
    }

    free(cmd_buff);
    exit(0);
}