# Pointeurs et nœuds — les bases

Fiche de rappel. Tout ce qui suit compile avec `cc -Wall -Wextra -Werror`.

---

## 1. La mémoire

Un programme C manipule deux zones principales :

```
adresses basses
┌────────────────┐
│ code           │  le programme compilé
├────────────────┤
│ data / bss     │  variables globales et static
├────────────────┤
│ heap      ↑    │  malloc / free  (grandit vers le haut)
│                │
│           ↓    │
│ stack          │  variables locales, paramètres (vers le bas)
└────────────────┘
adresses hautes
```

- **stack** : allocation automatique, libérée à la sortie de la fonction. Rapide, petite (quelques Mo).
- **heap** : allocation manuelle avec `malloc`, libérée avec `free`. Grande, lente, source de fuites.

Une variable a trois propriétés :

| Propriété | Exemple |
|---|---|
| un nom | `a` |
| une adresse | `0x7ffd...` |
| une valeur | `42` |

Un pointeur est une variable **dont la valeur est une adresse**.

---

## 2. Un pointeur = une adresse

```c
int	a;
int	*p;

a = 42;
p = &a;
```

```
a : adresse 0x1000, valeur 42
p : adresse 0x2000, valeur 0x1000  ──►  a
```

| Écriture | Sens |
|---|---|
| `int *p;` | `p` est un pointeur vers un `int` |
| `p = &a;` | `p` reçoit l'**adresse** de `a` |
| `*p` | la **valeur** stockée à l'adresse `p` |
| `*p = 7;` | écrire `7` à l'adresse `p` (donc dans `a`) |
| `p` | l'adresse elle-même |
| `&p` | l'adresse du pointeur |

```c
a = 42;
p = &a;
*p = 7;
/* a vaut maintenant 7 */
```

Le `*` a deux rôles selon le contexte :

```c
int	*p;      /* déclaration : "p est un pointeur"       */
*p = 5;      /* déréférencement : "la cible de p"          */
```

---

## 3. Passage par valeur vs par adresse

En C, **tout est passé par valeur**. Pour modifier une variable de l'appelant, il faut passer son adresse.

```c
void	broken_swap(int a, int b)   /* ne fait rien à l'extérieur */
{
	int	tmp;

	tmp = a;
	a = b;
	b = tmp;
}

void	real_swap(int *a, int *b)   /* modifie vraiment */
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
```

```c
int	x = 1;
int	y = 2;

real_swap(&x, &y);   /* x=2, y=1 */
```

Règle : **la fonction reçoit une copie**. Si tu veux écrire dans l'original, donne-lui son adresse.

---

## 4. Pointeur et tableau

Un tableau est un bloc contigu. Le nom du tableau « se décompose » en pointeur vers son premier élément.

```c
int	tab[4] = {10, 20, 30, 40};
int	*p;

p = tab;        /* identique à p = &tab[0]; */
```

Les deux écritures sont équivalentes :

```c
tab[i]   ==   *(tab + i)
p[i]     ==   *(p + i)
```

Attention :

```c
sizeof(tab)   /* 4 * sizeof(int) = 16 : taille du tableau */
sizeof(p)     /* sizeof(int *) = 8 : taille du pointeur   */
```

Un tableau passé à une fonction devient un pointeur : la taille est perdue.

```c
void	f(int tab[4])   /* en réalité : int *tab */
```

---

## 5. Arithmétique de pointeur

`p + 1` ne veut pas dire « +1 octet », mais « +1 élément ».

```c
int	tab[3] = {10, 20, 30};
int	*p = tab;

*(p + 0)   /* 10 */
*(p + 1)   /* 20 */
*(p + 2)   /* 30 */
```

Sur un `int *` (4 octets), `p + 1` avance de 4 octets. Sur un `char *`, de 1 octet.

```c
char	*s = "abc";

*s       /* 'a' */
*(s + 1) /* 'b' */
s[2]     /* 'c' */
```

C'est exactement le mécanisme utilisé pour parcourir une chaîne :

```c
while (*s)
{
	/* ... */
	s++;
}
```

---

## 6. Pointeur de pointeur

`int **pp` contient l'adresse d'un `int *`.

```c
int	a = 42;
int	*p = &a;
int	**pp = &p;
```

```
pp ──► p ──► a (42)
```

```c
*pp      /* p, c'est-à-dire &a */
**pp     /* a, c'est-à-dire 42  */
```

Utilité : modifier un pointeur depuis une fonction.

```c
void	set_null(int **p)
{
	*p = NULL;
}
```

C'est le même principe pour les nœuds : `t_list **head` permet de changer la tête de liste.

---

## 7. NULL

`NULL` est l'adresse zéro : « ne pointe vers rien ».

```c
int	*p = NULL;

if (!p)
	return (0);      /* toujours tester avant de déréférencer */
```

