/* Autores: Jorgeluis Guerra
**          Francisco Erberto */

#ifndef TREE_NODE_
#define TREE_NODE_

#define BYTE 256

/* Definicao da struct dos nos da arvore */
typedef struct tree_nodes {
   unsigned char      byte;
   tree_node*         esquerda;
   tree_node*         direita;
   tree_node*         pai;
} tree_node;

#endif  /* TREE_NODE_ */
