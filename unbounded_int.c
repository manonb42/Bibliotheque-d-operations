#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "unbounded_int.h"

// 1 // 
unbounded_int string2unbounded_int(const char *e) {
    unbounded_int new_unint = {'+', 0, NULL, NULL};

    if (e[0] == '+' || e[0] == '-') {
        if (strlen(e) == 1) {
            return invalid_unbounded_int();
        }
        new_unint.signe = e[0];
        e++;
    }
    if (strlen(e) == 0) {
        return invalid_unbounded_int();
    }
    while (e[0] == '0' && e[1] != 0) {
        e++;
    }
    for (size_t i = 0; i < strlen(e); i++) {
        if (!isdigit(e[i]) || !append(&new_unint, e[i])) {
            free_chiffre(new_unint.premier);
            return invalid_unbounded_int();
        }
    }
    return new_unint;
}


// 2 //  
unbounded_int ll2unbounded_int(long long i) {
    unbounded_int new_unint;

    if (i < 0) {
        new_unint.signe = '-';
        i = -i;
    } else {
        new_unint.signe = '+';
    }

    chiffre *premier = (chiffre *) malloc(sizeof(chiffre));
    chiffre *dernier = (chiffre *) malloc(sizeof(chiffre));
    chiffre *suiv = (chiffre *) malloc(sizeof(chiffre));

    if (premier == NULL || dernier == NULL || suiv == NULL) {
        if (premier != NULL) {
            free(premier);
        }
        if (dernier != NULL) {
            free(dernier);
        }
        if (suiv != NULL) {
            free(suiv);
        }
        return invalid_unbounded_int();
    }


    int len = 0;
    int value = (int) i;
    int r = value % 10;
    int quotient = value / 10;

    dernier->c = (char) (r + '0');
    dernier->suivant = NULL;
    suiv = dernier; // var tmp qui pointe vers dernier
    new_unint.dernier = dernier;
    len++;

    while (quotient > 9) {
        value = quotient;
        r = value % 10;
        quotient = value / 10;

        chiffre *courant = (chiffre *) malloc(sizeof(chiffre)); // chiffre ds la boucle
        if (courant == NULL) {
            free(premier);
            free_chiffre(suiv);
            return invalid_unbounded_int();
        }

        courant->c = (char) (r + '0');
        courant->suivant = suiv;
        suiv->precedent = courant;
        suiv = courant;    //courant devient le suivant pour la prochaine iteration
        len++;
    }

    premier->c = (char) (quotient + '0'); //quotient
    premier->precedent = NULL;
    premier->suivant = suiv;
    new_unint.premier = premier;
    len++;
    new_unint.len = len;
    return new_unint;
}

// 3 // 
char *unbounded_int2string(unbounded_int i) {
    char *new_char = (char *) malloc((i.len) * sizeof(char) + 1);
    assert(new_char);

    new_char[0] = i.signe;
    chiffre *ptr = i.premier;

    for (size_t j = 1; j <= i.len; j++) {
        new_char[j] = ptr->c;
        ptr = ptr->suivant;
    }
    return new_char;
}


// 4 //
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
    if (a.signe == '*' || b.signe == '*') {
        perror("Un des entiers est invalide.");
        exit(1);
    }

    if (a.signe != b.signe) {  // S'ils ne sont pas de meme signe
        if ((a.len == 1 && b.len == 1) && ((a.premier)->c == '0' && (b.premier)->c == '0')) {
            return 0;
        }
        if (a.signe == '+') {
            return 1;
        }
        return -1;

    } else {    // ils sont de meme signe
        if (a.len != b.len) {        // ils sont de meme signe mais pas de m�me longueur
            if (a.len < b.len) {
                if (a.signe == '-') {
                    return 1;
                }
                return -1;
            } else {
                if (a.signe == '-') {
                    return -1;
                }
                return 1;
            }
        } else {    // ils sont de meme signe et de même longueur
            chiffre *ptr_a = a.premier;
            chiffre *ptr_b = b.premier;
            do {
                if (ptr_a->c != ptr_b->c) {
                    if (ptr_a->c < ptr_b->c) {
                        if (a.signe == '+') {
                            return -1;
                        }
                        return 1;
                    } else {
                        if (a.signe == '+') {
                            return 1;
                        }
                        return -1;
                    }
                }
                ptr_a = ptr_a->suivant;
                ptr_b = ptr_b->suivant;
            } while (ptr_a->suivant != NULL && ptr_b->suivant != NULL);
            return 0; // Si on arrive ici ce qu'on a parcourir les deux unbounded jusq'au bout sans trouver de différence
        }
    }
}


