// Autores: Jorgeluis Guerra
//          Francisco Erberto

#ifndef DECOMPRESS_FUNCTIONS__
#define DECOMPRESS_FUNCTIONS__

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

#include "NodeArvore.h"
#include "common_functions.h"

// Prototipos das funcoes para Descompressao

// Funcao que ler e interpreta os bytes do arquivo compactado, separando conforme seus significados
void interpretFile(std::ifstream& file, unsigned& originalFileLength, std::string& originalFilename,
                   unsigned& treeArraySize, std::vector<unsigned char>& treeArray,
                   unsigned& compactedFileSize, std::vector<unsigned char>& compactedFile);

// Funcao que decodifica a arvore de Huffman, a reconstroi e recria tambem a lista de Nos folha
NodeArvore* decodeTree(std::vector<unsigned char>& treeArray, std::vector<NodeArvore*>& listaNos,
                       int& idx);

// Funcao que descomprime arquivo compactado
void decompressFile(unsigned fileLength, std::vector<bool> bytesCodes[],
                    std::vector<unsigned char>& uncompressedFile,
                    std::vector<unsigned char>& compactedFile, std::vector<NodeArvore*>& listaNos);

// Funcao que abre arquivo de saida do descompressor
std::ofstream createUncompressedFile(const char* originalFilename);

// Funcao que grava conteudo descompactado no arquivo de saida
void writeUncompressedFile(std::ofstream& outputFile, std::vector<unsigned char>& uncompressedFile);

#endif
