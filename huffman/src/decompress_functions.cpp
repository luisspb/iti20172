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
