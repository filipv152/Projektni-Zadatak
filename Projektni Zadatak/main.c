#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "matrix.h"

int main(void) {

	FILE* fp = NULL;
	inicijalizacija(fp);
	int uvijet = 1;

	while (uvijet) {
		uvijet = izbornik("trgovina.bin");
	}
	printf("Zavrsetak programa!\n");
	return 0;

	return 0;
}

/*
problem kada se unosi prvi artikl u svijezu datoteku podatci nisu dobri,
promijena preferneci ne radi
*/
