#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <iomanip>

using namespace std;

struct Coordenates {
    double x;
    double y;
};

struct Distancia {
    Coordenates firstCoord;
    Coordenates secondCoord;
    double calculaDistancia(){
        return sqrt( pow((firstCoord.x - secondCoord.x), 2) + pow((firstCoord.y - secondCoord.y), 2) );
    }
};

int main(){
    ifstream inPontos;

    inPontos.open("pontos.txt");

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
      cout << setprecision(2) << "Distancia " << contador << " " << distancia.calculaDistancia() << endl;
        contador++;
    }

    return 0;
}
