#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "datastruct.h"
#include "parser.h"

int error_handler(int error_type) {
	switch (error_type) {
		case(1) :
			fprintf(stderr, "erro : imagem de output invalida ou faltando\n");
			return 1;
		break;

		case(2) :
			fprintf(stderr, "erro : valor nao inteiro encontrado na imagem ou imagem corrompida\n");
			return 2;
		break;

		case(3) :
			fprintf(stderr, "erro : ao obter memoria do input\n");
			return 3;
		break;

		case(4) :
			fprintf(stderr, "erro : ao allocar memoria do output\n");
			return 4;
		break;

		case(5):
			fprintf(stderr, "erro : ao enviar o output\n");
			return 5;
		break;
	}

	return 0;
}

// aloca espaco para um array duplo de ints
 static unsigned char** Allocar_array_duplo(int tam_x, int tam_y) {
	 unsigned char** arrayDuplo = malloc(tam_y * sizeof(unsigned char*));
	if (!arrayDuplo) {
		return NULL;
	}

	for (int i = 0; i < tam_y; i++) {
		arrayDuplo[i] = malloc(tam_x * sizeof(unsigned char));
		if (!arrayDuplo[i]) {
			for (int j = 0; j < i; j++) {
				free(arrayDuplo[j]);
			}
			free(arrayDuplo);
			return NULL;
		}
	}

	return arrayDuplo;
}

 // retorna se uma coordenada existe em uma matriz
 int Coordenada_existe(int tam_x, int tam_y, int coord_x, int coord_y) {
	 if (coord_x >= 0 && coord_x < tam_x) {
		 if (coord_y >= 0 && coord_y < tam_y) {
			 return 1;
		 }
	 }

	 return 0;
 }

//----------------------------------------------------------------------------------

 // alloca espaco de memoria e preenche de forma vazia um formato imagem
 imagem* Allocar_imagem(char* fileType, int tam_x, int tam_y, int max_value) {
	 imagem* result_pgm = malloc(sizeof(imagem));
	 if (result_pgm == 0) {
		 return NULL;
	 }

	 // insere os valores da imagem
	 result_pgm->tam_x = tam_x;
	 result_pgm->tam_y = tam_y;
	 result_pgm->max_value = max_value;
	 result_pgm->fileType[0] = fileType[0];
	 result_pgm->fileType[1] = fileType[1];

	 // aloca espaco da matriz de resultado
	 result_pgm->pixel = Allocar_array_duplo(tam_x, tam_y);
	 if (result_pgm->pixel == 0) {
		 free(result_pgm);
		 return NULL;
	 }

	 // sem erros
	 return result_pgm;
 }

// limpa a memoria de uma estrutura pgm
imagem* Destroi_imagem(imagem* imagem_pgm) {
	for (int i = 0; i < imagem_pgm->tam_y; i++) {
		free(imagem_pgm->pixel[i]);
	}
	free(imagem_pgm->pixel);

	free(imagem_pgm);

	return NULL;
}
 
//----------------------------------------------------------------------------------

// envia uma estrutura pgm para uma stream de arquivo
static int envia_imagem_para_stream(FILE* file_handler, imagem* imagem_pgm) {

	fprintf(file_handler, "%s\n", imagem_pgm->fileType);
	fprintf(file_handler, "# arquivo filtrado, codigo de Luiz\n");

	fprintf(file_handler, "%d  %d\n", imagem_pgm->tam_x, imagem_pgm->tam_y);
	fprintf(file_handler, "%d\n", imagem_pgm->max_value);

	if (strcmp(imagem_pgm->fileType, "P5") == 0) {
		for (int i = 0; i < imagem_pgm->tam_y; i++) {
			for (int j = 0; j < imagem_pgm->tam_x; j++) {
				fprintf(file_handler, "%c", (char)(imagem_pgm->pixel[i][j]));
			}
		}
	}
	else {
		for (int i = 0; i < imagem_pgm->tam_y; i++) {
			for (int j = 0; j < imagem_pgm->tam_x; j++) {
				fprintf(file_handler, "%3d ", imagem_pgm->pixel[i][j]);
			}
			fprintf(file_handler, "\n");
		}
	}

	return 0; /* sem erros */
}

// obtem o arquivo de output, envia ele e por fim limpa a memoria usada
// utiliza 'Salvar_imagem'
int Envia_output(imagem* imagem_pgm, int argc, char** argv) {

	FILE* file_handler = obtem_arquivo_output(argc, argv);
	if (!file_handler) {
		return error_handler(1);
	}

	int temp = envia_imagem_para_stream(file_handler, imagem_pgm);

	imagem_pgm = Destroi_imagem(imagem_pgm);
	if (file_handler != stdout)
		fclose(file_handler);

	return temp;
}

//----------------------------------------------------------------------------------

// obtem de uma stream de arquivo uma imagem pgm
static int Extrair_imagem(FILE* file_handler, imagem* imagem_pgm) {

	// cria um buffer para pular linha do arquivo
	char buffer[BUFSIZ];
	
	// obtem o tipo de arquivo
	fscanf(file_handler, "%s", imagem_pgm->fileType);
	
	// obtem informacoes gerais
	Extrair_proximo_int(file_handler, &imagem_pgm->tam_x, buffer);
	Extrair_proximo_int(file_handler, &imagem_pgm->tam_y, buffer);
	if (strcmp(imagem_pgm->fileType, "P1") == 0) {
		imagem_pgm->max_value = 1;
	}
	else {
		Extrair_proximo_int(file_handler, &imagem_pgm->max_value, buffer);
	}

	//	allocar espaco para a imagem original
	imagem_pgm->pixel = Allocar_array_duplo(imagem_pgm->tam_x, imagem_pgm->tam_y);

	//	copiar a imagem original
	if (strcmp(imagem_pgm->fileType, "P5") == 0) {
		for (int i = 0; i < imagem_pgm->tam_y; i++) {
			for (int j = 0; j < imagem_pgm->tam_x; j++) {
				imagem_pgm->pixel[i][j] = getc(file_handler);

				if (imagem_pgm->pixel[i][j] == EOF) {
					// caso ele falhe em ler um valor
					return error_handler(2);
				}
			}
		}

	}
	else {
		for (int i = 0; i < imagem_pgm->tam_y; i++) {
			for (int j = 0; j < imagem_pgm->tam_x; j++) {
				if (fscanf(file_handler, "%hhd", &imagem_pgm->pixel[i][j]) <= 0) {
					// caso ele falhe em ler um valor
					return error_handler(2);
				}
			}
		}
	}

	//	sem erros
	return 0;
}

// extrai uma pgm de um input, ou do stdin caso nao possua
static int Preenche_input(imagem* imagem_pgm, int argc, char** argv) {
	
	FILE* file_handler = obtem_arquivo_input(argc, argv);
	
	/* preenche matriz pgm de origem */
	int temp = Extrair_imagem(file_handler, imagem_pgm);
	
	return temp;
}

// alloca espaço e preenche com informacao a imagem de input
imagem* Obtem_Input(int argc, char** argv) {
	imagem* original_pgm = malloc(sizeof(imagem));
	if (original_pgm == 0) {
		return NULL;
	}
	
	// obtem de um arquivo a imagem
	if (Preenche_input(original_pgm, argc, argv) != 0) {
		free(original_pgm);
		return NULL;
	}
	
	// sem erros
	return original_pgm;
}