**Question 1** : Un Makefile est un type de fichier regroupant l'ensemble des instructions à effectuer par le compilateur, afin entre autres de ne pas devoir répéter ces instructions "à la main" dans la console.

La commande make va chercher un fichier intitulé "Makefile" dans le dossier courant et exécute son contenu.


**Question 2** : Le compilateur utilisé est GCC, comme on peut le voir dans les Makefile des sous-dossiers (/gps/src/lib/ptmx par exemple).


**Question 3** : Les librairies partagées (fichiers .so pour "shared object") sont des bibliothèques qui ne sont chargées que lorsqu'elles sont appelées, ce qui permet de faire des économies en mémoire. On les situe en général dans les répertoires /lib ou /usr/lib.


**Question 4** : La ligne de commande la plus "basique" pour compiler un fichier donner en un binaire exécutable suit la syntaxe suivante:

````
gcc file.c -o exe
````

où file.c est un fichier C, et exe le nom de l'exécutable en sortie.

**Question 5** : L'article situé à l'adresse http://cherrymick.com/create-a-simple-shared-library-in-c.html propose la solution suivante.

On définit le fichier lib_random.c simulant un lancer de dés tout ce qu'il y a de plus honnête, au caractère aléatoire garanti (source: https://xkcd.com/221/):

````
int getRandomNumber(void)
{
  return 4;
}
````

On l'accompagne du fichier interface lib_random.h:

````
int getRandomNumber(void);
````

On exécute ensuite les commandes suivantes:

````
gcc -shared -fPIC -o lib_random.so lib_random.c
 export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH
````

La première commande sert à compiler le fichier C en une librairie partagée et la deuxième indique aux fichiers utilisant la librairie où aller chercher celle-ci.
