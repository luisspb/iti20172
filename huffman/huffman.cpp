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
// A funcao que gera o array com a frequencia dos bytes recebe como argumentos a variavel do arquivo
// e o array em que armazenara as frequencias, ela retorna o tamanho do arquivo em bytes
unsigned countByteFrequency(std::ifstream& file, unsigned* bytesArray);
// A funcao que controi a arvore de Huffman recebe a lista de Nos e devolve o ponteiro para a raiz
NodeArvore* buildHuffmanTree(std::vector<NodeArvore*>& listaNos);
// Funcoes que percorrem a arvore e cria o codigo, um array com as posicoes sendo os bytes e os
// valores do array sendo os codigos
void traverseTree(NodeArvore* raiz, std::vector<bool> bytesCodes[], std::vector<bool> code);
// A funcao que comprime o arquivo em um vector de bytes
void compactFile(std::vector<bool> bytesCodes[], std::vector<char>& compressedFile);
// Funcao que grava a arvore em um array (para posterior gravacao em arquivo)
void convertTree(NodeArvore* raiz, std::vector<int>& treeArray);
// A funcao que abrira o arquivo de saida
std::ofstream createFile(char* filename);
// Funcao que grava o arquivo de saida com o cabecalho (Numero de bytes seguido da arvore em array)
// mais o conteudo compactado do arquivo de entrada
void writeFile(std::ofstream& outputFile, unsigned fileLength, std::vector<int>& treeArray,
               std::vector<char>& compressedFile);

// -------------------------------------------------------------------------------------------------

int main (int argc, char *argv[]) {

   // Arquivo a ser aberto para processamento
   std::ifstream inputFile;
   // Cria array de frequencia de bytes com todas as posicoes
   // inicialmente com zero
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
   std::vector<char> compressedFile;
   // Array para armanezar a arvore que sera gravado no arquivo de saida
   std::vector<int> treeArray;
   // Variavel que tera o stream do arquivo de saida
   std::ofstream outputFile;

   inputFile = openFile(argv[2]);

   fileLength = countByteFrequency(inputFile, bytesArray);

   for (int i = 0; i < BYTE; i++) {
      if (bytesArray[i] != 0) {
         listaNos.push_back(new NodeArvore(i, bytesArray[i], nullptr, nullptr, nullptr));
      }
   }

   // Constroi a arvore de Huffman
   raiz = buildHuffmanTree(listaNos);

   // Percorre a arvore e cria o codigo
   // Passa um vector de bool nao incializado
   std::vector<bool> code;
   traverseTree(raiz, bytesCodes, code);

   // Compacta o arquivo em um vector de bytes
//   compactFile(bytesCodes, compressedFile);

   // Grava arvore em array
//   convertTree(raiz, treeArray);

   // Cria e grava em novo arquivo o cabecalho e em seguida o arquivo que foi compactado
   // O cabecalho comeca com 4 bytes que representam o numero de bytes do arquivo original, seguidos
   // do array da arvore de Huffman
   // Formato do arquivo gerado:
   // --------------- // ------------------ // ------------------ //
   // Numero de bytes // Array com a arvore // Arquivo compactado //
   // --------------- // ------------------ // ------------------ //
//   outputFile = createFile(argv[2]);
//   writeFile(outputFile, fileLength, treeArray, compressedFile);

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

unsigned countByteFrequency(std::ifstream& file, unsigned* bytesArray) {

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

   for (unsigned i = 0; i < length; i++) {
      file.read(&buffer, 1);
      bytesArray[static_cast<unsigned char>(buffer)]++;
   }

   return length;
}

NodeArvore* buildHuffmanTree(std::vector<NodeArvore*>& listaNos) {
   NodeArvore* one;
   NodeArvore* two;
   NodeArvore* internalNode = nullptr;

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

void traverseTree(NodeArvore* raiz, std::vector<bool> bytesCodes[], std::vector<bool> code) {

         std::vector<bool> left, right;
         // Quando vai pro no esquerdo, acrescenta um '0' ao codigo
         left = code;
         left.push_back(0);
         // Quando vai pro no direito, acrescenta um '1' ao codigo
         right = code;
         right.push_back(1);

      if (raiz->getByte() > -1)
         bytesCodes[raiz->getByte()] = code;

      if (raiz->getEsquerda() != nullptr)
         traverseTree(raiz->getEsquerda(), bytesCodes, left);
      if (raiz->getDireita() != nullptr)
         traverseTree(raiz->getDireita(), bytesCodes, right);
}
