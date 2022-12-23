// obtem dos argumentos o limiar
float Descobrir_limiar(int argc, char** argv);

// obtem dos argumentos a marcara
int Descobrir_mascara(int argc, char** argv);

// obtem dos argumentos o angulo
double Descobrir_angulo(int argc, char** argv);

// retorna o arquivo que representa o output
FILE* obtem_arquivo_output(int argc, char** argv);

// retorna o arquivo que representa o input
FILE* obtem_arquivo_input(int argc, char** argv);

// extrai um proximo int, pulando a linha caso nao encontre
void Extrair_proximo_int(FILE* file_handler, int* info, char* buffer);