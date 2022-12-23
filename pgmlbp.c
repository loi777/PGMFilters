#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "datastruct.h"
#include "parser.h"

// passa o filtro de lbp em todos os pixeis de input e salva no output
static void filtro_lbp(unsigned char** input, unsigned char** output, int tam_x, int tam_y) {
	int sum;

	for (int i = 0; i < tam_y; i++) {
		for (int j = 0; j < tam_x; j++) {
			sum = 0;

			if (i > 0) {
				if (j > 0) sum += 1 * (input[i][j] <= input[i - 1][j - 1]);
				sum += 2 * (input[i][j] <= input[i - 1][j]);
				if (j < (tam_x - 1)) sum += 4 * (input[i][j] <= input[i - 1][j + 1]);
			}

			if (j > 0) sum += 8 * (input[i][j] <= input[i][j - 1]);
			if (j < (tam_x - 1)) sum += 16 * (input[i][j] <= input[i][j + 1]);

			if (i < (tam_y - 1)) {
				if (j > 0) sum += 32 * (input[i][j] <= input[i + 1][j - 1]);
				sum += 64 * (input[i][j] <= input[i + 1][j]);
				if (j < (tam_x - 1)) sum += 128 * (input[i][j] <= input[i + 1][j + 1]);
			}

			output[i][j] = sum;
		}
	}
}

//----------------------------------------------------------------------------------

int main(int argc, char** argv) {

	// aloca espaco e obtem a imagem original
	imagem* original_pgm = Obtem_Input(argc, argv);
	if (original_pgm == 0) {
		error_handler(3);
	}

	//-----------------------------------------------------
	
	// aloca espaco da matriz de resultado
	imagem* result_pgm = Allocar_imagem(original_pgm->fileType, original_pgm->tam_x, original_pgm->tam_y, 255);
	if (result_pgm == 0) {
		original_pgm = Destroi_imagem(original_pgm);
		error_handler(4);
	}
	
	//-----------------------------------------------------

	// passar pgm pelo filtro
	filtro_lbp(original_pgm->pixel, result_pgm->pixel, result_pgm->tam_x, result_pgm->tam_y);

	// agora que filtramos a imagem original limpamos sua memoria
	original_pgm = Destroi_imagem(original_pgm);

	//-----------------------------------------------------

	// salvar imagem apos filtro
	if (Envia_output(result_pgm, argc, argv) != 0) {
		error_handler(5);
	}

	return 0; /* sem erros */
}