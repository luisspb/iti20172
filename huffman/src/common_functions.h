// Autores: Jorgeluis Guerra
//          Francisco Erberto

#ifndef COMMON_FUNCTIONS__
#define COMMON_FUNCTIONS__

#include <vector>
#include <algorithm>

#include "NodeArvore.h"

// Prototipos das funcoes em comum, tanto para a Compressao como para a Descompressao

// A funcao que controi a arvore de Huffman recebe uma copia da lista de Nos e devolve o ponteiro
// para a raiz
NodeArvore* buildHuffmanTree(std::vector<NodeArvore*> listaNos);

// Funcao que percorrem a arvore e cria o codigo, um array com as posicoes sendo os bytes e os
// valores do array sendo os codigos
void traverseTree(NodeArvore* raiz, std::vector<bool> bytesCodes[], std::vector<bool> code);

// Funcao que decrementa a frequencia do byte decodificado, recria a arvore e o codigo
void updateTree (unsigned char byte, std::vector<NodeArvore*>& listaNos, NodeArvore* raiz,
                 std::vector<bool> bytesCodes[]);

#endif
