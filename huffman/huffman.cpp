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
#include <algorithm>

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
   NodeArvore* getEsquerda();
   NodeArvore* getDireita();
   void setPai(NodeArvore* pai);
   static bool compare(NodeArvore* one, NodeArvore* two);  // Metodo de classe, nao precisa de instancia
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

NodeArvore* NodeArvore::getEsquerda() {
   return this->esquerda;
}

NodeArvore* NodeArvore::getDireita() {
   return this->direita;
}

void NodeArvore::setPai(NodeArvore* pai) {
   this->pai = pai;
}

bool NodeArvore::compare(NodeArvore* one, NodeArvore* two) {
   return one->frequencia > two->frequencia;
}

// Prototipos de funcoes

// A funcao que abre o arquivo recebe nome do arquivo com a extensao
std::ifstream openFile(char* filename);
unsigned int* countByteFrequency(std::ifstream& file);
// A funcao que controi a arvore de Huffman recebe a lista de Nos e devolve o ponteiro para a raiz
NodeArvore* buildHuffmanTree(std::vector<NodeArvore*>& listaNos);
// Funcoes que percorrem a arvore e cria o codigo, um array com as posicoes sendo os bytes e os
// valores do array sendo os codigos
void traverseTree(NodeArvore* raiz, unsigned int* bytesCodes);
void traverseMore(NodeArvore* node, unsigned int* bytesCodes, unsigned int code);

// -------------------------------------------------------------------------------------------------

int main (int argc, char *argv[]) {

   // Arquivo a ser aberto para processamento
   std::ifstream file;
   // Vetor para armanezar os nos que vao compor a arvore de Huffman
   std::vector<NodeArvore*> listaNos;
   // Ponteiro para a raiz da arvore que sera construida
   NodeArvore* raiz;

   file = openFile(argv[2]);

   unsigned int* bytesArray = countByteFrequency(file);

   for (int i = 0; i < BYTE; i++) {
      if (bytesArray[i] != 0) {
         listaNos.push_back(new NodeArvore(i, bytesArray[i], nullptr, nullptr, nullptr));
      }
   }

   // Constroi a arvore de Huffman
   raiz = buildHuffmanTree(listaNos);

   // Percorre a arvore e cria o codigo
   traverseTree(raiz, bytesArray);

   for (int i = 0; i < BYTE; i++)
      std::cout << "Byte: " << i << ", Codigo: " << bytesArray[i] << std::endl;

   return 0;
}

// -------------------------------------------------------------------------------------------------

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

NodeArvore* buildHuffmanTree(std::vector<NodeArvore*>& listaNos) {
   NodeArvore* one;
   NodeArvore* two;
   NodeArvore* internalNode;

   while (listaNos.size() > 1) {
      std::sort(listaNos.begin(), listaNos.end(), NodeArvore::compare);

      one = listaNos[listaNos.size()-1];
      listaNos.pop_back();
      two = listaNos[listaNos.size()-1];
      listaNos.pop_back();
      // O valor da variavel membro byte do internalNode eh sempre -1 porque ele nao representa um
      // byte apenas
      internalNode = new NodeArvore(-1, one->getFrequencia() + two->getFrequencia(), one, two, nullptr);
      one->setPai(internalNode);
      two->setPai(internalNode);
      listaNos.push_back(internalNode);
   }

   return internalNode;
}

void traverseTree(NodeArvore* raiz, unsigned int* bytesCodes) {

   if (raiz->getEsquerda() != nullptr)
      traverseMore(raiz->getEsquerda(), bytesCodes, 0);
   if (raiz->getDireita() != nullptr)
      traverseMore(raiz->getDireita(), bytesCodes, 1);
}

void traverseMore(NodeArvore* node, unsigned int* bytesCodes, unsigned int code) {

   if (node->getByte() > -1)
      bytesCodes[node->getByte()] = code;

   if (node->getEsquerda() != nullptr)
      traverseMore(node->getEsquerda(), bytesCodes, (code<<0));
   if (node->getDireita() != nullptr)
      traverseMore(node->getDireita(), bytesCodes, (code<<0)+1);
}