// 5 // à changer 
int unbounded_int_cmp_ll(unbounded_int a, long long b) {
    return unbounded_int_cmp_unbounded_int(a, ll2unbounded_int(b));
}


// 6 // il faut enlever les 0 au debut 
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b) {

    unbounded_int unbounded_somme;
    unbounded_int abs_a = a;
    unbounded_int abs_b = b;

    abs_a.signe = '+';
    abs_b.signe = '+';

    if (a.signe == '+' && b.signe == '+') { // les deux sont positifs
        unbounded_somme = add_not_negatif_unbound_int(a, b);
    } else if (a.signe == '-' && b.signe == '-') { // les deux sont negatifs
        unbounded_somme = add_not_negatif_unbound_int(abs_a, abs_b);
        unbounded_somme.signe = '-';
    } else if (a.signe == '+' && b.signe == '-') { // a positif et b negatif

        if (unbounded_int_cmp_unbounded_int(a, abs_b) == -1) { // si a est plus petit que abs(b)
            unbounded_somme = sub_not_negatif_unbound_int(abs_b, a);
            unbounded_somme.signe = '-';
        } else { // si a est plus petit que abs(b)
            unbounded_somme = sub_not_negatif_unbound_int(a, abs_b);
        }
    } else { // si b est positif et a negatif
        if (unbounded_int_cmp_unbounded_int(b, abs_a) == -1) { // si b est plus petit que abs(a)
            unbounded_somme = sub_not_negatif_unbound_int(abs_a, b);
            unbounded_somme.signe = '-';
        } else { // si b est plus petit que abs(a)
            unbounded_somme = sub_not_negatif_unbound_int(b, abs_a);
        }
    }
    return unbounded_somme;
}

// 7 // il faut enlever les 0 au debut 
unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b) {

    unbounded_int unbounded_diff;
    unbounded_int abs_a = a;
    unbounded_int abs_b = b;

    if (a.signe == '+' && b.signe == '+') { // si les deux sont positifs
        if (unbounded_int_cmp_unbounded_int(a, b) == -1) { // si a est plus petit que b
            unbounded_diff = sub_not_negatif_unbound_int(b, a);
            unbounded_diff.signe = '-';
        } else { // si b est plus petit que a
            unbounded_diff = sub_not_negatif_unbound_int(a, b);
        }
    } else if (a.signe == '-' && b.signe == '-') { // si les deux sont n�gatifs
        abs_a.signe = '+';
        abs_b.signe = '+';

        if (unbounded_int_cmp_unbounded_int(abs_a, abs_b) == -1) { // si abs(a) est plus petit que abs(b)
            unbounded_diff = sub_not_negatif_unbound_int(abs_b, abs_a);
        } else { // si abs(b) est plus petit que abs(a)
            unbounded_diff = sub_not_negatif_unbound_int(abs_a, abs_b);
        }
    } else if (a.signe == '+' && b.signe == '-') { // si a est positif et b negatif
        abs_b.signe = '+';
        unbounded_diff = add_not_negatif_unbound_int(a, abs_b);
    } else { // si a est negatif et b positif
        abs_a.signe = '+';
        unbounded_diff = add_not_negatif_unbound_int(abs_a, b);
        unbounded_diff.signe = '-';
    }
    // est ce qu'il y a vrm besoin de faire ça ? sachant qu'on gere dej� les 0 dans les fonctions qui créent des unbounded int
    while (unbounded_diff.premier->c == '0' &&
           unbounded_diff.premier->suivant != NULL) { // s'il y a des 0 avant le nombre
        unbounded_diff.premier = unbounded_diff.premier->suivant;
        unbounded_diff.premier->precedent = NULL;
        unbounded_diff.len = unbounded_diff.len - 1;
        // il faut pas aussi faire genre
        // unbounded_diff.premier->suivant->precedent = unbounded_diff.premier ?
        // (lol)
    }
    if (unbounded_diff.len == 1 && unbounded_diff.premier->c == '0') {
        unbounded_diff.signe = '+';
    }

    return unbounded_diff;
}

