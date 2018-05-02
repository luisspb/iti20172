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
#include <stdlib.h>
#include <string.h>
#include "tree_node.h"
#include "functions.h"

int main (int argc, char* argv[]) {
   // Variaveis para guardar os argumentos passados para o programa
   char* option;
   unsigned dictMaxSize;
   char* inputFilename;
   char* outputFilename;

   // Arquivo a ser aberto para processamento
   FILE* inputFile = NULL;
   // Variavel para guardar o tamanho em bytes do arquivo de entrada
   size_t inputFilesize;
   // Variavel que tera o stream do arquivo de saida
   FILE* outputFile = NULL;
   // Variavel para guardar o tamanho em bytes do arquivo de saida
   size_t outputFilesize;
   // Ponteiro para o array que contera o conteudo do arquivo carregado
   byte* fileArray = NULL;
   // Ponteiro para o array que contera o conteudo do arquivo processado
   byte* processedArray = NULL;
   // Ponteiro para a raiz da arvore do dicionario
   TreeNode* treeRoot = NULL;

   // Trata os argumentos que foram passados ao programa
   // Primeiro testa se o programa foi executado com algum argumento de entrada, depois se ou o
   // argumento 'c' de compressao ou o 'x' de descompressao foi passado e se, de acordo com a opcao
   // selecionada ('c' ou 'x'), a quantidade de argumentos esta correta
   if ((argc < 2)
       || ((*argv[1] == 'c') && (argc != 5))
       || ((*argv[1] == 'x') && (argc != 4))) {
      printf ("\nWrong syntax!\n");
      printf ("Use: lzw c dict_max_size input_file output_file\n");
      printf ("Or use: lzw x input_file output_file\n\n");
      exit(1);
   }
   else {
      // Argumento da opcao escolhida ('c' para compressao, ou 'x' para descompressao)
      option = (char*) malloc (strlen(argv[1]));
      strcpy (option, argv[1]);
      if (*option == 'c') {
         // Tamanho do dicionario
         dictMaxSize = atoi (argv[2]);
         // Tamanho minimo do dicionario deve ser 256, que equivale ao dicionario inicial
         if (dictMaxSize < BYTE)
            dictMaxSize = BYTE;
         // Nomes dos arquivos de entrada e saida
         inputFilename = (char*) malloc (strlen(argv[3]));
         strcpy (inputFilename, argv[3]);
         outputFilename = (char*) malloc (strlen(argv[4]));
         strcpy (outputFilename, argv[4]);
      }
      else {  // Se opcao for 'x'
         // Nomes dos arquivos de entrada e saida
         inputFilename = (char*) malloc (strlen(argv[2]));
         strcpy (inputFilename, argv[2]);
         outputFilename = (char*) malloc (strlen(argv[3]));
         strcpy (outputFilename, argv[3]);
      }
   }

   // Abre arquivo
   inputFile = openInputFile (inputFilename);
   // Carrega arquivo na memoria
   fileArray = loadFile (inputFile, &inputFilesize);
   // Cria o dicionario inicial
   treeRoot = initDict();

   if (*option == 'c') {
      processedArray = compress(dictMaxSize, fileArray, inputFilesize, treeRoot, &outputFilesize);
   }
   // else if (*option == 'x') {
   //    processedArray = decompress(fileArray, treeRoot);
   // }

   // writeFile (dictMaxSize, processedArray);

   // close inputFile;  /* Pseudocodigo */
   // close outputFile;  /* Pseudocodigo */
   // Free dos mallocs /* Implementar */
   return 0;
}
