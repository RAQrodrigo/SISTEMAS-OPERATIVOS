#include <assert.h>
#include <stdbool.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> // Define stdin 0, stdout 1, stderr 2. Sirve para fork(), execvp()
#include <sys/types.h> // Define pid_t (pid es process ID o identificador de proceso)
#include <sys/wait.h> // Para la funcion wait()
#include <sys/stat.h>
#include <fcntl.h> // Incluye palabras como O_CREAT, permisos de open, etc

#include "strextra.h"
#include "command.h"
#include "execute.h"
#include "builtin.h"

/* redir_int y redir_out son dos funciones que sirven para tomar un comando para setear la redirecciones en el caso que la tengan y devolver
un booleano que responda a si tiene redirección de entrada7 salida respectivamente.
*/

static bool redir_int(scommand cmd){

        bool is_redir = scommand_get_redir_in(cmd) != NULL;
        
        if (is_redir) {

            int fd = open(scommand_get_redir_in(cmd),0, O_WRONLY); 

            if(fd == -1){
                fprintf(stderr, "open\n");
                exit(EXIT_FAILURE);
            }

            dup2(fd,0); // cambio la entrada al archivo indicado           

            close(fd);  
       }

    return is_redir;
}    


static bool redir_out(scommand cmd) {

        bool is_redir = scommand_get_redir_out(cmd) != NULL;

        if (is_redir) { 

            int fd = open(scommand_get_redir_out(cmd), O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

            if(fd == -1){
                perror("open");
                exit(EXIT_FAILURE);
            }
            
            dup2(fd,1); // Cambio la salida al archivo indicado

            close(fd);              
        }

        return is_redir;
}

//función para tomar el pipeline y ejecutarlo

void execute_pipeline(pipeline apipe) {

    scommand first_cmd = scommand_new();

    first_cmd = pipeline_front(apipe);

    char *cmd = "";

    cmd = strmerge(scommand_front(first_cmd), "");

    if (builtin_alone(apipe)) {                 //verifica si es un comando interno único y si lo es lo ejecuta y termina
        builtin_run(first_cmd);
    } else {

        int p[2];                               //crea la tubería para unir los dos procesos en caso de que existan
    
        if (pipe(p) == -1) {
               perror("pipe");
               exit(EXIT_FAILURE);
        }

        pid_t pid = fork();                     //se crea un proceso hijo para ejecutar el primer comando

        if (pid == -1){
            perror("error al crear un hijo");
            exit(EXIT_FAILURE);
        }
        
        if (pid == 0) {
            
            close(p[0]);

            redir_int(first_cmd);                   //se usan las funciones redin_int y redir_out para settear las redirecciones
            
            bool temp = redir_out(first_cmd);
            if (pipeline_length(apipe) > 1 && !temp) {
                dup2(p[1],1);
            }

            close(p[1]);  //cierro la punta de escritura            

            if (builtin_is_internal(first_cmd)){exit(EXIT_SUCCESS);}        //en el caso de que sea comando interno no se ejecuta
            
            int error = execvp(cmd, get_arguments(first_cmd));              //ejecuto el programa                     
            
            if(error == -1){
                perror("execvp");
                exit(EXIT_FAILURE);
            }                     
        
        
        } else {

            pipeline_pop_front(apipe);

            if (pipeline_length(apipe) == 1) {   // Si aún quedan comandos en el pipeline se carga el siguiente.

                scommand second_cmd = scommand_new();

                second_cmd = pipeline_front(apipe);

                char *cmd = "";

                cmd = strmerge(scommand_front(second_cmd), "");

                close(p[1]);
                
                pid_t pid = fork();             //se crea un segundo hijo para ejecutar el segundo comando
                
                if (pid == -1){
                    perror("error al crear un hijo");
                    exit(EXIT_FAILURE);
                }

                if (pid == 0){
                
                    redir_out(second_cmd);

                    bool temp = redir_int(second_cmd);
                    
                    if (!temp)
                        dup2(p[0],0);

                    close(p[0]); 

                    if (builtin_is_internal(second_cmd))        
                        exit(EXIT_FAILURE);
                    
                    int error = execvp(cmd, get_arguments(second_cmd));                      
                    if(error == -1){
                        perror("execvp");
                        exit(EXIT_FAILURE);
                    }              
                } 
            }else if (pipeline_length(apipe) > 1){
                printf("Eroor: se ingresaron más comando de los esperados\n");
            }
           
            if (!pipeline_get_wait(apipe)){     //se implementa la espera en el caso de haber &
                wait(NULL);                    
                wait(NULL);
            }
        }
    }
}


