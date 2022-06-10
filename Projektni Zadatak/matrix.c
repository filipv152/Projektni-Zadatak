#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "matrix.h"

static int brojArtikala = 0;

void inicijalizacija(FILE* fp) {
	fp = fopen("trgovina.bin", "rb");
	if (fp == NULL) {
		fp = fopen("trgovina.bin", "wb+");
		fwrite(&brojArtikala, sizeof(brojArtikala), 1, fp);
		fclose(fp);
	}
	else {
		fclose(fp);
	}
}

int izbornik(const char* const imeDatoteke) {
	printf("\t\t\t\t-Izbornik-\n");
	printf("\t\t\t\t~~~~~~~~~~\n");
	printf("--------------------");
	printf("Odaberite jednu od ponudenih opcija:");
	printf("--------------------\n");
	printf("\t\t\tOpcija 1: dodavanje artikla!\n");
	printf("\t\t\tOpcija 2: ispisivanje artikla!\n");
	printf("\t\t\tOpcija 3: pretrazivanje artikla!\n");
	printf("\t\t\tOpcija 4: promijena preferenci artikala!\n");
	printf("\t\t\tOpcija 5: brisanje artikla!\n");
	printf("\t\t\tOpcija 6: brisanje datoteke i izlazak iz programa!\n");
	printf("\t\t\tOpcija 7: izlaz iz programa!\n");
	printf("----------------------------------------------------------------------------\n");
	char uvjet;
	static ARTIKL* poljeArtikala = NULL;
	static ARTIKL* pronadeniArtikl = NULL;
	uvjet = getch();
	switch (uvjet) {
	case '1':
		dodajArtikl("trgovina.bin", poljeArtikala);
		break;
	case '2':
		poljeArtikala = (ARTIKL*)citanjeArtikala("trgovina.bin");
		if (poljeArtikala == NULL) {
			exit(EXIT_FAILURE);
		}
		ispisivanjeArtikala(poljeArtikala);
		break;
	case '3':
		pronadeniArtikl = (ARTIKL*)pretrazivanjeArtikala(poljeArtikala);
		break;
	case '4':
		promjenaKarakteristikaArikla(poljeArtikala, "trgovina.bin");
		break;
	case '5':
		pronadeniArtikl = (ARTIKL*)pretrazivanjeArtikala(poljeArtikala);
		brisanjeArtikala(&pronadeniArtikl, poljeArtikala, "trgovina.bin");
		break;
	case '6':
		brisanjeDatoteke("trgovina.bin");
		uvjet = izlazIzPrograma(poljeArtikala);
		break;
	case '7':
		uvjet = izlazIzPrograma(poljeArtikala);
		break;

	default:
		uvjet = 0;
	}
	return uvjet;
}

void dodajArtikl(const char* const imeDatoteke, const ARTIKL* poljeArtikala) {
	FILE* pF = fopen(imeDatoteke, "rb+");
	if (pF == NULL) {
		perror("Dodavanje artikala u datoteku trgovina.bin");
		exit(EXIT_FAILURE);
	}
	poljeArtikala = (ARTIKL*)citanjeArtikala("trgovina.bin");
	if (poljeArtikala == NULL) {
		exit(EXIT_FAILURE);
	}
	fread(&brojArtikala, sizeof(int), 1, pF);
	ARTIKL temp = { 0 };
	temp.br = brojArtikala;
	printf("Unesite kategoriju artikla!\n");
	getchar();
	scanf(" %19[^\n]", temp.kategorija);
	printf("Unesite ime artikla!\n");
	getchar();
	scanf("%39[^\n]", temp.ime);
	printf("Unesite proizvodaca artikla!\n");
	getchar();
	scanf("%39[^\n]", temp.proizvodac);
	printf("Unesite id artikla!\n");
	getchar();
	scanf("%d", &temp.id);
	printf("Unesite kolicinu artikla!\n");
	getchar();
	scanf("%d", &temp.kolicina);
	printf("Unesite masu artikla u gramima!\n");
	getchar();
	scanf("%d", &temp.masaUGramima);
	printf("Ima li proizvod glutena (Da/Ne)?\n");
	getchar();
	scanf("%2[^\n]", temp.gluten);
	printf("Ima li proizvod secera (Da/Ne)?\n");
	getchar();
	scanf("%2[^\n]", temp.secer);

	fseek(pF, sizeof(ARTIKL) * brojArtikala, SEEK_CUR);
	fwrite(&temp, sizeof(ARTIKL), 1, pF);
	rewind(pF);
	brojArtikala++;
	fwrite(&brojArtikala, sizeof(int), 1, pF);
	fclose(pF);
}

void* citanjeArtikala(const char* imeDatoteke) {
	FILE* pF = fopen("trgovina.bin", "rb");
	if (pF == NULL) {
		perror("Ucitavanje artikala iz datoteke trgovina.bin");
		return NULL;
	}
	fread(&brojArtikala, sizeof(int), 1, pF);
	ARTIKL* poljeArtikala = (ARTIKL*)calloc(brojArtikala, sizeof(ARTIKL));
	if (poljeArtikala == NULL) {
		perror("Zauzimanje memorije za trgovinu");
		return NULL;
	}
	fread(poljeArtikala, sizeof(ARTIKL), brojArtikala, pF);
	fclose(pF);
	return poljeArtikala;
}

