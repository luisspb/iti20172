// Autores: Jorgeluis Guerra
//          Francisco Erberto

#include "decompress_functions.h"

// Funcoes para Compressao

void interpretFile(std::ifstream& file, unsigned& originalFileLength, std::string& originalFilename,
                   unsigned& treeArraySize, std::vector<unsigned char>& treeArray,
                   unsigned& compactedFileSize, std::vector<unsigned char>& compactedFile) {
   // A leitura do arquivo se da byte por byte, cada byte eh primeiro armazenado no buffer
   char buffer;

   std::cout << "Interpreting compacted file..." << std::endl;

   // Formato do arquivo:
   // 1) Tamanho do arquivo original
   // 2) Nome do arquivo original (com extensao)
   // 3) Tamanho da arvore codificada em um array
   // 4) Arvore codificada
   // 5) Tamanho do arquivo depois de compactado
   // 6) Arquivo compactado

   // 1) Tamanho do arquivo original
   originalFileLength = 0;
   for (int i = 0; i < 4; i++) {
      file.read(&buffer, 1);
      originalFileLength += pow(BYTE, i) * static_cast<unsigned char>(buffer);
   }
   std::cout << "Original file length: " << originalFileLength << std::endl;

   // 'Limpa' o buffer por seguranca
   buffer = 0;

   // 2) Nome do arquivo original (com extensao)
   // Le caracteres ate encontrar o 'ponto' da extensao do nome do arquivo
   while (buffer != '.') {
      file.read(&buffer, 1);
      originalFilename.push_back(buffer);
   }
   //Encontrando o ponto, resta apenas ler a extensao (considera-se a extensao sempre de 3 letras)
   for (int i = 0; i < 3; i++) {
      file.read(&buffer, 1);
      originalFilename.push_back(buffer);
   }
   std::cout << "Original filename: " << originalFilename << std::endl;

   // 3) Tamanho da arvore codificada em um array
   treeArraySize = 0;
   for (int i = 0; i < 4; i++) {
      file.read(&buffer, 1);
      treeArraySize += pow(BYTE, i) * static_cast<unsigned char>(buffer);
   }
   std::cout << "Coded tree array size: " << treeArraySize << std::endl;

   // 4) Arvore codificada
   // Le a arvore de tras para frente, vai facilitar no processo de decodificacao
   treeArray.resize(treeArraySize);
   for (int i = treeArraySize - 1; i >= 0; i--) {
      file.read(&buffer, 1);
      treeArray[i] = static_cast<unsigned char>(buffer);
   }

   // 5) Tamanho do arquivo depois de compactado
   compactedFileSize = 0;
   for (int i = 0; i < 4; i++) {
      file.read(&buffer, 1);
      compactedFileSize += pow(BYTE, i) * static_cast<unsigned char>(buffer);
   }
   std::cout << "Size of compressed bytes: " << compactedFileSize << std::endl;

   // 6) Arquivo compactado
   for (unsigned i = 0; i < compactedFileSize; i++) {
      file.read(&buffer, 1);
      compactedFile.push_back(static_cast<unsigned char>(buffer));
   }
}

//https://stackoverflow.com/questions/759707/efficient-way-of-storing-huffman-tree
NodeArvore* decodeTree(std::vector<unsigned char>& treeArray) {
   int byte;
   unsigned frequency = 0;
   NodeArvore* left;
   NodeArvore* right;

   if (treeArray.back() == 1) {
      treeArray.pop_back();
      byte = treeArray.back();
      treeArray.pop_back();
      for (int i = 0; i < 4; i++) {
         frequency += pow(BYTE, i) * treeArray.back();
         treeArray.pop_back();
      }
      return new NodeArvore(byte, frequency, nullptr, nullptr, nullptr);
   }
   else {
      treeArray.pop_back();
      left = decodeTree(treeArray);
      right = decodeTree(treeArray);
      return new NodeArvore(-1, 0, left, right, nullptr);
   }
}

void decompressFile(unsigned fileLength, std::vector<bool> bytesCodes[],
                    std::vector<unsigned char>& uncompressedFile,
                    std::vector<unsigned char>& compactedFile) {
   // Variaveis de 1 byte e 1 bit
   unsigned char byte;
   bool bit;
   // Variavel para guardar a palavra codigo atual
   std::vector<bool> code;
   // Variavel que diz se foi encontrado uma palavra de codigo dentro do array de codigos
   bool match;
   // Variavel que vai contando quantos bytes ja foram decodificados
   unsigned length = 0;

   for (unsigned i = 0; i < compactedFile.size(); i++) {
      byte = compactedFile[i];

      for (int j = 0; j < 8; j++) {
         // Le bit mais significativo
         bit = byte / 128;
         byte = byte << 1;
         // Vai montando uma palavra do codigo com o bit lido
         code.push_back(bit);

         for (unsigned l = 0; l < BYTE; l++) {
            if (bytesCodes[l].size()) {
               // Se match permanecer true, palavra codigo foi encontrada
               match = true;
               for (unsigned m = 0; m < bytesCodes[l].size(); m++)
                  // Se o codigo atual nao for igual ao do array de codigo dos bytes que esta sendo
                  // comparado, tente o proximo (match false e break)
                  if ((m >= code.size()) || (code[m] != bytesCodes[l][m])) {
                     match = false;
                     break;
                  }
               if (match) {
                  // Caso o codigo seja igual, byte ja pode ser decodificado com o valor 'l'
                  uncompressedFile.push_back(l);
                  // Incrementa o contador de bytes decodificados
                  length++;
                  // Se ja decodificou o numero de bytes esperado, pode retornar da funcao de
                  // descompressao
                  if (length >= fileLength)
                     return;
                  // Caso ainda nao terminou o processo de descompressao, continue...
                  // ...variavel com o codigo atual eh limpa
                  code.resize(0);
               }  // if (match)
            }  // if (bytesCodes[l].size())
            // Se o codigo igual ao de algum byte, pode parar a procura atual
            if (!code.size())
               break;
         }  // for (unsigned l = 0...
      }  // for (int j = 0...
   }  // for (unsigned i = 0...
}

std::ofstream createUncompressedFile(const char* originalFilename) {
   std::ofstream file;

   std::cout << "Opening output file: " << originalFilename << std::endl;
   file.open(originalFilename, std::ios::out | std::ios::binary);

   if(file.is_open()) {
      return file;
   }
   else {
      std::cerr << "Error opening output file: " << originalFilename << std::endl;
      exit(1);
   }
}

void writeUncompressedFile(std::ofstream& outputFile, std::vector<unsigned char>& uncompressedFile) {
   for (unsigned i = 0; i < uncompressedFile.size(); i++)
      outputFile << uncompressedFile[i];
}
