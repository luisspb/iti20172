/* Autores: Jorgeluis Guerra
**          Francisco Erberto */

#include "functions.h"

// Funcao que abre o arquivo recebe nome do arquivo com a extensao
FILE* openInputFile (char* filename) {
   FILE* file;

   printf ("Opening input file: %s\n", filename);
   file = fopen (filename, "rb");

   if (file == NULL) {
      printf ("Error opening input file: %s\n", filename);
      exit (1);
   }
   else
      return file;
}

// Funcao que carrega o arquivo na memoria
byte* loadFile (FILE* file) {
   byte* fileArray = NULL;
   size_t filesize, freadSize;

   // Descobre o tamanho do arquivo
   fseek (file, 0L, SEEK_END);
   filesize = ftell (file);
   fseek (file, 0L, SEEK_SET);

   // Aloca memoria para carregar o arquivo
   fileArray = (byte*) malloc (filesize);
   // Se alocacao deu errado, encerra o programa
   if (fileArray == NULL) {
      printf ("Cannot allocate memory for this file.\n");
      exit(1);
   }

   // Lê o arquivo e o carrega no array
   freadSize = fread (fileArray, sizeof(byte), filesize, file);

   // Testa se a leitura do arquido deu certo, caso contrario aborta o programa
   if (filesize != freadSize) {
      printf ("Cannot load this file.\n");
      exit(1);
   }

   return fileArray;
}

// Funcao que inicializa o dicionario
TreeNode* initDict (void) {
   TreeNode* treeRoot = NULL;

   // Inicializa a raiz
   treeRoot = createNode(0, NULL);

   // Altera o contador de filhos da raiz para 256 e aloca um array de 256 nos filhos para a raiz da
   // arvore
   treeRoot->childsCounter = BYTE;
   treeRoot->childs = (TreeNode**) malloc (sizeof(TreeNode*) * BYTE);

   // Cria o dicionario inicial basico, com os bytes adicionados como nos filhos da raiz
   for (unsigned i = 0; i < BYTE; i++) {
      byte* byteArray = (byte*) malloc (sizeof(byte));
      *byteArray = (byte) i;
      treeRoot->childs[i] = createNode (i, byteArray);
   }

   return treeRoot;
}

// Funcao que realiza a compressao do arquivo
byte* compress(unsigned dictMaxSize, byte* fileArray, TreeNode* treeRoot) {
   // Ponteiro para o array que contera o conteudo do arquivo processado
   byte* compressedArray = NULL;
   // Variavel auxiliar para gravacao do tamanho maximo do dicionario no array do arquivo de saida
   unsigned dictMaxSizeEncode = dictMaxSize;

   // Inicialmente coloca no array do arquivo de saida, com 4 bytes, o numero do tamanho maximo do
   // dicionario
   compressedArray = (byte*) malloc(4);
   compressedArray[0] = dictMaxSizeEncode % BYTE;
   dictMaxSizeEncode /= BYTE;
   compressedArray[1] = dictMaxSizeEncode % BYTE;
   dictMaxSizeEncode /= BYTE;
   compressedArray[2] = dictMaxSizeEncode % BYTE;
   compressedArray[3] = dictMaxSizeEncode / BYTE;

   /* Gera array com o conteudo do arquivo compactado
   ** O cabecalho do arquivo sera composto apenas pelo numero do tamanho maximo do dicionario.
   ** Formato do arquivo gerado:
   ** ----------------- // --------------- //
   ** Tamanho maximo do // Nome do arquivo //
   ** dicionario        // original        //
   ** ----------------- // --------------- */

   return compressedArray;
}

// void decompress(FILE inputFile, FILE outputFile, tipo tree) {
//    // Variavel para guardar o tamanho do arquivo original em bytes
//    unsigned originalFileLength;
//    // String para guardar o nome do arquivo original mais sua extensao
//    std::string originalFilename;
//    // Variavel para o tamanho do array da arvore que sera lida do arquivo compactado
//    unsigned treeArraySize;
//    // Array para armanezar a arvore codificada
//    std::vector<byte> treeArray;
//    // Variavel para o tamanho dos bytes compactados do arquivo original
//    unsigned compactedFileSize;
//    // Array para armanezar os bytes lidos do arquivo compactado
//    std::vector<byte> compactedFile;
//    // Ponteiro para a raiz da arvore que sera reconstruida
//    NodeArvore* raiz;
//    // Array ponteiros para os vetores de codigos dos bytes
//    std::vector<bool> bytesCodes[BYTE];
//    // Vector de bytes para receber o arquivo descompactado
//    std::vector<byte> uncompressedFile;
//    // Vetor para armanezar os nos folha que serao recuperados da arvore de Huffman
//    std::vector<NodeArvore*> listaNos;
//
//    interpretFile(inputFile, originalFileLength, originalFilename, treeArraySize, treeArray,
//                  compactedFileSize, compactedFile);
//
//    // Chama a funcao com um valor de idx que sera usado como identificador unico de cada
//    // InternalNode
//    int idx = 0;
//    raiz = decodeTree(treeArray, listaNos, idx);
//
// #ifdef DEBUG
//    std::cout << "-- Primeira Lista --" << std::endl;
//    for (unsigned i = 0; i < listaNos.size(); i++)
//       std::cout << "Byte: " << listaNos[i]->getByte() << ", Frequencia: "
//                 << listaNos[i]->getFrequencia() << std::endl;
//
//    std::cout << "-- Nova Arvore --" << std::endl;
// #endif
//    // Percorre a arvore e recria o codigo
//    // Passa um vector de bool nao incializado
//    std::vector<bool> code;
//    traverseTree(raiz, bytesCodes, code);
//
// #ifdef DEBUG
//    std::cout << "-- Primeiro Codigo --" << std::endl;
//    for (unsigned i = 0; i < BYTE; i++) {
//       if (bytesCodes[i].size()) {
//          std::cout << "Byte: " << i << ", Codigo: ";
//          for (unsigned j = 0; j < bytesCodes[i].size(); j++)
//             std::cout << bytesCodes[i][j];
//          std::cout << std::endl;
//       }
//    }
// #endif
//
//    decompressFile(originalFileLength, bytesCodes, uncompressedFile, compactedFile, listaNos);
//
//    outputFile = createUncompressedFile(originalFilename.c_str());
//
//    writeUncompressedFile(outputFile, uncompressedFile);
// }
