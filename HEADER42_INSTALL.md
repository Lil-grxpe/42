# Header 42 — installation (vim)

À faire une fois par machine, avant le premier rendu.

## 1. Vérifier vim

```bash
vim --version | head -1
```

Attendu : `VIM - Vi IMproved 9.x`. Si `vim` absent :

```bash
sudo apt install -y vim git
```

## 2. Installer le plugin

```bash
mkdir -p ~/.vim/pack/42/start
git clone --depth 1 https://github.com/42Paris/42header ~/.vim/pack/42/start/42header
```

Sans git (un seul fichier, suffisant) :

```bash
mkdir -p ~/.vim/plugin
curl -fsSL -o ~/.vim/plugin/stdheader.vim \
  https://raw.githubusercontent.com/42Paris/42header/master/plugin/stdheader.vim
```

Les deux méthodes marchent. Ne pas faire les deux à la fois.

## 3. Déclarer login + mail 42

Créer ou compléter `~/.vimrc` :

```vim
let g:user42 = 'LOGIN'
let g:mail42 = 'LOGIN@student.42.fr'
```

Remplacer `LOGIN` (ex. `lil_grxpe`). Sans ces variables, le header sort avec
`user` / `user@host`.

Alternative par variables d'environnement, dans `~/.bashrc` ou `~/.zshrc` :

```bash
export USER='LOGIN'
export MAIL='LOGIN@student.42.fr'
```

## 4. Vérifier

```bash
printf 'int x;\n' > /tmp/t.c
vim -N -c 'Stdheader' -c 'wq' /tmp/t.c
head -11 /tmp/t.c
```

Attendu : 11 lignes, `/* ... */`, art 42, ton login.

Contrôle largeur :

```bash
awk 'NR<=11{print length($0)}' /tmp/t.c | sort -u
```

Doit afficher une seule valeur : `80`.

## 5. Sur le cluster 42

Le dépôt fournit un script :

```bash
./set_header.sh
```

Il crée `~/.vimrc` et installe le plugin.

## Désinstaller

```bash
rm -rf ~/.vim/pack/42
rm -f ~/.vim/plugin/stdheader.vim
```

Puis retirer les lignes `g:user42` / `g:mail42` de `~/.vimrc`.

## Fichiers concernés

| Chemin | Rôle |
|---|---|
| `~/.vim/pack/42/start/42header/plugin/stdheader.vim` | plugin (méthode 1) |
| `~/.vim/plugin/stdheader.vim` | plugin (méthode 2) |
| `~/.vimrc` | login, mail, options |
