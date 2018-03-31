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
// Compressao: ./huffman -c <arquivo de entrada - descomprimido> <arquivo de saida - comprimido>
// Descompressao: ./huffman -x <arquivo de entrada - comprimido> <arquivo de saida - descomprimido>

#include <iostream>
#include <fstream>

#define BYTE 256
/** Definição da árvore */
typedef struct nodeArvore
{
    int                 frequencia;
    byte                c;
    struct nodeArvore   *esquerda;
    struct nodeArvore   *direita;
} nodeArvore;

/** Definição da fila de prioridade */

typedef struct nodeLista
{
    nodeArvore          *n;
    struct nodeLista    *proximo;
} nodeLista;

typedef struct lista
{
    nodeLista   *head;
    int         elementos;
} lista;

// Prototipos de funcoes
unsigned int* getByteFrequency(const char* filename);  // Filename with extension

// Função que faz alocação de memória e trata os ponteiros soltos acerca de nós da lista encadeada.

nodeLista *novoNodeLista(nodeArvore *nArv);

// Função que faz alocação de memória e trata os ponteiros soltos acerca de nós da árvore

nodeArvore *novoNodeArvore(byte c, int frequencia, nodeArvore *esquerda, nodeArvore *direita);

/*<*arvore> buildHuffmanTree(int *bytesTable);

freeHuffmanTree(<*arvore>)

compressFile(<nome_arquivo>, <*lista_bytes>, <nome_arquivo_comprimido>)

decompressFile(<nome_arquivo_comprimido>, <nome_arquivo>)*/


int main (int argc, char *argv[]) {

   unsigned int* bytesTable = getByteFrequency(argv[2]);

   return 0;
}

unsigned int* getByteFrequency(const char* filename) {  // nome do arquivo com a extensao

   static unsigned int frequencyTable[BYTE] = {0};
   std::ifstream file;
   unsigned int length;
   char buffer;

   std::cout << "Opening file: " << filename << std::endl;
   file.open(filename, std::ios::in | std::ios::binary);

   if(file.is_open()) {

      // Obtem o tamanho do arquivo
      file.seekg(0, std::ios::end);
      length = file.tellg();
      file.seekg(0, std::ios::beg);

      for (unsigned int i = 0; i < length; i++) {
         file.read(&buffer, 1);
         frequencyTable[static_cast<unsigned char>(buffer)]++;
      }

   }
   else {
      std::cerr << "Error opening file: " << filename << std::endl;
      exit(1);
   }

   return frequencyTable;

}

