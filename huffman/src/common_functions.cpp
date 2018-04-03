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

void updateTree (unsigned char byte, std::vector<NodeArvore*>& listaNos, NodeArvore* raiz,
                 std::vector<bool> bytesCodes[]) {
   // Procura byte na lista de nos
   for (unsigned j = 0; j < listaNos.size(); j++)
      if (listaNos[j]->getByte() == byte) {
         listaNos[j]->decrementaFrequencia();
         // Encontrado o byte na lista de nos, sua frequencia eh decrementada e o loop pode ser
         // interrompido
         break;
      }
   // Frequencia decrementada, agora a arvore deve ser reconstruida
   raiz = buildHuffmanTree(listaNos);

   // O codigo anterior tem que ser apagado
   for (unsigned j = 0; j < BYTE; j++)
      bytesCodes[j].resize(0);

   // E em seguida percorre a nova arvore e recria o codigo
   // Passa um vector de bool nao incializado
   std::vector<bool> code;
   traverseTree(raiz, bytesCodes, code);
}
