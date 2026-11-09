# C00 — Autorisé / Interdit

Règle de base du sujet : chaque exercice indique **`Allowed functions : write`**.
Tout ce qui n'est pas explicitement autorisé est **interdit** (triche = `-42`).
Cette page liste tout : structures de contrôle, fonctions, et le reste.

---

## 1. Structures de contrôle

| Structure | Statut | Remarque |
|---|---|---|
| Bloc `{ ... }` | ✅ Autorisé | Accolades obligatoires après `if`/`while`, sauf instruction unique sur une ligne |
| `if` | ✅ Autorisé | |
| `else if` | ✅ Autorisé | |
| `else` | ✅ Autorisé | |
| `while` | ✅ Autorisé | **Seule boucle autorisée** |
| `return` | ✅ Autorisé | `return (x);` avec parenthèses ; `return ;` pour `void` |
| `break` | ✅ Autorisé (non signalé par la norminette) | Sortie de boucle ; rarement utile en c00 |
| `continue` | ✅ Autorisé (non signalé par la norminette) | Rarement utile en c00 |
| `for` | ❌ Interdit (Norme) | Remplacer par `while` |
| `do ... while` | ❌ Interdit (Norme) | Remplacer par `while` |
| `switch` / `case` / `default` | ❌ Interdit (Norme) | Remplacer par `if` / `else if` |
| `goto` + labels | ❌ Interdit (Norme) | Restructurer le code |
| Ternaire `a ? b : c` | ❌ Interdit (Norme) | Remplacer par `if` / `else` |
| VLA `char buf[n];` | ❌ Interdit (Norme) | Taille fixe (pas de `malloc` en c00) |

Norminette codes associés : `FORBIDDEN_CS`, `GOTO_FBIDDEN`, `LABEL_FBIDDEN`, `TERNARY_FBIDDEN`, `VLA_FORBIDDEN`.

---

## 2. Fonctions de bibliothèque

### 2.1 Autorisée

| Fonction | Header | Usage en c00 |
|---|---|---|
| `write` | `<unistd.h>` | `write(1, &c, 1);` — seule fonction autorisée |

### 2.2 Interdites — `unistd.h`

`read`, `close`, `lseek`, `pipe`, `dup`, `dup2`, `fork`, `execve`, `exit`, `_exit`, `sleep`, `usleep`, `getpid`, `getppid`, `getcwd`, `chdir`, `access`, `isatty`, `ttyname`, `link`, `unlink`, `symlink`, `truncate`, `alarm`, `pause`, `kill`, `signal`, `getopt`…

> Seul `write` est permis dans tout `<unistd.h>`.

### 2.3 Interdites — `stdio.h`

`printf`, `ft_printf`, `fprintf`, `sprintf`, `snprintf`, `dprintf`, `puts`, `fputs`, `putchar`, `putc`, `getchar`, `gets`, `fgets`, `scanf`, `perror`, `fopen`, `fclose`, `fread`, `fwrite`, `fflush`, `remove`, `rename`…

> Y compris l'include : `#include <stdio.h>` est interdit.

### 2.4 Interdites — `string.h`

`strlen`, `strcpy`, `strncpy`, `strlcpy`, `strcat`, `strncat`, `strlcat`, `strcmp`, `strncmp`, `strdup`, `strndup`, `strchr`, `strrchr`, `strstr`, `strnstr`, `strtok`, `memset`, `memcpy`, `memmove`, `memcmp`, `memchr`, `bzero`

### 2.5 Interdites — `stdlib.h`

`malloc`, `calloc`, `realloc`, `free`, `exit`, `atoi`, `atol`, `atoll`, `strtol`, `abs`, `labs`, `rand`, `srand`, `qsort`, `bsearch`, `system`, `getenv`, `setenv`, `abort`

### 2.6 Interdites — `ctype.h`

`isalpha`, `isdigit`, `isalnum`, `isascii`, `isprint`, `isspace`, `isupper`, `islower`, `toupper`, `tolower`

### 2.7 Interdites — autres headers

