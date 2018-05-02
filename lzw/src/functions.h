/* Autores: Jorgeluis Guerra
**          Francisco Erberto */

#ifndef FUNCTIONS_
#define FUNCTIONS_

#include <stdio.h>
#include <stdlib.h>
#include "tree_node.h"

// Prototipo da funcao que abre o arquivo recebe nome do arquivo com a extensao
FILE* openInputFile (char* filename);

// Prototipo da funcao que carrega o arquivo na memoria
byte* loadFile (FILE* file, size_t* filesize);

// Prototipo da funcao que inicializa o dicionario
TreeNode* initDict (void);

// Prototipo da funcao que realiza todo o processo de Compressao
byte* compress(unsigned dictMaxSize, byte* fileArray, size_t filesize, TreeNode* treeRoot,
               size_t* compressedSize);

// // Prototipo da funcao que realiza a Descompressao
// void decompress(FILE inputFile, FILE outputFile, tipo tree);
// // Prototipo da funcao que codifica e comprime o arquivo de entrada
// encodeFile();
//
// writeCompressedFile(dictMaxSize, compactedFile);

#endif  /* FUNCTIONS_ */
