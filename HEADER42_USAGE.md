# Header 42 — utilisation (vim)

## Générer le header

Ouvrir le `.c`, être en mode **NORMAL** (touche `Échap`), puis :

```
:Stdheader
```

ou

```
F1
```

Le header est inséré en haut du fichier. Sauver : `:wq`.

## Ce que fait le plugin

| Élément | Valeur |
|---|---|
| Hauteur | 11 lignes |
| Largeur | 80 colonnes |
| Auteur | `g:user42` / `g:mail42` |
| `Created` | date du premier appel |
| `Updated` | date du dernier appel |

Relancer `:Stdheader` sur un fichier déjà headeré **met à jour** `Updated`,
sans dupliquer le bloc.

## Délimiteurs selon l'extension

| Extension | Délimiteurs |
|---|---|
| `.c` `.h` `.cpp` `.hpp` `.rs` `.go` `.java` | `/* */` |
| `.js` `.ts` | `//` |
| `.py` | `#` |
| `.vim` `.vimrc` | `"` |
| `.html` `.xml` | `<!-- -->` |
| `.sh` | `#` |

## Vérifier

```bash
head -11 ft_putchar.c
awk 'NR<=11{print length($0)}' ft_putchar.c | sort -u   # → 80
norminette ft_putchar.c
```

Le header doit être présent **avant** de lancer norminette : son absence est une
erreur de rendu.

## Ordre de travail

```
vim ex00/ft_putchar.c
:Stdheader      → header
i               → insertion
...code...
Échap :wq
norminette ex00/ft_putchar.c
```

## Problèmes courants

### F1 ouvre l'aide du terminal

Le terminal capture `F1` avant vim. Utiliser `:Stdheader`, ou réassigner la
touche du terminal (Terminator : Préférences → Raccourcis → Aide).

### Le header affiche `user` / `user@host`

`g:user42` et `g:mail42` ne sont pas définis. Ajouter dans `~/.vimrc` :

```vim
let g:user42 = 'LOGIN'
let g:mail42 = 'LOGIN@student.42.fr'
```

Puis relancer vim.

### Le header sort avec `#` et des `0` à la place de l'art 42

Vim tourne en mode `compatible` : les plugins et les continuations de lignes ne
sont pas interprétés correctement.

Cas typiques :

```bash
vi -u NONE        # pas de vimrc → compatible
vim -es           # mode ex → compatible
```

Corriger :

- lancer `vim` (pas `vi -u NONE`), avec un `~/.vimrc` existant ;
- si besoin de mode silencieux : `vim -N -es ...` (`-N` force `nocompatible`).

Vérifier :

```bash
vim -c 'set compatible?' -c 'qa!'
```

Attendu : `nocompatible`.

### Le header dépasse 80 colonnes

Le login + mail sont trop longs, ou le fichier a été édité à la main. Supprimer
les 11 lignes et relancer `:Stdheader`.

## Ne pas faire

- modifier le header à la main (le correcteur compare) ;
- mettre un header dans un fichier qui n'est pas demandé ;
- laisser un header vide (`By:` sans login).
