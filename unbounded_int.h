#ifndef UNBOUNDED_INT_H
#define UNBOUNDED_INT_H

#include <stdlib.h>

typedef struct chiffre {
   struct chiffre *suivant;
   char c;
   struct chiffre *precedent;
} chiffre;

typedef struct {
   char signe;
   size_t len;
   chiffre *premier;
   chiffre *dernier;
} unbounded_int;

unbounded_int string2unbounded_int(const char *e);

unbounded_int ll2unbounded_int(long long i);

char *unbounded_int2string(unbounded_int i);

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

int unbounded_int_cmp_ll(unbounded_int a, long long b);

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b);

chiffre* position(unsigned long i, unbounded_int a);

int append(unbounded_int *u, char c);

unbounded_int invalid_unbounded_int();

static unbounded_int add_not_negatif_unbound_int(const unbounded_int a, const unbounded_int b);

unbounded_int sub_not_negatif_unbound_int(const unbounded_int a, const unbounded_int b);

chiffre *alloc_chiffre(char cChar);
void free_chiffre(chiffre *pxChiffre);
void free_unbounded_int(unbounded_int **ppxUnboundedInt);
void free_chiffres(chiffre* ch);

int str_start_with(const char *str, const char *prefix);
char *strstrip(char *s);

#endif