// Autores: Jorgeluis Guerra
//          Francisco Erberto

#ifndef NODEARVORE__
#define NODEARVORE__

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

#endif
