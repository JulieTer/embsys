# Partie 1: Utilisation d'un simulateur GPS

## Norme NMEA

*La norme NMEA 0183 est une spécification pour la communication entre
équipements marins, dont les équipements GPS. Elle est définie et contrôlée
par la National Marine Electronics Association (NMEA), association américaine
de fabricants d'appareils électroniques maritimes, basée à Severna Park au
Maryland (États-Unis d'Amérique). La norme 0183 utilise une simple communication
série pour transmettre une phrase à un ou plusieurs écoutants. Une trame NMEA
utilise tous les caractères ASCII. (Wikipedia).*

Ces trames NMEA 0183 sont de tailles variables et codées en ASCII (caractères
8 bits) contrairement aux trames NMEA 2000. Elles commencent toutes par le
caractère *$* (excepté les trames venant de l'AIS commençant par *!*). Les deux
caractères suivants indiquent le type de matériel. Ainsi, on a par exemple pour
les trois premiers octets :

  * $GP : trame GPS
  * $HC : trame compas
  * $RA : trame radar
  * ...

Par exemple :

````
$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39
$GPGGA,123519,4807.038,N,01131.324,E,1,08,0.9,545.4,M,46.9,M, , *42
$GPGLL,4916.45,N,12311.12,W,225444,A
$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K
...
````

Aujourd'hui, nous allons travailler avec un simulateur GPS simplifié simulant
l'envoie de deux types de trames par liaison série virtuelle :

  * trame GLL : Geographic position, latitude / longitude
  * trame VTG : Track made good and ground speed

Ces trames sont envoyées periodiquement par le binaire *gps* que nous verrons
plus tard.

*NOTE* : Grâce aux équipements marins, une carte en ligne indique en temps réel
la position des navires : http://www.marinetraffic.com/fr/.

## Communication série

Dans la vraie vie, une communication série (ou USB) transmet ces trames NMEA
et un ordinateur peut alors les récupérer.

Lors de la connexion d'un port USB-Série, une entrée est créée par le kernel
dans le répertoire */dev/* (généralement par l'utilitaire *udev*). En examinant
les sorties du kernel, on peut trouver le port exact :

````
$ dmesg
...
[15976.212024] usb 2-2: new full-speed USB device number 2 using uhci_hcd
[15976.375039] usb 2-2: New USB device found, idVendor=067b, idProduct=2303
[15976.375044] usb 2-2: New USB device strings: Mfr=1, Product=2, SerialNumber=0
[15976.375047] usb 2-2: Product: USB-Serial Controller
[15976.375049] usb 2-2: Manufacturer: Prolific Technology Inc.
[15978.090200] usbcore: registered new interface driver usbserial
[15978.090219] usbcore: registered new interface driver usbserial_generic
[15978.090234] usbserial: USB Serial support registered for generic
[15978.174057] usbcore: registered new interface driver pl2303
[15978.174078] usbserial: USB Serial support registered for pl2303
[15978.174101] pl2303 2-2:1.0: pl2303 converter detected
[15978.186178] usb 2-2: pl2303 converter now attached to ttyUSB0
````

Ici, on s'attend à avoir un port */dev/ttyUSB0*.

Dans le cadre du TP, le simulateur va lui aussi créer une entrée dans
le */dev/*. Cependant, cette entrée étant virtuelle et créée par programmation,
elle ne sera pas visible à travers les messages kernel.


## Compilation du simulateur

Le simulateur GPS se trouve dans le répertoire *gps/* qui contient lui
même :

  * src : répertoire contenant les sources
  * bin : répertoire contenant les binaires (après compilation)
  * lib : répertoire contenant les librairies (après compilation)
  * include : répertoire contenant les headers (après compilation)
  * Makefile : fichier définissant les règles de compilation
  * run.sh : fichier lançant le simulateur GPS

Lancez la compilation :

````
$ make
````

Deux librairies sont compilées *lib/libnmea.so* et *lib/libptmx.so* ainsi que
le binaire *bin/gps*.

## Questions

En vous inspirant du contenu du répertoire *gps/*, répondre aux questions
suivantes:

**Question 1**: Qu'est ce qu'un Makefile? À quoi sert make?

Un makefile est un fichier texte résumant les étapes que doit suivre le compilateur afin
de compiler correctement un fichier en C par exemple. make est l'utilitaire qui
interprète le contenu du makefile.

**Question 2**: Quel compilateur est utilisé ici?

Le compilateur utilisé est GCC (GNU Compiler Collection).

**Question 3**: Qu'est ce qu'une librairie partagée?

Une librairie partagée est une librairie utilisable par plusieurs fichiers C
depuis n'importe quel dossier. Ils sont généralement placés dans /lib et sont
appelés comme des fichier header. L'avantage de ces librairies est quelles ne sont
pas appelées par défaut. Elle ne prennent donc de l'espace mémoire que lorsqu'elles
sont appelées.

**Question 4**: Donnez un exemple de fichier C et la ligne de commande
                correspondante pour obtenir un binaire exécutable (un hello
                world par exemple).

Pour compiler un exécutable *executable* à partir d'un fichier *fichier.c*, j'utilise
d'une manière courante la commande suivante :

````
gcc fichier.c -o [-Wall] executable
````

**Question 5**: Donnez un exemple de fichier C et les lignes de commandes
                correspondantes pour obtenir une librairie partagée.

Après une recherche, je suis tombé sur les lignes de code suivantes pour créer
une librairie partagée libdobj.so à partir d'un fichier obj.c définissant une
structure OBJDATA :

````
gcc -fPIC -c obj.c
gcc -shared -W1,-soname,libdobj.so.1 -o libdobj.so.1.0 obj.o
ln -s libdobj.so.1.0 libdobj.so.1
ln -s libdobj.so.1 libdobj.so
export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH
````

Le fichier C obj.c correspondant :

````
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct OBJDATA
{
char *name;
int version;
} OBJDATA;

void *init(char *name)
{
OBJDATA *data=(OBJDATA*)calloc(1,sizeof(OBJDATA));
if(name)
data->name=malloc(strlen(name)+1);
strcpy(data->name, name);
printf("Cree : %s\n", name);
return data;
}

void montre(void *data)
{
OBJDATA *d=(OBJDATA*)data;
printf("Montre : %s\n", d->name);
}

void detruit(void *data)
{
OBJDATA *d=(OBJDATA*)data;
if(d)
  {
if(d->name)
  {printf("Destruction : %s\n", d->name);
free(d->name);
}
free(d);
}
}
````

source : http://www.linux-france.org/article/memo/node113.html

En recherchant dans le fichier Makefile situé en ~/embsys/labs/sysprog/gps/src/lib/nmea, on peut voir un exemple plus simple d'implémentation d'une librairie *libnmea.so* à partir d'un fichier C *nmea.c*.
````
gcc -g -c -fPIC nmea.c -o nmea.o
gcc -g -shared -Wl,-soname,libnmea.so -o libnmea.so nmea.o -lm
export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH
````
A noter que dans les deux cas, la dernière ligne du Makefile permet de mettre à jour la variable d'environnement LD_LIBRARY_PATH qui répertorie les dossiers contenant les librairies à utiliser pour la compilation.


## À retenir

  * make et Makefile
  * compilation d'un binaire exécutable
  * compilation d'une librairie partagée
