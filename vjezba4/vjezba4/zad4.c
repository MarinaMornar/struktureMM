#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE 1024

//definiramo strukturu
typedef struct _Polinom* list;
typedef struct _Polinom {
	int Koef;
	int Pot;
	list next;
}Polinom;

int Unos(list P, int k, int p);  //unosi novi član sortirano po potenciji
int Ispis(list P);                
int Unos_red(list P, int i);     //čita jedan red iz datoteke
int Zbroj(list P1, list P2, list P);
int Umnozak(list P1, list P2, list P);

int main()
{
	Polinom Head1 = { .next = NULL, .Koef = 0, .Pot = 0 };
	Polinom Head2 = { .next = NULL, .Koef = 0, .Pot = 0 };
	Polinom HeadZbr = { .next = NULL, .Koef = 0, .Pot = 0 };
	Polinom HeadMnoz = { .next = NULL, .Koef = 0, .Pot = 0 };

	//učitavanje polinoma iz datoteke
	Unos_red(&Head1, 1);
	Unos_red(&Head2, 2);
	//ispis oba polinoma
	printf("Prvi: ");
	Ispis(Head1.next);
	printf("Drugi: ");
	Ispis(Head2.next);

	printf("Zbroj: ");
	Zbroj(&Head1, &Head2, &HeadZbr);
	Ispis(HeadZbr.next);

	printf("Umnozak: ");
	Umnozak(&Head1, &Head2, &HeadMnoz);
	Ispis(HeadMnoz.next);

	return 0;
}


int Unos(list P, int k, int p)
{
	list q, Prev;
	Prev = P;
	P = P->next;

	while (P != NULL && P->Pot > p)
	{
		Prev = P;
		P = P->next;
	}

	if (P != NULL && P->Pot == p)
		P->Koef += k;

	else
	{
		q = (list)malloc(sizeof(Polinom));
		if (!q)
		{
			printf("Neuspjesno alociranje\n");
			return -1;
		}

		q->Koef = k;
		q->Pot = p;

		q->next = Prev->next;
		Prev->next = q;
	}

	return 0;
}
int Ispis(list P)
{
	while (P != NULL)
	{
		printf("%d %d ", P->Koef, P->Pot);
		P = P->next;
	}

	printf("\n");
	return 0;
}

int Unos_red(list P, int i)
{
	int k = 0;
	int p = 0;
	int n = 0;
	char buffer[MAX_LINE] = { 0 };
	char* temp;
	FILE* f;

	f = fopen("polinomi.txt", "r");
	if (!f)
	{
		printf("Greska, datoteka se nije otvorila.\n");
		return -1;
	}

	if (i == 2)
		fgets(buffer, MAX_LINE, f); //kako bi se pomakli red u fileu ni�e za 2. polinom

	fgets(buffer, MAX_LINE, f);
	temp = buffer;

	while (strlen(temp) > 0)
	{
		sscanf(temp, "%d %d %n", &k, &p, &n); //na ovaj nacin zaobilazimo nepravilnosti u datoteci
		temp += n;

		Unos(P, k, p);
	}
}

int Zbroj(list P1, list P2, list P)
{
	list temp;
	while (P1 != NULL && P2 != NULL)
	{
		if (P1->Pot == P2->Pot)
		{
			Unos(P, P1->Koef + P2->Koef, P1->Pot);
			P1 = P1->next;
			P2 = P2->next;
		}

		else if (P1->Pot > P2->Pot)
		{
			Unos(P, P1->Koef, P1->Pot);
			P1 = P1->next;
		}

		else
		{
			Unos(P, P2->Koef, P2->Pot);
			P2 = P2->next;
		}
	}

	if (P1 != NULL)
		temp = P1;
	else
		temp = P2;

	while (temp != NULL)
	{
		Unos(P, temp->Koef, temp->Pot);
		temp = temp->next;
	}

	return 0;
}

int Umnozak(list P1, list P2, list P)
{
	list Pocetak = P2;
	while (P1 != NULL)
	{
		P2 = Pocetak;
		while (P2 != NULL)
		{
			Unos(P, P1->Koef * P2->Koef, P1->Pot + P2->Pot);
			P2 = P2->next;
		}
		P1 = P1->next;
	}

	return 0;
}