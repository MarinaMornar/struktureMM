#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//binarno stablo pretrazivanja
typedef struct _cvorStabla* Stablo;  //stablo je pokazivac na cvor
typedef struct _cvorStabla {
	int El;
	Stablo L;
	Stablo D;
}cvorStabla;

Stablo Dodaj(int x, Stablo S);
void Ispis_inorder(Stablo S);
Stablo Replace(Stablo S);
int Suma(Stablo S);
void Ispis_inorder_u_file(Stablo S, FILE* f);

int main()
{
	Stablo root = NULL;
	int suma = 0;
	/* zad pod a i b
	root = Dodaj(2, root);
	root = Dodaj(5, root);
	root = Dodaj(7, root);
	root = Dodaj(8, root);
	root = Dodaj(11, root);
	root = Dodaj(1, root);
	root = Dodaj(4, root);
	root = Dodaj(2, root);
	root = Dodaj(3, root);
	root = Dodaj(7, root);

	Ispis_inorder(root);

	printf("\nNakon replacea:\n");
	root = Replace(root);
	Ispis_inorder(root);
	*/
	int x;
	FILE* f;
	f = fopen("random stablo.txt", "w");
	if (!f)
	{
		printf("File se nije otvorio!\n");
		return 0;
	}
	srand(time(NULL));                        //generiranje slucajnih brojeva
	for (int i = 0; i < 20; i++)
	{
		x = (rand() % (90 - 10 + 1)) + 10;
		root = Dodaj(x, root);
	}
	root = Replace(root);
	Ispis_inorder_u_file(root, f);
	return 0;
}

Stablo Dodaj(int x, Stablo S)//u ovom zad je unos naopak
{							//livo ide veci, desno ide manji
	if (S == NULL)			//smiju se ponavljat elementi, tretiraju se kao veci
	{
		S = (Stablo)malloc(sizeof(cvorStabla));   //ako dodemo do praznog mjesta stvaramo novi cvor
		S->El = x;
		S->L = NULL;
		S->D = NULL;
	}
	else if (x > S->El || x == S->El)  //ako je  x veci ili jednak rekurzivno dodajemo u lijevo podstablo
		S->L = Dodaj(x, S->L);
	else if (x < S->El)               // ako je x manji dodajemo u desno podstablo
		S->D = Dodaj(x, S->D);
	return S;                         // vracamo KORIJEN STABLA( rekurzija se vraca prema gore)
}

void Ispis_inorder(Stablo S)        //ako cvor postoji prvo se ide lijevo i ispisuje se vrijednost pa se onda ide desno
{
	if (S != NULL)
	{
		Ispis_inorder(S->L);
		printf("%d ", S->El);
		Ispis_inorder(S->D);
	}
}
//zbroj svih potomaka cvora S
int Suma(Stablo S)                     
{
	int suma = 0;
	if (S == NULL)            //ako je stablo prazno suma je 0
		return suma + 0;
	if (S->L != NULL)                    //ako postoji lijevo dijete dodajemo njegovu vrijednost i zbroj njegovih potomaka
		suma += S->L->El + Suma(S->L);
	if (S->D != NULL)                    //istu stvar radimo i za desno dijete
		suma += S->D->El + Suma(S->D);
	return suma;
}

Stablo Replace(Stablo S)
{
	if (S != NULL)
	{
		S->El = Suma(S);       //prolazi kroz lijevo i desno podstablo i zbraja sve POTOMKE
		S->L = Replace(S->L);  //mijenja se vrijednost lijevog cvora i vrijednost njegove djece i pokazuje na IZMIJENJENO stablo
		S->D = Replace(S->D); //isto vrijedi za desno podstablo
	}
	return S;
}
//suma se racuna prije promjene djece

void Ispis_inorder_u_file(Stablo S, FILE* f)
{
	if (S != NULL)
	{
		Ispis_inorder_u_file(S->L, f);   // ulazimo u lijevo podstablo
		fprintf(f, "%d ", S->El);       // upisujemo vrijednost trenutnog cvora u datoteku
		Ispis_inorder_u_file(S->D, f);  // ulazimo u desno podstablo i opet vrijedi isto
	}
}