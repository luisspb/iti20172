// Autores: Jorgeluis Guerra
//          Francisco Erberto

#ifndef DECOMPRESS_FUNCTIONS__
#define DECOMPRESS_FUNCTIONS__

#include <iostream>
#include <fstream>
#include <vector>

#include "NodeArvore.h"

// Prototipos das funcoes para Descompressao

//
void interpretFile(std::ifstream& file, ...);

//
NodeArvore* decodeTree(...);

// Mesma funcao da compressao, copiar de la ou criar um arquivo de funcoes em comum!
void traverseTree(NodeArvore* raiz, std::vector<bool> bytesCodes[], std::vector<bool> code);

//
void decompressFile(...);

//
std::ofstream createUncompressedFile(char* originalFilename);

//
void writeUncompressedFile(...);

#endif
