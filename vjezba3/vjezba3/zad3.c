#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 20
#define MAX_LINE 1024

//struktura Person
typedef struct _Person* Poz;  // p je pokazivač na strukturu _Person
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
int UnosIza(char* x, char* name, char* surname, int birth, Poz P);
int UnosIspred(char* x, char* name, char* surname, int birth, Poz P);
int Ispis_uFile(Poz P, char* ime_filea);
int Unos_izFile(Poz P, char* ime_filea);
int SortListe(Poz P);

// stvaranje novog čvora(novi član liste)
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
    a->next = NULL;  // nema sljedećeg člana na kojeg ova osoba pokazuje

    return a;
}

// dinamički dodajemo novi element na početak liste
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
    P->next = a;   // pokazivač P sad pokazuje na novi čvor a

    return 0;
}

// ispis liste 
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
// umetanje na kraj liste
int UnosK(Poz P, char* name, char* surname, int birth)
{
    while (P->next != NULL)
        P = P->next;  //prolazimo kroz listu od jednog čvora do sljedećeg

    UnosP(P, name, surname, birth);

    return 0;
}
// pronalazi element po prezimenu
Poz Search(Poz P, char* surname)
{
    while (P != NULL && strcmp(P->surname, surname) != 0) // uspoređuje P->surname i surname i ako nisu isti uvjet je TRUE i nastavlja na sljedeći čvor
        P = P->next;

    return P;
}
// traži prethodnika čvora čije je prezime dano
Poz TraziPret(Poz P, char* surname)
{
    while (P->next != NULL && strcmp(P->next->surname, surname) != 0)
        P = P->next;

    if (P->next == NULL)
        return NULL;
    else
        return P;
}
// brisanje elementa po prezimenu
int Delete(Poz P, char* surname)
{
    Poz temp = NULL;
    P = TraziPret(P, surname);

    if (P == NULL)
        printf("Greska, nema elementa\n");
    else
    {
        temp = P->next;       //čvor koji se briše
        P->next = P->next->next;  //prethondnik preskače obrisani čvor
        free(temp);               // oslobađamo memoriju obrisanog čvora
    }

    return 0;
}
//unos novog elementa ispred i iza
int UnosIza(char* x, char* name, char* surname, int birth, Poz P)
{
    P = Search(P->next, x);   //tražimo čvor s prezimenom x i pozivamo UnosP da umetne novi nakon njega

    if (P != NULL)
        UnosP(P, name, surname, birth);
    else
        printf("Greska,element ne postoji.\n");

    return 0;
}

int UnosIspred(char* x, char* name, char* surname, int birth, Poz P)
{
    P = TraziPret(P, x);

    if (P != NULL)
        UnosP(P, name, surname, birth);
    else
        printf("Greska,element ne postoji.\n");

    return 0;
}
// upisuje listu u datoteku
int Ispis_uFile(Poz P, char* ime_filea)
{
    FILE* file = NULL;
    file = fopen(ime_filea, "w");
    if (!file)
    {
        printf("Datoteka se nije otvorila.\n");
        return -1;
    }

    while (P != NULL)
    {
        fprintf(file, "Name: %s, surname: %s, birth: %d\n", P->name, P->surname, P->birth);
        P = P->next;
    }

    fclose(file);

    return 0;
}
//čitanje iz datoteke u listu
int Unos_izFile(Poz P, char* ime_filea)
{
    FILE* file = fopen(ime_filea, "r"); //otvaranje datoteke
    if (!file)
    {
        printf("Datoteka se nije otvorila.\n");
        return -1;
    }

    char name[60], surname[60]; //rezerviramo prostor u memoriji za pohranjivanje tih podataka koje pročitamo iz datoteke
    int birth;

    while (fscanf(file, "%s %s %d", name, surname, &birth) == 3) //čitanje iz datoteke
    {
        UnosK(P, name, surname, birth); //dodaje novu osobu na kraj liste
    }

    fclose(file);
    return 0;
}

//sortiranje liste po prezimenima 
int SortListe(Poz P)
{
    
    Poz x = NULL, prevX = NULL, temp = NULL, end = NULL; // x=trenutni čvor koji uspoređujemo, prevX=prethodni,temp=varijabla za zamjenu čvorova

    while (P->next != end) // petlja se izvršava dok se ne sortira cijela lista
    {
        prevX = P;
        x = P->next;
        while (x->next != end)
        {
            if (strcmp(x->surname, x->next->surname) > 0) //ako je x veći od 0 treba ih zamijeniti
            {
                temp = x->next;        //privremeno spremamo sljedeći čvor
                prevX->next = temp;    //prethodni pokazuje na temp
                x->next = temp->next;  //x preskače temp i pokazuje na sljedeći
                temp->next = x;        //temp pokazuje na x

                x = temp;              // x pokazuje na novu poziciju
            }
            prevX = x;           // prebacujemo pokazivač na sljedeći čvor
            x = x->next;
        }
        end = x;
    }
    return 0;
}

int main()
{
    Poz P = NULL;
    Person Head = { .next = NULL, .name = {0}, .surname = {0}, .birth = 0 };  // glava liste bez podataka, izbjegavamo tretiranje prazne liste


    UnosP(&Head, "Ivo", "Ivic", 10);
    UnosP(&Head, "Pero", "Peric", 17);
    UnosK(&Head, "Ante", "Antic", 75);

    P = Search(Head.next, "Ivic");
    UnosP(P, "Bela", "Belic", 29);  //unosi iza searcha

    Delete(&Head, "Ivic");

    UnosIza("Belic", "Tonci", "Toncic", 64, &Head); //unosi iza danog prezimena(belic)

    UnosIspred("Belic", "Stipe", "Stipic", 46, &Head);

    SortListe(&Head);

    Ispis(Head.next);


    return 0;
}