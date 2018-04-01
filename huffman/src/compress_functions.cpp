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

NodeArvore* buildHuffmanTree(std::vector<NodeArvore*>& listaNos) {
   NodeArvore* one;
   NodeArvore* two;
   NodeArvore* internalNode = nullptr;

   while (listaNos.size() > 1) {
      std::sort(listaNos.begin(), listaNos.end(), NodeArvore::compare);

      one = listaNos[listaNos.size()-1];
      listaNos.pop_back();
      two = listaNos[listaNos.size()-1];
      listaNos.pop_back();
      // O valor da variavel membro byte do internalNode eh sempre -1 porque ele nao representa um
      // byte apenas
      internalNode = new NodeArvore(-1, one->getFrequencia() + two->getFrequencia(), one, two,
                                    nullptr);
      one->setPai(internalNode);
      two->setPai(internalNode);
      listaNos.push_back(internalNode);
   }

   return internalNode;
}

void traverseTree(NodeArvore* raiz, std::vector<bool> bytesCodes[], std::vector<bool> code) {
   std::vector<bool> left, right;
   // Quando vai pro no esquerdo, acrescenta um '0' ao codigo
   left = code;
   left.push_back(0);
   // Quando vai pro no direito, acrescenta um '1' ao codigo
   right = code;
   right.push_back(1);

   if (raiz->getByte() > -1)
      bytesCodes[raiz->getByte()] = code;

   if (raiz->getEsquerda() != nullptr)
      traverseTree(raiz->getEsquerda(), bytesCodes, left);
   if (raiz->getDireita() != nullptr)
      traverseTree(raiz->getDireita(), bytesCodes, right);
}

void compactFile(std::ifstream& file, unsigned fileLength, std::vector<bool> bytesCodes[],
                 std::vector<unsigned char>& compressedFile) {
   // A leitura do arquivo se da byte por byte, cada byte eh primeiro armazenado no buffer
   char buffer;
   // O buffer eh depois convertido num byte sem sinal
   unsigned char inputByte;
   // Contador para contar os bits ateh completar um byte
   unsigned char byteCounter = 0;
   // Byte que eh montado para depois ser colocado no arquivo compactado
   unsigned char outputByte = 0;

   // Volta para o inicio do arquivo de entrada
   file.seekg(0, std::ios::beg);

   for (unsigned i = 0; i < fileLength; i++) {
      file.read(&buffer, 1);
      inputByte = static_cast<unsigned char>(buffer);

      for (unsigned j = 0; j < bytesCodes[inputByte].size(); j++) {
         if (bytesCodes[inputByte][j])
            outputByte++;
         byteCounter++;
         if (byteCounter >= 8) {
            compressedFile.push_back(outputByte);
            byteCounter = 0;
            outputByte = 0;
         }
         else
            outputByte = outputByte << 1;
      }  // for (unsigned j = 0...
   }  // for (unsigned i = 0...

   // Se o ultimo byte precisar ser completado, o codigo a seguir o completa com zeros.
   // Note que o byte incompleto ja sai do loop dos for com um zero a mais no final, por isso agora
   // a comparacao eh com 7 e nao mais com 8
   while ((byteCounter > 0) && (byteCounter < 7)) {
      byteCounter++;
      outputByte = outputByte << 1;
   }
   // Se o que restou dos for aninhados foi um byte incompleto, agora ele esta completado com zeros
   // e sera acrescentado no array do arquivo compactado
   if (byteCounter >= 7)
      compressedFile.push_back(outputByte);

   std::cout << "Size of compressed bytes: " << compressedFile.size() << std::endl;

}  // void compactFile...

void encodeTree(NodeArvore* raiz, std::vector<unsigned char>& treeArray) {
   unsigned frequency;

   // Se eh um no folha, grava byte igual a 1 e em seguinda o valor do byte do no seguido do valor
   // da frequencia dele
   if (raiz->getByte() > -1) {
      treeArray.push_back(1);
      treeArray.push_back(raiz->getByte());
      // Divide o unsigned em 4 bytes e os grava no array
      frequency = raiz->getFrequencia();
      treeArray.push_back(frequency % 256);
      frequency /= 256;
      treeArray.push_back(frequency % 256);
      frequency /= 256;
      treeArray.push_back(frequency % 256);
      treeArray.push_back(frequency / 256);
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
               std::vector<unsigned char>& treeArray, std::vector<unsigned char>& compressedFile) {
   // Variavel auxiliar para gravar o nome do arquivo original
   unsigned charsCounter = 0;
   // Variavel auxiliar para quebrar o valor do tamanho do arquivo original em 4 bytes
   unsigned char fileLengthBytes[4];
   // Variaveis auxiliares para quebrar o valor do tamanho da arvore codificada em 4 bytes
   unsigned treeSize;
   unsigned char treeSizeBytes[4];
   // Variavel auxiliar para quebrar o valor do tamanho do arquivo compactado em 4 bytes
   unsigned compressedSize;
   unsigned char compressedSizeBytes[4];

   // Formato do arquivo:
   // 1) Tamanho do arquivo original
   // 2) Nome do arquivo original (com extensao)
   // 3) Tamanho da arvore codificada em um array
   // 4) Arvore codificada
   // 5) Tamanho do arquivo depois de compactado
   // 6) Arquivo compactado

   // 1) Tamanho do arquivo original
   fileLengthBytes[0] = fileLength % 256;
   fileLength /= 256;
   fileLengthBytes[1] = fileLength % 256;
   fileLength /= 256;
   fileLengthBytes[2] = fileLength % 256;
   fileLengthBytes[3] = fileLength / 256;
   for (int i = 0; i < 4; i++)
      file << fileLengthBytes[i];

   // 2) Nome do arquivo original (com extensao)
   while (originalFilename[charsCounter] != '\0') {
      file << (unsigned char) originalFilename[charsCounter];
      charsCounter++;
   }

   // 3) Tamanho da arvore codificada em um array
   treeSize = treeArray.size();
   treeSizeBytes[0] = treeSize % 256;
   treeSize /= 256;
   treeSizeBytes[1] = treeSize % 256;
   treeSize /= 256;
   treeSizeBytes[2] = treeSize % 256;
   treeSizeBytes[3] = treeSize / 256;
   for (int i = 0; i < 4; i++)
      file << treeSizeBytes[i];

   // 4) Arvore codificada
   for (unsigned i = 0; i < treeArray.size(); i++)
      file << treeArray[i];

   // 5) Tamanho do arquivo depois de compactado
   compressedSize = compressedFile.size();
   compressedSizeBytes[0] = compressedSize % 256;
   compressedSize /= 256;
   compressedSizeBytes[1] = compressedSize % 256;
   compressedSize /= 256;
   compressedSizeBytes[2] = compressedSize % 256;
   compressedSizeBytes[3] = compressedSize / 256;
   for (int i = 0; i < 4; i++)
      file << compressedSizeBytes[i];

   // 6) Arquivo compactado
   for (unsigned i = 0; i < compressedFile.size(); i++)
      file << compressedFile[i];

   std::cout << "File compressed!" << std::endl << std::endl;
}