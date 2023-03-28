#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct date {
	unsigned jour;
	unsigned mois;
	unsigned annee;
};

struct individu {
		char * nom;
		char * prenom;
		struct date * naissance;
};

struct opera {
	char * titre; /* le titre de l'opéra */
	struct date * date_creation; /* l'année de la création (i.e. de la première représentation) */
	char * ville_creation; /* ville de la création (i.e. de la première représentation) */
	struct individu * compositeur; /* nom du compositeur */
};

#define TAILLE_MAX 50 /* pour la fonction initialiser_tab_operas_fichier */
#define ENTREE argv[1] /* Nom du fichier pour initialiser le tableau d'adresses d'opéras */

void recherche_tab_opera_age (struct opera **, int, int);
int reorganiser_tab_opera_ville (struct opera **, int, const char *);
void trier_tab_opera_creation (struct opera **, int);

/** Fonction qui affiche les valeurs des champs de la date passée en argument */
void afficher_date(const struct date * d) {
	printf("%u/%u/%u", d->jour, d->mois, d->annee);
}

/* Fonction qui affiche le prénom, le nom et la date de naissance de l'individu pointé par ind */
void afficher_individu (const struct individu * ind) {
	printf("%s %s (", ind->prenom, ind->nom);
	afficher_date(ind->naissance);
	printf(")");
}

/** Fonction qui affiche le titre, le prénom et le nom du compositeur, (la date et la ville de création) de l'opéra op */
void afficher_opera (const struct opera * op) {
	afficher_individu(op->compositeur);
	printf(", %s (", op->titre);
	afficher_date(op->date_creation);
	printf(", %s)", op->ville_creation);
}

/** Fonction qui échange les adresses d'opéra pointées par les deux arguments */
void echanger_opera (struct opera **op1, struct opera **op2) {
	struct opera *temp = *op1;
	*op1 = *op2;
	*op2 = temp;
}

/** Variante */
/** Complexité : \Theta(N) si N est la taille du tableau */
/*
int reorganiser_tab_opera_ville (struct opera * tab[], int taille, const char * v) {
	int i; j;
	for (j = 0; j < taille && !strcmp(v, tab[j]->ville_creation); j ++);
	for (i = j + 1; i < n; i ++) {
		if (!strcmp(v, tab[i]->ville_creation)) {
			echanger_opera (&tab[i], &tab[j]);
			j ++;
		}
	}
	return j;
}
*/

/** Fonction qui renvoie -1 si d1 est antérieure à d2, */
/**          qui renvoie 1 si d1 est postérieure à d2, */
/**          qui renvoie 0 si les dates d1 et d2 sont identiques */
int comparer_date(const struct date * d1, const struct date * d2) {
	if(d1->annee < d2->annee)
		return -1;
	if (d1->annee > d2->annee)
		return 1;
	if (d1->mois < d2->mois)
		return -1;
	if (d1->mois > d2->mois)
		return 1;
	if (d1->jour < d2->jour)
		return -1;
	if (d1->jour > d2->jour)
		return 1;
	return 0;
}

/** Fonction qui calcule le nombre d'années révolues de l'individu ind à la date d */
/** Par exemple, un individu né le 7 avril 1950 a 49 années révolues le 7 avril 2000 et en a 50 le 8 avril 2000 */
/** Complexité : \Theta(1) */
int age_revolu (const struct individu * ind, const struct date * d) {
	struct date naissance_copie = *(ind->naissance);
	unsigned res = d->annee - (naissance_copie.annee + 1);
	naissance_copie.annee += res + 1;
	if (comparer_date(&naissance_copie, d) < 0)
		res ++;
	return res;
}

/** Fonction qui affiche tous les opéras du tableau tab, de taille taille, */
/* créés avant que leur compositeur ait a années révolues */
/** Complexité : \Theta(N) si N est la taille du tableau */
void recherche_tab_opera_age (struct opera *tab[], int taille, int a) {
	int i, a_revolu, trouve = 0;
	for (i = 0; i < taille; i ++) {
		if ( (a_revolu = age_revolu(tab[i]->compositeur, tab[i]->date_creation)) < a ) {
			trouve = 1;
			afficher_opera(tab[i]);
			printf("[%s avait %d ans]\n", tab[i]->compositeur->nom, a_revolu);
		}
	}
	if (!trouve)
		printf("Aucun opéra créé avant que son compositeur ait %d années révolues\n", a);
}

/** Fonction qui renvoie la position de l'opéra le plus anciennement créé dans le tableau d'opéras tab */
/** Complexité : \Theta(N) si N est la taille du tableau */
int recherche_pos_opera_ancien (struct opera *tab[], int taille) {
	int i;
  int pos_opera_ancien = 0;
  for(i = 1; i < taille; i ++) {
    if ( comparer_date(tab[i]->date_creation,tab[pos_opera_ancien]->date_creation) < 0)
      pos_opera_ancien = i;
  }
  return pos_opera_ancien;
}

/** Fonction qui trie le tableau d'opéras tab, de taille taille, */
/* selon l'ordre chronologique de création (tri par sélection) */
/** Complexité : \Theta(N^2) si N est la taille du tableau */
void trier_tab_opera_creation (struct opera *tab[], int taille) {
  int i;
  int pos_opera_ancien;
  for(i = 0; i < taille-1; i ++){
    pos_opera_ancien = recherche_pos_opera_ancien(tab + i, taille - i);
    echanger_opera(&tab[i], &tab[i + pos_opera_ancien]);
  }
}
