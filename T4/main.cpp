#include <iostream>
#include "Calculadora.hpp"

void testaFloat(void)
{
    Calculadora<float> calc;
    float x;

    calc.operando(0.5);
    calc.operador('^');
    calc.operando(2);
    
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

    calc.operador('(');
    calc.operador('(');
    calc.operando(3);
    calc.operador(')');
    calc.operador('*');
    calc.operando(4);
    calc.operador('+');
    calc.operando(5);
    calc.operador('^');
    calc.operando(2);
    calc.operador('-');
    calc.operando(1);
    calc.operador(')');
    //RIGHT 12+10 = 22
    //     
    //     5        
    // *   2       10
    // +   3    +   3
    // *   4    *   4


    if(calc.fim() == false)
        std::cout << "ERRO: formula errada!" << std::endl;
    
    x = calc.resultado();
    std::cout << "((3)*4+5^2-1) = " << x << std::endl;
    calc.destroi();
}

int main(int argc, char** argv)
{
    // testaFloat();
    testaInt();
    return 0;
}