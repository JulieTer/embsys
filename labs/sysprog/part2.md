## Exercice 1 : GDB et fichier core

**Question 1** : On obtient un segmentation fault; il y a donc une erreur non gérée qui débouche sur un arrêt involontaire du processus.

**Question 2** : Avec echo $? on obtient l'entier 139; la commande kill -l 11 indique que le signal reçu est SEGV. (11 vient de la soustraction de 128 à 139)

**Question 3** : On obtient la trace suivante:

````
    #0  strlen () at ../sysdeps/x86_64/strlen.S:106
    #1  0x00007f7df10ae69c in _IO_puts (str=0x0) at ioputs.c:35_
    #2  0x00007f7df1409b41 in knot_to_kmh_str (not=5.51000023, size=6,     format=0x7f7df140a00f "%05.1f", kmh_str=0x7ffe7034c410 "010.2") at nmea.c:23
    #3  0x00007f7df1409f98 in nmea_vtg (vtg=0x7ffe7034c450) at nmea.c:178
    #4  0x0000000000400bc4 in write_vtg (fd=3) at gps.c:40
    #5  0x0000000000400e3c in main () at gps.c:109
````

Les fonctions strlen et ioputs des lignes #0 et #1 sont relativement "standards"; il est donc peu probable que l'erreur vienne de là. Elle est donc plus probablement située dans les fonctions des lignes suivantes (#2 à #5).

A étudier de plus près le code de la fonction knot_to_kmh_str (#2), il semblerait que l'on soit rentré dans le cas GPS_OK non définie, ce qui cause la sortie du fait de puts(NULL).

**Question 4** : En mode interactif, on obtient le message suivant:

````
    /home/u/cours_linuxembarque/embsys/labs/sysprog/gps/bin/gps: error while loading shared libraries: libptmx.so: cannot open shared object file: No such file or directory
````

Une ou plusieurs librairies ne s'importent donc pas correctement; en l'occurence il s'agit de la librairie libptmx.so.

**Question 5** : D'après les informations obtenues avec la commande man ldd:

````
    ldd prints the shared objects (shared libraries) required by each program or shared object specified on the command line.
````

On va donc pouvoir utiliser ldd pour localiser la librairie qui ne s'est pas importée correctement, à l'aide du retour de la commande ldd ./gps:

````
  linux-vdso.so.1 =>  (0x00007fff14395000)
	libptmx.so => not found
	libnmea.so => not found
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f869138c000)
	/lib64/ld-linux-x86-64.so.2 (0x000055d7ce897000)
````

On constate qu'en plus de la librairie manquante libptmx.so (constatée en question 4), la librairie libnmea.so n'a pas non plus été trouvée.


**Question 6** : De cette ligne du fichier run.sh (qui indique où aller trouver les librairies), on déduit qu'il faut "ajouter" l'importation des librairies concernées dans le makefile afin de pouvoir également y accéder depuis le fichier compilé, sans avoir à nécessairement passer par le run.sh

````
    export LD_LIBRARY_PATH=$ROOT_DIR/lib
````

**Question 7** : De la commande man gdb on obtient:

````
       step
           Execute next program line (after
           stopping); step into any function calls
           in the line.

       next
           Execute next program line (after
           stopping); step over any function calls
           in the line.
````

Step (s) exécute bien le programme ligne par ligne et renvoie toutes les informations associées, alors que Next (n) n'informe que sur la fonction exécutée, sans rentrer dedans étape par étape.

**Question 8** : Comme beaucoup d'outils "à distance", celui-ci peut être utilisé pour résoudre des problèmes sans être sur place, même sans avoir nécessairement accès au code lui-même. Ce genre de situations peut arriver par exemple pour des clients chez qui on ne peut se déplacer, mais à qui l'on peut néanmoins porter assistance (dans une certaine mesure).




## Exercice 2 : LD_PRELOAD et sigaction

**Question 1** : En enlevant la partie "gestion d'erreur" de knot_to_kkmh_str repérée en question 3, on écrit la fonction suivante dans hook.c:

````
int knot_to_kmh_str(float not, size_t size, char * format, char * kmh_str)
{
    float kmh = KNOT_TO_KMH * not;
    snprintf(kmh_str, size, format, kmh);


    return kmh;
}
````

**Question 2** : En s'inspirant du Makefile proposé, on peut écrire des commandes adaptées à la compilation du fichier hook.c:

````
SONAME = libhook.so
GCC = gcc

all: hook
    $(GCC) -g -c -fPIC hook.c -o hook.o
    $(GCC) -g -shared -Wl,-soname,$(SONAME) -o $(SONAME) hook.o

hook:
    @echo "TODO : create a shared library with hook.c"
````

La compilation fonctionne sans problème.

**Question 3** : On hooke le binaire avec libhook.so en rajoutant la ligne suivante au fichier run.sh:

````
LD_PRELOAD=$(pwd)/libhook.so
````

La commande $sh run.sh n'amène pas à un segfault.

**Question 4** : Il serait tentant au premier abord d'utiliser $man printf, or on se rend compte que cecei renvoie plutôt l'utilisation de printf, et non un aperçu de son code. Pour cela, on va passer par le manuel plus général avec $man man. Printf est vraisemblablement située dans une bibliothèque (section 3). Avec $man 3 printf (pour lire la page printf de la section 3 du manuel), on obtient le prototype cherché:

````
int printf(const char *format, ...);
````

**Question 5** : Le gestionnaire de signaux est la fonction signals_handlers(int signal_number) dont le code est reporté ci-dessous.


````
void signals_handler(int signal_number)
{
    printf("Signal catched.\n");
    ptmxclose(&ptmx);
    exit(EXIT_SUCCESS);
}
````
