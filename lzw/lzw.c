/* Universidade Federal da Paraiba
** Centro de Informatica
** Disciplina: Introducao a Teoria da Informacao
** Professor: Teobaldo Junior
**
** Trabalho II - Implementacao em C de um compressor/descompressor usando o algoritmo LZW
**
** Alunos: Jorgeluis Guerra
**         Francisco Erberto
**
** Padrao C: GNU99 (dialeto GNU que implementa o padrao ISO C99)
** Compilador: gcc 7.3.1 (Red Hat 7.3.1-5)
** SO: Linux kernel 4.16.3-200.fc27.x86_64 (Fedora 27 Workstation) */

#include <stdio.h>
#include "tree_node.h"
#include "functions.h"

int main (int argc, char *argv[]) {

   // Arquivo a ser aberto para processamento
   FILE* inputFile = NULL;
   // Variavel que tera o stream do arquivo de saida
   FILE* outputFile = NULL;
   // Ponteiro para o array que contera o conteudo do arquivo carregado
   unsigned char* fileArray = NULL;
   // Ponteiro para o array que contera o conteudo do arquivo compactado ou descompactado
   unsigned char* processedArray = NULL;
   // Ponteiro para a raiz da arvore do dicionario
   TreeNode* treeRoot = NULL;

   // Testa se o programa foi chamado com a quantidade correta de argumentos
   /* Implementar */

   // Abre arquivo
   inputFile = openFile (argv[3]);
   // Carrega arquivo na memoria
   fileArray = loadFile (inputFile);
   // Cria o dicionario inicial
   treeRoot = initDict();

   printf ("Raiz da arvore inicializada: %lu %p %u %p\n\n", treeRoot->index,
           treeRoot->byteArray, treeRoot->childsCounter, (void*) treeRoot->childs);

   for (int i = 0; i < BYTE; i++)
      printf ("Filho da raiz: %lu %u %p\n", treeRoot->childs[i]->index,
              *treeRoot->childs[i]->byteArray, (void*) treeRoot->childs[i]->childs);

   // if (*argv[1] == 'c') {
   //    compress(dictSize, fileArray, processedArray, treeRoot);
   // }
   // else if (*argv[1] == 'x') {
   //    decompress(fileArray, processedArray, treeRoot);
   // }

   // close inputFile;  /* Pseudocodigo */
   // close outputFile;  /* Pseudocodigo */
   return 0;
}
