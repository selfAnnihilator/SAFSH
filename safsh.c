// safsh = Simple As Fuck shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>


void safsh_loop(void);
char *safsh_read_line(void);
char **safsh_split_line(char *line);
int safsh_launch(char **args);
int safsh_execute(char **args);
void handle_sigint(int sig);


#define SAFSH_PROMPT "> "
void safsh_loop(void){
    char *line;
    char **args;
    int status;

    do{
        printf(SAFSH_PROMPT);
        fflush(stdout);  // Force output of the prompt immediately

        line = safsh_read_line();
        args = safsh_split_line(line);
        status = safsh_execute(args);

        free(line);
        free(args);
    }while(status);
}


#define SAFSH_RL_BUFSIZE 1024
char *safsh_read_line(void){
    int bufsize = SAFSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer){
        fprintf(stderr, "safsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        // read a character
        c = getchar();

        // if we hit EOF or newline, terminate the string with null character and return
        if(c == EOF || c == '\n'){
            buffer[position] = '\0';
            return buffer;
        } else{
            buffer[position] = c;
        }
        position++;

        // if we have exceeded the buffer size, reallocate
        if(position >= bufsize){
            bufsize += SAFSH_RL_BUFSIZE;
            char *new_buffer = realloc(buffer, bufsize);
            if (!new_buffer) {
                free(buffer);
                fprintf(stderr, "safsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
            buffer = new_buffer;
        }
    }
}


#define SAFSH_TOK_BUFSIZE 64
#define SAFSH_TOK_DELIM " \t\r\n"
char **safsh_split_line(char *line){
    int bufsize = SAFSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if(!tokens){
        fprintf(stderr, "safsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, SAFSH_TOK_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= bufsize){
            bufsize += SAFSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens){
                fprintf(stderr, "safsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, SAFSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}


int safsh_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0){
        // child process
        if(execvp(args[0], args) == -1){
            perror("safsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0){
        // error forking
        perror("safsh");
    } else {
        // parent process
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


/*
    Function declarations for built-in shell commands.
*/

int safsh_cd(char **args);
int safsh_help(char **args);
int safsh_exit(char **args);


/*
    List of built-in commands, followed by their corresponding functions.
*/

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
};

int (*builtin_func[]) (char **) = {
    &safsh_cd,
    &safsh_help,
    &safsh_exit
};

int safsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/*
    built-in function implementations.
*/
int safsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "safsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("safsh");
        }
    }

    return 1;
}

int safsh_help(char **args) {
    int i;
    printf("selfAnnihilator's SAFSH\n");
    printf("Type program names and arguments and press enter.\n");
    printf("The following are built-in:\n");

    for(i = 0; i < safsh_num_builtins(); i++) {
        printf(" %s\n", builtin_str[i]);
    }

    printf("Use man command for information on other programs.\n");
    return 1;
}

int safsh_exit(char **args) {
    return 0;
}

int safsh_execute(char **args) {
    int i;

    if(args[0] == NULL){
        // an empty command was entered.
        return 1;
    }

    for(i = 0; i < safsh_num_builtins(); i++){
        if(strcmp(args[0], builtin_str[i]) == 0) {
            return builtin_func[i](args);
        }
    }

    return safsh_launch(args);
}

void handle_sigint(int sig){
    printf("safsh: caught SIGINT\n");
    write(STDOUT_FILENO, "\n> ", 3);
    fflush(stdout);
}


int main(int argc, char **argv) {
    // Load config files, if any.
    signal(SIGINT, handle_sigint);

    // run command loop.
    safsh_loop();

    // perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}