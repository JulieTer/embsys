#Execrice 1 : Multiplexage

**Question 1** : le champ indiqué par PTTY (/dev/pts/1) se comporte comme un "port": c'est là où on va envoyer les informations générées par le programme, et qu'on va lire pour récupérer les trames GPS.


**Question 2** : On ne peut fermer proprement l'application car on n'a pas de gestionnaire de signaux.

De plus, dans l'état actuel, les "sous-parties" du code de *reader.c* ne sont pas réutilisables. Il serait préférable de les rassembler dans des fonctions externes qu'on appelerait depuis le main.

Par ailleurs il peut être de bonne pratique de rajouter un default case dans le switch, en cas d'erreur non gérée.

Enfin, un code commenté est toujours d'autant plus "maintenable".


**Question 3** : D'après https://docs.novatel.com/oem7/Content/Logs/GPGLL.htm il apparaît que le sixième champ des trames GPGLL contient "UTC time status of position (hours/minutes/seconds/decimal seconds)".

En lançant le reader on constate qu'effectivement ce champ s'incrémente de 4 à chaque nouvelle trame (qui apparait envirno toutes les 4 secondes).


**Question 4** : On applique du multiplexage.

Avec FD_ZERO(&fdset) (ligne 56), on crée un bout de pipe au format file descriptor (il doit pouvoir recevoir des file descriptors).C'est la phase d'ouverture.
Avec FD_SET(fd, &fdset), on met dans cette structure le file descriptor fd préalablement défini avec int fd = open(port, O_RDWR | O_NOCTTY).

Après quoi on attend un évènement sur les fd (select/condition if FD_ISSET(fd,&fdset)). C'est la phase d'écoute, suite à quoi on lit et stocke le message dans la variable bytes.

Pour finir, on ferme le file descriptor (close(fd)).
