# Shell 00 — réponses

Fichier de référence (hors dépôt git pour ne pas polluer le rendu).
Chaque bloc « Réponse » contient uniquement ce qui est demandé.

---

## ex00 — Z

### Réponse

```bash
echo Z > z
```

### Explication

- `echo Z` : écrit `Z` suivi d'un retour à la ligne.
- `>` : redirige la sortie standard vers le fichier `z` (le crée, ou l'écrase s'il existe).

Vérification : `cat z` affiche `Z`.

---

## ex01 — testShell00.tar

### Réponse

```bash
printf '%*s' 40 '' | tr ' ' 'A' > testShell00
chmod 455 testShell00
tar -cf testShell00.tar testShell00
```

### Explication

- `printf '%*s' 40 ''` : affiche une chaîne de 40 espaces (`%*s` prend la largeur 40 en argument).
- `|` : envoie la sortie de `printf` vers l'entrée de `tr`.
- `tr ' ' 'A'` : remplace chaque espace par un `A` → 40 octets.
- `> testShell00` : écrit le résultat dans le fichier.
- `chmod 455` : applique les droits `r--r-xr-x` (voir tableau plus bas).
- `tar -cf` : `-c` crée une archive, `-f` indique le nom du fichier d'archive.

---

## ex02 — exo2.tar

### Réponse

```bash
mkdir test0 test2
printf 'AAAA' > test1
printf 'B' > test3
printf 'CC' > test4
ln test3 test5
ln -s test0 test6
chmod 715 test0
chmod 714 test1
chmod 504 test2
chmod 404 test3
chmod 641 test4
tar -cf exo2.tar *
```

### Explication

- `mkdir` : crée les dossiers `test0` et `test2`.
- `printf 'AAAA' > test1` : crée `test1` avec exactement 4 octets (idem 1 octet pour `test3`, 2 pour `test4`).
- `ln test3 test5` : crée un **lien physique**. `test3` et `test5` partagent le même inode → compteur de liens `2`.
- `ln -s test0 test6` : crée un **lien symbolique** `test6 -> test0` (taille affichée = 5, la longueur de `test0`).
- `chmod` : droits en octal.
- `tar -cf exo2.tar *` : archive tout le contenu du dossier (`*` exclut les fichiers cachés et est développé avant la création de l'archive, donc `exo2.tar` ne s'archive pas lui-même).

Correspondance octal → `ls -l` :

| Octal | Droits      |
|-------|-------------|
| 715   | `rwx--xr-x` |
| 714   | `rwx--xr--` |
| 504   | `r-x---r--` |
| 404   | `r-----r--` |
| 641   | `rw-r----x` |

---

## ex03 — id_rsa_pub

### Réponse

```bash
ssh-keygen -t ed25519 -f ~/.ssh/id_ed25519
cp ~/.ssh/id_ed25519.pub ex03/id_rsa_pub
```

### Explication

- `ssh-keygen` : génère une paire de clés.
- `-t ed25519` : type de clé (on peut aussi utiliser `-t rsa -b 4096`).
- `-f` : chemin du fichier de clé (la clé privée reste dans `~/.ssh/`, jamais dans le rendu).
- `cp` : copie la clé **publique** sous le nom imposé `id_rsa_pub`.

La clé publique doit aussi être ajoutée sur l'intranet (réglages SSH) pour autoriser le `git push`.

---

## ex04 — midLS

### Réponse

