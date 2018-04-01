// Autores: Jorgeluis Guerra
//          Francisco Erberto

#ifndef COMMON_FUNCTIONS__
#define COMMON_FUNCTIONS__

#include <vector>

#include "NodeArvore.h"

// Prototipos das funcoes em comum, tanto para a Compressao como para a Descompressao

// Funcoes que percorrem a arvore e cria o codigo, um array com as posicoes sendo os bytes e os
// valores do array sendo os codigos
void traverseTree(NodeArvore* raiz, std::vector<bool> bytesCodes[], std::vector<bool> code);

#endif
