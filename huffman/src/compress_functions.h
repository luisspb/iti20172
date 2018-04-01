// Autores: Jorgeluis Guerra
//          Francisco Erberto

#ifndef COMPRESS_FUNCTIONS__
#define COMPRESS_FUNCTIONS__

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

#include "NodeArvore.h"

// Prototipos das funcoes para Compressao

// A funcao que gera o array com a frequencia dos bytes recebe como argumentos a variavel do arquivo
// e o array em que armazenara as frequencias, ela retorna o tamanho do arquivo em bytes
unsigned countByteFrequency(std::ifstream& file, unsigned* bytesArray);

// A funcao que controi a arvore de Huffman recebe a lista de Nos e devolve o ponteiro para a raiz
NodeArvore* buildHuffmanTree(std::vector<NodeArvore*>& listaNos);

// A funcao que comprime o arquivo em um vector de bytes
void compressFile(std::ifstream& file, unsigned fileLength, std::vector<bool> bytesCodes[],
                 std::vector<unsigned char>& compactedFile);

// Funcao que grava a arvore em um array (para posterior gravacao em arquivo)
void encodeTree(NodeArvore* raiz, std::vector<unsigned char>& treeArray);

// A funcao que abrira o arquivo de saida
std::ofstream createCompressedFile(char* originalFilename);

// Funcao que grava o arquivo de saida com o cabecalho (Numero de bytes seguido da arvore em array)
// mais o conteudo compactado do arquivo de entrada
void writeCompressedFile(std::ofstream& file, unsigned fileLength, char* originalFilename,
                         std::vector<unsigned char>& treeArray,
                         std::vector<unsigned char>& compactedFile);

#endif
