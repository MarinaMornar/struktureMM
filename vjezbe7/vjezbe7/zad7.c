#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>



typedef struct _cvorStabla* Stablo;
typedef struct _cvorStabla {
	char ime[50];
	Stablo sibling; 
	Stablo child;   
} cvorStabla;

// stog za pamćenje putanje 
typedef struct _Stog* PozStog;
typedef struct _Stog {
	Stablo data;
	PozStog next;
} Stog;



// Stvaranje novog direktorija 
Stablo MakeDir(Stablo current, char* ime)
{
	Stablo q = (Stablo)malloc(sizeof(cvorStabla));
	if (!q)
	{
		printf("Greska pri alokaciji!\n");
		return current;
	}

	strcpy(q->ime, ime);
	q->sibling = NULL;
	q->child = NULL;

	if (current == NULL)
		return q;

	if (strcmp(current->ime, ime) > 0)
	{
		q->sibling = current;
		return q;
	}
	else if (strcmp(current->ime, ime) < 0)
		current->sibling = MakeDir(current->sibling, ime);
	else
	{
		printf("Direktorij vec postoji!\n");
		free(q);
	}
	return current;
}

// ulazak u direktorij 
Stablo ChangeDir(Stablo current, char* ime, PozStog stog)
{
	Stablo p = current->child;

	while (p != NULL)
	{
		if (strcmp(p->ime, ime) == 0)
		{
			PozStog q = (PozStog)malloc(sizeof(Stog));
			q->data = p;
			q->next = stog->next;
			stog->next = q;

			printf("Sad smo u: %s\n", p->ime);
			return p;
		}
		p = p->sibling;
	}

	printf("Direktorij ne postoji.\n");
	return current;
}

//vraćanje  u prethodni direktorij 
Stablo ChangeDirBack(Stablo current, PozStog stog)
{
	if (stog->next->next == NULL)
	{
		printf("Vec ste u root direktoriju!\n");
		return current;
	}

	PozStog temp = stog->next;
	Stablo p = temp->next->data;

	stog->next = temp->next;
	free(temp);

	printf("Sad smo u: %s\n", p->ime);
	return p;
}

// ispis direktorija 
void Dir(Stablo current)
{
	if (current == NULL)
	{
		printf("Direktorij je prazan.\n");
		return;
	}

	while (current != NULL)
	{
		printf("- %s\n", current->ime);
		current = current->sibling;
	}
}

// brisanje cijelog stabla
void BrisiSve(Stablo current)
{
	if (current == NULL)
		return;

	BrisiSve(current->sibling);
	BrisiSve(current->child);
	free(current);
}


void obicniPush(PozStog stog, Stablo root)
{
	PozStog q = (PozStog)malloc(sizeof(Stog));
	q->data = root;
	q->next = stog->next;
	stog->next = q;
}


int main()
{
	int izbor;
	char ime[50];

	cvorStabla Root;
	strcpy(Root.ime, "C:");
	Root.child = NULL;
	Root.sibling = NULL;

	Stablo current = &Root;

	Stog stog;
	stog.data = NULL;
	stog.next = NULL;

	obicniPush(&stog, current);

	printf("\nMENI:\n");
	printf("1 - md\n");
	printf("2 - cd dir\n");
	printf("3 - cd..\n");
	printf("4 - dir\n");
	printf("5 - brisi sve\n");
	printf("6 - izlaz\n");

	do
	{
		printf("\nIZBOR: ");
		scanf("%d", &izbor);

		switch (izbor)
		{
		case 1:
			printf("Ime direktorija: ");
			scanf("%s", ime);
			current->child = MakeDir(current->child, ime);
			break;

		case 2:
			printf("Ime direktorija: ");
			scanf("%s", ime);
			current = ChangeDir(current, ime, &stog);
			break;

		case 3:
			current = ChangeDirBack(current, &stog);
			break;

		case 4:
			Dir(current->child);
			break;

		case 5:
			BrisiSve(current->child);
			current->child = NULL;
			printf("Obrisano.\n");
			break;

		case 6:
			printf("Izlaz...\n");
			break;

		default:
			printf("Krivi unos!\n");
		}
	} while (izbor != 6);

	return 0;
}
