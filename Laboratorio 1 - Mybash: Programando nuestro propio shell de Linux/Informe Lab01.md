# Laboratorio 1

## **Integrantes**:
Lourdes Mamani (lourdes.mamani@mi.unc.edu.ar).

Gaston Ferrero (gaston.andres.ferrero@mi.unc.edu.ar).

Daniela Courel (daniela.courel@mi.unc.edu.ar).

Agustin Quevedo (rodrigo.quevedo@mi.unc.edu.ar).

## **Introduccion**

El siguiente informe desarrolla el proceso de creación de un programa mybash que simula el comportamiento de la terminal de linux. A través  de una serie de módulos llevamos a cabo un programa que tiene como objetivo comprender y hacer uso de las syscall, atender al funcionamiento de los procesos, tuberías, diferenciar comandos internos de externos, emular el comportamiento de otros programas, implementar tipos abstractos a partir de especificaciones dadas, y comenzar a familiarizarnos con la lectura de código y documentación específica.

## **modularización**

Nuestro bash consta de 6 archivos .h y 5 archivos .c , organizados por:

#### *Mybash*: 
Donde se encuentra el algoritmo principal. El espacio en donde convergen las funcionalidades que ofrecen los demás módulos. Es en este módulo donde está la llamada a la función para mostrar el prompt característico de la terminal.

### *Command*: 
Donde se encuentran las funciones para parsear los comandos ingresados.

### *Parsing*: 
Donde se encuentran los algoritmos de parseo. Este módulo es el encargado de tomar los datos que se escriben en el prompt de la terminal y guardarlos en variables de tipo pipeline. Esto nos permite luego tener facilidad para acceder a los datos guardados haciendo uso de las funciones que son propias del tipo de dato.

### *Builtin*: 
Este módulo contiene la implementación de los comandos internos. Además nos provee de funciones para ejecutarlos y para poder diferenciarlos de externos.

### *Execute*: 
Donde se encuentra el algoritmo para los comandos externos. Es aquí donde se hace uso de las syscall. Este módulo considera la diferenciación de procesos. El proceso mybash se corresponde con un proceso parent. A partir de él se crean procesos child dentro de los cuales se ejecutan los comandos que se leen del prompt, previamente parseados con el módulo parsing. Aquí también se consideran y ejecutan los casos en que existan operadores de redirección de entrada y salida. 

### *TADS utilizados*: 
En general todos los módulos hacen uso de los TADs scommand y pipeline para guardar y recuperar datos.

## **Técnicas de Programación**

Hicimos uso de memoria dinámica para implementar la especificación de los tipos abstractos scommand y pipeline. Para esto decidimos usar el tipo lista **GSList** que ofrece la librería Glib. 
En algunas ocasiones usamos la librería **strextra** para facilitar el trabajo con cadenas de texto.
Usamos **assert** para advertir errores en casos de pre y post condición.
En especial podemos destacar las syscall utilizadas en el módulo execute. Estas son: fork(), execvp(), wait(), pipe(), dup2(), open(), close().

## **Herramientas de programación**

Usamos Bitbucket y Git para introducirnos al uso de repositorios remotos.

Usamos Visual Studio Code como editor de código.

Usamos Live Share para hacer debugging de forma dinámica, similar al uso de google docs. Esto nos permite intervenir sobre el código en tiempo real, en donde cada integrante puede escribir y sobreescribir el código que considere necesario.

Uso de tests con Check, pruebas con printf desde mybash.c

Compilamos usando el archivo makefile del kickstart. Parte del debugging lo hicimos probando los test dados.

## **Desarrollo**

A lo largo del desarrollo acordamos estar atentos a posibles modularizaciones e implementar las funciones de forma tal que nos permitan luego abstraernos de su implementación y atender solo a su función de manera intuitiva. Como fue el caso al sumar la función get_arguments al TAD scommand.

Empezamos por el desarrollo de command.c que consiste en implementar funciones previamente especificadas en command.h.
Lo primero fue definir la estructura de scommand. Acordamos usar el tipo lista para almacenar los comandos y aprovechar las funcionalidades que ofrece el tipo GSList de la librería Glib. Agregar un TAD externo supuso dedicar tiempo a la lectura y comprensión de la documentación. 

Luego nos dividimos el trabajo, dos tomaron execute.c , uno parsing.c y otro se dedicó al desarrollo del módulo builtin.

### *builtin.c*: 
Iniciamos haciendo builtin_is_internal, luego builtin_alone y terminamos haciendo builtin_run. El mayor inconveniente fue implementarlo de tal forma que para agregar un comando no haya que hacer muchos cambios, su solución fue usar un arreglo que contiene todos los comandos que nos pedían.

### *parsing.c*: 
Leímos la descripción de cada función, preguntamos a profesores sobre dudas con respecto a funciones de parser.h y especificaciones de parsing.c. Fue difícil entender algunas funciones de parser.h y darse cuenta que hay funciones de parser.h que no son necesarias para implementar en parsing.c (como por ejemplo la función parser_skip_blanks), también hubo problemas a la hora de testearlo con make test-parsing así que hubo que testearlo manualmente desde el main con la función pipeline_to_string.

### *execute*:
el módulo execute se desarrolló en partes, comenzando a partir de ejemplos simples. Con casos de un solo comando empezamos a armar una estructura mientras avanzábamos con las lecturas de manual y comprensión de nuevos conceptos, en particular, el funcionamiento de procesos y syscalls. Luego, una vez testeado el primer boceto, continuamos agregando funcionalidades. Así, pudimos sumar argumentos a un comando simple, luego ejecutar operaciones de redirección de entrada y salida, y por último la tubería para conectar dos comandos.
Las funciones static de archivo execute.c acordamos mantenerlas en el file para mantener la encapsulación del módulo.

## **Concluciones**

Concluimos que aún tenemos dificultades con el uso de Git. Además de las ventajas de guardar el código y permitir volver a versiones anteriores también cuenta con sus con algunos obstáculos para sobreescribir archivos, lo que a lo largo del desarrollo del proyecto nos tomó mucho tiempo poder sortear y en casos sin las certezas de haber actuado bien. 
La comprensión de los conceptos teóricos acelera y permite escribir código de manera más eficiente. 



