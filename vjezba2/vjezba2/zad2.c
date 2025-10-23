#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 20

//struct _Person;
typedef struct _Person* Poz;
typedef struct _Person {
    char name[N];
    char surname[N];
    int birth;
    Poz next;
}Person;

Poz CreatePerson(char* name, char* surname, int birth);
int UnosP(Poz P, char* name, char* surname, int birth);
int Ispis(Poz P);
int UnosK(Poz P, char* name, char* surname, int birth);
Poz Search(Poz P, char* surname);
Poz TraziPret(Poz P, char* surname); //neka brise po prezimenu
int Delete(Poz P, char* surname);

Poz CreatePerson(char* name, char* surname, int birth)
{
    Poz a = NULL;
    a = (Poz)malloc(sizeof(Person));
    if (!a)
    {
        perror("Greska kod alociranja\n");
        return -1;
    }

    strcpy(a->name, name);
    strcpy(a->surname, surname);
    a->birth = birth;
    a->next = NULL;

    return a;
}

int UnosP(Poz P, char* name, char* surname, int birth)
{
    Poz a = NULL;
    a = (Poz)malloc(sizeof(Person));
    if (!a)
    {
        perror("Greska kod alociranja\n");
        return -1;
    }

    a = CreatePerson(name, surname, birth);
    if (!a)
        return -1;

    a->next = P->next;
    P->next = a;

    return 0;
}

int Ispis(Poz P)
{
    Poz temp = P;

    while (temp != NULL)
    {
        printf("Name: %s, surname: %s, birth: %d\n", temp->name, temp->surname, temp->birth);
        temp = temp->next;
    }

    return 0;
}

int UnosK(Poz P, char* name, char* surname, int birth)
{
    while (P->next != NULL)
        P = P->next;

    UnosP(P, name, surname, birth);
}

Poz Search(Poz P, char* surname)
{
    while (P != NULL && strcmp(P->surname, surname) != 0)
        P = P->next;

    return P;
}

Poz TraziPret(Poz P, char* surname)
{
    while (P->next != NULL && strcmp(P->next->surname, surname) != 0)
        P = P->next;

    if (P->next == NULL)
        return NULL;
    else
        return P;
}

int Delete(Poz P, char* surname)
{
    Poz temp = NULL;
    P = TraziPret(P, surname);

    if (P == NULL)
        printf("Greska, nema elementa\n");
    else
    {
        temp = P->next;
        P->next = P->next->next;
        free(temp);
    }

    return 0;
}

int main()
{
    Poz P = NULL;
    Person Head = { .next = NULL, .name = {0}, .surname = {0}, .birth = 0 };

    UnosP(&Head, "Ivo", "Ivic", 10);
    UnosP(&Head, "Pero", "Peric", 17);
    UnosK(&Head, "Ante", "Antic", 75);

    P = Search(Head.next, "Ivic");
    UnosP(P, "Bela", "Belic", 29);  //unosi iza searcha

    Delete(&Head, "Ivic");

    Ispis(Head.next);

    return 0;
}