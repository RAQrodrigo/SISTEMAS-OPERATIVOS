#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "syssem.h"

#define CANTIDAD_DE_SEMAFOROS_DISPONIBLES 10

struct semaphore semaphores[CANTIDAD_DE_SEMAFOROS_DISPONIBLES];

// Inicializacion. SEMAFOROS DESHABILITADOS



uint64 
sys_semaphores_disabled (void) {
  
  int n = 0;
	while (n < CANTIDAD_DE_SEMAFOROS_DISPONIBLES ) {

  acquire(&semaphores[n].semlock);  
	semaphores[n].value = -1;
  release(&semaphores[n].semlock); 
  n++;
  }

	return 0;
  
}


//POLITICAS DEL SEMAFORO

// UN SEMAFORO CON VALOR a -1 ESTA INHABILITADO PARA SU USO. 

// LLAMAR A semaphores_disabled ANTES DE INICIALIZAR LOS SEMÁFOROS

// SI SE INTENTA INCREMENTAR UN SEMAFORO POR ENCIMA DEL VALOR DE INICIALIZACIÓN NO SE HACE EL INCREMENTO.

uint64
sys_sem_open(void){
  int value, sem;
  argint(0, &sem);
  argint(1, &value);
  if (sem >= CANTIDAD_DE_SEMAFOROS && sem < 0) { // Chequea que no se acceda a un elemento que no exista en el arreglo de semaforos
    printf("Se trato de acceder a un semaforo que no existe");
    exit(1);
  }

  semaphores[sem].value = value; // Se espera que el value que se asigna sea mayor que 0
  semaphores[sem].init_value = value; //para casos borde

  initlock(&semaphores[sem].semlock, "sem");// Iniciar el candado con el nombre "sem" 

  return 0;
}

uint64
sys_sem_up(void){
  int sem;
  argint(0, &sem);
  //caso bode si es igual al de su inicializacion
 if(semaphores[sem].value == semaphores[sem].init_value){
    release(&semaphores[sem].semlock);
    return 0;
  }
//Por decision politica el semaforo no incrementa si el valor es menor que 0. Quiere decir que el semaforo esta inhabilitado para su uso
  if (semaphores[sem].value<0){
    panic("El semaforo esta inhabilitado");
    return 0;
  }else if (semaphores[sem].value==0){
    acquire(&semaphores[sem].semlock); //Agarro el candado para proteger la zona critica 
    semaphores[sem].value++; // Operacion criticas
    // LUEGO DE INCREMENTAR VERIFICAR DESPERTAR LOS PROCESOS QUE DUERMEN
    wakeup(&semaphores[sem]);
    release(&semaphores[sem].semlock);
  }

  return 0;
}

uint64
sys_sem_down(void){
  int sem;
  argint(0, &sem);
  acquire(&semaphores[sem].semlock); // PROTEGE LA OPERACION CRITICA
  semaphores[sem].value--; // Operación crítica.

  // LUEGO DE DECREMENTAR VERIFICAR EL VALOR DEL SEMAFORO Y DE ACUERDO A ESO PONER A DORMIR LOS PROCESOS
  if (semaphores[sem].value==0){
    sleep(&semaphores[sem], &semaphores[sem].semlock);
  }
  release(&semaphores[sem].semlock);// LIBERO EL CANDADO
  return 0;
}

uint64
sys_sem_close(void){
  int sem;
  argint(0, &sem);
  //caso borde si ya estaba cerrado anteriormente
  if(semaphores[sem].value == -1){
   return 0;
  }
  semaphores[sem].value = -1; //por politica

  return 0;
} 
