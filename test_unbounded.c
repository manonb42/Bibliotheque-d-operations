#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <string.h>


#include "unbounded_int.h" 



static void print_unbound_int(unbounded_int x){
	printf("Signe : %c\n", x.signe);
	printf("len : %zu\n", x.len);
	printf("Premier : %c\n", (x.premier)->c);
	if(x.len!=1){
		printf("Suivant du premier : %c\n", ((x.premier)->suivant)->c);
		printf("Precedent du dernier : %c\n", ((x.dernier)->precedent)->c);
	}
	printf("Dernier : %c\n", x.dernier->c);
	
	printf("\n--------------------------------------\n\n");
}

static void show_cmp_result(unbounded_int a, unbounded_int b){
	int result = unbounded_int_cmp_unbounded_int(a, b);
	
	if(result==-1){
		printf("%s < %s\n\n", unbounded_int2string(a), unbounded_int2string(b));
	}
	else if(result == 0) {
		printf("%s = %s\n\n", unbounded_int2string(a), unbounded_int2string(b));
	}
	else {
		printf("%s > %s\n\n", unbounded_int2string(a), unbounded_int2string(b));
	}
	
}

static void show_cmp_result_unb_lld(unbounded_int a, long long b){
	int result = unbounded_int_cmp_ll(a, b);
	
	if(result==-1){
		printf("%s < %lld\n\n", unbounded_int2string(a), b);
	}
	else if(result == 0) {
		printf("%s = %lld\n\n", unbounded_int2string(a), b);
	}
	else {
		printf("%s > %lld\n\n", unbounded_int2string(a), b);
	}
	
}



int main(){
	
    char *nombre="1";
    char nombre1[5]={'+','9','6','2','\0'};
    char nombre2[5]={'+','4','6','2','\0'};
    char nombre3[5]={'+','9','6','2','\0'};
    char nombre4[5]={'+','4','6','2','\0'};    
	char nombre5[5]={'+','9','6','2','\0'};
    char nombre6[6]={'-','4','6','2','3','\0'};
	long long y=5235689;

	printf("test fonction 1 \n");
    unbounded_int x = string2unbounded_int(nombre);
	print_unbound_int(x);

	
	printf("test fonction 2 \n");
	unbounded_int t = ll2unbounded_int(y);
	printf("Signe : %c\n", t.signe);
	printf("len : %zu\n", t.len);
	printf("Premier : %c\n", (t.premier)->c);
	printf("Suivant du premier : %c\n", ((t.premier)->suivant)->c);
	printf("Precedent du dernier : %c\n", ((t.dernier)->precedent)->c);
	printf("Dernier : %c\n", t.dernier->c);
	
	printf("\n--------------------------------------\n\n");


	printf("test fonction 3 \n");
	char* string = unbounded_int2string(x);
	printf("%s \n\n", string);


	
	printf("test fonction 4 \n");
	show_cmp_result(t, x);


	printf("test fonction 5 \n");
	show_cmp_result_unb_lld(x, y);

	
	printf("test fonction auxilaire pour la fonction 6 \n");
    unbounded_int somme = add_not_negatif_unbound_int(string2unbounded_int(nombre1), string2unbounded_int(nombre2));
    printf("Somme : %s\n\n", unbounded_int2string(somme));
    

	
	unbounded_int somme2 = add_not_negatif_unbound_int(string2unbounded_int(nombre5), string2unbounded_int(nombre6));
    printf("Somme : %s\n\n", unbounded_int2string(somme2));
	

	
    printf("test fonction auxilaire pour la fonction 7 \n");
    unbounded_int subs = sub_not_negatif_unbound_int(string2unbounded_int(nombre3), string2unbounded_int(nombre4));
    printf("Soustraction : %s\n\n", unbounded_int2string(subs));
    
    printf("test fonction 6 \n");
    unbounded_int somme_all = unbounded_int_somme(string2unbounded_int(nombre5), string2unbounded_int(nombre6));
    printf("La somme est : %s\n\n", unbounded_int2string(somme_all));
    
	printf("test fonction 7 \n");
    unbounded_int diff = unbounded_int_difference(string2unbounded_int(nombre5), string2unbounded_int(nombre6));
    printf("La difference est : %s\n\n", unbounded_int2string(diff));
    
	/*
	char* premier = "3";
	char* deuxieme = "5";
	unbounded_int first = string2unbounded_int(premier);
	unbounded_int second = string2unbounded_int(deuxieme);
	print_unbound_int(first);
	print_unbound_int(second);
	*/

	unbounded_int difference = unbounded_int_difference(first, second);
	char* diff = unbounded_int2string(difference);
	print("La difference est : %s\n", diff);

    return 0;
}

