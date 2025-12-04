#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// struktura
typedef struct _popis* Poz2;  
typedef struct _popis {  // predstavlja jedan artikl na popisu
	char naziv[50];
	int kolicina;
	double cijena;
	Poz2 next;
} popis;

typedef struct _racuni* Poz;
typedef struct _racuni {  //predstavja jedan račun
	char ime[50];
	int year;
	int mon;
	int day;
	popis Head;
	Poz next;
} racuni;


int UnosPopisa(Poz2 P, char* naziv, int kolicina, double cijena)  //unosimo artikl u listu artikala
{
	Poz2 q = (Poz2)malloc(sizeof(popis));
	if (!q) {
		printf("Neuspjelo alociranje!\n");
		return -1;
	}
	//postavlja polja novog artikla
	strcpy(q->naziv, naziv);
	q->kolicina = kolicina;
	q->cijena = cijena;

	while (P->next != NULL && strcmp(P->next->naziv, naziv) < 0) // traži mjesto za umetanje artikla tako da lista ostane sortirana
		P = P->next;

	q->next = P->next;
	P->next = q;

	return 0;
}

//ubacuje novi račun u listu računa
Poz UnosRacuna(Poz P, char* ime, int year, int mon, int day)
{
	Poz q = (Poz)malloc(sizeof(racuni));
	if (!q) {
		printf("Neuspjelo alociranje!\n");
		return NULL;
	}
	//alocira novi čvor za račun
	strcpy(q->ime, ime);
	q->year = year;
	q->mon = mon;
	q->day = day;
	q->Head.next = NULL;

	
	while (P->next != NULL &&  // pronalazi mjesto za umetanje novog računa sortiranog po datumu
		(P->next->year < year ||
			(P->next->year == year && P->next->mon < mon) ||
			(P->next->year == year && P->next->mon == mon && P->next->day < day)))
	{
		P = P->next;
	}

	q->next = P->next;
	P->next = q;

	return q;
}

int ProcitajDatoteku(char* ime_filea, Poz P) //učitava sve račune iz datoteke
{
	char ime[50], naziv[50];
	int year = 0, mon = 0, day = 0, kolicina = 0;
	double cijena = 0;

	FILE* f = fopen(ime_filea, "r");
	if (!f) {
		printf("Datoteka se nije otvorila.\n");
		return -1;
	}

	while (fscanf(f, "%s", ime) == 1)
	{   //ako uspije otvara računi.txt 
		FILE* f1 = fopen(ime, "r");
		if (!f1) {
			printf("Datoteka (%s) se nije otvorila.\n", ime);
			continue;
		}

		fscanf(f1, "%d-%d-%d", &year, &mon, &day);

		
		Poz novi = UnosRacuna(P, ime, year, mon, day);

		while (fscanf(f1, "%s %d %lf", naziv, &kolicina, &cijena) == 3)
		{
			UnosPopisa(&(novi->Head), naziv, kolicina, cijena);
		}

		fclose(f1);
	}

	fclose(f);
	return 0;
}

int Ispis(Poz P)
{
	while (P != NULL)
	{
		printf("\nRacun: %s (%d-%02d-%02d)\n", P->ime, P->year, P->mon, P->day);

		Poz2 P2 = P->Head.next;
		while (P2 != NULL)
		{
			printf("%s %d %.2lf\n", P2->naziv, P2->kolicina, P2->cijena);
			P2 = P2->next;
		}
		P = P->next;
	}
	return 0;
}

int main()
{
	racuni Head;
	Head.next = NULL;

	ProcitajDatoteku("racuni.txt", &Head);

	Ispis(Head.next);

	return 0;
}
