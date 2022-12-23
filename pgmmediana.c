#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "datastruct.h"
#include "parser.h"

// metodo padrao usado para o qsort
static int Metodo_comparador(const void* a, const void* b) {
	if (*(int*)a == *(int*)b) {
		return 0;
	}
	else {
		if (*(int*)a < *(int*)b) {
			return -1;
		}
		else {
			return 1;
		}
	}
}

// passa o filtro de mediana em todos os pixeis de input e salva no output
static void filtro_mediana(unsigned char** input, unsigned char** output, int tam_x, int tam_y, int mascara) {
	int* conjunto = malloc(mascara * mascara * sizeof(int));
	int elementos_totais;
	int mediana;

	int desvio = (int)((mascara - 1) / 2);

	for (int i = 0; i < tam_y; i++) {
		for (int j = 0; j < tam_x; j++) {
			elementos_totais = 0;

			for (int temp_i = 0; temp_i < mascara; temp_i++) {
				for (int temp_j = 0; temp_j < mascara; temp_j++) {
					if (Coordenada_existe(tam_x, tam_y, temp_j + j - desvio, temp_i + i - desvio)) {
						conjunto[elementos_totais] = input[i + temp_i - desvio][j + temp_j - desvio];
						elementos_totais++;
					}
				}
			}

			qsort(conjunto, elementos_totais, sizeof(int), Metodo_comparador);

			mediana = 0;
			if (elementos_totais % 2 == 0) {		// caso tenha par elementos
				mediana = (int)((conjunto[(int)((elementos_totais - 1) / 2)] + conjunto[(int)((elementos_totais + 1) / 2)])/2);
			}
			else {									// caso tenha impar elementos
				mediana = conjunto[(int)(elementos_totais /2)];
			}

			output[i][j] = mediana;
		}
	}

	free(conjunto);
}

//----------------------------------------------------------------------------------

int main(int argc, char** argv) {

	// obtem a mascara da mediana
	int mascara = Descobrir_mascara(argc, argv);
	if (mascara % 2 == 0) {
		fprintf(stderr, "aviso : mascara par encontrada, aumentando em 1");
		mascara++;
	}

	//-----------------------------------------------------

	// aloca espaco e obtem a imagem original
	imagem* original_pgm = Obtem_Input(argc, argv);
	if (original_pgm == NULL) {
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
	filtro_mediana(original_pgm->pixel, result_pgm->pixel, result_pgm->tam_x, result_pgm->tam_y, mascara);

	// agora que filtramos a imagem original limpamos sua memoria
	original_pgm = Destroi_imagem(original_pgm);

	//-----------------------------------------------------

	/* salvar imagem apos filtro */
	if (Envia_output(result_pgm, argc, argv) != 0) {
		error_handler(5);
	}

	return 0; /* sem erros */
}