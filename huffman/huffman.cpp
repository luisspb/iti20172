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
   unsigned            frequencia;
   NodeArvore*         esquerda;
   NodeArvore*         direita;
   NodeArvore*         pai;
public:
   NodeArvore(int byte, unsigned frequencia, NodeArvore* esquerda, NodeArvore* direita,
              NodeArvore* pai);
   int getByte();
   unsigned getFrequencia();
   NodeArvore* getEsquerda();
   NodeArvore* getDireita();
   void setPai(NodeArvore* pai);
   // Metodo de classe, nao precisa de instancia
   static bool compare(NodeArvore* one, NodeArvore* two);
};

NodeArvore::NodeArvore(int byte, unsigned frequencia, NodeArvore* esquerda, NodeArvore* direita,
                       NodeArvore* pai) {
   this->byte = byte;
   this->frequencia = frequencia;
   this->esquerda = esquerda;
   this->direita = direita;
   this->pai = pai;
}

int NodeArvore::getByte() {
   return this->byte;
}

unsigned NodeArvore::getFrequencia() {
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
void compactFile(std::ifstream& file, unsigned fileLength, std::vector<bool> bytesCodes[],
                 std::vector<unsigned char>& compressedFile);
// Funcao que grava a arvore em um array (para posterior gravacao em arquivo)
void encodeTree(NodeArvore* raiz, std::vector<unsigned char>& treeArray);
// A funcao que abrira o arquivo de saida
std::ofstream createFile(char* filename);
// Funcao que grava o arquivo de saida com o cabecalho (Numero de bytes seguido da arvore em array)
// mais o conteudo compactado do arquivo de entrada
void writeFile(std::ofstream& outputFile, unsigned fileLength, std::vector<unsigned char>& treeArray,
               std::vector<unsigned char>& compressedFile);

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
   std::vector<unsigned char> compressedFile;
   // Array para armanezar a arvore que sera gravado no arquivo de saida
   std::vector<unsigned char> treeArray;
   // Variavel que tera o stream do arquivo de saida
   std::ofstream outputFile;

   inputFile = openFile(argv[2]);

   fileLength = countByteFrequency(inputFile, bytesArray);

   // Cria a lista com os nos folha da arvore de Huffman
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
   compactFile(inputFile, fileLength, bytesCodes, compressedFile);

   // Grava arvore em array
   encodeTree(raiz, treeArray);
   std::cout << "Size of encoded tree: " << treeArray.size() << std::endl;
   for (unsigned i = 0; i < treeArray.size(); i++)
      std::cout << (unsigned) treeArray[i] << std::endl;

   // Cria e grava em novo arquivo o cabecalho e em seguida o arquivo que foi compactado
   // O cabecalho eh composto por (nessa ordem):
   // 1) 4 bytes que representam o numero de bytes do arquivo original
   // 2) 4 bytes que representam o numero de bytes do array da arvore
   // 3) O array da arvore
   // 4) 4 bytes que representam o numero de bytes do arquivo compactado
   //
   // Formato do arquivo gerado:
   // ------------------ // ------------------ // -------- // ------------------ // ---------- //
   // Numero de bytes do // Numero de bytes do // Array da // Numero de bytes do // Arquivo    //
   // arquivo original   // array da arvore    // arvore   // arquivo compactado // compactado //
   // ------------------ // ------------------ // -------- // ------------------ // ---------- //
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
   // A leitura do arquivo se da byte por byte, cada byte eh primeiro armazenado no buffer
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
      internalNode = new NodeArvore(-1, one->getFrequencia() + two->getFrequencia(), one, two,
                                    nullptr);
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

void compactFile(std::ifstream& file, unsigned fileLength, std::vector<bool> bytesCodes[],
                 std::vector<unsigned char>& compressedFile) {
   // A leitura do arquivo se da byte por byte, cada byte eh primeiro armazenado no buffer
   char buffer;
   // O buffer eh depois convertido num byte sem sinal
   unsigned char inputByte;
   // Contador para contar os bits ateh completar um byte
   unsigned char byteCounter = 0;
   // Byte que eh montado para depois ser colocado no arquivo compactado
   unsigned char outputByte = 0;

   // Volta para o inicio do arquivo de entrada
   file.seekg(0, std::ios::beg);

   for (unsigned i = 0; i < fileLength; i++) {
      file.read(&buffer, 1);
      inputByte = static_cast<unsigned char>(buffer);

      for (unsigned j = 0; j < bytesCodes[inputByte].size(); j++) {
         if (bytesCodes[inputByte][j])
            outputByte++;
         byteCounter++;
         if (byteCounter >= 8) {
            compressedFile.push_back(outputByte);
            byteCounter = 0;
            outputByte = 0;
         }
         else
            outputByte = outputByte << 1;
      }  // for (unsigned j = 0...
   }  // for (unsigned i = 0...

   // Se o ultimo byte precisar ser completado, o codigo a seguir o completa com zeros.
   // Note que o byte incompleto ja sai do loop dos for com um zero a mais no final, por isso agora
   // a comparacao eh com 7 e nao mais com 8
   while ((byteCounter > 0) && (byteCounter < 7)) {
      byteCounter++;
      outputByte = outputByte << 1;
   }
   // Se o que restou dos for aninhados foi um byte incompleto, agora ele esta completado com zeros
   // e sera acrescentado no array do arquivo compactado
   if (byteCounter >= 7)
      compressedFile.push_back(outputByte);

   std::cout << "Size of compressed bytes: " << compressedFile.size() << std::endl;

}  // void compactFile...

void encodeTree(NodeArvore* raiz, std::vector<unsigned char>& treeArray) {
   unsigned frequency;

   // Se eh um no folha, grava byte igual a 1 e em seguinda o valor do byte do no seguido do valor
   // da frequencia dele
   if (raiz->getByte() > -1) {
      treeArray.push_back(1);
      treeArray.push_back(raiz->getByte());
      // Divide o unsigned em 4 bytes e os grava no array
      frequency = raiz->getFrequencia();
      treeArray.push_back(frequency % 256);
      frequency /= 256;
      treeArray.push_back(frequency % 256);
      frequency /= 256;
      treeArray.push_back(frequency % 256);
      treeArray.push_back(frequency / 256);
   }
   // Se nao eh um no folha, grava byte igual a 0 e vai descendo na arvore, primeiro para a esquerda
   else {
      treeArray.push_back(0);
      encodeTree(raiz->getEsquerda(), treeArray);
      encodeTree(raiz->getDireita(), treeArray);
   }
}
