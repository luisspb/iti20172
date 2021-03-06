// Universidade Federal da Paraiba
// Centro de Informatica
// Disciplina: Introducao a Teoria da Informacao
// Professor: Teobaldo Junior
//
// Trabalho I - Implementacao em C++ de um compressor/descompressor usando o algoritmo de Huffman
// semi-adaptativo de decremento
//
// Alunos: Jorgeluis Guerra
//         Francisco Erberto
//
// Padrao C++: GNU++11 (dialeto GNU que implementa o padrao ISO C++11 - ISO/IEC 14882:2011)
// Compilador: g++ 4.8.2 (Ubuntu 4.8.2-19ubuntu1)
// SO: Linux kernel 4.15.12-301.fc27 x86_64 (Fedora 27 Workstation)
//
// Comando de compilacao: g++ -std=gnu++11 -pedantic -O2 -Wall -lm -o huffman huffman.cpp
//
// Sintaxe do binario:
// Compressao: ./huffman c <arquivo de entrada - descomprimido> <arquivo de saida - comprimido>
// Descompressao: ./huffman x <arquivo de entrada - comprimido> <arquivo de saida - descomprimido>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bitset>

#include "NodeArvore.h"
#include "common_functions.h"
#include "compress_functions.h"
#include "decompress_functions.h"

// Prototipo da funcao que abre o arquivo recebe nome do arquivo com a extensao
std::ifstream openFile(char* filename);
// Prototipo da funcao que realiza a Compressao
void compress(char* originalFilename, std::ifstream& inputFile, std::ofstream& outputFile);
// Prototipo da funcao que realiza a Descompressao
void decompress(std::ifstream& inputFile, std::ofstream& outputFile);

// -------------------------------------------------------------------------------------------------
int main (int argc, char *argv[]) {

   // Arquivo a ser aberto para processamento
   std::ifstream inputFile;
   // Variavel que tera o stream do arquivo de saida
   std::ofstream outputFile;

   inputFile = openFile(argv[2]);

   if (*argv[1] == 'c') {
      compress(argv[2], inputFile, outputFile);
   }
   else if (*argv[1] == 'x') {
      decompress(inputFile, outputFile);
   }

   inputFile.close();
   outputFile.close();
   return 0;
}
// -------------------------------------------------------------------------------------------------

std::ifstream openFile(char* filename) {
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

void compress(char* originalFilename, std::ifstream& inputFile, std::ofstream& outputFile) {
   // Cria array de frequencia de bytes com todas as posicoes inicialmente com zero
   unsigned bytesArray[BYTE] = {0};
   // Variavel para guardar o tamanho do arquivo em bytes
   unsigned fileLength;
   // Vetor para armanezar os nos que vao compor a arvore de Huffman
   std::vector<NodeArvore*> listaNos;
   // Ponteiro para a raiz da arvore que sera construida
   NodeArvore* raiz;
   // Array ponteiros para os vetores de codigos dos bytes
   std::vector<bool> bytesCodes[BYTE];
   // Vector de bytes para receber o arquivo compactado
   std::vector<unsigned char> compactedFile;
   // Array para armanezar a arvore que sera gravado no arquivo de saida
   std::vector<unsigned char> treeArray;

   fileLength = countByteFrequency(inputFile, bytesArray);

   // Cria a lista com os nos folha da arvore de Huffman
   for (int i = 0; i < BYTE; i++) {
      if (bytesArray[i] != 0) {
         listaNos.push_back(new NodeArvore(i, bytesArray[i], nullptr, nullptr, nullptr));
      }
   }

#ifdef DEBUG
   std::cout << "-- Primeira Lista --" << std::endl;
   for (unsigned i = 0; i < listaNos.size(); i++)
      std::cout << "Byte: " << listaNos[i]->getByte() << ", Frequencia: "
                << listaNos[i]->getFrequencia() << std::endl;
#endif

   // Constroi a arvore de Huffman
   // Chama a funcao com um valor de idx que sera usado como identificador unico de cada
   // InternalNode. O valor de idx passado sera o valor do primeiro InternalNode criado
   int idx = -1;
   raiz = buildHuffmanTree(listaNos, idx);

   #ifdef DEBUG
      std::cout << "-- Primeira Arvore --" << std::endl;
   #endif
   // Percorre a arvore e cria o codigo
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

   // Grava arvore em array
   // Como a compressao vai reconstruindo a arvore, a original precisa ser codificada antes de
   // comecar o processo que a altera
   encodeTree(raiz, treeArray);
   std::cout << "Size of encoded tree: " << treeArray.size() << std::endl;

   // Compacta o arquivo em um vector de bytes
   // Para o codigo de Huffman semi-adaptativo de decremento, a funcao que faz a compressao vai
   // decrementando a frequencia dos bytes codificados e reconstruindo a arvore. Por isso precisa-se
   // agora passar como argumento tambem a lista de nos
   compressFile(inputFile, fileLength, bytesCodes, compactedFile, listaNos);

#ifdef DEBUG
   std::cout << "-- Bytes compactados -- " << std::endl;
   for (unsigned i = 0; i < compactedFile.size(); i++) {
      std::bitset<8> bits(compactedFile[i]);
      std::cout << bits << std::endl;
   }
#endif

   // Cria e grava em novo arquivo o cabecalho e em seguida o arquivo que foi compactado
   // O cabecalho eh composto por (nessa ordem):
   // 1) 4 bytes que representam o numero de bytes do arquivo original
   // 2) Nome do arquivo original (incluindo a extensao do arquivo)
   // 3) 4 bytes que representam o numero de bytes do array da arvore
   // 4) O array da arvore
   // 5) 4 bytes que representam o numero de bytes do arquivo compactado
   //
   // Formato do arquivo gerado:
   // ------------------ // --------------- // ------------------ // -------- // ------------------
   // Numero de bytes do // Nome do arquivo // Numero de bytes do // Array da // Numero de bytes do
   // arquivo original   // original        // array da arvore    // arvore   // arquivo compactado
   // ------------------ // --------------- // ------------------ // -------- // ------------------
   // ---------- //
   // Arquivo    //
   // compactado //
   // ---------- //
   outputFile = createCompressedFile(originalFilename);
   writeCompressedFile(outputFile, fileLength, originalFilename, treeArray, compactedFile);
}

void decompress(std::ifstream& inputFile, std::ofstream& outputFile) {
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
