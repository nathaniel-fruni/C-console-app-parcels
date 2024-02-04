#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

typedef struct {
    char adresat[50];
    char mesto[50];
    float hmotnost;
    int poisteny;
} TBalik;

int porovnajHmotnost(const void *a, const void *b) {
    const TBalik *balikA = (const TBalik *)a;
    const TBalik *balikB = (const TBalik *)b;

    if (balikA->hmotnost < balikB->hmotnost) {
        return -1;
    } else if (balikA->hmotnost > balikB->hmotnost) {
        return 1;
    } else {
        return 0;
    }
}

int porovnajPoplatok (const void *a, const void *b) {
    const TBalik *balikA = (const TBalik *)a;
    const TBalik *balikB = (const TBalik *)b;

    float poplatokA = balikA->hmotnost * 3.0 + (balikA->poisteny ? 1.5 : 0);
    float poplatokB = balikB->hmotnost * 3.0 + (balikB->poisteny ? 1.5 : 0);
    if (poplatokA < poplatokB) {
        return -1;
    } else if (poplatokA > poplatokB) {
        return 1;
    } else {
        return 0;
    }
}

int porovnajAdresata(const void *a, const void *b) {
    return strcmp(((TBalik *)a)->adresat, ((TBalik *)b)->adresat);
}

float sucetHmotnosti(TBalik *baliky, int pocetBalikov, const char *adresat) {
    qsort(baliky, pocetBalikov, sizeof(TBalik), porovnajAdresata);

    // najdenie hladaneho adresata v usporiadanom poli balikov
    TBalik *vysledok = (TBalik *)bsearch(adresat, baliky, pocetBalikov, sizeof(TBalik), porovnajAdresata);

    // sucet hmotnosti balikov len hladaneho adresata
    float sucet = 0.0;
    int prvyVyskyt;

    if (vysledok != NULL) {
        vysledok++;
        if (strcmp(vysledok->adresat, adresat) == 0) prvyVyskyt = true;
        else prvyVyskyt = false;
        vysledok--;

        if (prvyVyskyt) {
            while (vysledok <= baliky + pocetBalikov && strcmp(vysledok->adresat, adresat) == 0) {
                sucet += vysledok->hmotnost;
                vysledok++;
            }
        } else {
            while (vysledok >= baliky && strcmp(vysledok->adresat, adresat) == 0) {
                sucet += vysledok->hmotnost;
                vysledok--;
            }
        }
    } else {
        return -1.0;
    }

    return sucet;
}

void zapisHmotnosti(TBalik *baliky, const char *nazovSuboru, const char *cieloveMesto) {
    FILE *fw = fopen(nazovSuboru, "w");
    if (fw == NULL) {
        printf("Chyba pri otvarani vystupneho suboru.\n");
        return;
    }

    int i = 0;

    while (strlen(baliky[i].mesto) > 0) {
        if (strcmp(baliky[i].mesto, cieloveMesto) == 0) {
            fprintf(fw, "%.2f\n", baliky[i].hmotnost);
        }
        i++;
    }

    fclose(fw);
}

int main() {

    // ULOHA 1
    FILE *fr;
    char nazovSuboru[50];
    int pocetBalikov = 0;
    char riadok[110];

    printf("Zadaj nazov suboru:\n");
    scanf("%49s", nazovSuboru);

    fr = fopen(nazovSuboru, "r");
    if (fr == NULL) {
        printf("Chyba pri otvarani suboru.");
        return 1;
    }

    // precitanie poctu balikov z prveho riadku
    fscanf(fr, "%d", &pocetBalikov);
    while (fgetc(fr) != '\n'); // nastavenie pointera na dalsi riadok

    TBalik* baliky;
    baliky = (TBalik *)malloc(pocetBalikov * sizeof(TBalik));

    // precitanie obsahu suboru a naplenenie pola
    for (int i = 0; i < pocetBalikov; i++) {
        if (fgets(riadok, sizeof(riadok), fr) != NULL) {
            sscanf(riadok, "%s %s %f %d",
                   baliky[i].adresat,
                   baliky[i].mesto,
                   &baliky[i].hmotnost,
                   &baliky[i].poisteny);
        } else {
            printf("Chyba pri citani %d. riadku v subore.", i + 1);
            break;
        }
    }

    fclose(fr);

    for (int i = 0; i < pocetBalikov; i++) {
        printf("%s %s %.2f %d\n", baliky[i].adresat, baliky[i].mesto, baliky[i].hmotnost, baliky[i].poisteny);
    }

    // ULOHA 2
    qsort(baliky, pocetBalikov, sizeof(TBalik), porovnajHmotnost);
    printf("Usporiadane podla hmotnosti:\n");
    for (int i = 0; i < pocetBalikov; i++) {
        printf("%s %s %.2f %d\n", baliky[i].adresat, baliky[i].mesto, baliky[i].hmotnost, baliky[i].poisteny);
    }

    qsort(baliky, pocetBalikov, sizeof(TBalik), porovnajPoplatok);
    printf("Usporiadane podla poplatku za dopravu:\n");
    for (int i = 0; i < pocetBalikov; i++) {
        printf("%s %s %.2f %d\n", baliky[i].adresat, baliky[i].mesto, baliky[i].hmotnost, baliky[i].poisteny);
    }

    // ULOHA 3
    char adresat[50];
    printf("Zadaj adresata, pre ktoreho chces sucet hmotnosti balikov:");
    scanf("%49s", adresat);

    float sucet = sucetHmotnosti(baliky, pocetBalikov, adresat);
    if (sucet != -1) {
        printf("%.2f\n", sucet);
    } else {
        printf("Adresat nenajdeny.\n");
    }

    // ULOHA 4
    char mesto[50];
    printf("Zadaj cielove mesto:");
    scanf("%49s", mesto);

    int obsahujeMesto = false;
    for (int i = 0; i < pocetBalikov; i++) {
        if (strcmp(baliky[i].mesto,mesto) == 0) {
            obsahujeMesto = true;
        }
    }

    if (obsahujeMesto) {
        zapisHmotnosti(baliky, "vystup3.txt", mesto);
    } else {
        printf("Mesto nenajdene.");
    }

    free(baliky);

    return 0;
}
