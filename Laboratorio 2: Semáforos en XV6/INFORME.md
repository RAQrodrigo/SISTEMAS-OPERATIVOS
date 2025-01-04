# Laboratorio 2

## **Integrantes**:
Lourdes Mamani (lourdes.mamani@mi.unc.edu.ar).

Gaston Ferrero (gaston.andres.ferrero@mi.unc.edu.ar).

Daniela Courel (daniela.courel@mi.unc.edu.ar).

Agustin Quevedo (rodrigo.quevedo@mi.unc.edu.ar).

## **Introducción:**
Hicimos un sistema de semáforos, para que imprima por pantallas 2 palabras (ping y pong) usando 2 procesos diferentes (en un proceso corre una función que imprime ping y en el otro la función que imprime pong). El sistema está pensado e implementado para que corra en XV6.
El semáforo se propone como una solución a los problemas de concurrencia. Nos permite tomar control sobre la ejecución de los procesos, y evitar así la ejecución simultánea y el orden de ejecuciones.

## **Explicación general:**

Antes de pasar a la implementación consideramos importante conocer que significa un semáforo y que implicancias tiene. 
En el libro The Little Book of Semaphores, Downye lo presenta como una estructura de datos útil para solucionar una variedad de problemas de sincronización.  El autor lo asocia a una variable de tipo entero con algunas especificaciones propias:

1. Cuando se crea el semáforo, se puede inicializar su valor con cualquier número entero pero luego de eso solo tendrán permitido operar sobre él las operaciones definidas como increment (incrementar uno) y decrement (decrementar 1). No se podrá leer el valor actual de un semáforo.

2. Cuando un hilo decrementa el semáforo, si el resultado es negativo, el hilo se bloquea a sí mismo y no puede continuar hasta que otro hilo vuelva a incrementar el semáforo.

3. Cuando un hilo incrementa el semáforo, si hay otros hilos esperando, uno de los hilos que espera se desbloqueará.

Antes de comenzar la implementación del semáforo en XV6 decidimos cambiar la política de la operación decrementar sobre números negativos y definimos a los semáforos en -1 como inhabilitados para su uso. Y cuando el usuario necesite hacer uso de un semáforo deberá inicializarlo. 
Existe una cantidad de semáforos limitada. Si el usuario quisiera inicializar más semáforos de los que dispone el sistema recibirá un mensaje de advertencia.

Para la implementación usamos el lock, la herramienta de control de sincronización que provee xv6. Es un TAD cuyo funcionamiento podría asociarse al de un candado. El candado se usa para proteger las operaciones críticas del programa (las operaciones críticas son aquellas en las cuales pueden suceder condiciones de carrera o problemas de concurrencia) y una vez llevada a cabo la operación se libera el candado.  

Luego de comprender el concepto definimos las operaciones del semáforo de la siguiente manera:

**sem_up**: Incrementa en 1 el valor del semáforo, además de verificar si el valor del semáforo es mayor a 0, en caso de que lo sea despierta el proceso que esté durmiendo. Si se intenta incrementar el semáforo por encima de su valor inicial , simplemente no se suma.

**sem_down**: Decrementa en 1 el valor del semáforo, además de verificar si el proceso es 0 , en caso de que lo sea duerme el proceso. 

**sem_open**: inicializa un semáforo con una valor inicial , además guarda ese valor en una variable para utilizarla en el control de casos bordes. 

**sem_close**: deshabilita el semáforo, asignando -1 a su valor. (política de desarrollo), también se fija si se cerró con anterioridad, en ese caso no se asigna -1.

**TADS utilizados**: Se usó el TAD lock el cual está implementado como una struct spinlock cuyo campo importantes es locked y cuyas operaciones asociadas son acquire y release. Implementamos el semáforo como un TAD usando una estructura que guarda el valor del semáforo, con un campo para una estructura spinlock y un tercer campo que guarda el valor de inicialización del semáforo.

## **Herramientas de programación:**

Nuestro entorno de desarrollo fue Visual Studio Code y git como controlador de versiones. También usamos gedit para escribir y leer código. 
Otra herramienta útil fue el Jam de Google o la pizarra de Google Meet para poder comunicar la lógica del programa a través de dibujos y esquemas.


## **Desarrollo:**
Iniciamos haciendo ejemplos simples sobre que debería entregar la función principal, para luego iniciar a buscar información sobre cómo implementar las operaciones del semáforo como llamadas al sistema.

Uno de nuestros problemas principales fue la comprensión de la documentación y el funcionamiento de xv6. 

Para entender la sintaxis en xv6 nos ayudó ver y estudiar las implementaciones de las funciones que usamos que estaban en xv6.

**Acquire** y **release** son operaciones de lock. **Acquire** toma un lock y lo cierra para proteger la operación crítica, así un proceso puede operar sin correr riesgo de interrupciones y condiciones de carrera. Y **release** libera el candado para que otros procesos puedan seguir operando sobre los datos compartidos.
 
**Sleep** y **wakeup**: son operaciones que permiten controlar la ejecución de los procesos y así hacer un aprovechamiento más eficiente del uso de los procesadores. En lugar de mantener un proceso en un loop que pregunta constantemente si ya puede ejecutarse, **sleep** lo pone a dormir hasta que otro proceso le despierte con **wakeup** para avisarle que ya sucedió el evento que esperaba y puede continuar su ejecución. La comunicación entre procesos se da por un canal de espera compartido que se pasa como argumento a las llamadas sleep y wakeup.(e.g. sleep(chan) y wakeup(chan)).

**Argint**. Xv6 no permite pasar argumentos desde espacio de usuario hacia espacio kernel. Por esto, cuando es necesario parametrizar una llamada a sistema xv6 proporciona la función argint para poder hacerlo. 

Luego de implementar las llamadas a sistema del semáforo lo último fue 
reordenar la lógica del uso del semáforo en el programa de usuario. 

## **Concluciones:**

Lo más complejo del laboratorio fue el estudio y comprensión de los conceptos que estructuran el programa. Esto demandó muchas horas de lectura de bibliografía y documentación. Consideramos una destreza importante poder relacionar la descripción metafórica de los conceptos con su implementación en código y así asociar el funcionamiento total del programa a una descripción narrativa del mismo. En el caso de este laboratorio una historia de candados que protegen información y barreras de semáforos que se abren y cierran habilitando secuencialmente el paso a los distintos procesos que se quieren ejecutar.

**Nota**: El trabajo lo hicimos entre todos por mas que no hubo muchos pusheos de todos los integrantes del grupo.

**Bibliografía**: 

xv6, a simple, Unix-like teaching operating system. Russ Cox, Frans Kaashoek, Robert Morris.

The little book of semaphores, Allen B. Downey.

