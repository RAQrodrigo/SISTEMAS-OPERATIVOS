#include "builtin.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "strextra.h"

char Internal_Commands[][10] = {"cd", "help", "exit"};

bool builtin_is_internal(scommand cmd){ 
    assert(cmd != NULL);

    bool is_internal = false;
    char *command = scommand_front(cmd);
    unsigned int Total_Commands = sizeof(Internal_Commands) / sizeof(Internal_Commands[0]);
    
    for(unsigned int i = 0;(!is_internal && i <= Total_Commands);++i) {
        is_internal = strcmp(command, Internal_Commands[i]) == 0;
    }
    return is_internal;
}

bool builtin_alone(pipeline p){
     assert(p != NULL);

    bool alone = false;
    
    if (pipeline_length(p) == 1) {
        scommand comando = pipeline_front(p);
        alone = builtin_is_internal(comando);
    }
    return alone;
}

void builtin_run(scommand cmd){
    assert(builtin_is_internal(cmd));
    char *input_command = strmerge(scommand_front(cmd),""); 
    scommand_pop_front(cmd);

    if( strcmp(input_command,"help") == 0){ //COMANDO HELP
        printf ("My bash \n");
        printf ("Creado por: Gaston Ferrero , Daniela Courel , Lourdes Mamani , Agustin Quevedo. \n");
        printf ("Comandos internos implementados: help,cd,exit: \n");
        printf ("help: Imprime por pantalla el nombre del shell,Sus creadores y los comandos internos. \n");
        printf ("cd: Permite navegar entre archivos. \n");
        printf ("exit: Cierra mybash \n");
        scommand_destroy(cmd);

    }else if (strcmp(input_command,"cd") == 0 ){
        if( scommand_length(cmd) != 1 ) { //COMANDO CD
            printf("Usá: cd <directory> \n" );
            scommand_destroy(cmd);

        }
        else if( chdir(scommand_front(cmd)) == 0 ) {
            printf( "Directorio cambio a %s\n", scommand_front(cmd) );
            scommand_destroy(cmd);
            
        } else {
            printf("Directorio invalido \n");
            scommand_destroy(cmd);
            
        }

    }else if(strcmp(input_command,"exit") == 0 ) { //COMANDO EXIT
        scommand_destroy(cmd);
        exit(EXIT_SUCCESS);
    }
}