void ispisivanjeArtikala(const ARTIKL* poljeArtikala) {
	if (poljeArtikala == NULL) {
		printf("Polje artikala je prazno!\n");
		return;
	}
	int i;
	for (i = 0; i < brojArtikala; i++)
	{
		printf("Artikl broj %d.\tKategorija: %s\tID: %d.\tIme: %s\tProizvodac: %s\nKolicina: %d\tMasa u gramima: %dg\tGluten: %s\tSecer: %s\n\n",
			i + 1,
			(poljeArtikala + i)->kategorija,
			(poljeArtikala + i)->id,
			(poljeArtikala + i)->ime,
			(poljeArtikala + i)->proizvodac,
			(poljeArtikala + i)->kolicina,
			(poljeArtikala + i)->masaUGramima,
			(poljeArtikala + i)->gluten,
			(poljeArtikala + i)->secer);
	}
}

void* pretrazivanjeArtikala(ARTIKL* poljeArtikala) {
	poljeArtikala = (ARTIKL*)citanjeArtikala("trgovina.bin");
	if (poljeArtikala == NULL) {
		exit(EXIT_FAILURE);
	}
	int i;
	char trazenoIme[20] = { '\0' };
	int trazeniBroj;
	char trazenaKategorija;
	printf("Po kojij kategoriji zelite pretrazivati?\n(1. Ime, 2. ID, 3. Kategorija, 4. Proizvodac)\n");
	trazenaKategorija = getch();
	switch (trazenaKategorija) {
	case '1':
		printf("Unesite ime proizvoda za pronalazak artikla.\n");
		getchar();
		scanf("%19[^\n]", trazenoIme);
		for (i = 0; i < brojArtikala; i++)
		{
			if (!strcmp(trazenoIme, (poljeArtikala + i)->ime)) {
				printf("Artikl je pronaden!\n");
				printf("Artikl broj %d.\tKategorija: %s\tID: %d.\tIme: %s\tProizvodac: %s\nKolicina: %d\tMasa u gramima: %dg\tGluten: %s\tSecer: %s\n\n",
					i + 1,
					(poljeArtikala + i)->kategorija,
					(poljeArtikala + i)->id,
					(poljeArtikala + i)->ime,
					(poljeArtikala + i)->proizvodac,
					(poljeArtikala + i)->kolicina,
					(poljeArtikala + i)->masaUGramima,
					(poljeArtikala + i)->gluten,
					(poljeArtikala + i)->secer);
				return (poljeArtikala + i);
			}
		}

		break;
	case '2':
		printf("Unesite ID proizvoda za pronalazak artikla.\n");
		scanf("%d", &trazeniBroj);
		for (i = 0; i < brojArtikala; i++)
		{
			if (trazeniBroj == (poljeArtikala + i)->id) {
				printf("Artikl je pronaden!\n");
				printf("Artikl broj %d.\tKategorija: %s\tID: %d.\tIme: %s\tProizvodac: %s\nKolicina: %d\tMasa u gramima: %dg\tGluten: %s\tSecer: %s\n\n",
					i + 1,
					(poljeArtikala + i)->kategorija,
					(poljeArtikala + i)->id,
					(poljeArtikala + i)->ime,
					(poljeArtikala + i)->proizvodac,
					(poljeArtikala + i)->kolicina,
					(poljeArtikala + i)->masaUGramima,
					(poljeArtikala + i)->gluten,
					(poljeArtikala + i)->secer);
				return (poljeArtikala + i);
			}
		}

		break;
	case '3':
		printf("Unesite kategoriju proizvoda za pronalazak artikla.\n");
		getchar();
		scanf("%19[^\n]", trazenoIme);
		for (i = 0; i < brojArtikala; i++)
		{
			if (!strcmp(trazenoIme, (poljeArtikala + i)->kategorija)) {
				printf("Artikl je pronaden!\n");
				printf("Artikl broj %d.\tKategorija: %s\tID: %d.\tIme: %s\tProizvodac: %s\nKolicina: %d\tMasa u gramima: %dg\tGluten: %s\tSecer: %s\n\n",
					i + 1,
					(poljeArtikala + i)->kategorija,
					(poljeArtikala + i)->id,
					(poljeArtikala + i)->ime,
					(poljeArtikala + i)->proizvodac,
					(poljeArtikala + i)->kolicina,
					(poljeArtikala + i)->masaUGramima,
					(poljeArtikala + i)->gluten,
					(poljeArtikala + i)->secer);
				return (poljeArtikala + i);
			}
		}

		break;
	case '4':
		printf("Unesite ime proizvodaca za pronalazak artikla.\n");
		getchar();
		scanf("%19[^\n]", trazenoIme);
		for (i = 0; i < brojArtikala; i++)
		{
			if (!strcmp(trazenoIme, (poljeArtikala + i)->proizvodac)) {
				printf("Artikl je pronaden!\n");
				printf("Artikl broj %d.\tKategorija: %s\tID: %d.\tIme: %s\tProizvodac: %s\nKolicina: %d\tMasa u gramima: %dg\tGluten: %s\tSecer: %s\n\n",
					i + 1,
					(poljeArtikala + i)->kategorija,
					(poljeArtikala + i)->id,
					(poljeArtikala + i)->ime,
					(poljeArtikala + i)->proizvodac,
					(poljeArtikala + i)->kolicina,
					(poljeArtikala + i)->masaUGramima,
					(poljeArtikala + i)->gluten,
					(poljeArtikala + i)->secer);
				return (poljeArtikala + i);
			}
		}

		break;
	default:
		printf("Krivo unesena vrijednost\n");
	}
	return NULL;
}