- `!p` est vrai si `p == NULL`.
- Déréférencer `NULL` → **segmentation fault**.
- Un pointeur non initialisé est **indéfini** (adresse aléatoire) : initialise toujours à `NULL`.

---

## 8. const

```c
const int	*p;    /* on ne peut pas modifier *p (la cible)   */
int *const	p;     /* on ne peut pas modifier p (le pointeur) */
```

En pratique, `char const *` (ou `const char *`) = chaîne en lecture seule.

---

## 9. malloc / free

```c
#include <stdlib.h>

int	*p;

p = malloc(sizeof(int));
if (!p)
	return (NULL);
*p = 42;
free(p);
```

Règles :

1. `malloc(n)` réserve `n` **octets** et renvoie l'adresse, ou `NULL` en cas d'échec.
2. Toujours vérifier le retour.
3. `sizeof(*p)` est plus sûr que `sizeof(int)` (si le type change, ça suit).
4. Chaque `malloc` doit avoir **exactement un** `free`.
5. Après `free(p)`, `p` est invalide : remets-le à `NULL` si tu le réutilises.

```c
p = malloc(sizeof(*p));
free(p);
p = NULL;
```

Ne jamais faire :

```c
free(p);
free(p);        /* double free → crash */
```

---

## 10. Le nœud

Un nœud est une `struct` qui contient **une donnée** et **un pointeur vers le nœud suivant**.

```c
typedef struct s_list
{
	int				value;
	struct s_list	*next;
}	t_list;
```

Un nœud seul :

```
┌──────────┬──────────┐
│ value: 1 │ next:NULL│
└──────────┴──────────┘
```

Le champ `next` est du même type que la struct : c'est une **récursion de type**. C'est légal en C parce que `struct s_list` est déjà déclaré au moment où `next` apparaît.

Deux conventions fréquentes à 42 :

```c
/* liste d'entiers */
typedef struct s_list
{
	int				value;
	struct s_list	*next;
}	t_list;

/* liste générique (libft) */
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
```

---

## 11. La liste chaînée

Une liste est une **chaîne de nœuds**. Le dernier pointe vers `NULL`.

```
head
 │
 ▼
┌────┬────┐    ┌────┬────┐    ┌────┬────┐
│ 1  │  ──┼───►│ 2  │  ──┼───►│ 3  │ NULL│
└────┴────┘    └────┴────┘    └────┴────┘
```

Le seul point d'entrée est `head`. Si tu perds `head`, tu perds la liste.

### 11.1 Créer un nœud

```c
t_list	*new_node(int v)
{
	t_list	*n;

	n = malloc(sizeof(t_list));
	if (!n)
		return (NULL);
	n->value = v;
	n->next = NULL;
	return (n);
}
```

`malloc` ne met pas à zéro : il faut initialiser **chaque champ**.

### 11.2 Parcourir

```c
void	print_list(t_list *head)
{
	while (head)
	{
		/* utiliser head->value */
		head = head->next;
	}
}
```

On ne modifie que la copie locale `head`, pas la tête réelle.

### 11.3 Insérer en tête

```c
void	push_front(t_list **head, t_list *n)
{
	if (!head || !n)
		return ;
	n->next = *head;
	*head = n;
}
```

Pourquoi `t_list **` ? Parce qu'on modifie la variable `head` de l'appelant, pas seulement le contenu d'un nœud.

```
avant : head ──► A ──► B ──► NULL
après : head ──► N ──► A ──► B ──► NULL
```

### 11.4 Ajouter en queue

```c
void	push_back(t_list **head, t_list *n)
{
	t_list	*cur;

	if (!head || !n)
		return ;
	if (!*head)
	{
		*head = n;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = n;
}
```

Il faut d'abord avancer jusqu'au **dernier** nœud (`cur->next == NULL`), puis accrocher.

### 11.5 Taille

```c
int	list_size(t_list *head)
{
	int	i;

	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return (i);
}
```

### 11.6 Chercher

```c
t_list	*find_node(t_list *head, int v)
{
	while (head)
	{
		if (head->value == v)
			return (head);
		head = head->next;
	}
	return (NULL);
}
```

### 11.7 Supprimer un nœud

Il faut garder le **précédent**, sinon on ne peut pas recoudre la chaîne.

```c
void	delete_node(t_list **head, int v)
{
	t_list	*cur;
	t_list	*prev;

	if (!head || !*head)
		return ;
	cur = *head;
	prev = NULL;
	while (cur)
	{
		if (cur->value == v)
		{
			if (prev)
				prev->next = cur->next;
			else
				*head = cur->next;
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}
```

```
avant : A ──► B ──► C
après : A ─────────► C        (B supprimé)
```

### 11.8 Libérer toute la liste

