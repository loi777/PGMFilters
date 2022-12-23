#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "datastruct.h"
#include "parser.h"

// passa o filtro de media em todos os pixeis de input e salva no output
static void filtro_media(unsigned char** input, unsigned char** output, int tam_x, int tam_y) {
	float sum;
	int somas_totais;

	for (int i = 0; i < tam_y; i++) {
		for (int j = 0; j < tam_x; j++) {
			sum = 0;
			somas_totais = 0;

			for (int temp_i = 0; temp_i < 3; temp_i++) {
				for (int temp_j = 0; temp_j < 3; temp_j++) {
					if (Coordenada_existe(tam_x, tam_y, j + temp_j - 1, i + temp_i - 1)) {
						sum += input[i + temp_i - 1][j + temp_j - 1];
						somas_totais++;
					}
				}
			}

			output[i][j] = (int)(sum/somas_totais);
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
	filtro_media(original_pgm->pixel, result_pgm->pixel, result_pgm->tam_x, result_pgm->tam_y);

	// agora que filtramos a imagem original limpamos sua memoria
	original_pgm = Destroi_imagem(original_pgm);

	//-----------------------------------------------------

	/* salvar imagem apos filtro */
	if (Envia_output(result_pgm, argc, argv) != 0) {
		error_handler(5);
	}

	return 0; /* sem erros */
}