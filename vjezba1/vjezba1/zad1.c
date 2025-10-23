#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MAX_NAME 50
#define MAX_LINE 1024
#define MAX_POINTS 15

typedef struct {
    char ime[MAX_NAME];
    char prezime[MAX_NAME];
    double bodovi;
} Student;

int brojRedakaUDatoteci()
{
    int brojac = 0;
    char redak[MAX_LINE];

    FILE* dat = fopen("students.txt", "r");
    if (dat == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return -1;
    }

    while (fgets(redak, MAX_LINE, dat) != NULL) {
        brojac++;
    }

    fclose(dat);
    return brojac;
}

int main()
{
    int i;
    int brojStudenata = brojRedakaUDatoteci();

    if (brojStudenata <= 0) {
        printf("Nema podataka u datoteci ili se datoteka ne moze otvoriti!\n");
        return -1;
    }

    FILE* dat = fopen("students.txt", "r");
    if (dat == NULL) {
        printf("Ne mogu ponovo otvoriti datoteku!\n");
        return -1;
    }

    Student* studenti = (Student*)malloc(brojStudenata * sizeof(Student));
    if (studenti == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        fclose(dat);
        return -1;
    }

    for (i = 0; i < brojStudenata; i++) {
        fscanf(dat, "%s %s %lf", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
    }

    printf("\nIme\t\tPrezime\t\tBodovi\t\tRelativno(%%)\n");

    for (i = 0; i < brojStudenata; i++) {
        double postotak = (studenti[i].bodovi / MAX_POINTS) * 100;
        printf("%-10s %-12s %.2lf\t\t%.2lf%%\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, postotak);
    }

    free(studenti);
    fclose(dat);

    return 0;
}