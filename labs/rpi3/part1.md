**Question 1** :  Décriver de manière plus précise l'utilité ainsi que la syntaxe
                de chacun des 3 fichiers mentionnés ci-dessus.

*configs/embsys_defconfig* traite la configuration du kernel pour la compilation (notamment, il indique si oui ou non on "charge" certains modules).
*busybox.config* comme le nom l'indique, il sert à configurer busybox.
*users.table* cette table répertorie les utilisateurs, les groupes auxquels ils appartiennent, leur place dans l'arborescence.

**Question 2** : En considérant que la cible est une carte RaspberryPi3 avec un
                OS 32 bits, quel est le fichier de configuration Buildroot par
                défaut à utiliser?

Depuis la tarball décompressée, on descend dans le répertoire *configs* et on étudie les fichiers présents avec ls.

Le seul candidat pour une Raspberry Pi 32 bits semble être *raspberrypi3_defconfig*.

**Question 3** : Que contient le répertoire *package*?

Il contient un ensemble de librairies et paquets (tels que *openssh*) dont on veut pouvoir disposer sur la machine cible. Il contient également de traditionnels fichiers de configurations ainsi qu'un Makefile.

En ouvrant différents paquets, on constate que la plupart contiennent:
 * des patches (*.patch*) à appliquer avant la compilation finale
 * des fichiers de configuration
 * des Makefile
 * occasionnellement un fichier de hashage à comparer avec le résultat attendu après compilation et installation (répertoires *httping* et *openssl* par exemple).

**Question 4** : Décrivez l'utilité des différents fichiers du répertoire
                *package/openssh*.

Le répertoire openssh contient les fichiers suivants:

````
-rw-rw-r-- 1 root root 1311 Sep  1  2017 0001-configure-ac-detect-mips-abi.patch
-rw-rw-r-- 1 root root 2204 Sep  1  2017 0002-configure-ac-properly-set-seccomp-audit-arch-for-mips64.patch
-rw-rw-r-- 1 root root 1383 Sep  1  2017 0003-fix-pam-uclibc-pthreads-clash.patch
-rw-rw-r-- 1 root root 1646 Sep  1  2017 0004-fix-howmany-include.patch
-rw-rw-r-- 1 root root  289 Sep  1  2017 Config.in
-rw-rw-r-- 1 root root  532 Sep  1  2017 S50sshd
-rw-rw-r-- 1 root root  157 Sep  1  2017 openssh.hash
-rw-rw-r-- 1 root root 2306 Sep  1  2017 openssh.mk
-rw-rw-r-- 1 root root  242 Sep  1  2017 sshd.service

````

* Les quatre premiers fichiers sont des patches, qui sont traités avant l'étape de compilation;
* Le fichier *Config.in* (configuration) est spécifique à l'utilisation de Buildroot;
* Le fichier S50sshd est un script d'initialisation pour le protocole do communication SSH;
* *openssh.mk* est un Makefile (regroupe les règles de compilation);
* *sshd.service* est utilisé pour utiliser le dossier d'init sans justement passer par la commande init.d;



**Question 5** : À quoi servent les fichiers du répertoire
                *boards/raspberrypi3*?

````
-rw-rw-r-- 1 root root  545 Sep  1  2017 genimage-raspberrypi.cfg
-rw-rw-r-- 1 root root  519 Sep  1  2017 genimage-raspberrypi0.cfg
-rw-rw-r-- 1 root root  487 Sep  1  2017 genimage-raspberrypi2.cfg
-rw-rw-r-- 1 root root  515 Sep  1  2017 genimage-raspberrypi3-64.cfg
-rw-rw-r-- 1 root root  547 Sep  1  2017 genimage-raspberrypi3.cfg
-rwxrwxr-x 1 root root  262 Sep  1  2017 post-build.sh
-rwxrwxr-x 1 root root 1324 Sep  1  2017 post-image.sh
-rw-rw-r-- 1 root root 2280 Sep  1  2017 readme.txt
````

On constate la présence de différents fichiers de configuration pour différents modèles de cartes.


**Question 6** : À quoi sert la commande `make embsys_defconfig`?

Elle indique qu'on envoie le signe de compiler selon la configuration par défaut d'embsys.


**Question 7** : En naviguant dans le menu, repérez:

* architecture matérielle cible : ARM (little endian)
* CPU ciblé : cortex-A53
* ABI : EABIhf (Embedded Application Binary Interface)
* librairie C utilisée : uClicb-ng (depuis onglet Toolchain)
* version du cross-compilateur : Buildroot 2017.08
* version du kernel : custom repository version 9126e25b0934bd7bd843763310ea4b34c6e139d0

**Question 8** : En recherchant dans l'interface de Buildroot, déterminez si le
                paquet *openssh* sera compilé et disponible dans l'OS cible. De
                même, retrouver cette information en analysant le fichier de
                configuration *embsys_defconfig*.

On constate dans l'interface de Buildroot (section Target packages/Networking applications) que le paquet *openssh* sera effectivement compilé (il est coché). Cela est confirmé dans le fichier *embsys_defconfig* à la ligne 2283, `BR2_PACKAGE_OPENSSH=y` (yes).


