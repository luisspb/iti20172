// Autores: Jorgeluis Guerra
//          Francisco Erberto

#include "compress_functions.h"

// Funcoes para Compressao

unsigned countByteFrequency(std::ifstream& file, unsigned* bytesArray) {
   // Variavel para guardar o tamanho do arquivo em bytes
   unsigned int length;
   // A leitura do arquivo se da byte por byte, cada byte eh primeiro armazenado no buffer
   char buffer;

   // Obtem o tamanho do arquivo em numero de bytes
   file.seekg(0, std::ios::end);
   length = file.tellg();
   file.seekg(0, std::ios::beg);

   std::cout << "Number of bytes read: " << length << std::endl;

   for (unsigned i = 0; i < length; i++) {
      file.read(&buffer, 1);
      bytesArray[static_cast<unsigned char>(buffer)]++;
   }

   return length;
}

void compressFile(std::ifstream& file, unsigned fileLength, std::vector<bool> bytesCodes[],
                  std::vector<unsigned char>& compactedFile, std::vector<NodeArvore*>& listaNos) {
   // A leitura do arquivo se da byte por byte, cada byte eh primeiro armazenado no buffer
   char buffer;
   // O buffer eh depois convertido num byte sem sinal
   unsigned char inputByte;
   // Contador para contar os bits ateh completar um byte
   unsigned char bitsCounter = 0;
   // Byte que eh montado para depois ser colocado no arquivo compactado
   unsigned char outputByte = 0;
   // Ponteiro para a raiz da arvore que sera reconstruida
   NodeArvore* raiz;

   // Volta para o inicio do arquivo de entrada
   file.seekg(0, std::ios::beg);

   // Le do primeiro ao penultimo byte do arquivo, porque o ultimo nao precisa ser codificado, ele
   // eh inferido no processo de descompressao.
   for (unsigned i = 0; i < fileLength - 1; i++) {
      file.read(&buffer, 1);
      inputByte = static_cast<unsigned char>(buffer);

      // Dado o byte a ser compactado, encontra-se ele no array de codigos e percorre-se os bits do
      // codigo
      for (unsigned j = 0; j < bytesCodes[inputByte].size(); j++) {
         // Se o bit do codigo eh 1, coloca o bit 1 no byte compactado
         if (bytesCodes[inputByte][j])
            outputByte++;
         // E incrementa-se o contador de bits
         bitsCounter++;
         // A qualquer momento, quando os bits compactados completarem 1 byte, ele eh gravado no
         // array de bytes do arquivo compactado
         if (bitsCounter >= 8) {
            compactedFile.push_back(outputByte);
            bitsCounter = 0;
            outputByte = 0;
         }
         else
            // Depois de lido o bit do codigo e caso nao tenha sido completado 1 byte, desloca-se a
            // sequencia de bits para a direita para receber o proximo bit
            outputByte = outputByte << 1;
      }  // for (unsigned j = 0...

      // Depois de codificado o byte, a sua frequencia eh decrementada na lista de nos
      for (unsigned j = 0; j < listaNos.size(); j++)
         // Procura byte na lista de nos
         if (listaNos[j]->getByte() == inputByte) {
            listaNos[j]->decrementaFrequencia();
            // Encontrado o byte na lista de nos, sua frequencia eh decrementada e o loop pode ser
            // interrompido
            break;
         }
      // Frequencia decrementada, agora a arvore deve ser reconstruida
      raiz = buildHuffmanTree(listaNos);

      // O codigo anterior tem que ser apagado
      for (unsigned j = 0; j < BYTE; j++)
         bytesCodes[j].resize(0);

      // E em seguida percorre a nova arvore e recria o codigo
      // Passa um vector de bool nao incializado
      std::vector<bool> code;
      traverseTree(raiz, bytesCodes, code);
   }  // for (unsigned i = 0...

   // Se o ultimo byte precisar ser completado, o codigo a seguir o completa com zeros.
   // Note que o byte incompleto ja sai do loop dos for com um zero a mais no final, por isso agora
   // a comparacao eh com 7 e nao mais com 8
   while ((bitsCounter > 0) && (bitsCounter < 7)) {
      bitsCounter++;
      outputByte = outputByte << 1;
   }
   // Se o que restou dos for aninhados foi um byte incompleto, agora ele esta completado com zeros
   // e sera acrescentado no array do arquivo compactado
   if (bitsCounter >= 7)
      compactedFile.push_back(outputByte);

   std::cout << "Size of compressed bytes: " << compactedFile.size() << std::endl;

}  // void compressFile...