// 8 // 

unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b) {
    unbounded_int mult_unbounded;
    int longueur = a.len + b.len;
    unsigned long n = a.len;
    unsigned long m = b.len;
    // mult_unbounded.len= longueur;

    chiffre *tmp = (chiffre *) malloc(sizeof(chiffre));
    if (tmp == NULL) {
        return invalid_unbounded_int();
    }
    tmp->c = '0';
    tmp->precedent = NULL;
    mult_unbounded.premier = tmp;

    for (int t = 1; t < longueur; t++) {
        chiffre *sv = (chiffre *) malloc(sizeof(chiffre));
        if (sv == NULL) {
            free_chiffre(mult_unbounded.premier); // ?
            // free(tmp) ?
            return invalid_unbounded_int();
        }
        sv->precedent = tmp;
        tmp->suivant = sv;
        tmp = sv;
        tmp->c = '0';
    }

    mult_unbounded.dernier = tmp;
    mult_unbounded.dernier->suivant = NULL;

    for (size_t i = 1; i < m; i++) {
        int r = 0;
        if (position(i, b)->c == '0') {
            continue;
        }
        for (size_t j = 0; j < n; j++) {
            int v = (position(i + j, mult_unbounded)->c - '0') + (position(j, a)->c - '0') * (position(i, b)->c - '0') +
                    r;
            position(i + j, mult_unbounded)->c = (v % 10) + '0';
            r = v / 10;
        }
        position(i + n, mult_unbounded)->c = r + '0';
    }
    if ((a.signe == '+' && b.signe == '+') || (a.signe == '-' && b.signe == '-')) { //signe
        mult_unbounded.signe = '+';
    } else {
        mult_unbounded.signe = '-';
    }
    mult_unbounded.len = n + m; // len


    // est ce qu'il y a vrm besoin de faire ça ? sachant qu'on gere dej� les 0 dans les fonctions qui cr�ent des unbounded int
    while (mult_unbounded.premier->c == '0' &&
           mult_unbounded.premier->suivant != NULL) { // s'il y a des 0 avant le nombre
        mult_unbounded.premier = mult_unbounded.premier->suivant;
        mult_unbounded.premier->precedent = NULL;
        mult_unbounded.len = mult_unbounded.len - 1;
        // il faut pas aussi faire genre
        // unbounded_diff.premier->suivant->precedent = unbounded_diff.premier ?
        // (lol)
    }
    if (mult_unbounded.len == 1 && mult_unbounded.premier->c == '0') {
        mult_unbounded.signe = '+';
    }
    return mult_unbounded;
}

// quotient 

// fonctions auxillaires
chiffre *position(unsigned long i, unbounded_int a) {
    chiffre *curs = a.dernier;
    for (unsigned long j = 0; j < i && curs != NULL; j++) {
        curs = curs->precedent;
    }
    return curs;
}

unbounded_int invalid_unbounded_int() {
    unbounded_int new_unint = {'*', 0, NULL, NULL};
    return new_unint;
}

int append(unbounded_int *u, char c) {
    chiffre *ch = (chiffre *) malloc(sizeof(chiffre));
    if (ch == NULL) {
        return 0;
    }
    ch->c = c;
    ch->suivant = NULL;
    ch->precedent = u->dernier;

    if (u->len == 0) {
        u->premier = ch;
    } else {
        u->dernier->suivant = ch;
    }
    u->dernier = ch;
    u->len++;

    return 1;
}

