#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//cvor binarnog stabla gradova
typedef struct _gradovi* Pok;
typedef struct _gradovi {
	char ime[50];
	int stanovnici;
	Pok L;
	Pok D;
}gradovi;

// cvor vezane liste drzava
typedef struct _drzave* Poz;
typedef struct _drzave {
	char ime[50];
	Pok data;
	Poz next;
}drzave;

int DodajDrzavu(Poz P, char* ime, char* ime_filea_grada)
{
	Poz q;  // q je novi cvor drzave
	q = (Poz)malloc(sizeof(drzave));  // alokacija memorije za drzavu
	if (!q)
	{
		printf("Neuspjela alokacija!\n");
		return 0;
	}

	Pok h;
	h = (Pok)malloc(sizeof(gradovi));  //pomocni cvor stabla gradova
	if (!h)
	{
		printf("Neuspjela alokacija!\n");
		return 0;
	}
	strcpy(h->ime, "");
	h->stanovnici = 0;
	h->L = NULL;
	h->D = NULL;
	//ucitavanje gradova u stablo
	strcpy(q->ime, ime);
	q->data = h;
	DodajGrad(q->data, ime_filea_grada);
	//sortirano po imenu ubacujemo u listu
	while (P->next != NULL && strcmp(P->next->ime, ime) < 0)//pomice pokazivac kroz listu dok god sljed drzava dolazi abecedno prije nove drzave koju ubacujemo
		P = P->next;
	q->next = P->next;  // ubacujemo drzavu u listu
	P->next = q;

	return 0;
}

Pok UnosGradaUStablo(Pok P, char* ime, int stanovnici)
{
	Pok q;

	if (P == NULL)     // tu nema cvora i treba ubaciti novi grad
	{
		q = (Pok)malloc(sizeof(gradovi));
		if (!q)
		{
			printf("Neuspjela alokacija!\n");
			return 0;
		}
		strcpy(q->ime, ime);
		q->stanovnici = stanovnici;
		q->L = NULL;
		q->D = NULL;
		return q; //VAMO IDE RETURN q!!!!!!! Tako da se novi cvor poveze s roditeljskim cvorom 
	}
	else if (stanovnici < P->stanovnici) // novi grad ima manje stanovnika pa ide u lijevo podstablo
		P->L = UnosGradaUStablo(P->L, ime, stanovnici);
	else if (stanovnici > P->stanovnici) // ima vise stanovnika pa ide desno
		P->D = UnosGradaUStablo(P->D, ime, stanovnici);
	else if (stanovnici == P->stanovnici)//sort po nazivu grada
	{
		if (strcmp(ime, P->ime) < 0) // ako ime dolazi abecedno prije ide lijevo
			P->L = UnosGradaUStablo(P->L, ime, stanovnici);
		else
			P->D = UnosGradaUStablo(P->D, ime, stanovnici);
	}

	return P;
}

int DodajGrad(Pok P, char* ime_filea_grada)
{
	char ime[50];
	int stanovnici;
	FILE* f;
	f = fopen(ime_filea_grada, "r");    //otvara datoteku gradova
	if (!f)
	{
		printf("Neuspjelo otvaranje filea!\n");
		return 0;
	}
	while (!feof(f))    //cita gradove dok ne dode do kraja datoteke                              
	{
		fscanf(f, "%s %d", ime, &stanovnici);        //cita ime grada i broj stanovnika
		P = UnosGradaUStablo(P, ime, stanovnici);    //ubacuje svaki u stablo
	}

	fclose(f);
	return 0;
}

int IspisDrzava(Poz P)
{
	while (P != NULL)
	{
		printf("%s\n", P->ime); // ispis naziva drzave
		IspisGradova(P->data);//poziva funkciju za ispis stabla gradova koje pripada drzavi koja se obraduje

		P = P->next;
	}

	return 0;
}

int IspisGradova(Pok P)//inorder
{
	if (P != NULL)
	{
		IspisGradova(P->L); // ispis svih gradova koji su manji
		printf("\t-%s %d\n", P->ime, P->stanovnici); //ispis trenutnog cvora
		IspisGradova(P->D); //ispis svih gradova koji su veci
	}
	return 0;
}


int Trazi(Poz P, char* ime_drzave, int br_stanovnika)
{
	while (P != NULL && strcmp(P->ime, ime_drzave) != 0) // dok ne dodemo do kraja liste i dok ne nademo trazeno ime drzave
		P = P->next;

	TraziGradove(P->data, br_stanovnika); //ispisat ce samo gradove s vise stanovnika od br_stanovnika
	return 0;
}

int TraziGradove(Pok P, int br_stanovnika)
{
	if (P != NULL)
	{
		if (P->stanovnici > br_stanovnika)  //ako grad ima vise stanovnika od zadanog broja
			printf("%s ", P->ime);

		TraziGradove(P->L, br_stanovnika);
		TraziGradove(P->D, br_stanovnika);
	}
	return 0;
}

int main()
{
	drzave Head;
	Head.next = NULL;
	Head.data = NULL;
	char ime[50], ime_filea_grada[50];
	FILE* f;
	f = fopen("drzave.txt", "r");    //otvaramo datoteku za citanje 
	if (!f)
	{
		printf("Neuspjelo otvaranje filea!\n");
		return 0;
	}

	while (!feof(f))
	{
	    fscanf(f, "%s %s", ime, ime_filea_grada);
		DodajDrzavu(&Head, ime, ime_filea_grada);
	}

	fclose(f);

	IspisDrzava(Head.next);

	printf("\n");
	Trazi(Head.next, "Francuska", 500000);//trazimo gradove francuske sa 
	//sa brojem stanovnika vecim od 500000
	return 0;
}