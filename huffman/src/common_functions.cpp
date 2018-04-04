// Autores: Jorgeluis Guerra
//          Francisco Erberto

#include "common_functions.h"

// Funcoes usadas na Compressao e Descompressao
void traverseTree(NodeArvore* raiz, std::vector<bool> bytesCodes[], std::vector<bool> code) {

#ifdef DEBUG
   //Imprime o identificador (byte) dos Nos a medida que percorre a arvore (preorder)
   std::cout << "Node: " << raiz->getByte() << std::endl;
#endif

   std::vector<bool> left, right;
   // Quando vai pro no esquerdo, acrescenta um '0' ao codigo
   left = code;
   left.push_back(0);
   // Quando vai pro no direito, acrescenta um '1' ao codigo
   right = code;
   right.push_back(1);

   if (raiz->getByte() > -1) {
      if (code.size())
         bytesCodes[raiz->getByte()] = code;
      // Esse else resolve qdo só restar a raiz da arvore no processo de decremento
      // Restando só a raiz, nao eh gerado nenhum codigo ateh aqui, portanto eh usado o bit 0 como
      // codigo
      else
         bytesCodes[raiz->getByte()].push_back(0);
   }

   if (raiz->getEsquerda() != nullptr)
      traverseTree(raiz->getEsquerda(), bytesCodes, left);
   if (raiz->getDireita() != nullptr)
      traverseTree(raiz->getDireita(), bytesCodes, right);
}

// A funcao de construcao da arvore tem que receber uma copia da lista de Nos, nao pode ser passagem
// por referencia, dado que a lista original tem que ser mantida para ir se atualizando a frequencia
// dos bytes e reconstruindo a arvore a medida que a codificacao vai sendo feita
NodeArvore* buildHuffmanTree(std::vector<NodeArvore*> listaNos, int idx) {
   NodeArvore* one;
   NodeArvore* two;
   NodeArvore* internalNode = nullptr;

   if (listaNos.size() == 1)
      return listaNos[0];

   while (listaNos.size() > 1) {
      // Primeiro ordena a lista de Nos pelos valores dos bytes, para garantir que na ordenacao por
      // frequencia, em caso de empate, os Nos fiquem ordenados sempre da mesma forma, seja na
      // compressao ou descompressao
      std::sort(listaNos.begin(), listaNos.end(), NodeArvore::compareByte);
      // Ordenacao dos Nos pelas frequencias
      std::sort(listaNos.begin(), listaNos.end(), NodeArvore::compareFrequencia);

      one = listaNos[listaNos.size()-1];
      listaNos.pop_back();
      two = listaNos[listaNos.size()-1];
      listaNos.pop_back();
      // O valor da variavel membro byte do internalNode eh sempre -1 porque ele nao representa um
      // byte apenas
      internalNode = new NodeArvore(idx, one->getFrequencia() + two->getFrequencia(), one, two,
                                    nullptr);
      // Decrementa a variavel idx para que cada internalNode possua um valor de "byte" diferente,
      // funciona como um identificador unico para o sorting
      idx--;

      one->setPai(internalNode);
      two->setPai(internalNode);
      listaNos.push_back(internalNode);
   }

   return internalNode;
}

void updateTree (unsigned char byte, std::vector<NodeArvore*>& listaNos, NodeArvore* raiz,
                 std::vector<bool> bytesCodes[]) {
   // Procura byte na lista de nos
   for (unsigned i = 0; i < listaNos.size(); i++)
      if (listaNos[i]->getByte() == byte) {
         // Encontrado o byte na lista de nos, sua frequencia eh decrementada
         listaNos[i]->decrementaFrequencia();
         // Eh preciso verificar se a frequencia do byte chegou a zero, se sim, o No desse byte deve
         // ser excluido da lista
         if (!listaNos[i]->getFrequencia()) {
            for (unsigned j = i; j < (listaNos.size() - 1); j++)
               // Vai movendo o No com o byte de frequencia nula ate este chegar na ultima posicao da
               // lista
               std::iter_swap(listaNos[j], listaNos[j+1]);
            // Estando o No com o byte de frequencia nula na ultima posicao do vector, basta retira-lo
            listaNos.pop_back();
         }
         // Agora que o byte foi decrementado, o loop pode ser interrompido
         break;
      }

#ifdef DEBUG
   std::cout << "-- Nova Lista --" << std::endl;
   for (unsigned i = 0; i < listaNos.size(); i++)
      std::cout << "Byte: " << listaNos[i]->getByte() << ", Frequencia: "
                << listaNos[i]->getFrequencia() << std::endl;
#endif

   // Frequencia decrementada, agora a arvore deve ser reconstruida
   // Primeiro deleta a arvore anterior
   if (raiz != nullptr)
      deleteTree(raiz);
   // Chama a funcao com um valor de idx que sera usado como identificador unico de cada
   // InternalNode. O valor de idx passado sera o valor do primeiro InternalNode criado
   int idx = -1;
   raiz = buildHuffmanTree(listaNos, idx);

   // O codigo anterior tem que ser apagado
   for (unsigned i = 0; i < BYTE; i++)
      bytesCodes[i].resize(0);

#ifdef DEBUG
   std::cout << "-- Nova Arvore --" << std::endl;
#endif
   // E em seguida percorre a nova arvore e recria o codigo
   // Passa um vector de bool nao incializado
   std::vector<bool> code;
   traverseTree(raiz, bytesCodes, code);

#ifdef DEBUG
   std::cout << "-- Novo Codigo --" << std::endl;
   for (unsigned i = 0; i < BYTE; i++) {
      if (bytesCodes[i].size()) {
         std::cout << "Byte: " << i << ", Codigo: ";
         for (unsigned j = 0; j < bytesCodes[i].size(); j++)
            std::cout << bytesCodes[i][j];
         std::cout << std::endl;
      }
   }
#endif
}

void deleteTree(NodeArvore* raiz) {
   if (raiz->getEsquerda() != nullptr)
      deleteTree(raiz->getEsquerda());
   if (raiz->getDireita() != nullptr)
      deleteTree(raiz->getDireita());
   delete raiz;
}