```c
void	free_list(t_list **head)
{
	t_list	*next;

	if (!head)
		return ;
	while (*head)
	{
		next = (*head)->next;   /* sauver avant de libérer */
		free(*head);
		*head = next;
	}
}
```

Ordre obligatoire : **lire `next` avant `free`**. Après `free`, le nœud n'existe plus.

---

## 12. Exemple complet

```c
#include <stdlib.h>
#include <unistd.h>

typedef struct s_list
{
	int				value;
	struct s_list	*next;
}	t_list;

static void	put_nbr(int n)
{
	char	c;

	if (n >= 10)
		put_nbr(n / 10);
	c = '0' + (n % 10);
	write(1, &c, 1);
}

t_list	*new_node(int v)
{
	t_list	*n;

	n = malloc(sizeof(t_list));
	if (!n)
		return (NULL);
	n->value = v;
	n->next = NULL;
	return (n);
}

void	push_back(t_list **head, t_list *n)
{
	t_list	*cur;

	if (!head || !n)
		return ;
	if (!*head)
	{
		*head = n;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = n;
}

void	print_list(t_list *head)
{
	while (head)
	{
		put_nbr(head->value);
		write(1, "\n", 1);
		head = head->next;
	}
}

void	free_list(t_list **head)
{
	t_list	*next;

	if (!head)
		return ;
	while (*head)
	{
		next = (*head)->next;
		free(*head);
		*head = next;
	}
}

int	main(void)
{
	t_list	*head;

	head = NULL;
	push_back(&head, new_node(1));
	push_back(&head, new_node(2));
	push_back(&head, new_node(3));
	print_list(head);
	free_list(&head);
	return (0);
}
```

Sortie :

```
1
2
3
```

---

## 13. Erreurs classiques

| Erreur | Conséquence | Correction |
|---|---|---|
| `int *p; *p = 5;` | écriture à une adresse aléatoire | `p = malloc(...)` ou `p = &a` |
| `p = malloc(sizeof(p));` | réserve 8 octets au lieu de la struct | `sizeof(*p)` |
| `free(p); free(p);` | double free, crash | un seul `free` par `malloc` |
| `free(p); *p = 1;` | use-after-free | ne plus toucher `p` après `free` |
| oublier `free` | fuite mémoire | un `free` par `malloc` |
| `while (cur->next)` avec `next` non initialisé | boucle infinie / crash | `n->next = NULL` |
| modifier `head` sans `**` | la liste de l'appelant ne change pas | `t_list **head` |
| `return (&local);` | pointeur vers une variable morte | `malloc` ou renvoyer une valeur |
| `sizeof(tab)` dans une fonction | donne la taille du pointeur | passer la taille en paramètre |
| déréférencer avant `if (!p)` | segfault | tester `NULL` d'abord |
| `cur = cur->next; free(cur);` | on a perdu le suivant | sauver `next` avant `free` |

---

## 14. Règles Norm 42

- Pas de `for`, `do...while`, `switch/case`, `goto`, ternaire.
- `if` et `while` autorisés.
- Déclarations **en haut** de fonction.
- Une instruction par ligne, max **25 lignes** par fonction.
- Max **4 paramètres**.
- Indentation : tabulations (affichées sur 4).
- Pas de `//`, uniquement `/* */`.
- `return (x);` avec parenthèses.
- 80 colonnes max.
- Chaque fichier `.c` commence par le header 42.
- Compilation : `cc -Wall -Wextra -Werror`.

---

## 15. Check-list avant de rendre

- [ ] Chaque `malloc` est vérifié.
- [ ] Chaque `malloc` a son `free`.
- [ ] `next` du dernier nœud vaut `NULL`.
- [ ] Les fonctions qui changent `head` prennent `t_list **`.
- [ ] Aucun déréférencement avant un test `NULL`.
- [ ] Pas de pointeur vers une variable locale renvoyé.
- [ ] `norminette` passe.
- [ ] `cc -Wall -Wextra -Werror` passe sans warning.
- [ ] Testé avec 0, 1 et plusieurs nœuds.

---

## 16. Mini-exercices

1. `int ft_strlen(char *s)` sans variable d'index, uniquement avec le pointeur.
2. `void ft_swap(int *a, int *b)`.
3. `t_list *ft_list_last(t_list *head)`.
4. `void ft_list_reverse(t_list **head)` sans allouer de nœud.
5. `int ft_list_size(t_list *head)`.
6. `void ft_list_clear(t_list **head)` puis vérifier avec `valgrind --leak-check=full`.

---

## 17. Modèle mental

```
pointeur   = flèche
nœud       = boîte + flèche vers la boîte suivante
liste      = chaîne de flèches
NULL       = fin de chaîne
t_list **  = flèche vers la flèche d'entrée
```

Si tu sais répondre à « où pointe cette flèche, et qui possède cette boîte ? », tu sais coder la liste.
