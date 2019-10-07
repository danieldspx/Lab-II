#pragma once 

#include <stack>
#include <iostream>
#include <cmath>
#include <vector>
#include<algorithm>

struct Operador {
    char op;
    int weight;
    bool operator<(Operador &o) {
        return weight < o.weight;
    }
    bool operator>(Operador &o) {
        return weight > o.weight;
    }
};

template<typename T>

struct Calculadora {

    // ver http://en.cppreference.com/w/cpp/container/stack
    std::stack<T> operandos;    // números, basicamente
    std::stack<char> operadores; // operadores: +, -, /, *
    std::vector<char> shadowOperadores;
    std::vector<Operador> orderedOperadores;
    std::vector<T> shadowOperandos;

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
        if(hasSyntaxError) return;//Caso tenha algum erro de sintaxe
        
        if(op == ')'){
            resolveStack();
        } else {
            operadores.push(op);
        }   
    }

    void insertionSort(std::vector<Operador> &array){
        int cursor;
        Operador key;
        bool end;
        for(int key_pos = 1; key_pos < array.size(); key_pos++){
            key = array[key_pos];
            cursor = key_pos - 1;
            while(cursor >= 0 && array[cursor] > key){
                array[cursor+1] = array[cursor];
                cursor--;
            }
            array[cursor+1] = key;
        }
    }

    void resolveStack(){
        if(operadores.empty()) return;

        shadowOperadores.clear();
        orderedOperadores.clear();
        shadowOperandos.clear();

        int count = 0;
        while(!operadores.empty()){//Copy Operadores
            if(operadores.top() != '('){
                count++;
                shadowOperadores.push_back(operadores.top());
                operadores.pop();//Remove the top element
            } else {
                operadores.pop();
                break;
            }
        }

        if(shadowOperadores.empty()) return;

        if(count+1 > operandos.size()){
            hasSyntaxError = true;
            return;
        }
        
        //O total de operadores e uma unidade menor que o numero de operandos 
        for(int i = 0; i < count+1; i++){//Copy Operandos
            shadowOperandos.push_back(operandos.top());
            operandos.pop();
        }

        if(shadowOperandos.size() <= shadowOperadores.size()){
            hasSyntaxError = true;
            return;
        }

        //Reverse os vector pq ai tirando da pilha vai ser como a precedencia da esquerda pra direita
        //Sem o reverse seria como se efetuassemos os calculos da direita pra esquerda.
        std::reverse(shadowOperadores.begin(), shadowOperadores.end());
        std::reverse(shadowOperandos.begin(), shadowOperandos.end());

        for(auto &v: shadowOperadores){
            orderedOperadores.push_back({v, operadorWeight(v)});
        }
        
        insertionSort(orderedOperadores);

        if(shadowOperadores.size() == 0){
            if(shadowOperandos.size() == 1){
                operandos.push(shadowOperandos.at(0));
            } else {
                hasSyntaxError = true;
            }
            return;
        }

        for(int i = 0; i < orderedOperadores.size(); i++){
            for(int pos = 0; shadowOperadores.size(); pos++){
                if(shadowOperadores.at(pos) == orderedOperadores.at(i).op){
                    //Send data to stack
                    operadores.push(shadowOperadores.at(pos));
                    eraseVectorCharPos(shadowOperadores, pos);

                    operandos.push(shadowOperandos.at(pos+1));
                    eraseVectorPos(shadowOperandos, pos+1);

                    operandos.push(shadowOperandos.at(pos));
                    eraseVectorPos(shadowOperandos, pos);
                    
                    //Calcula com o que tem na stack
                    calcula();

                    //Coloca no `shadowOperandos` pra continuar o processo e remove o resultado de `operandos`
                    insertInPos(shadowOperandos, pos, operandos.top());
                    if(i != orderedOperadores.size() - 1){//Se nao e a ultima operacao, entao remove da pilha operandos
                        operandos.pop();
                    }

                    break; 
                }
            }
        }
    }

    void eraseVectorPos(std::vector<T> &vect, int pos){
        vect.erase(vect.begin() + pos);
    }
    void eraseVectorCharPos(std::vector<char> &vect, int pos){
        vect.erase(vect.begin() + pos);
    }

    void insertInPos(std::vector<T> &vect, int pos, T val){
        vect.insert(vect.begin() + pos, val); 
    }

    // finaliza o calculo, retorna false se erro detectado
    bool fim(void) {
        if(!hasSyntaxError){
            resolveStack();
        }
        
        return !hasSyntaxError;
    }

    //Retorna true se a operacao foi realizada com sucesso
    bool calcula(){
        if(operadores.empty())
            return true;

        char operadorOP = operadores.top();
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

        switch (operadorOP){
            case '+':
                result = operando1 + operando2;
                break;
            case '-':
                result = operando1 - operando2;
                break;
            case '*':
                result = operando1 * operando2;
                break;
            case '/':
                result = operando1 / operando2;
                break;
            case '^':
                result = pow(operando1, operando2);
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

    int operadorWeight(char op){
        if(op == '^'){
            return 1;
        } else if(op == '*' || op == '/'){
            return 2;
        } else if(op == '+' || op == '-'){
            return 3;
        } else {
            hasSyntaxError = true;
            return 4;
        }
    }
};