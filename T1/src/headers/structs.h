#ifndef _structs_h
#define _structs_h
#include <math.h>

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

#endif