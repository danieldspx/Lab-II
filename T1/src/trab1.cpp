#include <iostream>
#include <fstream>
#include <vector>
#include "headers/structs.h"

using namespace std;

int main(){
    ifstream inPontos;

    inPontos.open("assets/pontos.txt");

    if(!inPontos.is_open()){
        cout << "Não foi possível abrir o arquivo texto" << endl;
        exit(1);
    }


    vector<Distancia> distancias;

    Coordenates firstCoord, secondCoord;

    while(!inPontos.eof()){
        inPontos >> firstCoord.x >> firstCoord.y;

        inPontos >> secondCoord.x >> secondCoord.y;

        distancias.push_back(Distancia{firstCoord, secondCoord});
    }

    int contador = 1;
    for(auto& distancia: distancias){
        printf("Distancia %d %.2lf\n", contador, distancia.calculaDistancia());//Shoter than cout sintax
        contador++;
    }

    return 0;
}