**Question 9** : Qu'est ce que busybox? À quoi sert la commande
                *make busybox-menuconfig*? Qu'obtiens t'on et que pouvons
                nous faire?

Busybox va nous permettre de gérer les librairies à importer sur le Root File System (RFS). La commande susdite nous permet d'obtenir un menu similaire à celui de la question précédente afin de consulter et changer la configuration de Busybox.

**Question 10**: Que contient le répertoire *output/host*? À quoi correspond
                 le binaire *output/host/usr/bin/arm-linux-gcc*?

Ce répertoire semble avoir l'architecture d'un répertoire root. Le binaire susdit a été créé compilation du kernel (c'est la combinaison du kernel et du RFS). Les répertoires doc, lib, man... assurent les fonctions courantes de tels répertoires (documentation, guide d'utilisation, etc).

**Question 11**: Décrire le résultat de la commande *file*. Que se passe t-il
                 si vous exécutez la commande *./hw*?

On obtient le retour suivant:

````
hw: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=da38aca55ac6c7a13a83bec5b9e9fb25035dd6cc, not stripped
````

*file* renvoie des informations sur le binaire hw obtenu: qu'il est en 64-bits, à quelle version du noyau il correspond, les informations de vérification du sha1...

*./hw* exécute le programme, qui affiche un Hello World dans la console.

**Question 12**: Utilisez la commande *file* sur le binaire résultant.
                 Quelle différences constatez vous par rapport au cas précédent
                 (binaire généré avec gcc)? Que se passe t-il si vous essayez
                 d'exécuter la commande *./hw*? Expliquez pourquoi.

On obtient le retour suivant:

````
hw: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-uClibc.so.0, not stripped
````

Ce retour est différent du précédent car il n'a pas été compilé de la même manière. Là où on utilisait un gcc "classique", on utilise maintenant la cross compilation du gcc des processeurs ARM.

On obtient de fait un binaire 32 bits, sans information de sha1.


**Question 13**: Que contient le répertoire *output/images*? Décrivez notamment
                 l'utilité des fichiers *rootfs.tar*, *zImage* et *sdcard.img*.

 On obtient la liste suivante:

````
total 573308
-rw-r--r-- 1 root root     17624 Nov 25 13:03 bcm2710-rpi-3-b.dtb
-rw-r--r-- 1 root root     16380 Nov 25 13:03 bcm2710-rpi-cm3.dtb
-rw-r--r-- 1 root root  33554432 Nov 25 13:04 boot.vfat
-rw-r--r-- 1 root root 209715200 Nov 25 13:04 rootfs.ext2
lrwxrwxrwx 1 root root        11 Nov 25 13:04 rootfs.ext4 -> rootfs.ext2
-rw-r--r-- 1 root root  95907840 Nov 25 13:04 rootfs.tar
drwxr-xr-x 3 root root      4096 Nov 25 12:45 rpi-firmware
-rw-r--r-- 1 root root 243270144 Nov 25 13:04 sdcard.img
-rw-r--r-- 1 root root   4566832 Nov 25 13:03 zImage
````

 * *rootfs.tar* est, comme son nom l'indique, le Root File System (compressé) qu'on souhaite avoir.
 * *sdcard.img* est l'image du noyau qu'on veut donner au processeur ARM, qui sera bootée par *zImage*.

**Question 14**: Que vous dis les résultats de la commande *file* lorsque vous
                 l'utilisez sur les fichiers *zImage* et *sdcard.img*?

````
root@07389f11f21c:~/buildroot-precompiled-2017.08/output/images# file zImage
zImage: Linux kernel ARM boot executable zImage (little-endian)
root@07389f11f21c:~/buildroot-precompiled-2017.08/output/images# file sdcard.img
sdcard.img: DOS/MBR boot sector; partition 1 : ID=0xc, active, start-CHS (0x0,0,2), end-CHS (0x4,20,17), startsector 1, 65536 sectors; partition 2 : ID=0x83, start-CHS (0x4,20,18), end-CHS (0x1d,146,54), startsector 65537, 409600 sectors
````

Ces informations viennent confirmer que *zImage* sert à booter, et fournissent des informations complémentaires sur les deux fichiers.


**Question 15**: Que contient le répertoire */tmp/rootfs*?

La commande `tar -xf output/images/rootfs.tar -C /tmp/rootfs` a décompressé la tarball rootfs (root file system) dans le répertoire *tmp/rootfs* nouvellement créé. On constate qu'il s'agit bien d'une architecture de répertoire root, de manière similaire à la question 10.


**Question complémentaire (demandée en cours)**: Définir ce que sont les outils d'automatisation avec nos propres mots.

Comme leur nom l'indique, les outils d'automatisation servent à automatiser un processus, en l'occurrence la création (construction et compilation) d'un système Linux qu'on veut déployer sur de l'embarqué. Buildroot par exemple est constitué d'un ensemble de Makefile et de patches. Ces outils permettent également de "personnaliser" ce Linux embarqué, via
