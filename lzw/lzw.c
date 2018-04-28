/* Universidade Federal da Paraiba
** Centro de Informatica
** Disciplina: Introducao a Teoria da Informacao
** Professor: Teobaldo Junior
**
** Trabalho II - Implementacao em C de um compressor/descompressor usando o algoritmo LZW
**
** Alunos: Jorgeluis Guerra
**         Francisco Erberto
**
** Padrao C: GNU99 (dialeto GNU que implementa o padrao ISO C99)
** Compilador: gcc 7.3.1 (Red Hat 7.3.1-5)
** SO: Linux kernel 4.16.3-200.fc27.x86_64 (Fedora 27 Workstation) */

#include <stdio.h>

#include "tree_node.h"
#include "functions.h"

/**************************************************************************************************/
int main (int argc, char *argv[]) {

   // Arquivo a ser aberto para processamento
   FILE* inputFile;
   // Variavel que tera o stream do arquivo de saida
   FILE* outputFile;

   inputFile = openFile(argv[3]);

   if (*argv[1] == 'c') {
      compress(dictSize, inputFile, outputFile, tree);
   }
   else if (*argv[1] == 'x') {
      decompress(inputFile, outputFile, tree);
   }

   close inputFile;  /* Pseudocodigo */
   close outputFile;  /* Pseudocodigo */
   return 0;
}
/**************************************************************************************************/

FILE* openFile(char* filename) {  /* Converter para C */
   std::ifstream file;

   std::cout << std::endl << "Opening input file: " << filename << std::endl;
   file.open(filename, std::ios::in | std::ios::binary);

   if(file.is_open()) {
      return file;
   }
   else {
      std::cerr << "Error opening input file: " << filename << std::endl;
      exit(1);
   }
}


void compress(tipo dictSize, FILE inputFile, FILE outputFile, tipo tree) {   /* Converter para C */
   /* Array para receber o arquivo compactado */
   array compactedFile;

   encodeFile();

   /* Cria e grava em novo arquivo o cabecalho e em seguida o arquivo que foi compactado
   ** O cabecalho eh composto apenas pelo numero do tamanho maximo do dicionario.
   ** Formato do arquivo gerado:
   ** ----------------- // --------------- //
   ** Tamanho maximo do // Nome do arquivo //
   ** dicionario        // original        //
   ** ----------------- // --------------- */
   writeCompressedFile(dictSize, compactedFile);
}


void decompress(FILE inputFile, FILE outputFile, tipo tree) {
   // Variavel para guardar o tamanho do arquivo original em bytes
   unsigned originalFileLength;
   // String para guardar o nome do arquivo original mais sua extensao
   std::string originalFilename;
   // Variavel para o tamanho do array da arvore que sera lida do arquivo compactado
   unsigned treeArraySize;
   // Array para armanezar a arvore codificada
   std::vector<unsigned char> treeArray;
   // Variavel para o tamanho dos bytes compactados do arquivo original
   unsigned compactedFileSize;
   // Array para armanezar os bytes lidos do arquivo compactado
   std::vector<unsigned char> compactedFile;
   // Ponteiro para a raiz da arvore que sera reconstruida
   NodeArvore* raiz;
   // Array ponteiros para os vetores de codigos dos bytes
   std::vector<bool> bytesCodes[BYTE];
   // Vector de bytes para receber o arquivo descompactado
   std::vector<unsigned char> uncompressedFile;
   // Vetor para armanezar os nos folha que serao recuperados da arvore de Huffman
   std::vector<NodeArvore*> listaNos;

   interpretFile(inputFile, originalFileLength, originalFilename, treeArraySize, treeArray,
                 compactedFileSize, compactedFile);

   // Chama a funcao com um valor de idx que sera usado como identificador unico de cada
   // InternalNode
   int idx = 0;
   raiz = decodeTree(treeArray, listaNos, idx);

#ifdef DEBUG
   std::cout << "-- Primeira Lista --" << std::endl;
   for (unsigned i = 0; i < listaNos.size(); i++)
      std::cout << "Byte: " << listaNos[i]->getByte() << ", Frequencia: "
                << listaNos[i]->getFrequencia() << std::endl;

   std::cout << "-- Nova Arvore --" << std::endl;
#endif
   // Percorre a arvore e recria o codigo
   // Passa um vector de bool nao incializado
   std::vector<bool> code;
   traverseTree(raiz, bytesCodes, code);

#ifdef DEBUG
   std::cout << "-- Primeiro Codigo --" << std::endl;
   for (unsigned i = 0; i < BYTE; i++) {
      if (bytesCodes[i].size()) {
         std::cout << "Byte: " << i << ", Codigo: ";
         for (unsigned j = 0; j < bytesCodes[i].size(); j++)
            std::cout << bytesCodes[i][j];
         std::cout << std::endl;
      }
   }
#endif

   decompressFile(originalFileLength, bytesCodes, uncompressedFile, compactedFile, listaNos);

   outputFile = createUncompressedFile(originalFilename.c_str());

   writeUncompressedFile(outputFile, uncompressedFile);
}
