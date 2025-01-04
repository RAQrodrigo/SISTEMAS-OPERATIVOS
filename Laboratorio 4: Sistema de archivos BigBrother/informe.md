# Laboratorio 4
 
## **Integrantes**:
Lourdes Mamani (lourdes.mamani@mi.unc.edu.ar).
 
Gaston Ferrero (gaston.andres.ferrero@mi.unc.edu.ar).
 
Daniela Courel (daniela.courel@mi.unc.edu.ar).
 
Agustin Quevedo (rodrigo.quevedo@mi.unc.edu.ar).
 
### **preguntas**
 
1. **Cuando se ejecuta el main con la opción -d, ¿qué se está mostrando en la pantalla?**
 
La opción -d te muestra en tiempo y a medida que se sigue ejecutando cualquier instrucción dentro de la imagen montada, diferentes mensajes provenientes de Fuse; sirve para debuggear con más detalle el código.

 
2. **¿Hay alguna manera de saber el nombre del archivo guardado en el cluster 157?**
 
Se puede ir recorriendo la tabla fat desde el 157 para "abajo"" (hasta el cluster 2) e ir buscando si hay algún cluster que su siguiente sea 157, si se encuentra se repite el mismo proceso hasta llegar al cluster 2, entonces se puede decir que ese último cluster encontrado va a ser el primer cluster de la cadena con seguridad, luego se pueden ir recorriendo todos los archivos y directorios desde el root e ir comparando ese cluster encontrado con el start_cluster de los diferentes fat_file.
 
 
3. **¿Dónde se guardan las entradas de directorio? ¿Cuántos archivos puede tener adentro un directorio en FAT32?**

Para entender a donde se guardan las entradas de directorio sigo el funcionamiento de mkdir. 
La entrada se guarda en el disco.
Un directorio puede tener dentro hasta 16 archivos. Luego se queda sin espacio. Un directorio dispone de un cluster de 512 bytes para almacenar datos. Dado que la estructura de un archivo es de 32 bytes el cluster solo puede almacenar 16 archivos.
16*32 bytes = 512 bytes
Notamos que se pueden crear más de 16 directorios sólo si hay menos de 16 archivos en ese directorio, pero tienen un comportamiento indeterminado, sobretodo después de desmontarlo y volverlo a montar.
 
 
4. **Cuando se ejecuta el comando como ls -l, el sistema operativo, ¿llama a algún programa de usuario? ¿A alguna llamada al sistema? ¿Cómo se conecta esto con FUSE? ¿Qué funciones de su código se ejecutan finalmente?**
 
Usando el comando strace podemos ver por consola las llamadas a sistema que ocurren cuando se llama a ls -l. Las llamadas a sistema tienen que interactuar con la interfaz de FUSE para acceder a los datos de los archivos que se van a listar. Se deben ejecutar las operaciones de lectura de FUSE. fat_fuse_read()
 
 
 
5. **¿Por qué tienen que escribir las entradas de directorio manualmente pero no tienen que guardar la tabla FAT cada vez que la modifican?**
 
Escribir una entrada de directorio implica que se actualice la tabla FAT para guardar la asignación de clusters. Posibilitando recuperar la distribución de los cluster asignados al directorio.
 
 
6. **Para los sistemas de archivos FAT32, la tabla FAT, ¿siempre tiene el mismo tamaño? En caso de que sí, ¿qué tamaño tiene?**
 
La tabla fat indica la distribución de archivos y directorios en el disco. Cada archivo o directorio tiene asignado un número de clusters (no necesariamente contiguos). Cada archivo tiene un cluster de inicio a partir del cual se puede leer cual es el siguiente cluster que contiene sus datos. El dato que indica el siguiente cluster es un número de 32 bytes. De modo que la cantidad máxima de redireccionamientos posibles para la tabla fat es de 2^32 = 4294967296, o 4GB.


### **observaciones**

Tuvimos un pequeño bug que no pudimos resolver que siempre la primer linea que se escribe en fs.log se escribe mal, pero el resto de lineas posteriores ya se escriben bien.

También en la parte 3 pudimos notar que en una computadora las imagenes no compilan correctamente pero en la del resto compilan bien o medianamente bien, incluso probando los tar.gz originales e imagenes originale (con algunos fallos, en algunas computadoras compilaba bien de a momentos sin siquiera llamar a rmdir ni rm y luego fallaban borrando archivos incluso cambiando continuamente las imagenes originales), se intentó probar rmdir y rm en las computadoras que compilaban mejor, lo mismo para las otras partes. 