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
	}
	else {
		fclose(fp);
	}
}

int izbornik(const char* const imeDatoteke) {
	printf("====================");
	printf("Odaberite jednu od ponudenih opcija:");
	printf("====================\n");
	printf("\t\t\tOpcija 1: dodavanje artikla!\n");
	printf("\t\t\tOpcija 2: citanje artikla!\n");
	printf("\t\t\tOpcija 3: ispisivanje artikla!\n");
	printf("\t\t\tOpcija 4: pretrazivanje artikla!\n");
	printf("\t\t\tOpcija 5: promijena preferenci artikala!\n");
	printf("\t\t\tOpcija 6: brisanje artikla!\n");
	printf("\t\t\tOpcija 7: brisanje datoteke i izlazak iz programa!\n");
	printf("\t\t\tOpcija 8: izlaz iz programa!\n");
	printf("======================================\
======================================\n");
	int uvijet = 0;
	static ARTIKL* poljeArtikala = NULL;
	static ARTIKL* pronadeniArtikl = NULL;
	scanf("%d", &uvijet);
	switch (uvijet) {
	case 1:
		dodajArtikl("trgovina.bin");
		break;
	case 2:
		if (poljeArtikala != NULL) {
			free(poljeArtikala);
			poljeArtikala = NULL;
		}
		poljeArtikala = (ARTIKL*)citanjeArtikala("trgovina.bin");
		if (poljeArtikala == NULL) {
			exit(EXIT_FAILURE);
		}
		break;
	case 3:
		ispisivanjeArtikala(poljeArtikala);
		break;
	case 4:
		pronadeniArtikl = (ARTIKL*)pretrazivanjeArtikala(poljeArtikala);
		break;
	case 5:
		promjenaPreferenci(poljeArtikala, imeDatoteke);
		break;
	case 6:
		brisanjeAtikala(&pronadeniArtikl, poljeArtikala, imeDatoteke);
		break;
	case 7:
		brisanjeDatoteke(imeDatoteke, poljeArtikala, uvijet);
		if (uvijet == 0) {
			return uvijet;
		}
		break;
	case 8:
		uvijet = izlazIzPrograma(poljeArtikala);
		break;

	default:
		uvijet = 0;
	}
	return uvijet;
}

