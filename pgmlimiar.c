#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "datastruct.h"
#include "parser.h"

// passa o filtro de limiar em todos os pixeis de input e salva no output
static void filtro_limiar(unsigned char** input, unsigned char** output, int tam_x, int tam_y, int original_max_value, float limiar_value) {
	int limiar_valor = (limiar_value * original_max_value);

	for (int i = 0; i < tam_y; i++) {
		for (int j = 0; j < tam_x; j++) {
			if (input[i][j] < limiar_valor) {
				output[i][j] = 0;
			}
			else {
				output[i][j] = 1;
			}
		}
	}
}

//----------------------------------------------------------------------------------

int main(int argc, char** argv) {
	
	// obtem o limiar
	float limiar = Descobrir_limiar(argc, argv);

	//-----------------------------------------------------

	// aloca espaco e obtem a imagem original
	imagem* original_pgm = Obtem_Input(argc, argv);
	if (original_pgm == 0) {
		error_handler(3);
	}

	//-----------------------------------------------------

	// aloca espaco da matriz de resultado
	imagem* result_pgm = Allocar_imagem(original_pgm->fileType, original_pgm->tam_x, original_pgm->tam_y, 1);
	if (result_pgm == 0) {
		original_pgm = Destroi_imagem(original_pgm);
		error_handler(4);
	}

	//-----------------------------------------------------

	// passar pgm pelo filtro
	filtro_limiar(original_pgm->pixel, result_pgm->pixel, result_pgm->tam_x, result_pgm->tam_y, original_pgm->max_value, limiar);

	// agora que filtramos a imagem original limpamos sua memoria
	original_pgm = Destroi_imagem(original_pgm);

	//-----------------------------------------------------

	/* salvar imagem apos filtro */
	if (Envia_output(result_pgm, argc, argv) != 0) {
		error_handler(5);
	}

	return 0; /* sem erros */
}