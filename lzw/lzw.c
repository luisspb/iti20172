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
   FILE* inputFile;
   // Variavel que tera o stream do arquivo de saida
   FILE* outputFile;
   // Ponteiro para o array que contera o conteudo do arquivo carregado
   unsigned char* fileArray;

   // Abre arquivo
   inputFile = openFile(argv[3]);

   // Carrega arquivo na memoria
   fileArray = loadFile (inputFile);

   printf ("Arquivo carregado com sucesso\n");

   // if (*argv[1] == 'c') {
   //    compress(dictSize, inputFile, outputFile, tree);
   // }
   // else if (*argv[1] == 'x') {
   //    decompress(inputFile, outputFile, tree);
   // }
   //
   // close inputFile;  /* Pseudocodigo */
   // close outputFile;  /* Pseudocodigo */
   return 0;
}
