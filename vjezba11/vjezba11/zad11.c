#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define VELICINA_HASH 11
//binarno stablo pretraživanja
typedef struct _gradovi* Pok;
typedef struct _gradovi {
	char ime[50];
	int stanovnici;
	Pok L;
	Pok D;
}gradovi;

//struktura drzave
typedef struct _drzave* Poz;
typedef struct _drzave {
	char ime[50];
	Pok data; //data pokazuje na stablo gradova
	Poz next;
}drzave;

int DodajDrzavu(Poz P, char* ime, char* ime_filea_grada)
{
	Poz q;
	q = (Poz)malloc(sizeof(drzave));
	if (!q)
	{
		printf("Neuspjela alokacija!\n");
		return 0;
	}

	Pok h;
	h = (Pok)malloc(sizeof(gradovi));
	if (!h)
	{
		printf("Neuspjela alokacija!\n");
		return 0;
	}
	//prazni čvor služi da ne moramo provjeravati NULL
	strcpy(h->ime, "");
	h->stanovnici = 0;
	h->L = NULL;
	h->D = NULL;

	strcpy(q->ime, ime);
	q->data = h;
	DodajGrad(q->data, ime_filea_grada);// otvara datoteku, čita gradove i svaki grad se ubacuje u stablo

	while (P->next != NULL && strcmp(P->next->ime, ime) < 0)//sortiranje po abecedi; prolazi kroz listu dok je njegovo ime manje od imena nove države
		P = P->next;
	q->next = P->next;
	P->next = q;

	return 0;
}

Pok UnosGradaUStablo(Pok P, char* ime, int stanovnici)
{
	Pok q;

	if (P == NULL)
	{
		q = (Pok)malloc(sizeof(gradovi)); // q je novi grad
		if (!q)
		{
			printf("Neuspjela alokacija!\n");
			return 0;
		}
		strcpy(q->ime, ime);
		q->stanovnici = stanovnici;
		q->L = NULL;
		q->D = NULL;
		return q; //VAMO IDE RETURN q!!!!!!! Na taj način omogućuje da se čvor poveže s roditeljem
	}
	else if (stanovnici < P->stanovnici)
		P->L = UnosGradaUStablo(P->L, ime, stanovnici); // novi grad ide lijevo
	else if (stanovnici > P->stanovnici)
		P->D = UnosGradaUStablo(P->D, ime, stanovnici); // novi grad ide desno
	else if (stanovnici == P->stanovnici)//sort po nazivu grada
	{
		if (strcmp(ime, P->ime) < 0)
			P->L = UnosGradaUStablo(P->L, ime, stanovnici); // ako je abecedno manji ide lijevo
		else
			P->D = UnosGradaUStablo(P->D, ime, stanovnici); // ako je abecedno veci ide desno
	}

	return P; // VRACA NEPROMJENJENI CVOR
}

int DodajGrad(Pok P, char* ime_filea_grada)
{
	char ime[50];
	int stanovnici;
	FILE* f;
	f = fopen(ime_filea_grada, "r"); // otvara datoteku za čitanje
	if (!f)
	{
		printf("Neuspjelo otvaranje filea!\n");
		return 0;
	}
	while (!feof(f)) // petlja traje dok ne dode do kraja datoteke
	{
		fscanf(f, "%s %d", ime, &stanovnici);
		P = UnosGradaUStablo(P, ime, stanovnici);
	}

	fclose(f);
	return 0;
}

int IspisDrzava(Poz P)
{
	while (P != NULL)
	{
		printf("%s\n", P->ime);
		IspisGradova(P->data->D);//u ispisu nece bit pocetne 0 ako se stavi P->data->D

		P = P->next;
	}

	return 0;
}

int IspisGradova(Pok P)//inorder
{
	if (P != NULL)
	{
		IspisGradova(P->L);
		printf("\t-%s %d\n", P->ime, P->stanovnici);
		IspisGradova(P->D);
	}
	return 0;
}


int Trazi(Poz P, char* ime_drzave, int br_stanovnika)
{
	while (P != NULL && strcmp(P->ime, ime_drzave) != 0) // dok lista nije gotova i dok ne nademo ime koje trazimo
		P = P->next;

	TraziGradove(P->data, br_stanovnika);
	return 0;
}

int TraziGradove(Pok P, int br_stanovnika)
{
	if (P != NULL)
	{
		if (P->stanovnici > br_stanovnika) //ako grad ima vise stanovnika ispise se ime grada
			printf("%s ", P->ime);

		TraziGradove(P->L, br_stanovnika);
		TraziGradove(P->D, br_stanovnika);
	}
	return 0;
}


int Hash_kljuc(char* rijec)//vraca kljuc
{
	int i, suma = 0;
	for (i = 0; i < 5; i++) // uzimamo prvih 5 slova imena drzave
		suma += (int)rijec[i]; 
	return suma % VELICINA_HASH; //dobivamo rezultat od 0 do 10 koji govori u koju "ladicu" hash funkcije ide drzava
}

int main()
{
	//deklariramo prakticki 11 headova za hash i onda na temelju kljuca dodajemo elemente u listu tog heada
	drzave Hash[VELICINA_HASH];
	for (int i = 0; i < VELICINA_HASH; i++)
	{
		Hash[i].data = NULL;
		strcpy(Hash[i].ime, "");
		Hash[i].next = NULL;
	}
	int kljuc;

	drzave Head;
	Head.next = NULL;
	Head.data = NULL;
	char ime[50], ime_filea_grada[50];
	FILE* f;
	f = fopen("drzave.txt", "r");
	if (!f)
	{
		printf("Neuspjelo otvaranje filea!\n");
		return 0;
	}

	while (!feof(f))
	{
		fscanf(f, "%s %s", ime, ime_filea_grada);

		kljuc = Hash_kljuc(ime); // odredi u koju "ladicu " hash tablice ide drzava

		DodajDrzavu(&Hash[kljuc], ime, ime_filea_grada);//sad ne dodajemo u head nego u hash od kljuca
	}

	fclose(f);

	for (int i = 0; i < VELICINA_HASH; i++)//kod ispisa ispisemo po redu svaki hash[kljuca]
		IspisDrzava(Hash[i].next);

	printf("\n");
	int br;
	printf("Unesite drzavu za pretrazit: ");
	scanf("%s", ime);
	printf("Unesite broj stanovnika: "); //ispisat ce gradove te drzave sa brojem stanovnika vecim od unesenog
	scanf("%d", &br);
	kljuc = Hash_kljuc(ime);
	Trazi(Hash[kljuc].next, ime, br);

	return 0;
}