int izlazIzPrograma(ARTIKL* poljeArtikala) {
	free(poljeArtikala);
	return 0;
}

void brisanjeArtikala(ARTIKL** const trazeniArtikl, const ARTIKL* poljeArtikala, const char* trgovina) {
	FILE* pF = fopen(trgovina, "wb");
	if (pF == NULL) {
		perror("Brisanje artikla iz datoteke trgovina.bin");
		exit(EXIT_FAILURE);
	}
	fseek(pF, sizeof(int), SEEK_SET);
	int i;
	int noviBrojacArtikala = 0;
	for (i = 0; i < brojArtikala; i++)
	{
		if (*trazeniArtikl != (poljeArtikala + i)) {
			fwrite((poljeArtikala + i), sizeof(ARTIKL), 1, pF);
			noviBrojacArtikala++;
		}
	}
	rewind(pF);
	fwrite(&noviBrojacArtikala, sizeof(int), 1, pF);
	fclose(pF);
	printf("Artikl je uspjesno obrisan!\n");
	*trazeniArtikl = NULL;
}

void brisanjeDatoteke(const char* trgovina) {
	printf("Zelite li uistinu obrisati datoteku %s, i izaci iz programa?\n", trgovina);
	printf("Utipkajte \"da\" ako uistinu zelite obrisati datoteku u suprotno utipkajte\"ne\"!\n");
	char potvrda[3] = { '\0' };
	scanf(" %2s", potvrda);
	if (!strcmp("da", potvrda)) {
		remove(trgovina) == 0 ? printf("Uspjesno obrisana datoteka %s!\n", trgovina) : printf("Neuspjesno brisanje datoteke %s!\n", trgovina);
	}
	FILE* fp = NULL;
}

void promjenaKarakteristikaArikla(ARTIKL* poljeArtikala, const char* const imeDatoteke) {
	FILE* pF = fopen(imeDatoteke, "rb+");
	if (pF == NULL) {
		perror("Promjena preferenci artikala u datoteci trgovina.bin");
		exit(EXIT_FAILURE);
	}
	poljeArtikala = (ARTIKL*)citanjeArtikala("trgovina.bin");
	if (poljeArtikala == NULL) {
		exit(EXIT_FAILURE);
	}
	ARTIKL temp = { 0 };
	char trazenaKategorija;
	int i, noviBroj = 0;
	printf("Upisite broj artikla koji zelite promijeniti: ");
	scanf("%d", &i);
	printf("Koju kategoriju zelite promijeniti?\n(1. Ime, 2. Kategoriju, 3. ID, 4. Proizvodaca, 5. Masu, 6. Gluten, 7. Secer, 8. Kolicinu)\n");
	trazenaKategorija = getch();

	switch (trazenaKategorija) {
	case '1':
		printf("Unesite novo ime artikla: ");
		scanf(" %19[^\n]", poljeArtikala[i - 1].ime);
		break;
	case '2':
		printf("Unesite novu kategoriju artikla: ");
		scanf(" %19[^\n]", poljeArtikala[i - 1].kategorija);
		break;
	case '3':
		printf("Unesite novi ID artikla: ");
		scanf("%d", &poljeArtikala[i - 1].id);
		break;
	case '4':
		printf("Unesite novog proizvodaca artikla: ");
		scanf(" %39[^\n]", poljeArtikala[i - 1].proizvodac);
		break;
	case '5':
		printf("Unesite novu masu artikla: ");
		scanf("%d", &poljeArtikala[i - 1].masaUGramima);
		break;
	case '6':
		printf("Unesite gluten artikla: ");
		scanf(" %2[^\n]", poljeArtikala[i - 1].gluten);
		break;
	case '7':
		printf("Unesite secer artikla: ");
		scanf(" %2[^\n]", poljeArtikala[i - 1].secer);
		break;
	case '8':
		printf("Unesite novu kolicinu artikla: ");
		scanf("%d", &poljeArtikala[i - 1].kolicina);
		break;
	default:
		printf("Krivi unos\n");
		fclose(pF);
	}
	fseek(pF, 4, SEEK_SET);
	fseek(pF, sizeof(ARTIKL) * (i - 1), SEEK_CUR);
	fwrite((poljeArtikala + i - 1), sizeof(ARTIKL), 1, pF);
	fclose(pF);
}
