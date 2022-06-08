#define _CRT_SECURE_NO_WARNINGS
#ifndef matrix_H
#define matrix_H

typedef struct artikl {
	char kategorija[20];
	char ime[40];
	char proizvodac[40];
	int id;
	int kolicina;
	int tezinaUGramima;
	char gluten[3];
	char secer[3];
	int br;
}ARTIKL;

void inicijalizacija(FILE*);
void dodajArtikl(const char* const);
void* citanjeArtikala(const char* const);
void ispisivanjeArtikala(const ARTIKL* const);
void* pretrazivanjeArtikala(ARTIKL* const);
int izlazIzPrograma(ARTIKL*);
void brisanjeArtikala(ARTIKL** const, const ARTIKL* const, const char* const);
void brisanjeDatoteke(const char*, ARTIKL*, int);
int izbornik(const char* const);
void promjenaPreferenci(ARTIKL*, const char* const);

#endif
