/* Autores: Jorgeluis Guerra
**          Francisco Erberto */

#ifndef FUNCTIONS_
#define FUNCTIONS_

#include <stdio.h>
#include <stdlib.h>
#include "tree_node.h"

// Prototipo da funcao que abre o arquivo recebe nome do arquivo com a extensao
FILE* openFile (char* filename);

// Prototipo da funcao que carrega o arquivo na memoria
unsigned char* loadFile (FILE* file);

// Prototipo da funcao que inicializa o dicionario
TreeNode* initDict (void);

// Prototipo da funcao que realiza a Compressao
// void compress(char* dictSize, unsigned char* fileArray, unsigned char* processedArray,
//               TreeNode* treeRoot);

// // Prototipo da funcao que realiza a Descompressao
// void decompress(FILE inputFile, FILE outputFile, tipo tree);
// // Prototipo da funcao que codifica e comprime o arquivo de entrada
// encodeFile();
//
// writeCompressedFile(dictSize, compactedFile);

#endif  /* FUNCTIONS_ */
