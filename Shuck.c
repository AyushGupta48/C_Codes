////////////////////////////////////////////////////////////////////////
// COMP1521 21t2 -- Assignment 2 -- shuck, A Simple Shell
// <https://www.cse.unsw.edu.au/~cs1521/21T2/assignments/ass2/index.html>
//
// Written by Ayush Gupta (z5362798) on July/August 2021.
//
// 2021-07-12    v1.0    Team COMP1521 <cs1521@cse.unsw.edu.au>
// 2021-07-21    v1.1    Team COMP1521 <cs1521@cse.unsw.edu.au>
//     * Adjust qualifiers and attributes in provided code,
//       to make `dcc -Werror' happy.
//

#include <sys/types.h>

#include <sys/stat.h>
#include <sys/wait.h>

#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// [[ TODO: put any extra `#include's here ]]
#include <limits.h>
#include <spawn.h>


// [[ TODO: put any `#define's here ]]


//
// Interactive prompt:
//     The default prompt displayed in `interactive' mode --- when both
//     standard input and standard output are connected to a TTY device.
//
static const char *const INTERACTIVE_PROMPT = "shuck& ";

//
// Default path:
//     If no `$PATH' variable is set in Shuck's environment, we fall
//     back to these directories as the `$PATH'.
//
static const char *const DEFAULT_PATH = "/bin:/usr/bin";

//
// Default history shown:
//     The number of history items shown by default; overridden by the
//     first argument to the `history' builtin command.
//     Remove the `unused' marker once you have implemented history.
//
static const int DEFAULT_HISTORY_SHOWN __attribute__((unused)) = 10;

//
// Input line length:
//     The length of the longest line of input we can read.
//
static const size_t MAX_LINE_CHARS = 1024;

//
// Special characters:
//     Characters that `tokenize' will return as words by themselves.
//
static const char *const SPECIAL_CHARS = "!><|";

//
// Word separators:
//     Characters that `tokenize' will use to delimit words.
//
static const char *const WORD_SEPARATORS = " \t\r\n";

// [[ TODO: put any extra constants here ]]


// [[ TODO: put any type definitions (i.e., `typedef', `struct', etc.) here ]]


static void execute_command(char **words, char **path, char **environment);
static void do_exit(char **words);
static int is_executable(char *pathname);
static char **tokenize(char *s, char *separators, char *special_chars);
static void free_tokens(char **tokens);

// [[ TODO: put any extra function prototypes here ]]


int main (void)
{
    // Ensure `stdout' is line-buffered for autotesting.
    setlinebuf(stdout);

    // Environment variables are pointed to by `environ', an array of
    // strings terminated by a NULL value -- something like:
    //     { "VAR1=value", "VAR2=value", NULL }
    extern char **environ;

    // Grab the `PATH' environment variable for our path.
    // If it isn't set, use the default path defined above.
    char *pathp;
    if ((pathp = getenv("PATH")) == NULL) {
        pathp = (char *) DEFAULT_PATH;
    }
    char **path = tokenize(pathp, ":", "");

    // Should this shell be interactive?
    bool interactive = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);

    // Main loop: print prompt, read line, execute command
    while (1) {
        // If `stdout' is a terminal (i.e., we're an interactive shell),
        // print a prompt before reading a line of input.
        if (interactive) {
            fputs(INTERACTIVE_PROMPT, stdout);
            fflush(stdout);
        }

        char line[MAX_LINE_CHARS];
        if (fgets(line, MAX_LINE_CHARS, stdin) == NULL)
            break;

        // Tokenise and execute the input line.
        char **command_words =
            tokenize(line, (char *) WORD_SEPARATORS, (char *) SPECIAL_CHARS);
        execute_command(command_words, path, environ);
        free_tokens(command_words);
    }
    
    

    free_tokens(path);
    return 0;
}


