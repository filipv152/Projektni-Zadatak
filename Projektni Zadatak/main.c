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
popravit za dodavanje 1. artikla, i za promijenu preferenci
*/
