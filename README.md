# Défi du combattant

Répertoire pour le défi du combattant

## Table des matières

- [Défi du combattant](#défi-du-combattant)
  - [Table des matières](#table-des-matières)
  - [Description du projet](#description-du-projet)
  - [Instructions pour commencer le projet sur votre ordinateur local.](#instructions-pour-commencer-le-projet-sur-votre-ordinateur-local)
  - [Usage](#usage)
  - [Contribuer](#contribuer)
    - [1. Branche](#1-branche)
      - [Préfixes](#préfixes)
    - [2. Commit](#2-commit)
    - [3. Push](#3-push)
    - [Guide GIT](#guide-git)

---
## Description du projet

Répertoire pour le défi du combattant, codé en C++.


## Instructions pour commencer le projet sur votre ordinateur local.

```bash
git clone git@github.com:Zupanico/defiCombattant.git
```

**OU**

Cloner avec GitKraken, GitHub Desktop ou un autre client Git de votre choix.


## Usage

Pour importer des librairies externes, il faut les mettre dans le dossier `lib` et les inclure dans le fichier `CMakeLists.txt`.
Ensuite, il faut les inclure dans le fichier `.cpp` où elles sont utilisées.

```c++
#include nom-de-la-librairie.h
```

## Contribuer

 ### 1. Branche
   Pour contribuer, il faut créer une nouvelle branche à partir de `master` et faire une pull request vers `master` lorsque le travail est terminé. Il est **TRÈS** important de ne pas travailler directement sur la branche `master`, pour ne pas briser le code source.
  ```bash
    git checkout -b prefix:nom-de-la-branche
   ```

  #### Préfixes
  Les préfixes sont les suivants:
  `feat:` pour une nouvelle fonctionnalité
  `fix:` pour une correction de bug
  `BREAKING CHANGE:` pour un changement qui n'est pas rétrocompatible et qui  nécessite une mise à jour de la version majeure
  `refactor:` pour un changement de code qui n'ajoute pas de fonctionnalité ou qui  ne corrige pas de bug
  `style:` pour un changement qui n'affecte pas le code (espace, formatage, etc.)
  `docs:` pour un changement qui affecte la documentation
  `test:` pour l'ajout de tests
  `chore:` pour un changement qui n'affecte pas le code (mise à jour des outils,  etc.)
  `misc:` pour tout autre changement

  <br/>

  ### 2. Commit
  Faire un commit avec un message clair et concis.
  Utiliser le format de message suivant: `prefix: message clair et concis`. *Se référer à [préfixes](#préfixes).*

  Il faut enregistrer les changements avec la commande suivante:
  ```bash	
    git add .
  ```
  *Cette commande ajoute tout les fichiers modifiés.*

  Puis les commettre avec la commande suivante:

  ```bash
    git commit -m "prefix: message clair et concis"
  ```

  Il est important de faire des petits commit avec des petits changement pour éviter des problèmes de fusion.

  ### 3. Push
  Une fois le ou les commits faits, il faut les pousser sur le répertoire distant.

  1. Pour éviter les erreurs et les conflits, il faut aller chercher les changements sur le répertoire distant
      ```bash
        git pull
      ```

  2. Si la branche n'existe pas sur le répertoire distant, il faut la créer.
      ```bash
        git push -u origin prefix:nom-de-la-branche
      ```

  3. Si la branche existe déjà sur le répertoire distant, il faut la mettre à jour.
      ```bash
        git push
      ```
<br/>

### Guide GIT
Se référer au [guide de GIT](https://github.com/Zupanico/guide-git) pour plus de détails.

---