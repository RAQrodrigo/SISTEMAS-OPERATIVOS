#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "builtin.h"

static void show_prompt(void) {
    printf ("mybash> ");
    fflush (stdout);
}

int main(int argc, char *argv[]) {
    pipeline the_pipe;
    Parser input;
    bool quit = false;

    input = parser_new(stdin); // input = "ls -l"

    while (!quit) {
        show_prompt();
        while(waitpid(-1, NULL, WNOHANG) > 0); // Mata los procesos zombi
        the_pipe = parse_pipeline(input);

        quit = parser_at_eof(input);
        
        if(the_pipe != NULL){
            execute_pipeline(the_pipe);

            the_pipe = pipeline_destroy(the_pipe);
        } else if(!quit){
            fprintf(stderr, "Error \n");
        }
    }
    parser_destroy(input); input = NULL;
    
    return EXIT_SUCCESS;
}