void dodajArtikl(const char* const imeDatoteke) {
	FILE* pF = fopen(imeDatoteke, "rb+");
	if (pF == NULL) {
		perror("Dodavanje artikala u datoteku trgovina.bin");
		exit(EXIT_FAILURE);
	}
	fread(&brojArtikala, sizeof(int), 1, pF);
	printf("Broj artikala u trgovini: %d.\n", brojArtikala);
	ARTIKL temp = { 0 };
	temp.br = brojArtikala;
	printf("Unesite kategoriju artikla!\n");
	getchar();
	scanf("%19[^\n]", temp.kategorija);
	printf("Unesite ime artikla!\n");
	getchar();
	scanf("%19[^\n]", temp.ime);
	printf("Unesite proizvodaca artikla!\n");
	getchar();
	scanf("%19[^\n]", temp.proizvodac);
	printf("Unesite id artikla!\n");
	getchar();
	scanf("%d", &temp.id);
	printf("Unesite kolicinu artikla!\n");
	getchar();
	scanf("%d", &temp.kolicina);
	printf("Unesite tezinu artikla u gramima!\n");
	getchar();
	scanf("%d", &temp.tezinaUGramima);
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

void* citanjeArtikala(const char* const imeDatoteke) {
	FILE* pF = fopen(imeDatoteke, "rb");
	if (pF == NULL) {
		perror("Ucitavanje artikala iz datoteke trgovina.bin");
		return NULL;
	}
	fread(&brojArtikala, sizeof(int), 1, pF);
	printf("Broj artikala u trgovini: %d.\n", brojArtikala);
	ARTIKL* poljeFilmova = (ARTIKL*)calloc(brojArtikala, sizeof(ARTIKL));
	if (poljeFilmova == NULL) {
		perror("Zauzimanje memorije za trgovinu");
		return NULL;
	}
	fread(poljeFilmova, sizeof(ARTIKL), brojArtikala, pF);
	return poljeFilmova;
}

void ispisivanjeArtikala(const ARTIKL* const poljeArtikala) {
	if (poljeArtikala == NULL) {
		printf("Polje artikala je prazno!\n");
		return;
	}
	int i;
	for (i = 0; i < brojArtikala; i++)
	{
		printf("Artikl broj %d.\tKategorija: %s\tID: %d.\tIme: %s\tProizvodac: %s\nKolicina: %d\tTezina u gramima: %dg\tGluten: %s\tSecer: %s\n\n",
			i + 1,
			(poljeArtikala + i)->kategorija,
			(poljeArtikala + i)->id,
			(poljeArtikala + i)->ime,
			(poljeArtikala + i)->proizvodac,
			(poljeArtikala + i)->kolicina,
			(poljeArtikala + i)->tezinaUGramima,
			(poljeArtikala + i)->gluten,
			(poljeArtikala + i)->secer);
	}
}

void* pretrazivanjeArtikala(ARTIKL* const poljeArtikala) {
	if (poljeArtikala == NULL) {
		printf("Polje artikala je prazno!\n");
		return NULL;
	}
	int i;
	char trazenoIme[20] = { '\0' };
	int trazeniBroj;
	int trazenaKategorija = 0;
	printf("Po kojij kategoriji zelite pretrazivati?\n(1. Ime, 2. ID, 3. Kategorija, 4. Proizvodac)\n");
	scanf("%d", &trazenaKategorija);
	switch (trazenaKategorija) {
	case 1:
		printf("Unesite ime proizvoda za pronalazak artikla.\n");
		getchar();
		scanf("%19[^\n]", trazenoIme);
		for (i = 0; i < brojArtikala; i++)
		{
			if (!strcmp(trazenoIme, (poljeArtikala + i)->ime)) {
				printf("Artikl je pronaden!\n");
			}
		}
		break;
	case 2:
		printf("Unesite ID proizvoda za pronalazak artikla.\n");
		scanf("%d", &trazeniBroj);
		for (i = 0; i < brojArtikala; i++)
		{
			if (trazeniBroj == (poljeArtikala + i)->id) {
				printf("Artikl je pronaden!\n");
			}
		}
		break;
	case 3:
		printf("Unesite kategoriju proizvoda za pronalazak artikla.\n");
		getchar();
		scanf("%19[^\n]", trazenoIme);
		for (i = 0; i < brojArtikala; i++)
		{
			if (!strcmp(trazenoIme, (poljeArtikala + i)->kategorija)) {
				printf("Artikl je pronaden!\n");
			}
		}
		break;
	case 4:
		printf("Unesite ime proizvodaca za pronalazak artikla.\n");
		getchar();
		scanf("%19[^\n]", trazenoIme);
		for (i = 0; i < brojArtikala; i++)
		{
			if (!strcmp(trazenoIme, (poljeArtikala + i)->proizvodac)) {
				printf("Artikl je pronaden!\n");
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

void brisanjeAtikala(ARTIKL** const pronadeniArtikl, const ARTIKL* const poljeArtikala, const char* const imeDatoteke) {
	FILE* pF = fopen(imeDatoteke, "wb");
	if (pF == NULL) {
		perror("Artikla studenta iz datoteke trgovina.bin");
		exit(EXIT_FAILURE);
	}
	fseek(pF, sizeof(int), SEEK_SET);
	int i;
	int noviBrojacStudenata = 0;
	for (i = 0; i < brojArtikala; i++)
	{
		if (*pronadeniArtikl != (poljeArtikala + i)) {
			fwrite((poljeArtikala + i), sizeof(ARTIKL), 1, pF);
			noviBrojacStudenata++;
		}
	}
	rewind(pF);
	fwrite(&noviBrojacStudenata, sizeof(int), 1, pF);
	fclose(pF);
	printf("Artikl je uspjesno obrisan!\n");
	*pronadeniArtikl = NULL;
}

void brisanjeDatoteke(const char* trgovina, ARTIKL* poljeArtikala, int uvijet) {
	printf("Zelite li uistinu obrisati datoteku %s, i izaci iz programa?\n", trgovina);
	printf("Utipkajte \"da\" ako uistinu zelite obrisati datoteku u suprotno utipkajte\"ne\"!\n");
	char potvrda[3] = { '\0' };
	scanf(" %2s", potvrda);
	if (!strcmp("da", potvrda)) {
		remove(trgovina) == 0 ? printf("Uspjesno obrisana datoteka %s!\n", trgovina) : printf("Neuspjesno brisanje datoteke %s!\n", trgovina);
	}
	FILE* fp = NULL;
	printf("Zelite li nastaviti s programom?\n");
	scanf(" %2s", potvrda);
	if (!strcmp("da", potvrda)) {
		inicijalizacija(fp);
	}
	else {
		uvijet = izlazIzPrograma(poljeArtikala);
	}
}

void promjenaPreferenci(ARTIKL* poljeArtikala, const char* const imeDatoteke) {
	FILE* pF = fopen(imeDatoteke, "rb+");
	if (pF == NULL) {
		perror("Promjena preferenci artikala u datoteci trgovina.bin");
		exit(EXIT_FAILURE);
	}

	char novoIme[20] = {" \0 "};
	int i, trazenaKategorija, noviIDiKol = 0;;
	printf("Upisite broj artikla koji želite promijeniti: ");
	scanf("%d", &i);
	printf("Koju kategoriju želite promijeniti?\n(1. Ime, 2. Kategoriju, 3. ID, 4. Proizvodac, 5. Tezinu, 6. Gluten, 7. Secer, 8. Kolicinu)\n");
	scanf("%d", &trazenaKategorija);

	switch (trazenaKategorija) {
	case 1:
		printf("Unesite novo ime artikla: ");
		getchar();
		scanf("%19[^\n]", novoIme);
		*(poljeArtikala + i - 1)->ime = *novoIme;

		break;
	case 2:
		printf("Unesite novu kategoriju artikla: ");
		getchar();
		scanf("%19[^\n]", novoIme);
		*(poljeArtikala + i - 1)->kategorija = *novoIme;
		break;
	case 3:
		printf("Unesite novi ID artikla: ");
		scanf("%d", &noviIDiKol);
		(poljeArtikala + i - 1)->id = noviIDiKol;
		break;
	case 4:
		printf("Unesite novg proizvodaca artikla: ");
		getchar();
		scanf("%19[^\n]", novoIme);
		*(poljeArtikala + i - 1)->proizvodac = *novoIme;
		break;
	case 5:
		printf("Unesite novu tezinu artikla: ");
		getchar();
		scanf("%19[^\n]", novoIme);
		(poljeArtikala + i - 1)->tezinaUGramima = *novoIme;
		break;
	case 6:
		printf("Unesite gluten artikla: ");
		getchar();
		scanf("%2[^\n]", novoIme);
		*(poljeArtikala + i - 1)->gluten = *novoIme;
		break;
	case 7:
		printf("Unesite secer artikla: ");
		getchar();
		scanf("%19[^\n]", novoIme);
		*(poljeArtikala + i - 1)->secer = *novoIme;
		break;
	case 8:
		printf("Unesite novu kolicinu artikla: ");
		scanf("%d", &noviIDiKol);
		(poljeArtikala + i - 1)->kolicina = noviIDiKol;
		break;
	default:
		printf("Krivi unos\n");
	 }
}
