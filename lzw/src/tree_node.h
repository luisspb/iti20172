/* Autores: Jorgeluis Guerra
**          Francisco Erberto */

#ifndef TREE_NODE_
#define TREE_NODE_

#include <stdlib.h>

#define BYTE 256

/* Definicao da struct dos nos da arvore */
typedef struct tree_node TreeNode;
struct tree_node {
   unsigned long   index;
   unsigned char*  byteArray;
   unsigned short  childsCounter;
   TreeNode**      childs;
};

// Prototipo da funcao que cria um noh da arvore
TreeNode* createNode (unsigned long index, unsigned char* byteArray);

// Prototipo da funcao que adiciona um noh filho a um noh qualquer da arvore
void addChild (TreeNode* node, unsigned long index, unsigned char* byteArray);

#endif  /* TREE_NODE_ */
