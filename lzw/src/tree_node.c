/* Autores: Jorgeluis Guerra
**          Francisco Erberto */

#include "tree_node.h"

// Funcao que cria um noh da arvore e retorna um ponteiro para ele
TreeNode* createNode (unsigned long index, unsigned char* byteArray) {
   TreeNode* node = NULL;

   node = (TreeNode*) malloc (sizeof(TreeNode));
   node->index = index;
   node->byteArray = byteArray;
   node->childsCounter = 0;
   node->childs = NULL;

   return node;
}

// Prototipo da funcao que adiciona um noh filho a um noh qualquer da arvore
void addChild (TreeNode* node, unsigned long index, unsigned char* byteArray) {
   TreeNode* child = NULL;

   // Cria o noh filho
   child = createNode (index, byteArray);

   // Incrementa o contador de filhos e caso seja o primeiro, aloca inicialmente a memoria para o
   // array de filhos; caso seja o segundo filho em diante, aumenta (realoca) o tamanho do array;
   /* Implementar */
}
