#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

//binarno stablo pretrazivanja
typedef struct _cvorStabla* Stablo;
typedef struct _cvorStabla {
	int El;
	Stablo L;
	Stablo D;
}cvorStabla;

Stablo StvoriPrazno(Stablo S);
Stablo Dodaj(int x, Stablo S);
Stablo TraziMin(Stablo S);
Stablo Brisi(int x, Stablo S);
Stablo Trazi(int x, Stablo S);
void Ispis_inorder(Stablo S);
void Ispis_postorder(Stablo S);
void Ispis_preorder(Stablo S);

int main()
{
	//inicijalizacija stabla
	int i, x, y, z;
	Stablo root = NULL;
	Stablo Temp = NULL;

	
	//početno punjenje stabla
	root = Dodaj(4, root);
	root = Dodaj(9, root);
	root = Dodaj(1, root);
	root = Dodaj(11, root);
	root = Dodaj(7, root);

	printf("MENI:\n");
	printf("Unos - 1\nIspis inorder - 2\nIspis preorder - 3"
		"\nIspis postorder - 4\nBrisi - 5\nTrazi - 6\nKraj - 7\n");
	do
	{
		printf("IZBOR: ");
		scanf("%d", &i);
		if (i == 1)
		{
			printf("Unesi broj za unit: ");
			scanf("%d", &x);
			root = Dodaj(x, root);
		}
		else if (i == 2)
			Ispis_inorder(root);
		else if (i == 3)
			Ispis_preorder(root);
		else if (i == 4)
			Ispis_postorder(root);
		else if (i == 5)
		{
			printf("Unesi broj za brisat: ");
			scanf("%d", &y);
			root = Brisi(y, root);
		}
		else if (i == 6)
		{
			printf("Unesi broj za trazit: ");
			scanf("%d", &z);
			Temp = Trazi(z, root);
			if (Temp != NULL)
			{
				printf("Nadeni El: %d\n", Temp->El);
				if (Temp->L != NULL)
					printf("Njegov L: %d\n", Temp->L->El);
				if (Temp->D != NULL)
					printf("Njegov D: %d\n", Temp->D->El);
			}
			else
				printf("Ne postoji taj el!\n");
		}
		else if (i != 7)
			printf("Krivi unos!\n");
		printf("\n");
	} while (i != 7);
	return 0;
}

Stablo StvoriPrazno(Stablo S)
{
	if (S != NULL)
	{
		StvoriPrazno(S->L);
		StvoriPrazno(S->D);
		free(S);
	}
	return NULL;
}

Stablo Dodaj(int x, Stablo S) 
{
	if (S == NULL)  //ovdje treba stvoriti novi čvor
	{
		S = (Stablo)malloc(sizeof(cvorStabla));
		S->El = x;
		S->L = NULL;
		S->D = NULL;
	}
	else if (x < S->El)           //ide u lijevo podstablo
		S->L = Dodaj(x, S->L);  
	else if (x > S->El)          //ide u desno podstablo
		S->D = Dodaj(x, S->D);  
	return S;  //moramo vratit pokazivač na čvor
}

Stablo TraziMin(Stablo S)
{
	while (S->L != NULL)
		S = S->L;
	return S;
}

Stablo Brisi(int x, Stablo S)
{
	Stablo Temp;
	if (S == NULL)
		printf("Nema ga!\n");
	else if (x < S->El)           //ne brišemo, nastavljamo tražit lijevo
		S->L = Brisi(x, S->L);   //vraća novi korijen tog podstabla
	else if (x > S->El)
		S->D = Brisi(x, S->D);
	else if (S->L != NULL && S->D != NULL)
	{
		//nađemo najmanji element desnog podstabla i zamijenimo vrijednost
		Temp = TraziMin(S->D);       
		S->El = Temp->El;
		S->D = Brisi(S->El, S->D); //brišemo kopirani element
	}
	else
	{
		Temp = S;
		if (S->L == NULL)
			S = S->D;
		else
			S = S->L;
		free(Temp);
	}
	return S;
}

Stablo Trazi(int x, Stablo S)
{
	if (S == NULL)
		return NULL;
	else if (x < S->El)         //manji su u lijevom podstablu
		return Trazi(x, S->L);
	else if (x > S->El)         //veci su u desnom podstablu
		return Trazi(x, S->D);
	else
		return S;
}

void Ispis_inorder(Stablo S)
{
	if (S != NULL)
	{
		Ispis_inorder(S->L);
		printf("%d ", S->El);
		Ispis_inorder(S->D);
	}
}

void Ispis_postorder(Stablo S)
{
	if (S != NULL)
	{
		Ispis_postorder(S->L);
		Ispis_postorder(S->D);
		printf("%d ", S->El);
	}
}

void Ispis_preorder(Stablo S)
{
	if (S != NULL)
	{
		printf("%d ", S->El);
		Ispis_preorder(S->L);
		Ispis_preorder(S->D);
	}
}