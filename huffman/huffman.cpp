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

// Definição da árvore
typedef struct nodeArvore
{
    int                 frequencia;
    byte                c;
    struct nodeArvore   *esquerda;
    struct nodeArvore   *direita;
} nodeArvore;

// Definição da fila de prioridade (implementada como lista simplesmente encadeada)
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


// Prototipos das funcoes
std::ifstream openFile (char* filename);  // Recebe nome do arquivo com a extensao
unsigned int* countByteFrequency(std::ifstream& file);

nodeLista *novoNodeLista(nodeArvore *nArv);
nodeArvore *novoNodeArvore(byte c, int frequencia, nodeArvore *esquerda, nodeArvore *direita);
nodeArvore *hopMinLista(lista *l);

/*<*arvore> buildHuffmanTree(int *bytesTable);

freeHuffmanTree(<*arvore>)

compressFile(<nome_arquivo>, <*lista_bytes>, <nome_arquivo_comprimido>)

decompressFile(<nome_arquivo_comprimido>, <nome_arquivo>)*/


int main (int argc, char *argv[]) {

   // Arquivo a ser aberto para processamento
   std::ifstream file;

   file = openFile(argv[2]);

   unsigned int* bytesArray = countByteFrequency(file);

   for (int i = 0; i < BYTE; i++)
      std::cout << "Byte " << i << ", Frequencia: " << bytesArray[i] << std::endl;

   return 0;
}

std::ifstream openFile(char* filename) {

   std::ifstream file;

   std::cout << "Opening file: " << filename << std::endl;
   file.open(filename, std::ios::in | std::ios::binary);

   if(file.is_open()) {
      return file;
   }
   else {
      std::cerr << "Error opening file: " << filename << std::endl;
      exit(1);
   }
}

unsigned int* countByteFrequency(std::ifstream& file) {

   // Cria array de frequencia de bytes com todas as posicoes
   // inicialmente com zero
   static unsigned int frequencyArray[BYTE] = {0};
   // Variavel para guardar o tamanho do arquivo em bytes
   unsigned int length;
   // A leitura do arquivo se da byte por byte, cada byte eh
   // primeiro armazenado no buffer
   char buffer;

   // Obtem o tamanho do arquivo em numero de bytes
   file.seekg(0, std::ios::end);
   length = file.tellg();
   file.seekg(0, std::ios::beg);

   std::cout << "Number of read bytes: " << length << std::endl;

   for (unsigned int i = 0; i < length; i++) {
      file.read(&buffer, 1);
      frequencyArray[static_cast<unsigned char>(buffer)]++;
   }

   return frequencyArray;
}
