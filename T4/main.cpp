#include <iostream>
#include "Calculadora.hpp"

void testaFloat(void)
{
    Calculadora<float> calc;
    float x;
    //2.2/1*3.2
    // calc.operando(2.2);
    // calc.operador('/');
    // calc.operando(1);
    // calc.operador('*');
    // calc.operando(3.2);

    // 3.9+7*3-7.8/3^2
    // calc.operando(3.9);
    // calc.operador('+');
    // calc.operando(7);
    // calc.operador('*');
    // calc.operando(3);
    // calc.operador('-');
    // calc.operando(7.8);
    // calc.operador('/');
    // calc.operando(3);
    // calc.operador('^');
    // calc.operando(2);

    // 2.1*(3.9+7*3-7.8/3^2)+(2.2/1*3.2)
    calc.operando(2.1);
    calc.operador('*');
    calc.operador('(');
    calc.operando(3.9);
    calc.operador('+');
    calc.operando(7);
    calc.operador('*');
    calc.operando(3);
    calc.operador('-');
    calc.operando(7.8);
    calc.operador('/');
    calc.operando(3);
    calc.operador('^');
    calc.operando(2);
    calc.operador(')');
    calc.operador('+');
    calc.operador('(');
    calc.operando(2.2);
    calc.operador('/');
    calc.operando(1);
    calc.operador('*');
    calc.operando(3.2);
    calc.operador(')');
    
    if(calc.fim() == false)
        std::cout << "ERRO: formula errada!" << std::endl;
    
    x = calc.resultado();
    std::cout << "2.1*(3.9+7*3-7.8/3^2)+(2.2/1*3.2) = " << x << std::endl;
    calc.destroi();
}

void testaInt(void)
{
    Calculadora<int> calc;
    int x;

    // 2*(3+7*3-8/4^2)+(8/1*4)
    calc.operando(2);
    calc.operador('*');
    calc.operador('(');
    calc.operando(3);
    calc.operador('+');
    calc.operando(7);
    calc.operador('*');
    calc.operando(3);
    calc.operador('-');
    calc.operando(8);
    calc.operador('/');
    calc.operando(4);
    calc.operador('^');
    calc.operando(2);
    calc.operador(')');
    calc.operador('+');
    calc.operador('(');
    calc.operando(8);
    calc.operador('/');
    calc.operando(1);
    calc.operador('*');
    calc.operando(4);
    calc.operador(')');


    if(calc.fim() == false)
        std::cout << "ERRO: formula errada!" << std::endl;
    
    x = calc.resultado();
    std::cout << "2*(3+7*3-8/4^2)+(8/1*4) = " << x << std::endl;
    calc.destroi();
}

int main(int argc, char** argv)
{
    testaFloat();
    testaInt();
    return 0;
}