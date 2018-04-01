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

//
void decompressFile(...);

//
std::ofstream createUncompressedFile(char* originalFilename);

//
void writeUncompressedFile(...);

#endif
