***REPONSES buildroot.md***


**Question 1**: 3 fichiers présents
*configs/embsys_defconfig* : Fichier de configuration du kernel. Il permet de sélectionner quels modules du kernel doivent être compilés, ce qui permet de sélectionner les paquets qui composeront notre kernel final sur la machine cible. Ce fichier dépend de plusieurs paramètres tels que l'architecture du processeur de la machine cible, le chaîne de compilation à utiliser...
*busybox.config* : Fichier de configuration de busybox. Il permet de sélectionner les librairies que nous souhaitons installer sur la machine cible.
*user.table* : Fichier répertoriant les utilisateurs par défaut du système d'exploitation.

**Question 2**:
Dans le dossier *~/buildroot-precompiled-2017.08/configs*, il y a de nombreuses configurations buildroot disponibles dont *raspberrypi3_defconfig* qui semble utilisable pour une raspberry 32 bits.

**Question 3**:
le dossier *~/buildroot-precompiled-2017.08/package* contient les librairies, paquets et drivers que l'on souhaite installer sur la machine cible.
Le dossier *~/buildroot-precompiled-2017.08/package/openssh* contient les fichiers nécessaires pour gérer les connections SSH (Secure SHell).
Les *.patch* sont des patchs qui s'ont appliqués avant la compilation par le config.in.
*Config.in* est un fichier de configuration pour buildroot.
le fichier *.hash* contient le résultat par un utilitaire de vérification comme sha32 et permet de vérifier la bonne installation du paquet.
Le fichier *.mk* est un makefile.
le fichier *.service* est un service.
L'exécutable *S50sshd* est en charge de la mise en place de la communication SSH.

**Question 4**:
La commande make permet de lancer la commande makefile "embsys_defconfig" qui précompile les dépendances nécessaires à la compilation du noyau.

**Question 5**:
Architecture matérielle ciblée : ARM, endianess : little-endian
CPU ciblé : cortex-A53
L'ABI : EABIhf (Embedded Application Binary Interface hard-float)

Librairie C utilisée : microClibc-ng
Version du cross-compilateur : gcc 6.x
Version du kernel : sur https://github.com/raspberrypi/linux.git, kernel linux 4.14.

**Question 6**:
Après une recherche de la chaîne de caractères "openssh" sur l'interface de buildroot :

````
Symbol: BR2_PACKAGE_OPENSSH [=y]
Type  : boolean                                                         Prompt: openssh                                                          Location:                                                               -> Target packages                                                 
(1)   -> Networking applications                                        Defined at package/openssh/Config.in:1                                Depends on: BR2_USE_MMU [=y]
Selects: BR2_PACKAGE_OPENSSL [=y] && BR2_PACKAGE_ZLIB [=y]        Selected by: BR2_PACKAGE_SSHFS [=n] && BR2_USE_WCHAR [=y] && BR2_TOOL
````

La ligne *BR2_PACKAGE_OPENSSH [=y]* indique qu'en effet le paquet ssh sera compilé dans l'OS cible.
Dans le fichier *configs/embsys_defconfig*, on retrouve à la ligne 2283 la même consigne :
````
BR2_PACKAGE_OPENSSH=y
````

**Question 7**:
Busybox est un utilitaire regroupant un grand nombre de librairies et paquets. Il est doté d'une interface graphique permettant de rapidement sélectionner les paquets que nous souhaitons installer sur la machine cible. La commande make busybox-menuconfig permet de faire les liens logiques entre le kernel de la machine cible et les librairies que nous avons sélectionné.

**Question 8**:
````
~/buildroot-precompiled-2017.08/output/host# ls
arm-buildroot-linux-uclibcgnueabihf  doc  include  libexec  sbin   usr
bin				     etc  lib	   man	    share
````
Ce dossier semble correspondre à un dossier root. On y trouve :
Le dossier *arm-buildroot-linux-uclibcgnueabihf* contient le RFS et le kernel compilé.
Le dossier doc contient de la documentation et les copyright sur les différents composants installés.
Le dossier lib contient les librairies notamment C.
Le dossier lib_exec contient les librairies nécessaires à la compilation par gcc.
Le dossier sbin contient des utilitaires comme fsck.
Le dossier bin possède des exécutables tels que des utilitaires en ligne de commande.
Le dossier man contient le manuel d'utilisation des utilitaires.

Le dossier usr contient les espaces utilisateurs qui eux-même possèdent une arborescence semblable au dossier root.