int prepend(unbounded_int *u, char c) {
    chiffre *ch = (chiffre *) malloc(sizeof(chiffre));
    if (ch == NULL) {
        return 0;
    }
    ch->c = c;
    ch->suivant = u->premier;
    ch->precedent = NULL;

    if (u->len == 0) {
        u->dernier = ch;
    } else {
        u->premier->suivant = ch;
    }
    u->premier = ch;
    u->len++;

    return 1;
}

chiffre *alloc_chiffre(char cChar) {
    printf("assign 31\n");
    chiffre *pxChiffre = (chiffre *) calloc(1, sizeof(chiffre));
    printf("assign 32\n");
    // ou malloc(sizeof(chiffre)) ?
    pxChiffre->c = cChar;
    printf("assign 33\n");
    return pxChiffre;
}

void free_chiffre(chiffre *pxChiffre) {
    free(pxChiffre);
}

void free_unbounded_int(unbounded_int **ppxUnboundedInt) {
    if (ppxUnboundedInt && *ppxUnboundedInt) {
        unbounded_int *pxUnboundedInt = *ppxUnboundedInt;
        size_t szCounter = 0;
        chiffre *pxChiffre = pxUnboundedInt->premier;

        while (szCounter < pxUnboundedInt->len) {
            chiffre *pxSuivant = pxChiffre->suivant;
            free(pxChiffre);
            pxChiffre = pxSuivant;

            ++szCounter;
        }

        free(*ppxUnboundedInt);
        *ppxUnboundedInt = NULL;
    }
}

void free_chiffres(chiffre* ch){
    chiffre* tmp = ch->suivant;
    free(ch);
    if(tmp != NULL){
        free_chiffre(tmp);
    }
}


static unbounded_int add_not_negatif_unbound_int(const unbounded_int a, const unbounded_int b) {
    unbounded_int res;
    res.signe = '+';

    chiffre *a_curr = a.dernier;
    chiffre *b_curr = b.dernier;
    int retenue = 0;

    while (a_curr != NULL || b_curr != NULL || retenue != 0) {
        int a_val = 0;
        int b_val = 0;
        if (a_curr != NULL) {
            a_val = a_curr->c - '0';
            a_curr = a_curr->precedent;
        }
        if (b_curr != NULL) {
            b_val = b_curr->c - '0';
            b_curr = b_curr->precedent;
        }
        int temp = a_val + b_val + retenue;
        retenue = temp >= 10 ? 1 : 0;
        if (!prepend(&res, '0' + temp - (retenue ? 10 : 0))) {
            free_chiffres(res.premier);
            return invalid_unbounded_int();
        }
    }
    return res;
}

unbounded_int sub_not_negatif_unbound_int(const unbounded_int a, const unbounded_int b) {

    unbounded_int unbounded_subs_pos;
    unbounded_subs_pos.signe = '+';

    chiffre *a_ptr = a.dernier;
    chiffre *b_ptr = b.dernier;
    chiffre *courant = NULL;
    chiffre *suiv = NULL;

    int r = 0;
    int len = 0;

    while (!(a_ptr == NULL && b_ptr == NULL)) {

        courant = (chiffre *) malloc(sizeof(chiffre));

        int a1 = (a_ptr->c) - '0';
        a_ptr = a_ptr->precedent;

        int b1;
        if (b_ptr == NULL) {
            b1 = 0;
        } else {
            b1 = (b_ptr->c) - '0';
            b_ptr = b_ptr->precedent;
        }
        if ((a1 - b1 + r) >= 0) {
            courant->c = (char) ((a1 - b1 + r) + '0');
            r = 0;
        } else {
            courant->c = (char) ((a1 - b1 + r + 10) + '0');
            r = -1;
        }
        if (len == 0) {
            courant->suivant = NULL;
            unbounded_subs_pos.dernier = courant;
            suiv = courant;
        } else {
            suiv->precedent = courant;
            courant->suivant = suiv;
            suiv = courant;
        }
        len++;

        if (a_ptr == NULL) {
            unbounded_subs_pos.premier = courant;
        }
    }
    unbounded_subs_pos.len = len;
    return unbounded_subs_pos;
}