| Header | Exemples de fonctions interdites |
|---|---|
| `<math.h>` | `pow`, `sqrt`, `fabs`, `floor`, `ceil`, `sin`, `cos` |
| `<time.h>` | `time`, `clock`, `difftime`, `mktime`, `strftime` |
| `<fcntl.h>` | `open`, `creat`, `fcntl` |
| `<sys/stat.h>` | `stat`, `lstat`, `fstat`, `chmod` |
| `<sys/wait.h>` | `wait`, `waitpid` |
| `<signal.h>` | `signal`, `sigaction`, `kill` |
| `<dirent.h>` | `opendir`, `readdir`, `closedir` |
| `<errno.h>` | variable `errno` (lecture/écriture) |

### 2.8 Fonctions personnelles

- ❌ Appeler une fonction `ft_*` d'un autre exercice : le fichier est compilé **seul** par la Moulinette → ne compile pas.
- ✅ Définir **une seule** fonction demandée par l'exercice (le prototype imposé).
- ❌ Ajouter un `main` : c00 demande des fonctions, pas des programmes.

---

## 3. Autorisé : syntaxe et opérateurs

| Catégorie | Autorisé |
|---|---|
| Types | `char`, `int`, `void`, `unsigned int`, `long`, `short`… |
| Déclaration | `char c;`, `int i;` (une tabulation entre type et nom) |
| Arithmétique | `+`, `-`, `*`, `/`, `%`, `++`, `--` |
| Comparaison | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Logique | `&&`, `\|\|`, `!` |
| Bit à bit | `&`, `\|`, `^`, `~`, `<<`, `>>` |
| Affectation | `=`, `+=`, `-=`, `*=`, `/=`, `%=` |
| Adresse / pointeur | `&c` (nécessaire pour `write`), `*p` |
| Cast | `(char)n`, `(unsigned int)n` |
| Littéraux | `'a'`, `'0'`, `42`, `'\n'` |
| Préprocesseur | `#include <unistd.h>`, `#define` de constantes |
| Commentaires | Hors du corps d'une fonction uniquement |

---

## 4. Accès direct à la mémoire

- ✅ `write(1, &c, 1)` : l'adresse d'une variable est autorisée.
- ✅ Tableaux de taille fixe (`char tab[10];`) : autorisés par la Norme, mais inutiles en c00.
- ❌ Allocation dynamique : `malloc`, `calloc`, `realloc`, `free`.

---

## 5. Les 9 exercices de c00

| Ex | Fichier | Prototype | Fonctions autorisées |
|---|---|---|---|
| 00 | `ft_putchar.c` | `void ft_putchar(char c);` | `write` |
| 01 | `ft_print_alphabet.c` | `void ft_print_alphabet(void);` | `write` |
| 02 | `ft_print_reverse_alphabet.c` | `void ft_print_reverse_alphabet(void);` | `write` |
| 03 | `ft_print_numbers.c` | `void ft_print_numbers(void);` | `write` |
| 04 | `ft_is_negative.c` | `void ft_is_negative(int n);` | `write` |
| 05 | `ft_print_comb.c` | `void ft_print_comb(void);` | `write` |
| 06 | `ft_print_comb2.c` | `void ft_print_comb2(void);` | `write` |
| 07 | `ft_putnbr.c` | `void ft_putnbr(int nb);` | `write` |
| 08 | `ft_print_combn.c` | `void ft_print_combn(int n);` | `write` |

---

## 6. Contraintes de la Norme (rappel utile c00)

- Header 42 dans chaque `.c`.
- Tabulations (pas d'espaces) pour l'indentation.
- 80 colonnes max.
- 25 lignes max par fonction.
- 5 fonctions max par fichier.
- 4 arguments max par fonction.
- `if` / `while` avec accolades (sauf instruction unique sur une ligne).
- `return (valeur);` entre parenthèses.
- Pas de commentaire dans le corps d'une fonction.
- Pas de variable globale non `const`/`static`.
- Une instruction par ligne ; opérateurs logiques en début de ligne en cas de coupure.
- Compilation : `cc -Wall -Wextra -Werror`.

---

## 7. Vérifier qu'aucune fonction interdite n'est appelée

```bash
cc -Wall -Wextra -Werror -c ft_putchar.c -o ft_putchar.o
nm -u ft_putchar.o
```

La sortie ne doit contenir que `write` (et éventuellement `_GLOBAL_OFFSET_TABLE_`).
Toute autre ligne = fonction interdite utilisée.
