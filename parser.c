#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// obtem o limiar dos argumentos
float Descobrir_limiar(int argc, char** argv) {

	for (int i = 1; i < argc - 1; i++) {
		if (strcmp(argv[i], "-l") == 0) {
			return atof(argv[i + 1]);
		}
	}

	fprintf(stderr, "aviso : limiar nao encontrado, usando 0.5\n");
	return 0.5;
}

// obtem a mascara dos argumentos
int Descobrir_mascara(int argc, char** argv) {
	for (int i = 1; i < argc - 1; i++) {
		if (strcmp(argv[i], "-m") == 0) {
			return atoi(argv[i + 1]);
		}
	}

	fprintf(stderr, "aviso : mascara nao encontrada, assumindo 3\n");
	return 3;
}

// obtem dos argumentos o angulo ou o padrao caso nao tenha
double Descobrir_angulo(int argc, char** argv) {
	for (int i = 1; i < argc - 1; i++) {
		if (strcmp(argv[i], "-a") == 0) {
			return strtod(argv[i + 1], argv);
		}
	}

	fprintf(stderr, "aviso : angulo nao encontrado, assumindo 90\n");
	return 90.0;
}

//--------------------------------------------------------------------------

// procura e retorna output, ou stdout caso nao tenha
FILE* obtem_arquivo_output(int argc, char** argv) {
	for (int i = 1; i < argc - 1; i++) {
		if (strcmp(argv[i], "-o") == 0) {
			return fopen(argv[i + 1], "w");
		}
	}

	fprintf(stderr, "aviso : output nao encontrado, usando stdout\n");
	return stdout;
}

// procura e retorna input, ou stdin caso nao tenha
FILE* obtem_arquivo_input(int argc, char** argv) {
	for (int i = 1; i < argc - 1; i++) {
		if (strcmp(argv[i], "-i") == 0) {
			return fopen(argv[i + 1], "rb");
		}
	}

	fprintf(stderr, "aviso : input nao encontrado, usando stdin\n");
	return stdin;
}

// extrai o proximo valor inteiro de um arquivo, caso faltando pula uma linha
void Extrair_proximo_int(FILE* file_handler, int* info, char* buffer) {
	while (fscanf(file_handler, "%d", info) <= 0) {
		fgets(buffer, BUFSIZ, file_handler);
	}
}