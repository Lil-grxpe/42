# Guide complet — Norminette (Norm v4.1)

> **Environnement de référence** : `norminette 3.3.60`, Python 3.14.4, Ubuntu 26.04 (WSL2)
> **Norme vérifiée** : Norm v4.1 (celle du PDF officiel livré avec l'outil)
> **Dépôt officiel** : <https://github.com/42School/norminette>

---

## Sommaire

1. [Qu'est-ce que la norminette ?](#1-quest-ce-que-la-norminette-)
2. [Installation](#2-installation)
3. [Utilisation : commandes et options](#3-utilisation--commandes-et-options)
4. [Lire la sortie et les codes de retour](#4-lire-la-sortie-et-les-codes-de-retour)
5. [La Norm v4.1 en résumé](#5-la-norm-v41-en-résumé)
6. [Catalogue des erreurs les plus fréquentes](#6-catalogue-des-erreurs-les-plus-fréquentes)
7. [Le header 42](#7-le-header-42)
8. [Automatiser : Makefile, hook git, CI, IDE](#8-automatiser--makefile-hook-git-ci-ide)
9. [Dépannage / FAQ](#9-dépannage--faq)
10. [Aide-mémoire](#10-aide-mémoire)

---

## 1. Qu'est-ce que la norminette ?

La **norminette** est l'outil officiel de 42 qui vérifie le respect de la **Norm** (style de code C).
C'est un analyseur statique : il lit les fichiers `.c` / `.h` et signale chaque écart de style, avec
le fichier, la ligne, la colonne, un code d'erreur et un message.

**Ce qu'elle vérifie**

- la présence et la forme du header 42 ;
- l'indentation, les espaces, les tabulations, la longueur des lignes ;
- le nommage (snake_case, préfixes `s_`, `t_`, `u_`, `e_`, `g_`) ;
- la structure des fonctions (taille, nombre, paramètres, variables) ;
- les constructions interdites (`for`, `switch`, `goto`, ternaire, VLA…) ;
- les includes, les macros, les commentaires.

**Ce qu'elle ne vérifie pas**

- la **correction** du code (c'est le rôle de la moulinette et de tes tests) ;
- les **leaks**, les comportements indéfinis, les fonctions interdites par le sujet ;
- le **Makefile** (il est évalué à l'oral / par la moulinette, pas par la norminette) ;
- le C++ (la norminette est un outil C).

> ⚠️ **Norminette OK ≠ projet validé.** Un code parfaitement normé peut fuir, boucler, ou utiliser
> `printf` alors que le sujet l'interdit. La norminette est une **condition nécessaire**, pas suffisante.

---

## 2. Installation

### 2.1 Sur cette machine (déjà fait)

```bash
# Ubuntu 26.04 n'embarque plus pip : on l'a bootstrappé en user
curl -fsSL https://bootstrap.pypa.io/get-pip.py -o /tmp/get-pip.py
python3 /tmp/get-pip.py --user --break-system-packages

# Installation de la norminette (user, sans sudo)
python3 -m pip install --user --break-system-packages norminette

# Vérification
norminette --version
# norminette 3.3.60, Python 3.14.4, Linux-...-x86_64-with-glibc2.43
```

Le binaire est dans `~/.local/bin/norminette`, déjà dans le `PATH` (`~/.zshrc`, `~/.bashrc`).

```bash
which norminette     # /home/lil_grxpe/.local/bin/norminette
```

### 2.2 Sur le cluster 42 (recommandé : pipx)

```bash
pipx install norminette
# ou, si pipx n'est pas dispo :
python3 -m pip install --user norminette
```

### 2.3 macOS

```bash
brew install pipx
pipx install norminette
```

### 2.4 Windows (PowerShell, hors WSL)

```powershell
py -m pip install --user norminette
```

### 2.5 Mise à jour

```bash
python3 -m pip install --user --break-system-packages -U norminette
# ou
pipx upgrade norminette
```

Prérequis : **Python ≥ 3.10**.

---

## 3. Utilisation : commandes et options

### 3.1 Commandes de base

```bash
norminette                     # dossier courant (récursif)
norminette .                   # idem, explicite
norminette ft_putchar.c        # un fichier
norminette srcs/ includes/     # plusieurs dossiers
norminette c00 c01 c02         # plusieurs projets
norminette **/*.c              # glob (zsh)
```

### 3.2 Options

| Option | Effet |
|---|---|
| `-h`, `--help` | Aide |
| `-v`, `--version` | Version + Python + OS |
| `-o`, `--only-filename` | Affiche `fichier.c` au lieu du chemin complet (idéal CI) |
| `-R <flag>` | Compatibilité norminette 2 (seul `CheckDefine` a un effet : ignore les `#define`) |
| `-f json`, `--format json` | Sortie JSON (machines / CI) |
| `-f humanized` | Sortie lisible (défaut) |
| `--no-colors` | Désactive les couleurs (logs, CI) |
| `--use-gitignore` | Ignore les fichiers listés dans `.gitignore` (via `git check-ignore`) |
| `--cfile <contenu>` | Analyse du contenu C passé en argument plutôt qu'un fichier |
| `--hfile <contenu>` | Idem pour un header |
| `--filename <nom>` | Nom affiché quand `--cfile` / `--hfile` est utilisé |
| `-d`, `-dd` | Debug / tokenisation complète (développement de la norminette) |

### 3.3 Exemples concrets sur ce dépôt

```bash
cd ~/42-piscine

norminette c00/ex00/ft_putchar.c    # ft_putchar.c: OK!
norminette c03                      # tous les .c du dossier
norminette .                        # tout le dépôt
norminette -o c03                   # sans le chemin complet
```

Ignorer ce qui est dans `.gitignore` :

```bash
norminette --use-gitignore .
```

Sortie JSON exploitable par un script :

```bash
norminette -f json c00
# {"files":[{"path":".../ft_putchar.c","status":"OK","errors":[]}]}
```

> 💡 `-R` n'est là que pour la compatibilité avec l'ancienne norminette 2.
> `norminette -R CheckForbiddenSourceHeader` est accepté mais **n'a plus d'effet réel** en v3 ;
> seul `-R CheckDefine` change quelque chose (les `#define` ne sont plus vérifiés).

---

## 4. Lire la sortie et les codes de retour

### 4.1 Format

```
fichier.c: OK!
fichier.c: Error!
Error: CODE_ERREUR   (line:  12, col:   5):   message explicite
Notice: CODE_NOTICE  (line:  13, col:   1):   message informatif
```

- `OK!` : aucun problème (les `Notice` éventuelles ne comptent pas).
- `Error!` : au moins une erreur bloquante → la moulinette refusera le rendu.
- `Notice` : information, **ne fait pas échouer** la norminette (ex. variable globale).

Exemple réel (fichier volontairement fautif) :

```
err.c: Error!
Notice: GLOBAL_VAR_DETECTED  (line:  15, col:   1):  Global variable present in file. Make sure it is a reasonable choice.
Error: TOO_MANY_ARGS        (line:  17, col:  43):  Function has more than 4 arguments
Error: MULT_DECL_LINE       (line:  19, col:  10):  Multiple declarations on a single line
Error: DECL_ASSIGN_LINE     (line:  20, col:  11):  Declaration and assignation on a single line
Error: WRONG_SCOPE_COMMENT  (line:  21, col:   5):  Comment is invalid in this scope
Error: TERNARY_FBIDDEN      (line:  23, col:  19):  Ternaries are forbidden
Error: RETURN_PARENTHESIS   (line:  26, col:  12):  Return value must be in parenthesis
```

### 4.2 Codes de retour

| Code | Signification |
|---|---|
| `0` | Tous les fichiers sont OK (les `Notice` seules n'échouent pas) |
| `1` | Au moins une erreur, ou erreur d'analyse / d'argument |

C'est ce qui permet de l'utiliser dans un `Makefile`, un hook git ou une CI :

```bash
norminette -o . || echo "Norm KO, on ne push pas"
```

---

## 5. La Norm v4.1 en résumé

Résumé des règles **effectivement contrôlées** (PDF officiel : `pdf/en.norm.tex` du dépôt 42School/norminette).

### 5.1 Nommage

- `struct` → préfixe `s_` ; `typedef` → `t_` ; `union` → `u_` ; `enum` → `e_` ; globale → `g_`.
- Identifiants (variables, fonctions, types) : **minuscules, chiffres et `_`** uniquement (snake_case).
- Fichiers et dossiers : minuscules, chiffres et `_`.
- Caractères non-ASCII interdits (sauf dans les chaînes et les `char`).
- Variables globales non `const`/`static` interdites (sauf projet qui l'autorise) → `Notice` de la norminette.
- Le fichier doit **compiler**.

### 5.2 Mise en forme

- **25 lignes max par fonction**, accolades de la fonction non comptées.
- **80 colonnes max par ligne**, commentaires inclus (une tabulation compte pour 4 colonnes).
- Une **ligne vide** entre deux fonctions (au moins).
- Indentation par **tabulations de 4 caractères** (pas 4 espaces).
- Les accolades sont **seules sur leur ligne** (sauf `struct`/`enum`/`union`).
- Ligne vide = vraiment vide ; pas d'espace/tab en fin de ligne ; jamais 2 lignes vides consécutives ; jamais 2 espaces consécutifs.
- Déclarations **au début** de la fonction, **une variable par ligne**.
- Alignement des noms de variables sur la même colonne dans un scope.
- `*` des pointeurs **collé au nom** : `char    *str;`
- Pas de déclaration + initialisation sur la même ligne (sauf globales autorisées, `static`, `const`).
- Une ligne vide **uniquement** entre les déclarations et le reste du corps ; aucune autre ligne vide dans une fonction.
- Une seule instruction / structure de contrôle par ligne.
- Une ligne peut être coupée : continuation indentée, **opérateurs en début de ligne**.
- Virgule et point-virgule suivis d'un espace (sauf fin de ligne).
- Un et un seul espace autour des opérateurs.
- Mot-clé suivi d'un espace, sauf les types (`int`, `char`…) et `sizeof`.
- `if`/`while` avec accolades, sauf instruction unique sur une seule ligne.

### 5.3 Fonctions

- **4 paramètres nommés maximum**.
- Pas d'arguments → prototype explicite `(void)`.
- Paramètres nommés dans les prototypes.
- **5 variables maximum par fonction**.
- `return` entre parenthèses sauf retour vide : `return (x);` / `return ;`
- **Une seule tabulation** entre le type de retour et le nom : `int\tmain(void)`.

### 5.4 `typedef`, `struct`, `enum`, `union`

- Espace après le mot-clé : `struct s_foo`, `typedef struct s_foo`.
- Indentation habituelle à l'intérieur des accolades.
- Noms alignés sur la même colonne dans le scope.
- **Interdiction de déclarer une structure dans un `.c`** (uniquement dans un `.h`).

### 5.5 Headers (`.h`)

- Contenu autorisé : includes, déclarations, defines, prototypes, macros.
- **Includes en début de fichier.**
- Interdiction d'inclure un `.c`.
- Protection contre la double inclusion : `ft_foo.h` → `FT_FOO_H`.
- Includes inutilisés interdits (règle `(*)`, vérifiée par la moulinette).
- Un header doit aussi commencer par le **header 42**.

### 5.6 Header 42 (voir §7)

- Chaque `.c` et `.h` commence **immédiatement** par le bloc de commentaire 42.
- Il contient le nom du fichier, le login + email `@student.campus`, dates de création et de mise à jour.
- Il doit être mis à jour automatiquement à chaque sauvegarde.

### 5.7 Macros et préprocesseur

- `#define` réservés aux valeurs littérales / constantes.
- Macros multilignes interdites.
- Noms de macros **en majuscules**.
- Directives indentées dans les blocs `#if` / `#ifdef` / `#ifndef`.
- Préprocesseur **uniquement en scope global** (pas dans une fonction).

### 5.8 Interdits

| Interdit | Alternative |
|---|---|
| `for` | `while` |
| `do ... while` | `while` |
| `switch` / `case` | `if` / `else if` |
| `goto` et labels | restructurer |
| ternaire `? :` | `if` / `else` |
| VLA (`char buf[n];`) | `malloc` ou taille fixe |
| type implicite (`unsigned x;`) | `unsigned int x;` |

### 5.9 Commentaires

- **Interdits dans le corps d'une fonction** (autorisés en fin de ligne ou sur leur propre ligne, hors corps).
- En anglais, utiles ; un commentaire ne doit pas servir à justifier une fonction « fourre-tout ».

### 5.10 Fichiers

- Pas d'include de `.c`.
- **5 définitions de fonctions maximum par `.c`**.

### 5.11 Makefile

Non vérifié par la norminette. Règles attendues à l'évaluation : `$(NAME)`, `all` (cible par défaut),
`clean`, `fclean`, `re` ; pas de relink ; pas de `*.c` dans les dépendances ; nommer explicitement
tous les fichiers sources.

---

## 6. Catalogue des erreurs les plus fréquentes

| Code | Message | Cause | Correction |
|---|---|---|---|
| `INVALID_HEADER` | Missing or invalid 42 header | Header absent, décalé ou modifié | Générer le header 42 (§7) |
| `SPACE_REPLACE_TAB` | Found space when expecting tab | Indentation avec des espaces | Remplacer par des tabulations (4) |
| `MISSING_TAB_FUNC` | missing tab before function name | Espaces entre type et nom | Une seule tabulation : `int\tmain(void)` |
| `MISSING_TAB_VAR` | missing tab before variable name | Idem pour une variable | `int\ti;` |
| `SPACE_BEFORE_FUNC` | Found space when expecting tab before function name | Espaces au lieu d'une tab | Idem |
| `LINE_TOO_LONG` | line too long | Ligne > 80 colonnes | Couper la ligne, opérateur en début de continuation |
| `TOO_MANY_LINES` | Function has more than 25 lines | Fonction trop longue | Découper en fonctions (5 max/fichier) |
| `TOO_MANY_FUNCS` | Too many functions in file | > 5 fonctions dans le `.c` | Nouveau fichier |
| `TOO_MANY_ARGS` | Function has more than 4 arguments | > 4 paramètres | Regrouper dans une `struct` ou découper |
| `TOO_MANY_VARS_FUNC` | Too many variables declarations in a function | > 5 variables locales | Réutiliser / découper la fonction |
| `DECL_ASSIGN_LINE` | Declaration and assignation on a single line | `int i = 0;` | `int i;` puis `i = 0;` (sauf static/const/global) |
| `MULT_DECL_LINE` | Multiple declarations on a single line | `int a, b;` | Une déclaration par ligne |
| `MULT_ASSIGN_LINE` | Multiple assignations on a single line | `a = 1; b = 2;` | Une par ligne |
| `VAR_DECL_START_FUNC` | Variable declaration not at start of function | Déclaration après du code | Remonter les déclarations |
| `NL_AFTER_VAR_DECL` | Variable declarations must be followed by a newline | Pas de ligne vide après les déclarations | Ajouter **une** ligne vide |
| `EMPTY_LINE_FUNCTION` | Empty line in function | Ligne vide au milieu du corps | Supprimer |
| `MISALIGNED_VAR_DECL` | Misaligned variable declaration | Noms non alignés | Aligner avec des tabulations |
| `CONSECUTIVE_SPC` | Two or more consecutives spaces | Double espace | Un seul espace |
| `EOL_OPERATOR` | Logic operator at the end of line | `&&` en fin de ligne | Mettre l'opérateur au début de la ligne suivante |
| `RETURN_PARENTHESIS` | Return value must be in parenthesis | `return x;` | `return (x);` |
| `NO_ARGS_VOID` | Empty function argument requires void | `int f()` | `int f(void)` |
| `FORBIDDEN_CS` | Forbidden control structure | `for`, `switch`, `do…while` | `while` / `if` |
| `GOTO_FBIDDEN` / `LABEL_FBIDDEN` | goto/label interdit | `goto` | Restructurer |
| `TERNARY_FBIDDEN` | Ternaries are forbidden | `a ? b : c` | `if` / `else` |
| `VLA_FORBIDDEN` | Variable length array forbidden | `char s[n];` | `malloc` ou taille fixe |
| `WRONG_SCOPE_COMMENT` | Comment is invalid in this scope | Commentaire dans une fonction | Sortir le commentaire du corps |
| `BRACE_SHOULD_EOL` | Expected newline after brace | `{` suivi de code | Accolade seule sur sa ligne |
| `EMPTY_LINE_FILE_START` | Empty line at start of file | Ligne vide avant le header | Supprimer |
| `FORBIDDEN_CHAR_NAME` | user defined identifiers should contain only lowercase… | Majuscule / caractère interdit | Renommer en snake_case |
| `GLOBAL_VAR_DETECTED` | Global variable present in file… | Variable globale | `Notice` : préférer `static`/`const` |
| `HEADER_PROT_ALL`, `HEADER_PROT_NAME`… | protection de header incorrecte | `#ifndef` absent/mal nommé | `FT_FOO_H` pour `ft_foo.h` |

### 6.1 Avant / après

**Déclarations**

```c
/* KO */
int	i = 0, j;
char *s;

/* OK */
int		i;
int		j;
char	*s;

i = 0;
```

**Boucle `for`**

```c
/* KO */
for (i = 0; i < n; i++)
	tab[i] = 0;

/* OK */
i = 0;
while (i < n)
{
	tab[i] = 0;
	i++;
}
```

**Ternaire**

```c
/* KO */
max = a > b ? a : b;

/* OK */
max = a;
if (b > a)
	max = b;
```

**Ligne trop longue**

```c
/* KO */
ft_printf("un message vraiment tres long qui depasse largement les quatre-vingts colonnes\n");

/* OK */
ft_printf("un message vraiment tres long qui depasse largement "
	"les quatre-vingts colonnes\n");
```

---

## 7. Le header 42

Chaque fichier `.c` / `.h` doit commencer par ce bloc (80 colonnes, exactement) :

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lil_grxpe <lil_grxpe@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:08:54 by lil_grxpe         #+#    #+#             */
/*   Updated: 2026/10/25 12:08:54 by lil_grxpe        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
```

### 7.1 Le générer

- **VS Code / Antigravity IDE** : extension « 42 Header » (ou « 42header »), raccourci par défaut
  `Ctrl+Alt+H`, renseigner login + email dans les settings.
- **Vim** : `:Stdheader` (plugin 42), ou `F1`.
- **Emacs** : `C-c C-h`.
- **Cluster** : le header est configuré par défaut dans les éditeurs.

### 7.2 Script fourni (testé, sans dépendance)

```python
#!/usr/bin/env python3
"""Genere l'en-tete 42 officiel (Norm v4.1) pour un fichier .c/.h.
Usage: ./42header.py fichier.c [login] [email]
"""
import sys, datetime, pathlib

W = 76  # largeur entre "/*" et "*/" (total 80 colonnes)

def line(left, right):
    pad = W - len(left) - len(right)
    if pad < 0:
        raise SystemExit("nom de fichier / login / email trop long")
    return "/*" + left + " " * pad + right + "*/"

def header(filename, login, email):
    d = datetime.datetime.now().strftime("%Y/%m/%d %H:%M:%S")
    out = [
        "/* " + "*" * 74 + " */",
        "/*" + " " * 76 + "*/",
        line(" " * 56, ":::      ::::::::   "),
        line("   " + filename, ":+:      :+:    :+:   "),
        line(" " * 52, "+:+ +:+         +:+     "),
        line("   By: " + login + " <" + email + ">", "+#+  +:+       +#+        "),
        line(" " * 48, "+#+#+#+#+#+   +#+           "),
        line("   Created: " + d + " by " + login, "#+#    #+#             "),
        line("   Updated: " + d + " by " + login, "###   ########.fr       "),
        "/*" + " " * 76 + "*/",
        "/* " + "*" * 74 + " */",
    ]
    return "\n".join(out) + "\n"

if __name__ == "__main__":
    f = sys.argv[1]
    login = sys.argv[2] if len(sys.argv) > 2 else "lil_grxpe"
    email = sys.argv[3] if len(sys.argv) > 3 else f"{login}@student.42.fr"
    sys.stdout.write(header(pathlib.Path(f).name, login, email))
```

Utilisation :

```bash
chmod +x 42header.py
./42header.py ft_putchar.c > /tmp/h.txt
cat /tmp/h.txt ft_putchar.c > /tmp/new.c && mv /tmp/new.c ft_putchar.c
```

> Le header doit être **mis à jour à chaque sauvegarde** (ligne `Updated:`).
> Ne jamais éditer à la main les colonnes : utiliser l'extension / le script.

---

## 8. Automatiser : Makefile, hook git, CI, IDE

### 8.1 Cible Makefile

```make
norm:
	norminette -o $(SRCS) $(HEADERS)

.PHONY: norm
```

```bash
make norm
```

### 8.2 Hook git `pre-commit`

`.git/hooks/pre-commit` (puis `chmod +x`) :

```sh
#!/bin/sh
files=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(c|h)$')
[ -z "$files" ] && exit 0
norminette -o $files || {
	echo "❌ Norminette KO : commit annule"
	exit 1
}
```

### 8.3 GitHub Actions

`.github/workflows/norminette.yml` :

```yaml
name: norminette
on: [push, pull_request]
jobs:
  norm:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-python@v5
        with:
          python-version: "3.12"
      - run: python -m pip install norminette
      - run: norminette -o .
```

### 8.4 IDE (VS Code / Antigravity)

Extension → chercher **Norminette** (souligne les erreurs en direct).

Ou une tâche `Ctrl+Shift+B` — `.vscode/tasks.json` :

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "norminette",
      "type": "shell",
      "command": "norminette",
      "args": ["-o", "${file}"],
      "group": "test",
      "problemMatcher": []
    }
  ]
}
```

### 8.5 Mode « watch »

```bash
# avec entr (sudo apt install entr)
find . -name '*.c' -o -name '*.h' | entr -c norminette -o .

# ou en boucle simple
while sleep 2; do clear; norminette -o .; done
```

---

## 9. Dépannage / FAQ

**`norminette: command not found`**
→ `~/.local/bin` n'est pas dans le `PATH` :
```bash
export PATH="$HOME/.local/bin:$PATH"
```
(ajouter la ligne dans `~/.zshrc` / `~/.bashrc`)

**`error: externally-managed-environment`**
→ Ubuntu récent protège le Python système. Trois solutions :
```bash
python3 -m pip install --user --break-system-packages norminette   # simple
pipx install norminette                                           # propre
python3 -m venv ~/.venvs/norm && source ~/.venvs/norm/bin/activate && pip install norminette
```

**`INVALID_HEADER` sur un `.h`**
→ Le header est obligatoire aussi dans les headers, pas seulement dans les `.c`.

**J'ai une `Notice` mais le fichier est `OK!`**
→ Normal : `GLOBAL_VAR_DETECTED` est une information, elle ne fait pas échouer la norminette.
La moulinette, elle, peut la compter comme erreur (variable globale non `const`/`static`).

**La norminette dit OK mais la moulinette refuse**
→ La moulinette teste en plus : compilation, leaks, fonctions interdites, comportement, règles `(*)`
du PDF (includes inutilisés, commentaires utiles, etc.). La norminette ne couvre que le style.

**Mon éditeur affiche 4 espaces alors que j'ai des tabs**
→ Configurer l'éditeur : tabulation = **4 colonnes**, insertion d'espaces désactivée.
Sinon la norminette voit `SPACE_REPLACE_TAB` / `MIXED_SPACE_TAB`.

**« line too long » alors que la ligne semble courte**
→ Une tabulation compte pour 4 colonnes, et la règle inclut les commentaires.

**Analyser un fichier sans header**
→ Impossible de « désactiver » la règle : la norminette v3 n'a pas de config par fichier.
Utiliser un fichier temporaire avec header, ou `--cfile` avec le header inclus.

**La norminette est lente sur un gros dépôt**
→ Cibler les dossiers (`norminette c00 c01`) ou utiliser `-o` ; la 3.3.x est déjà bien plus rapide
que la 3.0.

---

## 10. Aide-mémoire

```bash
norminette .                          # tout vérifier
norminette -o .                       # sans chemin complet
norminette -f json .                  # JSON
norminette --use-gitignore .          # ignorer .gitignore
norminette --no-colors .              # sans couleurs
norminette -R CheckDefine .           # ignorer les #define
norminette --version                  # version
```

**Limites à retenir**

| Règle | Limite |
|---|---|
| Lignes par fonction | 25 (hors accolades de la fonction) |
| Colonnes par ligne | 80 (tab = 4) |
| Fonctions par fichier | 5 |
| Paramètres par fonction | 4 |
| Variables par fonction | 5 |
| Indentation | tabulations de 4 |

**Interdits** : `for`, `do…while`, `switch`, `case`, `goto`, ternaire, VLA, type implicite,
commentaire dans une fonction, déclaration dans un `.c` pour `struct`, double espace, ligne > 80.

**Computer-friendly** : `norminette -o .` retourne `0` (OK) ou `1` (KO).

---

## Liens utiles

- Dépôt officiel : <https://github.com/42School/norminette>
- PDF de la norme (v4.1) : <https://github.com/42School/norminette/tree/master/pdf>
- Releases : <https://github.com/42School/norminette/releases>
- Extension VS Code « 42 Header » / « Norminette »
