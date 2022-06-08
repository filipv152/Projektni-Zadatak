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
	printf("\t\t\t\t-Izbornik-\n");
	printf("\t\t\t\t~~~~~~~~~~\n");
	printf("--------------------");
	printf("Odaberite jednu od ponudenih opcija:");
	printf("--------------------\n");
	printf("\t\t\tOpcija 1: dodavanje artikla!\n");
	printf("\t\t\tOpcija 2: citanje artikla!\n");
	printf("\t\t\tOpcija 3: ispisivanje artikla!\n");
	printf("\t\t\tOpcija 4: pretrazivanje artikla!\n");
	printf("\t\t\tOpcija 5: promijena preferenci artikala!\n");
	printf("\t\t\tOpcija 6: brisanje artikla!\n");
	printf("\t\t\tOpcija 7: brisanje datoteke i izlazak iz programa!\n");
	printf("\t\t\tOpcija 8: izlaz iz programa!\n");
	printf("----------------------------------------------------------------------------\n");
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
		printf("prije pa: %p\n", pronadeniArtikl);
		pronadeniArtikl = (ARTIKL*)pretrazivanjeArtikala(poljeArtikala);
		printf("poslje pa: %p\n", pronadeniArtikl);
		break;
	case 5:
		promjenaPreferenci(poljeArtikala, "trgovina.bin");
		break;
	case 6:
		printf("predana memorija: %p\n", pronadeniArtikl);
		brisanjeArtikala(&pronadeniArtikl, poljeArtikala, "trgovina.bin");
		break;
	case 7:
		brisanjeDatoteke("trgovina.bin");
		uvijet = izlazIzPrograma(poljeArtikala);
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
	scanf("%39[^\n]", temp.kategorija);
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
	ARTIKL* poljeArtikala = (ARTIKL*)calloc(brojArtikala, sizeof(ARTIKL));
	if (poljeArtikala == NULL) {
		perror("Zauzimanje memorije za trgovinu");
		return NULL;
	}
	fread(poljeArtikala, sizeof(ARTIKL), brojArtikala, pF);
	fclose(pF);
	return poljeArtikala;
}

