#include <iostream>
#include <fstream>
#include <vector>

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
        printf("Distancia %d %.2lf\n", contador, distancia.calculaDistancia());//Shoter than cout sintax
        contador++;
    }

    return 0;
}
