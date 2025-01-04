#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"



static scommand parse_scommand(Parser p) {
   
    arg_kind_t args;

    scommand result = scommand_new();
    char *string_arg; 

    string_arg = parser_next_argument(p, &args); 

   while (string_arg != NULL)
   {

        if(args == ARG_INPUT ){
            scommand_set_redir_in(result, string_arg);
            
        }else if(args == ARG_OUTPUT){
            scommand_set_redir_out(result, string_arg);

        }else if(args == ARG_NORMAL && string_arg != NULL){
            
            scommand_push_back(result, string_arg);
        }
       
        string_arg =  parser_next_argument(p, &args);
   }

    return result;
}

pipeline parse_pipeline(Parser p) {
   assert(p != NULL); 

    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false, another_pipe=true;
    cmd = parse_scommand(p);

    bool nulo = (cmd != NULL);
    
    while(another_pipe && !error && nulo) { //otro pipe 
        
        pipeline_push_back(result, cmd);

        parser_op_pipe(p, &another_pipe);

        error = parser_at_eof(p);
        cmd = parse_scommand(p);
        nulo = (cmd != NULL);

    }
    /* Opcionalmente un OP_BACKGROUND al final */
    bool  was_op_background, garbage;
    
    parser_op_background(p, &was_op_background);
    pipeline_set_wait(result , was_op_background);

    /* Tolerancia a espacios posteriores */
    /* Consumir todo lo que hay inclusive el \n */

    parser_garbage(p, &garbage);
    /* Si hubo error, hacemos cleanup */
    if(garbage == true){
        result = pipeline_destroy(result);
        //devolver un error de ruido
    }

    return result;
}