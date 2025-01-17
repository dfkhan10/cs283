#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
void reverse_string(char *, char*, int);
void print_words(char *, int);
//add additional prototypes here

int count_words(char *buff, int len, int str_len) {
    (void)len; // Get rid of unused parameter warning
    
    int word_count = 0;
    int in_word = 0; // Boolean to track if we are inside a word

    for (int i = 0; i < str_len+1; i++) {
        // printf("Debug: %c\n", *(buff + i));
        if (*(buff + i) != ' ' && in_word == 0) {
            word_count++;
            in_word = 1; // Start of a word
        } else if (*(buff + i) == ' ') {
            in_word = 0; // End of a word
        }
    }
    return word_count;
}

void reverse_string(char *buff, char *user_str, int len) {
    if (!buff || !user_str || len <= 0) {
        return;  // Error: Invalid input
    }

    int input_len = strlen(user_str);  // Length of input string
    int i = 0;

    // Reverse the input string into the buffer
    for (i = 0; i < input_len && i < len; i++) {
        buff[i] = user_str[input_len - 1 - i];
    }

    // Fill the remainder of the buffer with dots
    for (; i < len; i++) {
        buff[i] = '.';
    }
}


void print_words(char *buff, int str_len) {
    int word_count = 0;
    char *start = NULL;

    printf("Word Print\n----------\n");

    for (char *ptr = buff; ptr < buff + str_len; ptr++) {
        if (*ptr != ' ' && *ptr != '.' && start == NULL) {
            start = ptr; // Start of a word
        } else if ((*ptr == ' ' || *ptr == '.') && start != NULL) {
            int word_len = ptr - start;
            word_count++;
            printf("%d. %.*s(%d)\n", word_count, word_len, start, word_len);
            start = NULL; // Reset start for the next word
        }
    }

    // Handle last word
    if (start != NULL) {
        int word_len = (buff + str_len) - start;
        word_count++;
        printf("%d. %.*s (%d)\n", word_count, word_len, start, word_len);
    }

    printf("\nNumber of words returned: %d\n" ,word_count);
}

int setup_buff(char *buff, char *user_str, int len) {
    //TODO: #4:  Implement the setup buff as per the directions
    if (!buff || !user_str || len <= 0) {
        return -2; // Error: Invalid input
    }

    char *ptr = buff;  // Destination pointer (buffer)
    char *src = user_str;  // Source pointer (input string)
    int count = 0;  // Number of characters copied
    int in_word = 0;  // Flag to track if we are inside a word

    // Skip leading spaces
    while (*src == ' ') {
        src++;
    }

    // Copy characters to the buffer
    while (*src != '\0' && count < len) {
        if (*src == ' ') {
            if (in_word) {
                // Add a single space after a word
                *ptr = ' ';
                ptr++;
                count++;
                in_word = 0;  // Word ended
            }
        } else {
            // Copy non-space character
            *ptr = *src;
            ptr++;
            count++;
            in_word = 1;  // Inside a word
        }

        src++;
    }

    // If the buffer is full but the string is not complete, return error
    if (*src != '\0') {
        return -1; // Buffer too small
    }

    // Remove trailing space, if any
    if (count > 0 && *(ptr - 1) == ' ') {
        ptr--;
        count--;
    }

    // Fill the rest of the buffer with dots
    while (count < len) {
        *ptr = '.';
        ptr++;
        count++;
    }

    return count;  // Return the total number of characters in the buffer
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar(']');
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // This check ensures that the program has been invoked with at least one argument (argc >= 2) and that 
    // the first argument starts with a - (indicating a flag). If argc < 2, dereferencing argv[1] would lead
    //  to undefined behavior (accessing an invalid memory location). By first checking argc < 2, the program 
    // avoids this scenario, making the access to argv[1] safe.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // This check ensures that the user has provided both the required option (argv[1]) and the input string (argv[2]).
    //  If argc < 3, it means the input string is missing, and the program cannot proceed further. The check avoids 
    // segmentation faults or accessing invalid memory locations.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ);
    if (!buff) {
        printf("Error: Memory allocation failed\n");
        exit(99);
    }

    // printf("test");
    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    // printf("DEBUG: %d\n", user_str_len);
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt) {
    case 'c':
        rc = count_words(buff, BUFFER_SZ, user_str_len);
        if (rc < 0) {
            printf("Error counting words, rc = %d\n", rc);
            exit(2);
        }
        printf("Word Count: %d\n", rc);
        break;

    case 'r': { // Reverse string
        reverse_string(buff, input_string, user_str_len);
        break;
    }

    case 'w': { // Word print
        rc = setup_buff(buff, input_string, BUFFER_SZ);
        if (rc < 0) {
            exit(1); // Exit with non-zero status if buffer setup fails
        }
        print_words(buff, user_str_len);
        break;
    }

    case 'x': { // Word search and replace
        if (argc != 5) {
            printf("Error: Incorrect number of arguments for -x flag\n");
            exit(1); // Exit with an error code if there are not exactly 5 arguments
        }
        
        // Ensure all arguments are provided
        char *input_str = argv[2];  // String to process
        char *old_word = argv[3];   // Word to search for
        char *new_word = argv[4];   // Word to replace with

        (void) input_str;  // Get rid of unused variable warning
        (void) old_word;  // Get rid of unused variable warning
        (void) new_word;  // Get rid of unused variable warning

        // Print "Not Implemented!" as per the requirement
        printf("Not Implemented!\n");

        // Return an error code for the feature not being implemented
        exit(1);
        break;
    }

    default:
        usage(argv[0]);
        exit(1);
}


    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//          Passing both the pointer (buff) and the length (len) to helper functions is a good practice 
//          because it improves code safety, reusability, and clarity. It ensures functions handle memory
//          safely by preventing buffer overflows and makes them adaptable to different buffer sizes, 
//          reducing dependence on global constants like BUFFER_SZ. This approach also enhances readability
//          by explicitly conveying constraints and supports future changes in buffer size without requiring 
//          function modifications, resulting in more modular and maintainable code.