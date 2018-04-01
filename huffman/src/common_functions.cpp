// Autores: Jorgeluis Guerra
//          Francisco Erberto

#include "common_functions.h"

// Funcoes usadas na Compressao e Descompressao
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
