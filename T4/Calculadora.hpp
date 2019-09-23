#pragma once 

#include <stack>
#include <iostream>
#include <cmath>

template<typename T>
struct Calculadora {

    // ver http://en.cppreference.com/w/cpp/container/stack
    std::stack<T> operandos;    // números, basicamente
    std::stack<char> operadores; // operadores: +, -, /, *
    bool hasSyntaxError = false;
    // podem incluir mais dados, se necessário

    // cria a calculadora
    void cria(void){
    }

    // destroi a calculadora, limpa as pilhas
    void destroi(void){
        while(operandos.empty() == false)
            operandos.pop();
        while(operadores.empty() == false)
            operadores.pop();
    }

    // insere um novo operando para o calculo
    void operando(T op) {
        operandos.push(op);
    }

    // insere um novo operador para o calculo (pode ser '+', '-', '*', '/', '^')
    // se for um abre parenteses '(' tudo ok. 
    // se for um fecha parenteses ')', deve-se calcular tudo dentro.
    void operador(char op){
        // verficar aqui o operador inserido
        if(op == ')'){
            while(!operadores.empty() && operadores.top() != '(')
                calcula();

            if(!operadores.empty()){
                operadores.pop();
            } else {
                hasSyntaxError = true;
            }
        } else {
            operadores.push(op);
        }
        
    }

    // finaliza o calculo, retorna false se erro detectado
    bool fim(void) {
        while (!operadores.empty())
        {
            if(!calcula())
                return false;
        }
        
        return true && !hasSyntaxError;
    }

    //Retorna true se a operacao foi realizada com sucesso
    bool calcula(){
        if(operadores.empty())
            return true;

        char operador = operadores.top();
        operadores.pop();

        if(operandos.size() < 2) {
            hasSyntaxError = true;
            return false;
        }

        T operando1 = operandos.top();
        operandos.pop();
        T operando2 = operandos.top();
        operandos.pop();
        
        T result;

        switch (operador){
            case '+':
                result = operando1 + operando2;
                break;
            case '-':
                result = operando2 - operando1;
                break;
            case '*':
                result = operando2 * operando1;
                break;
            case '/':
                result = operando2 / operando1;
                break;
            case '^':
                result = pow(operando2, operando1);
                break;
            default:
                hasSyntaxError = true;
                return false;
        }

        operandos.push(result);
        return true;
    }

    // retorna o resultado calculo (topo da pilha de operandos)
    // esse resultado sera parcial se ainda nao foi chamado calc_fim().
    T resultado(void) {
        return operandos.top();
    }
};