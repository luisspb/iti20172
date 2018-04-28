/* Autores: Jorgeluis Guerra
**          Francisco Erberto */

#ifndef FUNCTIONS_
#define FUNCTIONS_

#include "tree_node.h"

/**/

// Prototipo da funcao que abre o arquivo recebe nome do arquivo com a extensao
FILE* openFile(char* filename);
// Prototipo da funcao que realiza a Compressao
void compress(tipo dictSize, FILE inputFile, FILE outputFile, tipo tree);
// Prototipo da funcao que realiza a Descompressao
void decompress(FILE inputFile, FILE outputFile, tipo tree);
// Prototipo da funcao que codifica e comprime o arquivo de entrada
encodeFile();

writeCompressedFile(dictSize, compactedFile);

#endif  /* FUNCTIONS_ */
