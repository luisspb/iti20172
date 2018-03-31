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
#include <vector>

#define BYTE 256

// Definicao da classe dos nos da arvore
class NodeArvore {
   int                 byte;
   int                 frequencia;
   NodeArvore*         esquerda;
   NodeArvore*         direita;
   NodeArvore*         pai;
public:
   NodeArvore(int byte, int frequencia, NodeArvore* esquerda, NodeArvore* direita, NodeArvore* pai);
   int getByte();
   int getFrequencia();
};

NodeArvore::NodeArvore(int byte, int frequencia, NodeArvore* esquerda, NodeArvore* direita, NodeArvore* pai) {
   this->byte = byte;
   this->frequencia = frequencia;
   this->esquerda = esquerda;
   this->direita = direita;
   this->pai = pai;
}

int NodeArvore::getByte() {
   return this->byte;
}

int NodeArvore::getFrequencia() {
   return this->frequencia;
}

// Definição da fila de prioridade
// typedef struct nodeLista
// {
//     nodeArvore          *n;
//     struct nodeLista    *proximo;
// } nodeLista;
//
// typedef struct lista
// {
//     nodeLista   *head;
//     int         elementos;
// } lista;

// Prototipos de funcoes
std::ifstream openFile (char* filename);  // Recebe nome do arquivo com a extensao
unsigned int* countByteFrequency(std::ifstream& file);

// Função que faz alocação de memória e trata os ponteiros soltos acerca de nós da lista encadeada.
// nodeLista *novoNodeLista(nodeArvore *nArv);

// Função que faz alocação de memória e trata os ponteiros soltos acerca de nós da árvore
// nodeArvore *novoNodeArvore(byte c, int frequencia, nodeArvore *esquerda, nodeArvore *direita);

// Codigo velho
/*<*arvore> buildHuffmanTree(int *bytesTable);
freeHuffmanTree(<*arvore>)
compressFile(<nome_arquivo>, <*lista_bytes>, <nome_arquivo_comprimido>)
decompressFile(<nome_arquivo_comprimido>, <nome_arquivo>)*/

int main (int argc, char *argv[]) {

   // Arquivo a ser aberto para processamento
   std::ifstream file;
   // Vetor para armanezar os nos que vao compor a arvore de Huffman
   std::vector <NodeArvore*> listaNos;

   file = openFile(argv[2]);

   unsigned int* bytesArray = countByteFrequency(file);

   for (int i = 0; i < BYTE; i++) {
      if (bytesArray[i] != 0) {
         listaNos.push_back(new NodeArvore(i, bytesArray[i], nullptr, nullptr, nullptr));
      }
   }

   std::cout << "Size of the list: " << listaNos.size() << std::endl;
   for (int i = 0; i < listaNos.size(); i++) {
      std::cout << "Byte: " << listaNos[i]->getByte();
      std::cout << ", Frequencia: " << listaNos[i]->getFrequencia() << std::endl;
   }

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

   std::cout << "Number of bytes read: " << length << std::endl;

   for (unsigned int i = 0; i < length; i++) {
      file.read(&buffer, 1);
      frequencyArray[static_cast<unsigned char>(buffer)]++;
   }

   return frequencyArray;
}
