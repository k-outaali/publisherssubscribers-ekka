# Spécification fonctionnelles : Publisher/Subscriber

## Exigence V1 : Un publisher et n subscribers, mode sampling

### V1-E1
#### Description
``open`` permet à un subscriber d'ouvrir la catégorie ``p_categorie`` avec l'option ``O_RDONLY`` spécifiée grâce au paramètre ``p_options``. Si la catégorie est ouverte normalement, ``open`` retourne le descripteur de fichier permettant au subscriber de recevoir les messages.  
#### Test
Vérifier que l'utilisateur est un subscriber.
Vérifier valeur de retour (file descriptor).
</br>

### V1-E2
#### Description
``open`` retourne -1 lors d'un appel pour une catégorie ``p_categorie`` lorsque les subscribers tentent d'ouvrir la catégorie plus de 4 fois, i.e. plus de 4 subscribers ont tenté d'ouvrir le canal pour la catégorie donnée
#### Test
Vérifier que l'utilisateur est un subscriber.
Ouvrir une catégorie plus de 4 fois et vérifier le résultat.
</br>


### V1-E3
#### Description
Pour ``open``, comme usuellement avec Unix, ``p_mode`` spécifie les permissions d'accès en fonction de l'identité de propriétaire du processus (``user``, ``group`` et ``other``). 
#### Test
Vérifier avec 4 permissions (``user``, ``group``, ``other`` et une autre)
</br>


### V1-E4
#### Description
``open`` retourne -1 lors d'un appel pour un fichier qui n'existe pas.
#### Test
Valeur de retour pour un fichier qui n'existe pas (-1).
</br>


### V1-E5
#### Description
``open`` retourne -1 lors d'un appel avec une option différente des valeurs ``O_RDONLY`` et ``O_WRONLY``
#### Test
Valeur de retour pour un parametre différent de ``O_RDONLY`` ou ``O_WRONLY`` (-1).
</br>


### V1-E6
#### Description
Tout appel à ``ioctl`` retourne -1
#### Test
Vérifier valeur de retour (-1).
</br>


### V1-E7
#### Description
``close`` permet de terminer les communications pour le descripteur ``p_fd``. ``close`` retourne 0 si la fermeture s'est déroulée correctement.
#### Test
Vérifier valeur de retour pour une bonne fermeture (0).
</br>


### V1-E8
#### Description
``close`` retourne -1 lors d'un appel si le paramètre ``p_fd`` ne correspond à aucun fichier ouvert.
#### Test
Vérifier valeur de retour pour un fichier non ouvert (-1).
</br>


### V1-E9
#### Description
``write`` permet au publisher d'écrire la chaine de caractères ``p_message`` dans le canal.  ``p_size`` doit être égal à la taille du message. Si le message a pu être écrit normalement, ``write`` retourne la taille du message finalement écrit (hors taille de la catégorie).
#### Test
Vérifier valeur de retour pour différente taille de message.
</br>


### V1-E10
#### Description
``write`` retourne -1 lors d'un appel si ``p_size`` est strictement inférieur à 1, i.e. un message doit avoir une taille minimale de 1 caractère \0 compris.
#### Test
Vérifier valeur de retour pour `p_size` inférieur à 1 (-1).
</br>


### V1-E11
#### Description
``write`` retourne -1 lors d'un appel si le paramètre ``p_fd`` ne désigne pas un fichier ouvert.
#### Test
Vérifier valeur de retour pour un fichier pas ouvert.
</br>


### V1-E12
#### Description
``read`` permet aux subscribers de lire la chaine de caractères ``p_message`` dans le canal. ``p_size`` doit être égal à la taille du message. Si le message a pu être lu normalement, ``read`` retourne la taille du message finalement lu (hors taille de la catégorie). 
#### Test
Vérifier si `p_size` égale la taille du message.
Vérifier la valeur de retour.
</br>


### V1-E13
#### Description
``read`` retourne -1 lors d'un appel si ``p_size`` est inférieur à 1, i.e. un message doit avoir une taille minimale de 1 caractère.
#### Test
Vérifier valeur de retour pour un `p_size` inférieur à 1.
</br>


### V1-E14
#### Description
``read`` retourne -1 lors d'un appel si le paramètre ``p_fd`` ne désigne pas un fichier ouvert.
#### Test
Vérifier valeur de retour pour un fichier non existant (-1).
</br>


### V1-E15
#### Description
``read`` retourne -1 lors d'un appel si ``p_size`` est différent du nombre de caractère effectivement lu. 
#### Test
Vérifier que le nombre de caractère lu est égale à `p_size`
</br>


### V1-E16
#### Description
Les messages ont une taille variable mais strictement inférieure à 1024 octets. Le paramètre ``p_size`` de ``read`` et ``write`` ne peut être supérieur à la taille maximale de message. 
#### Test
Vérifier que le message est bien inférieur à 1024.
</br>


### V1-E17
#### Description
Un subscriber (resp. publisher) peut lire (resp. écrire) différentes catégories.  
#### Test
Vérifier qu'un subscriber ne peut pas écrire.
Vérifier qu'un subscriber peut lire.
Vérifier qu'un publisher peut écrire.
Vérifier qu'un publisher ne peut pas lire.
</br>


### V1-E18
#### Description
Lors de l'appel à la fonction ``read``, on obtient le nombre d'octets demandé dans la limite de la taille effective du message. ``read`` retourne le nombre de caractères effectivement lu. 
#### Test
Vérifier la valeur de retour de `read` (égale au nombre de caractère effectivement lu).
</br>


