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

// Prototipos das funcoes para Descompressao

// Funcao que ler e interpreta os bytes do arquivo compactado, separando conforme seus significados
void interpretFile(std::ifstream& file, unsigned& originalFileLength, std::string& originalFilename,
                   unsigned& treeArraySize, std::vector<unsigned char>& treeArray,
                   unsigned& compactedFileSize, std::vector<unsigned char>& compactedFile);

// Funcao que decodifica a arvore de Huffman e a reconstroi
NodeArvore* decodeTree(std::vector<unsigned char>& treeArray);

// Funcao que descomprime arquivo compactado
void decompressFile(unsigned fileLength, std::vector<bool> bytesCodes[],
                    std::vector<unsigned char>& uncompressedFile,
                    std::vector<unsigned char>& compactedFile);

//
std::ofstream createUncompressedFile(char* originalFilename);

//
void writeUncompressedFile(...);

#endif
