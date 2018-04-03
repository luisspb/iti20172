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

// A funcao de construcao da arvore tem que receber uma copia da lista de Nos, nao pode ser passagem
// por referencia, dado que a lista original tem que ser mantida para ir se atualizando a frequencia
// dos bytes e reconstruindo a arvore a medida que a codificacao vai sendo feita
NodeArvore* buildHuffmanTree(std::vector<NodeArvore*> listaNos) {
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