### V1-E19
#### Description
Pour toute erreur soulevée lors de l'appel d'une des fonctions, la variable globale ``errno`` doit être positionnée avec l'erreur signalée. 
#### Test
Vérifier que errno est positionnée pour chaque erreur signalée.
</br>



## Exigence V2 : Plusieurs publisher et subscribers, mode sampling

### V2-E1
#### Description
La taille maximale des messages est par défaut de 1024 caractères.
#### Test
Test sur différentes taille.
</br>


### V2-E2
#### Description
Le nombre maximum de publishers a par défaut la valeur 10.
#### Test
Test dépasser 10 publishers.
</br>


### V2-E3
#### Description
Le nombre maximum de subscribers a par défaut la valeur 10.
#### Test
Test dépasser 10 subscribers.
</br>


### V2-E4
#### Description
Tout appel de la fonction ``ioctl`` après l’ouverture du fichier avec ``open`` retourne -1.
#### Test
Test valeur de retour `ioctl`.
</br>


### V2-E5
#### Description
Tout appel à la fonction ``ioctl`` avec une valeur <=0 pour l’argument ``p_options`` retourne -1.
#### Test
Test valeur de retour `ioctl` pour différent `p_options`.
</br>


### V2-E6
#### Description
Tout appel à la fonction ``ioctl`` avec une valeur différente de ``PS_MSGSIZE``, ``PS_NPUBLISHER`` et ``PS_NSUBSCRIBER`` pour l’argument ``p_request`` retourne -1.
#### Test
Test valeur de retour `ioctl` pour différent `p_request`.
</br>


### V2-E7
#### Description
``ioctl`` modifie la taille des identifiants lorsque la fonction est invoquée avec la valeur ``PS_MSGSIZE`` pour l’argument ``p_request``. La nouvelle taille maximale des messages est alors donnée par l’argument ``p_options``. En cas de succès, ``ioctl`` retourne 0. 
#### Test
Test valeur de retour `ioctl` pour `PS_MSGSIZE`.
Test ecriture et lecture pour la nouvelle taille.
</br>


### V2-E8
#### Description
``ioctl`` modifie la taille des identifiants lorsque la fonction est invoquée avec la valeur ``PS_ NPUBLISHER`` pour l’argument ``p_request``. Le nouveau nombre maximum de publishers est alors donné par l’argument ``p_options``. En cas de succès, ``ioctl`` retourne 0.
#### Test
Test valeur de retour `ioctl` pour `PS_ NPUBLISHER`.
Test dépasser le nouveau nombre max de publishers.
</br>


### V2-E9
#### Description
``ioctl`` modifie la taille des identifiants lorsque la fonction est invoquée avec la valeur ``PS_NSUBSCRIBER`` pour l’argument ``p_request``. Le nouveau nombre maximum de subscribers est alors donné par l’argument ``p_options``. En cas de succès, ``ioctl`` retourne 0.
#### Test
Test valeur de retour `ioctl` pour `PS_NSUBSCRIBER`.
Test dépasser le nouveau nombre max subscribers.
</br>


## Exigence V3 : mode FIFO

### V3-E1
#### Description
Les messages sont stockés dans la file d’attente associée à la catégorie jusqu’à sa lecture par tous les subscribers. En d’autres terme, un message stocké dans le canal ne peut être supprimé de sa file d’attente avant que tous les subscribers n’aient pu le lire.
#### Test
</br>


### V3-E2
#### Description
Lorsque la file d’attente associée à une catégorie comporte plusieurs messages, alors pour tous les subscribers l’ordre de lecture des messages est l’ordre d’arrivée des messages dans le canal. En d’autres termes, tous les subscribers reçoivent toutes les messages dans le même ordre qui est l’ordre d’arrivée dans le canal.
#### Test
</br>


## Test

### Fonction open
Vérifier que l'utilisateur est un subscriber.
Vérifier valeur de retour (file descriptor).
Ouvrir une catégorie plus de 4 fois et vérifier le résultat (-1).
Valeur de retour pour un fichier qui n'existe pas (-1).
Valeur de retour pour un parametre différent de ``O_RDONLY`` ou ``O_WRONLY`` (-1).
Vérifier avec 4 permissions (``user``, ``group``, ``other`` et une autre)

### Fonction write
Vérifier valeur de retour pour différente taille de message.
Vérifier valeur de retour pour `p_size` inférieur à 1 (-1).
Vérifier valeur de retour pour un fichier pas ouvert (-1).

### Fonction read
Vérifier si `p_size` égale la taille du message.
Vérifier la valeur de retour (taille msg lu).
Vérifier valeur de retour pour un `p_size` inférieur à 1 (-1).
Vérifier valeur de retour pour un fichier non existant (-1).
Vérifier valeur de retour si nombre de caractère lu différent de ``p_size`` (-1).
Vérifier la valeur de retour de `read` (égale au nombre de caractère effectivement lu, test sur pls taille de message).

### Fonction ioctl
Vérifier valeur de retour (-1).

### Fonction close
Vérifier valeur de retour pour une bonne fermeture (0).
Vérifier valeur de retour pour un fichier non ouvert (-1).

### Global
Vérifier que le message est bien inférieur à 1024. (pour read et write)
Vérifier qu'un subscriber ne peut pas écrire.
Vérifier qu'un subscriber peut lire.
Vérifier qu'un publisher peut écrire.
Vérifier qu'un publisher ne peut pas lire.
Vérifier que errno est positionnée pour chaque erreur signalée.
