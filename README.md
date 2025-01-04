# Resumen Laboratorios - Sistemas Operativos

## Laboratorio 1: MyBash

### Objetivos
- Utilizar los mecanismos de concurrencia y comunicación de gruesa granularidad que brinda UNIX.
- Comprender cómo un intérprete de línea de comandos refleja la arquitectura y estructura interna de las primitivas de comunicación y concurrencia.
- Implementar un intérprete de línea de comandos (shell) de forma sencilla.
- Aplicar buenas prácticas de programación, como:
  - Estilo de código uniforme.
  - Uso de Tipos Abstractos de Datos (TAD) opacos.
  - Pruebas unitarias y de caja negra.
  - Programación defensiva.
  - Uso de herramientas de depuración de programas y memoria.

### Objetivos de Implementación
- Codificar un shell similar a **bash** (Bourne Again Shell), llamado `mybash`, con las siguientes funcionalidades:
  - Ejecución de comandos en modo **foreground** y **background**.
  - Redirección de entrada y salida estándar.
  - Uso de **pipes** entre comandos.
- Implementar comandos internos:
  - `cd` para cambiar de directorio.
  - `help` para mostrar ayuda.
  - `exit` para salir del shell.
- Soportar la salida mediante `CTRL-D` (carácter de fin de transmisión - EOT).
- Ser robusto ante entradas incompletas o inválidas.

### Requisitos de Implementación
- Utilizar TADs opacos.
- Evitar pérdidas de memoria, salvo casos justificados y documentados (e.g., bibliotecas externas).
- Seguir buenas prácticas de programación:
  - Modularización adecuada.
  - Nombres descriptivos para variables y funciones.
  - Comentarios claros y consistentes.
  - Uniformidad idiomática en el estilo de código.

---

## Laboratorio 2: Semáforos en XV6

### Descripción
- Implementación de un sistema de **semáforos nombrados** en **XV6** (sistema operativo con fines académicos).
- Los semáforos serán implementados en espacio de kernel y accesibles mediante **syscalls** desde espacio de usuario.
- Inspirado en los semáforos nombrados definidos en **POSIX**, este mecanismo sirve para la sincronización entre procesos.

---

## Laboratorio 3: Planificador de Procesos

### Descripción
- El planificador apropiativo de **XV6** utiliza un algoritmo sencillo para distribuir tiempo de CPU entre procesos, lo que puede generar ineficiencias.
- Objetivos del laboratorio:
  - Estudiar el funcionamiento del scheduler original de **XV6**.
  - Analizar qué procesos se benefician o perjudican con la política de planificación actual.
  - Desarrollar una implementación propia para reemplazar la política existente, cumpliendo con condiciones específicas para mejorar el rendimiento.

---

## Laboratorio 4: BigBrother FS

### Descripción
- Trabajo con un sistema de archivos similar a FAT, inicialmente con una interfaz limitada (e.g., sin soporte para eliminar directorios con `rmdir`).
- Objetivos:
  - Completar parte de la interfaz para ampliar la funcionalidad del sistema de archivos.
  - Comprender la flexibilidad que provee un sistema de archivos en UNIX.
  - Añadir funcionalidades para monitorear la actividad del sistema de archivos de manera transparente al usuario.
- Este proyecto destaca la importancia de los sistemas operativos de código abierto al explorar su flexibilidad y capacidad de extensión.

## Informacion adicional
Debido a que los laboratorios son demasiado extensos, solo puse los objetivos de cada uno. Dentro de estos estaran los enunciados correspondientes.
