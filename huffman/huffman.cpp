//Universidade Federal da Paraiba
//Centro de Informatica
//Disciplina: [1107186] - Estruturas de Dados (2014.2) - Turma 01
//Professor: Christian Azambuja Pagot
//
//Trabalho II - Implementacao em C++ de um compressor/descompressor usando a codificacao de Huffman
//
//Aluno: Jorgeluis Andrade Guerra - 11325713
//
//Padrao C++: GNU++11 (dialeto GNU que implementa o padrao ISO C++11 - ISO/IEC 14882:2011)
//Compilador: g++ 4.8.2 (Ubuntu 4.8.2-19ubuntu1)
//SO: Linux kernel 3.13.0-45-generic x86_64 (Ubuntu 14.04.1 LTS)
//
//Comando de compilacao: g++ -std=gnu++11 -pedantic -O2 -Wall -lm -o huffman huffman.cpp
//
//Sintaxe do binario:
//Compressao: ./huffman -c <arquivo de entrada - descomprimido> <arquivo de saida - comprimido>
//Descompressao: ./huffman -x <arquivo de entrada - comprimido> <arquivo de saida - descomprimido>

#include <iostream>
#include <fstream>

#define BYTE 256


//Functions prototypes
unsigned int* getByteFrequency(const char* filename);  //Filename with extension

/*<*arvore> buildHuffmanTree(int *bytesTable);

freeHuffmanTree(<*arvore>)

compressFile(<nome_arquivo>, <*lista_bytes>, <nome_arquivo_comprimido>)

decompressFile(<nome_arquivo_comprimido>, <nome_arquivo>)*/


int main (int argc, char *argv[]) {

   unsigned int* bytesTable = getByteFrequency(argv[2]);

   return 0;
}

unsigned int* getByteFrequency(const char* filename) {  //Filename with extension

   static unsigned int frequencyTable[BYTE] = {0};
   std::ifstream file;
   unsigned int length;
   char buffer;

   std::cout << "Opening file: " << filename << std::endl;
   file.open(filename, std::ios::in | std::ios::binary);

   if(file.is_open()) {

      //Get the length of the file
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

