// Autores: Jorgeluis Guerra
//          Francisco Erberto

#include "NodeArvore.h"

// Implementacao dos metodos da classe dos nos da arvore
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

void NodeArvore::decrementaFrequencia() {
   this->frequencia = this->frequencia - 1;
}
