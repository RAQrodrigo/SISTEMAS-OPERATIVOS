#include <stdio.h> // para entradas y salidas
#include <stdlib.h> // para 
#include <stdbool.h> // para booleanos
#include <glib.h>
#include <assert.h>
#include <string.h>
#include "command.h"
#include "strextra.h"

// Command.c OFFICIAL

// TAD Scommand

// ESTRUCTURA scommand_s

struct scommand_s {
    GSList *cmd;
    char *in;
    char *out;
}; 

// CONSTRUCTORES

scommand scommand_new(void){

    scommand s = NULL;

    s = malloc(sizeof(struct scommand_s));

    s->cmd = NULL;
    s->in = NULL;
    s->out = NULL;
    
    assert(s != NULL && scommand_is_empty (s) &&
    scommand_get_redir_in (s) == NULL &&
    scommand_get_redir_out (s) == NULL);
    return s;
   
}

scommand scommand_destroy(scommand self){
    assert(self != NULL);

    while(self->cmd != NULL){
        scommand_pop_front(self);
    }

    if(self->in != NULL){
        free(self->in);
        self->in = NULL;

    }else if(self->out != NULL){
        free(self->out);
        self->out = NULL;

    }
    
    free(self);
    self = NULL;

    assert(self == NULL);
    return self;
}


// MODIFICADORES


void scommand_push_back(scommand self, char * argument){
    assert(self!=NULL && argument!=NULL);

    self->cmd = g_slist_append(self->cmd, argument);    
    
    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self){

    assert(self!=NULL && !scommand_is_empty(self));

    free((char*)g_slist_nth_data (self->cmd, 0));
    self->cmd = g_slist_remove_link(self->cmd, self->cmd);

}

void scommand_set_redir_in(scommand self, char * filename){
    assert(self!=NULL);
    self->in = filename;
}

void scommand_set_redir_out(scommand self, char * filename){
    assert(self!=NULL);
    self->out = filename ;
}

char **get_arguments(scommand cmd) {

        unsigned int final = scommand_length(cmd) +1;
        
        char ** argumentos = malloc(final * sizeof(char *));
        argumentos[0]= NULL;

        if (scommand_length(cmd) > 0) {

            GSList *auxiliar = cmd->cmd;

            for (size_t i = 0; i < scommand_length(cmd); i++)
            {
                    char * arg = malloc(sizeof(char *));
                    arg = auxiliar->data;
                    
                    argumentos[i] = arg;

                    auxiliar = auxiliar -> next;
            }
            
            argumentos[final] = malloc(sizeof(char *));
            argumentos[final]= NULL;
        }

        return argumentos; 
}


// PROYECTORES


bool scommand_is_empty(const scommand self){
    assert(self!=NULL);
    return (self->cmd == NULL);
}

unsigned int scommand_length(const scommand self) {
    assert(self!=NULL);

    return g_slist_length(self->cmd);
    
}

char * scommand_front(const scommand self){
    assert(self!=NULL && !scommand_is_empty(self));
    
    char *result = self->cmd->data;

    assert(result!=NULL);
    return result;
}

char * scommand_get_redir_in(const scommand self){
    assert(self!=NULL);
    return self->in;
}


char * scommand_get_redir_out(const scommand self){
    assert(self!=NULL);
    return self->out;
}

char * scommand_to_string(const scommand self){
    assert(self!=NULL);

    GSList *aux = self->cmd; //este cmd apunta a la misma lista
    
    char * result = strdup(""); // result = "ls"

    if(aux != NULL){
    result = strmerge(result, aux->data);
    aux = aux->next; // Movemos al siguiente elemento
    
    while (aux != NULL) {
        result = strmerge(result," ");
        result = strmerge(result,aux->data);
        aux = aux->next;
    }
    
    }
    
    if (self->in != NULL){
        result = strmerge(result," < ");
        result = strmerge(result,scommand_get_redir_in(self));
    }
    if (self->out != NULL){
        result = strmerge(result," > ");
        result = strmerge(result,scommand_get_redir_out(self));
    }
    
    assert(scommand_is_empty(self) ||
    scommand_get_redir_in(self)==NULL || scommand_get_redir_out(self)==NULL ||
    strlen(result)>0);
    
    return result;
}


// TAD PIPELINE

// Estructura pipeline

struct pipeline_s{
    GSList * scommandos;
    bool wait;
};


// CONSTRUCTORES 


pipeline pipeline_new(void){

    pipeline p = NULL;

    p = malloc(sizeof(struct pipeline_s));

    p->scommandos = NULL;
    p->wait = true;
    assert(p != NULL
    && pipeline_is_empty(p)
    && pipeline_get_wait(p));

    return p;
}

// Aqui va pipeline_destroy
pipeline pipeline_destroy(pipeline self){
    assert(self != NULL);
    if(self->scommandos != NULL){

        g_slist_free_full(self->scommandos,free);
        self->scommandos = NULL;
    }
    free(self);
    self = NULL;

    assert(self == NULL);

    return self;
}

// MODIFICADORES


void pipeline_push_back(pipeline self, scommand sc){
    assert(self!=NULL && sc!=NULL);

    self->scommandos = g_slist_append(self->scommandos, sc);
    
    assert(!pipeline_is_empty(self));
}


void pipeline_pop_front(pipeline self){
    assert(self!=NULL && !pipeline_is_empty(self));

    gpointer com = g_slist_nth_data(self->scommandos,0);
 
    self->scommandos = g_slist_remove( self->scommandos, com);

    GSList * aux = g_slist_alloc ();
    self->scommandos = g_slist_remove_link (self->scommandos, aux);
    
    g_slist_free_1 (aux);
 }

void pipeline_set_wait(pipeline self, const bool w){
    assert(self!=NULL);
    self->wait = w ; 
 }


 // PROYECTORES


bool pipeline_is_empty(const pipeline self){
    assert(self != NULL);
    return(self->scommandos == NULL);
}

unsigned int pipeline_length(const pipeline self) {
    assert(self != NULL);
    return g_slist_length (self->scommandos);
    assert((pipeline_length(self)==0) == pipeline_is_empty(self));

}


scommand pipeline_front(const pipeline self){
    assert(self!= NULL && !pipeline_is_empty(self));
   	
   	scommand result = g_slist_nth_data(self->scommandos,0);
    
    assert(result!=NULL);
    return result;
}

bool pipeline_get_wait(const pipeline self){
    assert(self!=NULL);
    return (self->wait);
}



char *pipeline_to_string(const pipeline self){
    assert(self!=NULL);
       
    GSList *aux = self->scommandos; //este cmd apunta la lista
    
    char *result = strdup(""); 

    if(aux != NULL){
        result = strmerge(result, scommand_to_string(aux->data));
        aux = aux->next; // Movemos al siguiente elemento
    
        while (aux != NULL) {
            result = strmerge(result," | ");
            result = strmerge(result,scommand_to_string(aux->data));
            aux = aux->next;
        }
   
        if(pipeline_get_wait(self)) {
    	    result = strmerge(result," & ");
        }
    }

   assert(pipeline_is_empty(self) || pipeline_get_wait(self) || strlen(result)>0);
    return result;
}