//
// Execute a command, and wait until it finishes.
//
//  * `words': a NULL-terminated array of words from the input command line
//  * `path': a NULL-terminated array of directories to search in;
//  * `environment': a NULL-terminated array of environment variables.
//
static void execute_command(char **words, char **path, char **environment)
{
    assert(words != NULL);
    assert(path != NULL);
    assert(environment != NULL);

    char *program = words[0];
    char pathname[PATH_MAX]; 

    //Following variables used for subset 1
    char program_path[PATH_MAX];
    int found_program = 0;
    int arg_size = 0;

    if (program == NULL) {
        // nothing to do
        return;
    }

    if (strcmp(program, "exit") == 0) {
        do_exit(words);
        // `do_exit' will only return if there was an error.
        return;
    }

    //////////////////////SUBSET 0///////////////////////////
    
    //If cd is enterred
    if (strcmp(program, "cd") == 0) {
        
        //If directory is specified after cd command
        if (words[1] != NULL) {
            // chdir() changes the current working directory
            // of the calling process         
            if (chdir(words[1]) == 0) {
                return;
            }
            
            else if (chdir(words[1]) != 0) {
                fprintf(stderr, "cd: %s: No such file or directory\n", words[1]);
            }
        }      
        //If directory is not specified, return to home directory
        else {
            chdir(getenv("HOME"));
        }
        return;
    }
    
    //If pwd is enterred
    if (strcmp(program, "pwd") == 0) {
        // The getcwd() function copies an absolute pathname of the current
        // working directory to the array
        getcwd(pathname, sizeof pathname);
        printf("current directory is '%s'\n", pathname);
        return;
    }
    
     
    /////////////////////////SUBSET 1////////////////////////

    //This for loop works in case the the first character is not a '/'
    for (int j = 0; j < strlen(program); j++) {
        // If a command contains a '/' I do not need to 
        // search for it in the path directories
        if (program[j] == '/') {
            found_program = 1;
            strcpy(program_path, program);
            break;
        }

    }
   
    //Look through path directory till program is found
    for (int counter = 0; path[counter] != NULL; counter++) {
        
        //found_program works as a bool
        if (found_program == 0) {
            strcpy(program_path, path[counter]);
            arg_size = strlen(program_path);
            
            //Add  '/' at end of directory path
            program_path[arg_size] = '/';
            program_path[arg_size + 1] = '\0';
            
            strcat(program_path, program);
                
            if (is_executable(program_path)) {
                found_program = 1;                
            }         
        }
    } 
    
    if (!found_program) {           
        fprintf(stderr, "%s: command not found\n", program);
        return;
    }

    //Taken inspiration from lecture slides
    pid_t pid;
    extern char **environ;

    // The posix_spawn() function is used to create a new child process
    // that executes a specified file
    if (posix_spawn(&pid, program_path, NULL, NULL, words, environ) != 0) {
        fprintf(stderr, "%s: command not found\n", program);
        exit(1);
    }
    
    int exit_status;
    if (waitpid(pid, &exit_status, 0) == -1) {
        perror("waitpid");
        exit(1);
    }
    if ( WIFEXITED(exit_status) )
    {
        exit_status = WEXITSTATUS(exit_status);        
        
    }    
    printf("%s exit status = %d\n", program_path, exit_status);
    
    //////////////////////SUBSET 2///////////////////////////

    // Again taken inspiration from lecture slides
    // Appending to $HOME/.shuck_history
    char shuck_history[MAX_LINE_CHARS];
    char *home = getenv("HOME");
    char *history_path = ".shuck_history";

    strcpy(shuck_history, home);
    strcat(shuck_history, "/");
    strcat(shuck_history, history_path);

    FILE *output_stream = fopen(shuck_history, "a");

    if (output_stream == NULL) {
        perror("shuck_history");
        return;
    }


    // Printing commands into $HOME/.shuck_history
    for (int i = 0; words[i] != NULL; i++) 
    {
        fprintf(output_stream, "%s ", words[i]);
    }

    //NewLine for new command into history
    fprintf(output_stream, "\n");

    fclose(output_stream);     
    
}

//
// Implement the `exit' shell built-in, which exits the shell.
//
// Synopsis: exit [exit-status]
// Examples:
//     % exit
//     % exit 1
//
static void do_exit(char **words)
{
    assert(words != NULL);
    assert(strcmp(words[0], "exit") == 0);

    int exit_status = 0;

    if (words[1] != NULL && words[2] != NULL) {
        // { "exit", "word", "word", ... }
        fprintf(stderr, "exit: too many arguments\n");

    } else if (words[1] != NULL) {
        // { "exit", something, NULL }
        char *endptr;
        exit_status = (int) strtol(words[1], &endptr, 10);
        if (*endptr != '\0') {
            fprintf(stderr, "exit: %s: numeric argument required\n", words[1]);
        }
    }

    exit(exit_status);
}


//
// Check whether this process can execute a file.  This function will be
// useful while searching through the list of directories in the path to
// find an executable file.
//
static int is_executable(char *pathname)
{
    struct stat s;
    return
        // does the file exist?
        stat(pathname, &s) == 0 &&
        // is the file a regular file?
        S_ISREG(s.st_mode) &&
        // can we execute it?
        faccessat(AT_FDCWD, pathname, X_OK, AT_EACCESS) == 0;
}


//
// Split a string 's' into pieces by any one of a set of separators.
//
// Returns an array of strings, with the last element being `NULL'.
// The array itself, and the strings, are allocated with `malloc(3)';
// the provided `free_token' function can deallocate this.
//
static char **tokenize(char *s, char *separators, char *special_chars)
{
    size_t n_tokens = 0;

    // Allocate space for tokens.  We don't know how many tokens there
    // are yet --- pessimistically assume that every single character
    // will turn into a token.  (We fix this later.)
    char **tokens = calloc((strlen(s) + 1), sizeof *tokens);
    assert(tokens != NULL);

    while (*s != '\0') {
        // We are pointing at zero or more of any of the separators.
        // Skip all leading instances of the separators.
        s += strspn(s, separators);

        // Trailing separators after the last token mean that, at this
        // point, we are looking at the end of the string, so:
        if (*s == '\0') {
            break;
        }

        // Now, `s' points at one or more characters we want to keep.
        // The number of non-separator characters is the token length.
        size_t length = strcspn(s, separators);
        size_t length_without_specials = strcspn(s, special_chars);
        if (length_without_specials == 0) {
            length_without_specials = 1;
        }
        if (length_without_specials < length) {
            length = length_without_specials;
        }

        // Allocate a copy of the token.
        char *token = strndup(s, length);
        assert(token != NULL);
        s += length;

        // Add this token.
        tokens[n_tokens] = token;
        n_tokens++;
    }

    // Add the final `NULL'.
    tokens[n_tokens] = NULL;

    // Finally, shrink our array back down to the correct size.
    tokens = realloc(tokens, (n_tokens + 1) * sizeof *tokens);
    assert(tokens != NULL);

    return tokens;
}


//
// Free an array of strings as returned by `tokenize'.
//
static void free_tokens(char **tokens)
{
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}