Le binaire output/host/usr/bin/arm-linux-gcc correspond au binaire du compilateur gcc pour processeurs ARM.

**Question 9**:
````
root@5b113897e1b6:~/buildroot-precompiled-2017.08/output/host# file hw
hw: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=aeb0a450fe32efb19ded5a57c8e9129c2d6b283a, not stripped
````
La commande file permet d'avoir des informations sur un binaire. Ici, on apprend qu'il s'agit d'un paquet 64 bits partagé pour noyau GNU/Linux 2.6.32. Elle nous fournit aussi la checksum sha1 du binaire afin de vérifier sa conformité.

Lancer la commande ./hw lancera l'execution du programme hw qui affichera "Hello World!" dans le terminal.

**Question 10**:
````
root@5b113897e1b6:~/buildroot-precompiled-2017.08/output/host# ./usr/bin/arm-linux-gcc helloworld.c -o hw
root@5b113897e1b6:~/buildroot-precompiled-2017.08/output/host# file hw
hw: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-uClibc.so.0, not stripped
````
Cette fois-ci il s'agit d'un executable pour processeurs ARM 32-bits utilisant la librairie C uClibc. La commande file ne fournit cependant plus la clé sha1. Cette fois-ci, l'executable a été compilé par cross-compilation avec la version de gcc adaptée aux processeurs ARM comme sur la raspberry3.

**Question 11**:
````
root@5b113897e1b6:~/buildroot-precompiled-2017.08/output/images# ls -lk
total 573312
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
la tarball rootfs.tar est le root file system compressé tel qu'il sera installé sur la machine cible. Le fichier sdcard.img est l'image du noyau GNU/Linux pour ARM. Le fichier zImage est le boot de la machine cible à installer.


**Question 12**:
````
root@5b113897e1b6:~/buildroot-precompiled-2017.08/output/images# file zImage
zImage: Linux kernel ARM boot executable zImage (little-endian)
root@5b113897e1b6:~/buildroot-precompiled-2017.08/output/images# file sdcard.img
sdcard.img: DOS/MBR boot sector; partition 1 : ID=0xc, active, start-CHS (0x0,0,2), end-CHS (0x4,20,17), startsector 1, 65536 sectors; partition 2 : ID=0x83, start-CHS (0x4,20,18), end-CHS (0x1d,146,54), startsector 65537, 409600 sectors
````

**Question 13**:
Les commandes fournies décompressent la tarball contenant le root file system. On obtient donc le root file system décompressé.

***REPONSES QEMU.md***

**Question 2**: À quoi sert la commande *chroot*?

chroot - run command or interactive shell with special root directory
Dans notre cas, on exécute le binaire bin/busybox sous l'identifiant "ash".

**Question 3**: Que se passe-t-il? Pourquoi?
````
/ # ./hw
Hello Worlds!
````
L'exécution du programme helloworld fonctionne sur le simulateur de raspberry3 car il a été compilé à l'aide du compilateur généré par cross-compilation.

***REPONSES flash_uboot.md***

**Question 1**: Une fois le flashage terminé, combien y-a t-il de partition
                sur la carte SD? Que contiennent-elles?
La carte SD contient deux partitions, l'une contenant le RFS et le noyau. La seconde partition contient le boot.

**Question 2**: Lire la
                [datasheet](https://components101.com/microcontrollers/raspberry-pi-3-pinout-features-datasheet)
                de la RPI3. Quels sont les ports TX/RX?

Les ports série 0 sont les ports GPIO14 et GPIO15. Il faut alors connecter ces deux ports ainsi qu'un port d'alimentation et une masse.

Ensuite, branchez l'adaptateur USB-TTL sur les ports TX/RX et ouvrez un
terminal série (gtkterm, minicom, ...). Finalement, connectez vous au réseau
avec un cable Ethernet, insérez la carte SD et démarrez la RPI3.

**Question 3**: Quelle est la configuration du port série permettant une
               communication avec la RPI3 (baud, etc)?

Les paramètres sont :
- baudrate 9600
- 1 bit de stop
- Pas de bit de parité

Puis, connectez vous en tant que *user* sur la RPI3 (lire le fichier
*users.tables* pour déterminer le mot de passe).

**Question 4**: Déterminez l'adresse IP de votre RPI3. Quelle commande
                avez-vous utilisé?

**Question 5**: Testez la connection ssh en tant que *user* et *root*. Quelle
                différence observez-vous? Pourquoi? Où est-ce configuré?






**EOF**
