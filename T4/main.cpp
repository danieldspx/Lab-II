#include <iostream>
#include "Calculadora.hpp"

void testaFloat(void)
{
    Calculadora<float> calc;
    float x;

    calc.operando(13.1);
    calc.operador('+');
    calc.operando(17.6);
    
    if(calc.fim() == false)
        std::cout << "ERRO: formula errada!" << std::endl;
    
    x = calc.resultado();
    std::cout << "13.1+17.6 = " << x << std::endl;
    calc.destroi();
}

void testaInt(void)
{
    Calculadora<int> calc;
    int x;

    // calc.operador('(');
    calc.operador('(');
    calc.operando(2);
    calc.operador(')');
    calc.operador('+');
    calc.operando(1);
    calc.operador('^');
    calc.operando(2);
    // calc.operador(')');
    // calc.operador('*');
    // calc.operando(2);


    if(calc.fim() == false)
        std::cout << "ERRO: formula errada!" << std::endl;
    
    x = calc.resultado();
    std::cout << "((2+13)+2)*2 = " << x << std::endl;
    calc.destroi();
}

int main(int argc, char** argv)
{
    testaFloat();
    testaInt();
    return 0;
}