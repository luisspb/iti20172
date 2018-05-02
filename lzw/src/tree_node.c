/* Autores: Jorgeluis Guerra
**          Francisco Erberto */

#include "tree_node.h"

// Funcao que cria um noh da arvore e retorna um ponteiro para ele
TreeNode* createNode (unsigned long index, unsigned char* byteArray) {
   TreeNode* node = NULL;

   node = (TreeNode*) malloc (sizeof(TreeNode));
   node->index = index;
   node->byteArray = byteArray;
   node->childs = NULL;

   return node;
}
