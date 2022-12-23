// informacoes da imagem
typedef struct pgm_data {
	int tam_x;			// borda maxima horizontal
	int tam_y;			// borda maxima vertical, de cima para baixo
	int max_value;		// pixel com o maior valor, 0 = preto, 255 = branco
	char fileType[2];	// possui o nome em caracteres do formato
	unsigned char** pixel;
} imagem;

// informacoes para o filtro de rotacao
typedef struct rotation_data {
	int max_x;
	int min_x;
	int max_y;
	int min_y;
	double rot;
} rot_data;


// funcoes gerais

// uma funcao para tratar saidas de erro
int error_handler(int error_type);
// uma funcao que testa se uma coordenada existe
int Coordenada_existe(int tam_x, int tam_y, int coord_x, int coord_y);


// funcoes de allocagem de imagem

// uma funcao que limpa a memoria alocada para uma imagem
imagem* Destroi_imagem(imagem* imagem_pgm);
// uma funcao que alloca espaço de memoria para uma imagem
imagem* Allocar_imagem(char* fileType, int tam_x, int tam_y, int max_value);

// funcoes de tratamento de imagem

// uma funcao que obtem dos argumentos o arquivo de entrada
// e obtem suas informacoes
imagem* Obtem_Input(int argc, char** argv);

int Envia_output(imagem* imagem_pgm, int argc, char** argv);


// essa livraria utiliza as saidas de erro: 1 a 3