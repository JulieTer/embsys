**Question 1** : À quoi sert l'option *--cap-add* lors du lancement d'un
                nouveau coneneur Docker?

*--cap-add* permet d'attribuer des capacités Linux au Docker, ici des capacités sysadmin. Cela peut être restreint, par exemple, à des capacités plus orientées réseau avec le paramètre NET_ADMIN.

**Question 2**: À quoi sert la commande *chroot*?

D'après `man chroot`:

`chroot - run command or interactive shell with special root directory`

On lance une commande ou un prompt avec le répertoire racine souhaité (ici *ash*).

**Question 3**: Que se passe-t-il? Pourquoi?

On obtient bien un affichage de "Hello Worlds!" (ce qui est le comportement attendu). En effet, le fichier *hw.c* a été compilé pour du ARM et notre QEMU "simule" un ARM.