void ispisivanjeArtikala(const ARTIKL* const poljeArtikala) {
	if (poljeArtikala == NULL) {
		printf("Polje artikala je prazno!\n");
		return;
	}
	int i;
	for (i = 0; i < brojArtikala; i++)
	{
		printf("mem: %p\tArtikl broj %d.\tKategorija: %s\tID: %d.\tIme: %s\tProizvodac: %s\nKolicina: %d\tTezina u gramima: %dg\tGluten: %s\tSecer: %s\n\n",
			(poljeArtikala+i),
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
				return (poljeArtikala + i);
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
				return (poljeArtikala + i);
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
				return (poljeArtikala + i);
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

void brisanjeArtikala(ARTIKL** const trazeniArtikl, const ARTIKL* const poljeArtikala, const char* const trgovina) {
	FILE* pF = fopen(trgovina, "wb");
	if (pF == NULL) {
		perror("Brisanje studenta iz datoteke studenti.bin");
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

void promjenaPreferenci(ARTIKL* poljeArtikala, const char* const imeDatoteke) {
	FILE* pF = fopen(imeDatoteke, "rb+");
	if (pF == NULL) {
		perror("Promjena preferenci artikala u datoteci trgovina.bin");
		exit(EXIT_FAILURE);
	}
	ARTIKL temp = { 0 };
	char novoIme[20] = {" \0 "};
	int i, trazenaKategorija, noviBroj = 0;;
	printf("Upisite broj artikla koji zelite promijeniti: ");
	scanf("%d", &i);
	printf("Koju kategoriju zelite promijeniti?\n(1. Ime, 2. Kategoriju, 3. ID, 4. Proizvodac, 5. Tezinu, 6. Gluten, 7. Secer, 8. Kolicinu)\n");
	scanf("%d", &trazenaKategorija);

	switch (trazenaKategorija) {
	case 1:
		printf("Unesite novo ime artikla: ");
		getchar();
		scanf("%s", &novoIme);
		*(poljeArtikala + i - 1)->ime = *novoIme;
		while (fread(&temp, sizeof(temp), 1, pF) == 1) {
			if (strcmp(temp.ime, novoIme) == 0) {
				fseek(pF, sizeof(temp), 1);
				fwrite(&temp, sizeof(temp), 1, pF);
				if (temp.ime == novoIme) {
					printf("\n Uspiješno azurirano!\n");
				}
				else {
					printf("\n Pogreska u azuriranju!\n");
				}
			}
		}
		fclose(pF);
		break;
	case 2:
		printf("Unesite novu kategoriju artikla: ");
		getchar();
		scanf("%19[^\n]", novoIme);
		*(poljeArtikala + i - 1)->kategorija = *novoIme;
		while (fread(&temp, sizeof(temp), 1, pF) == 1) {
			if (strcmp(temp.kategorija, novoIme) == 0) {
				fseek(pF, sizeof(temp), 1);
				fwrite(&temp, sizeof(temp), 1, pF);
				if (temp.kategorija == novoIme) {
					printf("\n Uspiješno azurirano!\n");
				}
				else {
					printf("\n Pogreska u azuriranju!\n");
				}
				
			}
		}
		fclose(pF);
		break;
	case 3:
		printf("Unesite novi ID artikla: ");
		scanf("%d", &noviBroj);
		(poljeArtikala + i - 1)->id = noviBroj;
		while (fread(&temp, sizeof(temp), 1, pF) == 1) {
			if (temp.id == noviBroj) {
				fseek(pF, sizeof(temp), 1);
				fwrite(&temp, sizeof(temp), 1, pF);
				if (temp.id == noviBroj) {
					printf("\n Uspiješno azurirano!\n");
				}
				else {
					printf("\n Pogreska u azuriranju!\n");
				}
				
			}
		}
		
		fclose(pF);
		break;
	case 4:
		printf("Unesite novg proizvodaca artikla: ");
		getchar();
		scanf("%19[^\n]", novoIme);
		*(poljeArtikala + i - 1)->proizvodac = *novoIme;
		while (fread(&temp, sizeof(temp), 1, pF) == 1) {
			if (strcmp(temp.proizvodac, novoIme) == 0) {
				fseek(pF, sizeof(temp), 1);
				fwrite(&temp, sizeof(temp), 1, pF);
				if (temp.proizvodac == novoIme) {
					printf("\n Uspiješno azurirano!\n");
				}
				else {
					printf("\n Pogreska u azuriranju!\n");
				}
				
			}
		}
		fclose(pF);
		break;
	case 5:
		printf("Unesite novu tezinu artikla: ");
		getchar();
		printf("Unesite novi ID artikla: ");
		scanf("%d", &noviBroj);
		(poljeArtikala + i - 1)->tezinaUGramima = noviBroj;
		while (fread(&temp, sizeof(temp), 1, pF) == 1) {
			if (temp.tezinaUGramima == noviBroj) {
				fseek(pF, sizeof(temp), 1);
				fwrite(&temp, sizeof(temp), 1, pF);
				if (temp.tezinaUGramima == noviBroj) {
					printf("\n Uspiješno azurirano!\n");
				}
				else {
					printf("\n Pogreska u azuriranju!\n");
				}
				
			}
		}
		fclose(pF);
		break;
	case 6:
		printf("Unesite gluten artikla: ");
		getchar();
		scanf("%2[^\n]", novoIme);
		*(poljeArtikala + i - 1)->gluten = *novoIme;
		while (fread(&temp, sizeof(temp), 1, pF) == 1) {
			if (strcmp(temp.gluten, novoIme) == 0) {
				fseek(pF, sizeof(temp), 1);
				fwrite(&temp, sizeof(temp), 1, pF);
				if (temp.gluten == novoIme) {
					printf("\n Uspiješno azurirano!\n");
				}
				else {
					printf("\n Pogreska u azuriranju!\n");
				}
				
			}
		}
		fclose(pF);
		break;
	case 7:
		printf("Unesite secer artikla: ");
		getchar();
		scanf("%19[^\n]", novoIme);
		*(poljeArtikala + i - 1)->secer = *novoIme;
		while (fread(&temp, sizeof(temp), 1, pF) == 1) {
			if (strcmp(temp.secer, novoIme) == 0) {
				fseek(pF, sizeof(temp), 1);
				fwrite(&temp, sizeof(temp), 1, pF);
				if (temp.secer == novoIme) {
					printf("\n Uspiješno azurirano!\n");
				}
				else {
					printf("\n Pogreska u azuriranju!\n");
				}
				
			}
		}
		fclose(pF);
		break;
	case 8:
		printf("Unesite novi ID artikla: ");
		scanf("%d", &noviBroj);
		(poljeArtikala + i - 1)->kolicina = noviBroj;
		while (fread(&temp, sizeof(temp), 1, pF) == 1) {
			if (temp.kolicina == noviBroj) {
				fseek(pF, sizeof(temp), 1);
				fwrite(&temp, sizeof(temp), 1, pF);
				if (temp.kolicina == noviBroj) {
					printf("\n Uspiješno azurirano!\n");
				}
				else {
					printf("\n Pogreska u azuriranju!\n");
				}
				
			}
		}
		fclose(pF);
		break;
	default:
		printf("Krivi unos\n");
		fclose(pF); 
	 }
}
