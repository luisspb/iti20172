/* Autores: Jorgeluis Guerra
**          Francisco Erberto */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
byte* loadFile (FILE* file, size_t* filesize) {
   byte* fileArray = NULL;
   size_t freadSize;

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

   // Le o arquivo e o carrega no array
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
   for (unsigned i = 0; i < BYTE; i++)
      treeRoot->childs[i] = createNode (i, (byte) i);

   return treeRoot;
}

//Função que ler os 8 bits e retorna um numero inteior ou ZERO em caso de erro

int LerBits(FILE* arquivo){

   int valor;
   static bool left = true;
   
   unsigned char buf[2];
   if(feft){
   
      //Se "esquerda", Ler 2 bytes e calcular inteiro
      if(fread(buf, 1, 2, arquivo) != 2){
      
         return 0;
      }
      if(feof(arquivo)){
      
         valor = (bu[0] << 8| buf[1];
      }
      else{
         valor = (bu[0] << 4| buf[1] >> 4);
      }
                  left = false;
   }else{
      
      fseek(arquivo, -1, SEEK_CUR);
      if(fread(buf,1,2,arquivo)!= 2){
         return 0;
      }
      valor = ((buf[0] & 0X0F) << 8 | buf[1];
               left = true;
               
               }
               return valor;
}

/* Funcao que realiza a compressao do arquivo e gera array com o conteudo do arquivo compactado
** O cabecalho do arquivo sera composto apenas pelo numero do tamanho maximo do dicionario.
** Formato do arquivo gerado:
** ----------------- // --------------- //
** Tamanho maximo do // Nome do arquivo //
** dicionario        // original        //
** ----------------- // --------------- */
byte* compress(unsigned dictMaxSize, byte* fileArray, size_t filesize, TreeNode* treeRoot,
               size_t* compressedSize) {
   // Ponteiro para o array que contera o conteudo do arquivo processado
   byte* compressedArray = NULL;
   // Variavel que guarda quantos bytes livres alocados ainda possui o array do conteudo compactado
   unsigned short freeSize;
   // Variavel auxiliar para gravacao do tamanho maximo do dicionario no array do arquivo de saida
   unsigned dictMaxSizeEncode = dictMaxSize;
   // Buffer para armanezar os bytes lidos do array que possui o conteudo do arquivo
   byte byteBuffer;
   // Ponteiro que vai percorrendo a arvore e fazendo o match da sequencia de bytes
   TreeNode* matchPtr = treeRoot;
   // Variavel que guarda o numero do indice da proxima entrada do dicionario
   unsigned nextIndex = BYTE;

   // Inicialmente coloca no array do arquivo de saida, com 4 bytes, o numero do tamanho maximo do
   // dicionario
   // O array eh alocado com 256 bytes e eh aumentado sempre de 256 e 256 bytes; ser aumentado a
   // cada byte reduziria bastante a perfomance do programa
   compressedArray = (byte*) malloc(BYTE);
   freeSize = BYTE;
   compressedArray[0] = dictMaxSizeEncode % BYTE;
   dictMaxSizeEncode /= BYTE;
   compressedArray[1] = dictMaxSizeEncode % BYTE;
   dictMaxSizeEncode /= BYTE;
   compressedArray[2] = dictMaxSizeEncode % BYTE;
   compressedArray[3] = dictMaxSizeEncode / BYTE;
   freeSize -= 4;
   compressedSize += 4;

   // Le byte por byte do arquivo, o codifica, atualiza dicionario e vai construindo o array com o
   // conteudo compactado
   for (size_t i = 0; i < filesize; i++) {
      byteBuffer = fileArray[i];

      for (unsigned short j = 0; j < matchPtr->childsCounter; j++)
         // Se encontrar o match encerra a procura, byte ou sequencia de bytes esta no dicionario
         if (byteBuffer == matchPtr->byte)
            break;

      // Se houve match, atualiza o ponteiro de matching
      if (j < matchPtr->childsCounter) {
         matchPtr = matchPtr->childs[j];
      }
      // Caso nao tenha havido o match
      else {
         // Adiciona entrada ao dicionario
         addChild (matchPtr, nextIndex, byteBuffer);
      }
   }

   return compressedArray;
}

   void decompress(FILE * arquivo, FILE *dest){
   
      //carrega_dicionario_inicial(dicionario);
      int dictPos = 256;
      int anterior; //codigo anterior
      int atual; //codigo atual
      
      unsigned char* antString, atualString;
      
      // Ler o primeiro caracter 
      if(dicionario[atual] == NULL){
      
         return 1;
      }
      //Copia Blocos de Memoria
      memcpy(atualString, dicionario[atual], size+1);
      fprintf(dest, "%s", atualString);
      anterior = atual;
      antString = atualString;
      
      //Ler o restante dos caracteres
      while((atual = lerBits(arquivo)) && atual){
      
         if(dicionario[atual] == NULL){
         
            return 1;
         }
         size = strlen((char*)dicionario[atual]);
         atualString = calloc(size + 2, sizeof(char));
         if(atualString == NULL){
         
            return 1;
         }
         memcpy(atualString, dicionario[atual], size+1);
         fprintf(dest, "%s", atualString);
      }
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
