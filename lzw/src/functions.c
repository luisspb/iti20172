/* Autores: Jorgeluis Guerra
**          Francisco Erberto */

#include "tree_node.h"
#include "functions.h"

FILE* openFile(char* filename) {  /* Converter para C */
   FILE* file;

   printf ("Opening input file: %s\n", filename);
   fopen(filename, "rb");

   if (file == NULL) {
      perror("Error opening input file: %s", filename);
      exit(1);
   }
   else
      return file;

   if(file.is_open()) {

   }
   else {
      std::cerr << "Error opening input file: " << filename << std::endl;

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