Contenu exact du fichier `midLS` (une seule ligne, rien d'autre) :

```bash
ls -mpt
```

### Explication

- `ls` : liste le contenu du dossier courant.
- `-m` : sortie sur une seule ligne, entrées séparées par `, ` (virgule + espace).
- `-p` : ajoute un `/` à la fin des noms de dossiers.
- `-t` : trie par date de modification, du plus récent au plus ancien.

Sans `-a`, les fichiers cachés, `.` et `..` sont déjà exclus.

> Le fichier ne doit contenir **que** la commande : pas de `#!/bin/bash`.

---

## ex05 — git_commit.sh

### Réponse

Contenu du fichier `ex05/git_commit.sh` :

```bash
#!/bin/sh
git log --format=%H -n 5
```

### Explication

- `#!/bin/sh` : interpréteur (le sujet impose `/bin/sh`).
- `git log` : affiche l'historique des commits.
- `--format=%H` : n'affiche que l'identifiant complet (SHA-1 de 40 caractères) de chaque commit.
- `-n 5` : limite aux 5 derniers commits.

Rendre le script exécutable : `chmod +x ex05/git_commit.sh`.

---

## ex06 — git_ignore.sh

### Réponse

Contenu du fichier `ex06/git_ignore.sh` :

```bash
#!/bin/sh
git ls-files --others --ignored --exclude-standard
```

### Explication

- `git ls-files` : liste les fichiers connus de Git.
- `--others` : inclut les fichiers non suivis (untracked).
- `--ignored` : ne garde que ceux ignorés.
- `--exclude-standard` : applique les règles d'exclusion standards (`.gitignore`, `.git/info/exclude`, exclusion globale).

---

## ex07 — diff

### Réponse

Le fichier à rendre est `b`. Son contenu exact (538 octets) :

```text
Episode V, A NEW H0PE It is a period of civil war
Rebel spaceships, striking from a hidden base, have won their first victory against the evil Galactic Empire. 
During the battle, Rebel spies managed to steal secret plans to the Empire's ultimate weapon, the STAR DEATH, an armored space station with enough power to destroy an entire planet.


Pursued by the Empire's sinister agents,
Princess Mehdi races home aboard her starship, custodian of the stolen plans that can save her people and restore the dictatorship to the galaxie..




```

Points invisibles importants :

- la 2e ligne se termine par **un espace** (après `Empire.`) ;
- il y a **2 lignes vides** après la ligne `... entire planet.` ;
- il y a **4 lignes vides** à la fin du fichier.

Avec le fichier `a` du sujet, `diff a b` produit exactement le `sw.diff` attendu :

```text
1,2c1,8
< STARWARS
< Episode IV, A NEW HOPE It is a period of civil war.
---
> Episode V, A NEW H0PE It is a period of civil war
> Rebel spaceships, striking from a hidden base, have won their first victory against the evil Galactic Empire. 
> During the battle, Rebel spies managed to steal secret plans to the Empire's ultimate weapon, the STAR DEATH, an armored space station with enough power to destroy an entire planet.
> 
> 
> Pursued by the Empire's sinister agents,
> Princess Mehdi races home aboard her starship, custodian of the stolen plans that can save her people and restore the dictatorship to the galaxie..
> 
4,6d9
< Rebel spaceships, striking from a hidden base, have won their first victory against the evil Galactic Empire.
< During the battle, Rebel spies managed to steal secret plans to the Empire's ultimate weapon, the DEATH STAR,
< an armored space station with enough power to destroy an entire planet.
8d10
< Pursued by the Empire's sinister agents, Princess Leia races home aboard her starship, custodian of the stolen plans that can save her people and restore freedom to the galaxy...
```

### Explication

- `diff a b` : compare ligne par ligne les fichiers `a` et `b`.
- `> sw.diff` : écrit le résultat dans `sw.diff`.
- `c` = changement, `d` = suppression, `a` = ajout ; `<` = ligne de `a`, `>` = ligne de `b`.
- `patch` applique un `sw.diff` à un fichier (`patch fichier < sw.diff`).

Test local : `diff a b > sw.diff` doit donner un `sw.diff` identique à celui utilisé par le correcteur.

---

## ex08 — clean

### Réponse

Contenu exact du fichier `clean` (une seule ligne) :

```bash
find . -type f \( -name '*~' -o -name '#*#' \) -print -delete
```

### Explication

- `find .` : cherche récursivement depuis le dossier courant.
- `-type f` : uniquement les fichiers (pas les dossiers).
- `\( ... \)` : groupe les conditions (nécessaire pour combiner avec `-o`).
- `-name '*~'` : nom terminé par `~`.
- `-o` : OU logique.
- `-name '#*#'` : nom commençant **et** finissant par `#`.
- `-print` : affiche le chemin des fichiers trouvés.
- `-delete` : supprime les fichiers trouvés.

Une seule commande, pas de `;` ni `&&`.

---

## ex09 — ft_magic

### Réponse

Contenu exact du fichier `ft_magic` :

```text
41	string	42	42 file
```

(les séparateurs sont des tabulations)

### Explication

- `41` : offset en octets, numéroté à partir de 0 → le 42e octet est à l'offset 41.
- `string` : type de donnée testée (chaîne de caractères).
- `42` : valeur attendue à cet offset.
- `42 file` : description affichée par `file` quand la signature correspond.

Test :

```bash
printf '%41s42' '' > fichier42
file -m ft_magic fichier42
```

Résultat attendu : `fichier42: 42 file`.
