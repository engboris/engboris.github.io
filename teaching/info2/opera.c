/** Fonction qui alloue de l'espace sur le tas pour une chaîne de n caractères */
/** et renvoie l'adresse du bloc, de taille n+1, alloué */
char * allouer_chaine (int n) {
  char *str = malloc((n+1)*sizeof(char));
	if (str == NULL) {
		perror("Échec allocation chaine de caractères (struct individu ou ville)");
		exit(EXIT_FAILURE);
	}
  return str;
}

/** Fonction qui alloue de l'espace sur le tas pour une date */
/** et renvoie l'adresse du bloc alloué */
struct date * allouer_date (void) {
	struct date * d = malloc(sizeof(struct date));
	if (d == NULL) {
		perror("Échec allocation struct date");
		exit(EXIT_FAILURE);
	}
  return d;
}

/** Fonction qui alloue de l'espace sur le tas pour un individu */
/** et renvoie l'adresse du bloc alloué */
struct individu * allouer_individu (void) {
  struct individu * ind = malloc(sizeof(struct individu));
	if (ind == NULL) {
		perror("Échec allocation struct individu");
		exit(EXIT_FAILURE);
	}
  return ind;
}

/** Fonction qui alloue de l'espace sur le tas pour un opéra */
/* et renvoie l'adresse du bloc alloué */
struct opera * allouer_opera (void) {
		struct opera * op = malloc(sizeof(struct opera));
		if (op == NULL) {
			perror("Échec allocation opéra");
			exit(EXIT_FAILURE);
		}
		return op;
}

/** Fonction qui libère toute la mémoire occupée par l'individu pointé par ind */
void detruire_individu (struct individu ** ind) {
	free((*ind)->nom);
	free((*ind)->prenom);
	free((*ind)->naissance);
  free(*ind);
  *ind = NULL;
}

/** Fonction qui libère toute la mémoire occupée par l'opéra pointé par op */
void detruire_opera (struct opera ** op) {
	free((*op)->titre);
	free((*op)->ville_creation);
	free((*op)->date_creation);
	detruire_individu(&(*op)->compositeur);
	free(*op);
  *op = NULL;
}

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

/** Fonction qui crée et renvoie l'adresse d'un tableau d'opéras de taille taille */
struct opera ** allouer_tab_opera (int taille) {
	int i;
	struct opera ** res = malloc (sizeof(struct opera *) * taille);
	for (i = 0; i < taille; i ++)
		res[i] = allouer_opera();
	return res;
}

/** Fonction qui initiale un tableau d'opéras avec les opéras */
/* stockés dans le fichier de nom n_fich */
/* et renvoie l'adresse ainsi initialisé */
/* (l'entier pointé par le 2e paramètre d'entrée */
/* prend la valeur du nombre d'opéras référencés dans le tableau initialisé) */
struct opera ** initialiser_tab_opera_fichier (char *n_fich, int * taille) {
	int i;
  FILE * f;
  struct opera ** tab_res;
  struct opera * op = allouer_opera();
  /** Ouverture du fichier de nom n_fich en lecture */
	if ((f=fopen(n_fich,"r")) == NULL) {
		fprintf(stderr, "fichier %s introuvable \n", n_fich);
		exit(EXIT_FAILURE);
	}
	fscanf(f,"%d\n", taille);
  /* et stockage de son contenu dans le tableau tab_operas */
  tab_res = allouer_tab_opera(*taille);
	/* On suppose que les titres, villes de création et noms et prénoms */
  /* des compositeurs des opéras du fichier ENTREE ont au plus TAILLE_MAX caractères */
	op->titre = allouer_chaine(TAILLE_MAX);
	op->date_creation = allouer_date();
	op->ville_creation = allouer_chaine(TAILLE_MAX);
	op->compositeur = allouer_individu();
	op->compositeur->nom = allouer_chaine(TAILLE_MAX);
	op->compositeur->prenom = allouer_chaine(TAILLE_MAX);
	op->compositeur->naissance = allouer_date();
	/* récupération des opéras stockés dans le fichier */
	for (i = 0; i < *taille; i++) {
		fscanf(f,"%s\n", op->titre);
		fscanf(f,"%u/%u/%u %s\n", &op->date_creation->jour, &op->date_creation->mois, &op->date_creation->annee, op->ville_creation);
		fscanf(f,"%s %s (%u/%u/%u)\n", op->compositeur->prenom, op->compositeur->nom, &op->compositeur->naissance->jour, &op->compositeur->naissance->mois,&op->compositeur->naissance->annee);
    tab_res[i] = allouer_opera();
    tab_res[i]->titre = allouer_chaine(strlen(op->titre));
    strcpy(tab_res[i]->titre, op->titre);
    tab_res[i]->date_creation = allouer_date();
    *(tab_res[i]->date_creation) = *(op->date_creation);
    tab_res[i]->ville_creation = allouer_chaine(strlen(op->ville_creation));
    strcpy(tab_res[i]->ville_creation, op->ville_creation);
    tab_res[i]->compositeur = allouer_individu();
    tab_res[i]->compositeur->nom = allouer_chaine(strlen(op->compositeur->nom));
    strcpy(tab_res[i]->compositeur->nom, op->compositeur->nom);
    tab_res[i]->compositeur->prenom = allouer_chaine(strlen(op->compositeur->prenom));
    strcpy(tab_res[i]->compositeur->prenom, op->compositeur->prenom);
    tab_res[i]->compositeur->naissance = allouer_date();
    *(tab_res[i]->compositeur->naissance) = *(op->compositeur->naissance);
	}
	detruire_opera(&op);
  /* fermeture du fichier */
  fclose(f);
  return tab_res;
}
