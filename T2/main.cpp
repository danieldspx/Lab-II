#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

// Estrutura  do Editor
struct Editor {
   std::vector<char*> linhas;

   void insere(std::string& linha) {
      char* newLinha = new char[linha.length() + 1];
      std::strcpy(newLinha, linha.c_str());
      linhas.push_back(newLinha);
   }

   char* remove(void) {
      char* l = linhas.back();  // pega ultima linha
      linhas.pop_back();        // remove ultima linha
      return l;
   }

   int tamanho(void) {
      return linhas.size();
   }

   void destroi(void) {
      for (auto& linha : linhas) {
         delete linha;
      }
   }
};  // fim do editor

// programa
int main(int argc, char** argv) {
   std::string linha;
   Editor editor;

   std::ifstream entrada{"texto.txt"};
   while (std::getline(entrada, linha)) {
      editor.insere(linha);
   }

   auto tamanho = editor.tamanho();
   for (auto i = 0; i < tamanho; i++) {
        char *linhaRef = editor.remove();
        std::cout << "DEL: " << linhaRef << std::endl;
        delete linhaRef;
   }

   editor.destroi();

   return 0;
}