#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "datastruct.h"
#include "parser.h"

// gera um angulo em radiano de um angulo de grau
double radians(double degrees) {
	return (degrees * M_PI) / 180;
}

// rotaciona x em antihorario em agulo
static int translate_coordX(int x, int y, double angle) {
	return (int)((x * cos(radians(angle))) - (y * sin(radians(angle))));
}

// rotaciona y em antihorario em angulo
static int translate_coordY(int x, int y, double angle) {
	return (int)((x * sin(radians(angle))) + (y * cos(radians(angle))));
}

//----------------------------------------------------------------------------------

// compara e redimensiona as bordas de um rot_data
static void comparar_dimensao(int temp_x, int temp_y, rot_data* info_filtro) {
	if (temp_x < info_filtro->min_x)
		info_filtro->min_x = temp_x;
	if (temp_x > info_filtro->max_x)
		info_filtro->max_x = temp_x;

	if (temp_y < info_filtro->min_y)
		info_filtro->min_y = temp_y;
	if (temp_y > info_filtro->max_y)
		info_filtro->max_y = temp_y;
}

// retorna os valores minimos e maximos das bordas da nova imagem
static void Gerar_bordas(int tam_x, int tam_y, rot_data* info_filtro) {
	info_filtro->min_x = 0;
	info_filtro->min_y = 0;
	info_filtro->max_x = 0;
	info_filtro->max_y = 0;

	int temp_x, temp_y;

	// pixel direita superior {tam_x-1, 0}
	temp_x = translate_coordX(tam_x, 0, info_filtro->rot);
	temp_y = translate_coordY(tam_x, 0, info_filtro->rot);

	comparar_dimensao(temp_x, temp_y, info_filtro);

	// pixel direito inferior {tamx-1, tam_y-1}
	temp_x = translate_coordX(tam_x, tam_y, info_filtro->rot);
	temp_y = translate_coordY(tam_x, tam_y, info_filtro->rot);

	comparar_dimensao(temp_x, temp_y, info_filtro);

	// pixel esquerdo inferior {0, tam_y-1}
	temp_x = translate_coordX(0, tam_y, info_filtro->rot);
	temp_y = translate_coordY(0, tam_y, info_filtro->rot);

	comparar_dimensao(temp_x, temp_y, info_filtro);
}

//----------------------------------------------------------------------------------

// transforma todos os pixeis de input na matriz de output
// tambem deixa a matriz de output branca por padrao
static void filtro_rot(imagem* input, imagem* output, rot_data* info_filtro) {

	int translated_x, translated_y;

	for (int i = 0; i < output->tam_y; i++) {
		for (int j = 0; j < output->tam_x; j++) {
			translated_x = translate_coordX(j + 1 + info_filtro->min_x, i + 1 + info_filtro->min_y, -(info_filtro->rot));
			translated_y = translate_coordY(j + 1 + info_filtro->min_x, i + 1 + info_filtro->min_y, -(info_filtro->rot));
			// nao sei ao certo, mas esse algoritmo nao funciona adequadamente sem os +1
			// provavelmente por culpa do arredondamento

			if (Coordenada_existe(input->tam_x, input->tam_y, translated_x, translated_y)) {
				output->pixel[i][j] = input->pixel[translated_y][translated_x];
			} else {
				output->pixel[i][j] = output->max_value;
			}
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

	// aloca espaco para o filtro e obtem suas informacoes
	rot_data* info_filtro = malloc(sizeof(rot_data));
	if (info_filtro == 0) {
		original_pgm = Destroi_imagem(original_pgm);
		fprintf(stderr, "erro : ao alocar memoria do filtro\n");
		return 6;
	}
	info_filtro->rot = Descobrir_angulo(argc, argv);
	Gerar_bordas(original_pgm->tam_x, original_pgm->tam_y, info_filtro);

	//-----------------------------------------------------

	// aloca espaco da matriz de resultado
	imagem* result_pgm = Allocar_imagem(original_pgm->fileType, info_filtro->max_x - info_filtro->min_x, info_filtro->max_y - info_filtro->min_y, original_pgm->max_value);
	if (result_pgm == 0) {
		original_pgm = Destroi_imagem(original_pgm);
		error_handler(4);
	}

	//-----------------------------------------------------

	// passar pgm pelo filtro
	filtro_rot(original_pgm, result_pgm, info_filtro);

	// agora que filtramos a imagem original limpamos sua memoria
	original_pgm = Destroi_imagem(original_pgm);
	free(info_filtro);
	
	//-----------------------------------------------------

	/* salvar imagem apos filtro */
	if (Envia_output(result_pgm, argc, argv) != 0) {
		error_handler(5);
	}

	return 0; /* sem erros */
}
