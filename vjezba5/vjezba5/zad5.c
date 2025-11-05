#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024
//osnovna struktura
typedef struct _stog* Poz;
typedef struct _stog {
    double El;
    Poz next;
} stog;

int Push(Poz P, double El);
double Pop(Poz P); // POP vraća double
int racun(Poz P, char* filename);

int main()
{
    double rezultat = 0;
    stog Head;
    Head.next = NULL;

    rezultat = racun(&Head, "postfix.txt");
    printf("Rezultat: %lf\n", rezultat);

    return 0;
}
//umetanje elementa na vrh stoga
int Push(Poz P, double x)
{
    Poz q;

    q = (Poz)malloc(sizeof(stog));
    if (!q)
    {
        printf("Neuspjelo alociranje.\n");
        return -1;
    }

    q->El = x;
    q->next = P->next;
    P->next = q; //novi čvor postaje novi vrh

    return 0;
}
// uklanjamo i vraćamo element s vrha stoga
double Pop(Poz P)
{
    Poz temp = NULL;
    double broj = 0;

    temp = P->next;  //uzimamo pokazivač na trenutni vrh
    if (temp == NULL)
    {
        printf("Nema nijednog elementa u stogu.\n");
        return 0.0;
    }

    broj = temp->El;           //očitavamo tu vrijednost iz vrha
    P->next = P->next->next;  //zaobilazimo čvor koji smo uklonili
    free(temp);              //oslobađamo memoriju uklonjenog čvora

    return broj;
}
//učitavanje postfix izraza iz datoteke i računamo rezultat
int racun(Poz P, char* filename)
{
    FILE* f;
    char buffer[MAX_SIZE] = { 0 };
    char* current_buffer;
    double number = 0;
    int br = 0;
    int status = 0;
    char znak;
    double rezultat = 0;

    f = fopen(filename, "r");  //otvaramo datoteku kao tekst
    if (!f)
    {
        printf("File se nije otvorio.\n");
        return -1;
    }

    if (!fgets(buffer, MAX_SIZE, f)) {   //čita jedan red teksta iz datoteke i sprema ga u buffer
        printf("Datoteka je prazna ili greska pri citanju.\n");
        fclose(f);  
        return -1;
    }
    fclose(f);
    current_buffer = buffer;   //pokazivač current_buffer sad pokazuje na početak buffer niza

    //osigurava da prođemo kroz cijeli red
    while (strlen(current_buffer) > 0)
    {
        
        status = sscanf(current_buffer, "%lf %n", &number, &br);
        if (status == 1) // ako je broj uspješno pročitan
        {
            Push(P, number);  //stavljamo broj na vrh stoga
        }
        else 
        {
            int br2 = 0;
            if (sscanf(current_buffer, " %c %n", &znak, &br2) != 1) {   //uzima jedan znak i na kraju se vraća broj znakova koje je sscanf pročitao
                
                printf("Nepoznat token ili greska pri parsiranju.\n");
                return -1;
            }

            double b = Pop(P);  //vrh stoga je zadnji ubačeni broj tj. b
            double a = Pop(P);

            //provjera operatora
            if (znak == '+')
                rezultat = a + b;
            else if (znak == '*')
                rezultat = a * b;
            else if (znak == '-')
                rezultat = a - b;
            else if (znak == '/')
            {
                if (b == 0.0) {
                    printf("Greska: dijeljenje s nulom.\n");
                    return -1;
                }
                rezultat = a / b;
            }
            else
            {
                printf("Krivi znak: %c\n", znak);
                return -1;
            }
            Push(P, rezultat);   //rezultat stavljamo nazad na stog

            
            br = br2;
        }

        if (br == 0) break;

        current_buffer += br;  //prebacujemo pokazivač na sljedeći token
    }

    rezultat = Pop(P); 
    return (int)rezultat == rezultat ? (int)rezultat : rezultat; 
}
 