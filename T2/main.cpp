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

   std::string remove(void) {
      std::string linhaReturn = linhas.back();  // pega ultima linha
      delete linhas.back();
      linhas.pop_back();        // remove ultima linha
      return linhaReturn;
   }

   int tamanho(void) {
      return linhas.size();
   }

   void destroi(void) {
      for(auto &linha: linhas){
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
      linha = editor.remove();
      std::cout << "DEL: " << linha << std::endl;
   }

   return 0;
}