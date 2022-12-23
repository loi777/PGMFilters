#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "datastruct.h"
#include "parser.h"

static void filtro_nega(unsigned char** input, unsigned char** output, int tam_x, int tam_y, int max_value) {
	for (int i = 0; i < tam_y; i++) {
		for (int j = 0; j < tam_x; j++) {
			output[i][j] = max_value - input[i][j];
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
	imagem* result_pgm = Allocar_imagem(original_pgm->fileType, original_pgm->tam_x, original_pgm->tam_y, original_pgm->max_value);
	if (result_pgm == 0) {
		original_pgm = Destroi_imagem(original_pgm);
		error_handler(4);
	}

	//-----------------------------------------------------

	// passar pgm pelo filtro
	filtro_nega(original_pgm->pixel, result_pgm->pixel, result_pgm->tam_x, result_pgm->tam_y, result_pgm->max_value);

	// agora que filtramos a imagem original limpamos sua memoria
	original_pgm = Destroi_imagem(original_pgm);

	//-----------------------------------------------------

	/* salvar imagem apos filtro */
	if (Envia_output(result_pgm, argc, argv) != 0) {
		error_handler(5);
	}

	return 0; /* sem erros */
}