//https://stackoverflow.com/questions/759707/efficient-way-of-storing-huffman-tree
void encodeTree(NodeArvore* raiz, std::vector<unsigned char>& treeArray) {
   unsigned frequency;

   // Se eh um no folha, grava byte igual a 1 e em seguinda o valor do byte do no seguido do valor
   // da frequencia dele
   if (raiz->getByte() > -1) {
      treeArray.push_back(1);
      treeArray.push_back(raiz->getByte());
      // Divide o unsigned em 4 bytes e os grava no array
      frequency = raiz->getFrequencia();
      treeArray.push_back(frequency % BYTE);
      frequency /= BYTE;
      treeArray.push_back(frequency % BYTE);
      frequency /= BYTE;
      treeArray.push_back(frequency % BYTE);
      treeArray.push_back(frequency / BYTE);
   }
   // Se nao eh um no folha, grava byte igual a 0 e vai descendo na arvore, primeiro para a esquerda
   else {
      treeArray.push_back(0);
      encodeTree(raiz->getEsquerda(), treeArray);
      encodeTree(raiz->getDireita(), treeArray);
   }
}

std::ofstream createCompressedFile(char* originalFilename) {
   std::ofstream file;
   unsigned charsCounter = 0;
   char filename[BYTE];

   strcpy(filename, originalFilename);

   while (filename[charsCounter] != '\0')
      charsCounter++;

   // Muda a entensao do arquivo
   filename[charsCounter-3] = 'h';
   filename[charsCounter-2] = 'u';
   filename[charsCounter-1] = 'f';

   std::cout << "Opening output file: " << filename << std::endl;
   file.open(filename, std::ios::out | std::ios::binary);

   if(file.is_open()) {
      return file;
   }
   else {
      std::cerr << "Error opening output file: " << filename << std::endl;
      exit(1);
   }
}

void writeCompressedFile(std::ofstream& file, unsigned fileLength, char* originalFilename,
               std::vector<unsigned char>& treeArray, std::vector<unsigned char>& compactedFile) {
   // Variavel auxiliar para gravar o nome do arquivo original
   unsigned charsCounter = 0;
   // Variavel auxiliar para quebrar o valor do tamanho do arquivo original em 4 bytes
   unsigned char fileLengthBytes[4];
   // Variaveis auxiliares para quebrar o valor do tamanho da arvore codificada em 4 bytes
   unsigned treeSize;
   unsigned char treeSizeBytes[4];
   // Variavel auxiliar para quebrar o valor do tamanho do arquivo compactado em 4 bytes
   unsigned compactedSize;
   unsigned char compactedSizeBytes[4];

   // Formato do arquivo:
   // 1) Tamanho do arquivo original
   // 2) Nome do arquivo original (com extensao)
   // 3) Tamanho da arvore codificada em um array
   // 4) Arvore codificada
   // 5) Tamanho do arquivo depois de compactado
   // 6) Arquivo compactado

   // 1) Tamanho do arquivo original
   fileLengthBytes[0] = fileLength % BYTE;
   fileLength /= BYTE;
   fileLengthBytes[1] = fileLength % BYTE;
   fileLength /= BYTE;
   fileLengthBytes[2] = fileLength % BYTE;
   fileLengthBytes[3] = fileLength / BYTE;
   for (int i = 0; i < 4; i++)
      file << fileLengthBytes[i];

   // 2) Nome do arquivo original (com extensao)
   while (originalFilename[charsCounter] != '\0') {
      file << (unsigned char) originalFilename[charsCounter];
      charsCounter++;
   }

   // 3) Tamanho da arvore codificada em um array
   treeSize = treeArray.size();
   treeSizeBytes[0] = treeSize % BYTE;
   treeSize /= BYTE;
   treeSizeBytes[1] = treeSize % BYTE;
   treeSize /= BYTE;
   treeSizeBytes[2] = treeSize % BYTE;
   treeSizeBytes[3] = treeSize / BYTE;
   for (int i = 0; i < 4; i++)
      file << treeSizeBytes[i];

   // 4) Arvore codificada
   for (unsigned i = 0; i < treeArray.size(); i++)
      file << treeArray[i];

   // 5) Tamanho do arquivo depois de compactado
   compactedSize = compactedFile.size();
   compactedSizeBytes[0] = compactedSize % BYTE;
   compactedSize /= BYTE;
   compactedSizeBytes[1] = compactedSize % BYTE;
   compactedSize /= BYTE;
   compactedSizeBytes[2] = compactedSize % BYTE;
   compactedSizeBytes[3] = compactedSize / BYTE;
   for (int i = 0; i < 4; i++)
      file << compactedSizeBytes[i];

   // 6) Arquivo compactado
   for (unsigned i = 0; i < compactedFile.size(); i++)
      file << compactedFile[i];

   std::cout << "File compressed!" << std::endl << std::endl;
}
