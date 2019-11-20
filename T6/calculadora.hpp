#pragma once

#include <stack>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <list>
#include <map>
#include <ctime>


using namespace std;


struct Operador
{
    char op;
    int weight;
    bool operator<(Operador &o)
    {
        return weight < o.weight;
    }
    bool operator>(Operador &o)
    {
        return weight > o.weight;
    }
};

template <typename T>
struct Node
{
    bool isSymbol;
    char operador; //Simbolo
    T operando;    //Numero
    int hash;

    Node(char op, int hashID)
    {
        isSymbol = true;
        operador = op;
        operando = 0;
        hash = hashID;
    }

    Node(T op, int hashID)
    {
        isSymbol = false;
        operador = 0;
        operando = op;
        hash = hashID;
    }

    Node(int hashID)
    {
        isSymbol = false;
        operador = 0;
        operando = 0;
        hash = hashID;
    }

    bool operator==(const Node<T> &o) const
    {
        if (isSymbol == o.isSymbol)
        {
            return hash == o.hash;
            if (isSymbol)
            {
                return operador == o.operador;
            }
            else
            {
                return operando == o.operando;
            }
        }
        return false;
    }

    bool operator<(const Node<T> &o) const
    {
        return hash < o.hash;
        if (isSymbol == o.isSymbol)
        {
            if (isSymbol)
            {
                return operador < o.operador;
            }
            else
            {
                return operando < o.operando;
            }
        }
        else
        {
            if (isSymbol)
            {
                return operador < o.operando;
            }
            else
            { //No o.isSymbol = true
                return operando < o.operador;
            }
        }
    }
};

template <typename T>
struct Calculadora
{
    std::stack<Node<T>> operandos;      // números, basicamente
    std::stack<char> operadores;        // operadores: +, -, /, *
    std::stack<Node<T>> operandosNode;  // numeros
    std::stack<Node<T>> operadoresNode; // operadores: +, -, /, *
    std::map<Node<T>, std::vector<Node<T>>> tree;
    std::vector<char> shadowOperadores;
    std::vector<Operador> orderedOperadores;
    std::vector<T> shadowOperandos;
    std::vector<Node<T>> shadowOperadoresNode;
    std::vector<Node<T>> shadowOperandosNode;
    T   resultadoCalc;
    int globalHashID = 0;
    
    bool hasSyntaxError = false;

    int getHash(){
        return globalHashID++;
    }

    void reset()
    {
        resultadoCalc = 0;
        operandos = std::stack<Node<T>>();
        operadores = std::stack<char>();
        shadowOperadores.clear();
        orderedOperadores.clear();
        shadowOperandos.clear();
        shadowOperadoresNode.clear();
        shadowOperandosNode.clear();
        hasSyntaxError = false;
        tree.clear();
        operandosNode = std::stack<Node<T>>();
        operadoresNode = std::stack<Node<T>>();
    }

    // destroi a calculadora, limpa as pilhas
    void destroi()
    {
        while (operandos.empty() == false)
            operandos.pop();
        while (operadores.empty() == false)
            operadores.pop();
    }

    // insere um novo operando para o calculo
    void operando(T op)
    {
        Node<T> val((T)op, getHash());
        operandos.push(val);
    }

    // insere um novo operador para o calculo (pode ser '+', '-', '*', '/', '^')
    // se for um abre parenteses '(' tudo ok.
    // se for um fecha parenteses ')', deve-se calcular tudo dentro.
    void operador(char op)
    {
        if (hasSyntaxError)
            return; //Caso tenha algum erro de sintaxe

        if (op == ')')
        {
            resolveStack();
        }
        else
        {

            if ((op == '-' || op == '+') && operadores.empty() && operandos.empty())
            {
                Node<T> val((T)0, getHash());
                operandos.push(val); //This is because the expression can start with e.g.: -NUM so we make it 0-NUM
            }
            operadores.push(op);
        }
    }

    void insertionSort(std::vector<Operador> &array)
    {
        int cursor;
        Operador key;
        for (int key_pos = 1; key_pos < static_cast<int>(array.size()); key_pos++)
        {
            key = array[key_pos];
            cursor = key_pos - 1;
            while (cursor >= 0 && array[cursor] > key)
            {
                array[cursor + 1] = array[cursor];
                cursor--;
            }
            array[cursor + 1] = key;
        }
    }

    void linkNodes()
    {
        if (operadoresNode.empty() || operandosNode.size() < 2)
        {
            hasSyntaxError = true;
            return;
        }
        Node<T> root = operadoresNode.top();
        operadoresNode.pop();
        Node<T> childRight = operandosNode.top();
        operandosNode.pop();
        Node<T> childLeft = operandosNode.top();
        operandosNode.pop();
        std::vector<Node<T>> children = {childLeft, childRight};
        tree.insert(std::pair<Node<T>, std::vector<Node<T>>>(root, children));
        operandos.push(root);
    }

    void resolveStack()
    {
        if (operadores.empty())
            return;

        shadowOperadores.clear();
        orderedOperadores.clear();
        shadowOperandos.clear();
        shadowOperadoresNode.clear();
        shadowOperandosNode.clear();

        int count = 0;
        while (!operadores.empty())
        { //Copy Operadores
            if (operadores.top() != '(')
            {
                count++;
                shadowOperadores.push_back(operadores.top());
                operadores.pop(); //Remove the top element
            }
            else
            {
                operadores.pop();
                break;
            }
        }

        if (shadowOperadores.empty())
            return;

        if (count + 1 > static_cast<int>(operandos.size()) + static_cast<int>(operandosNode.size()))
        {
            hasSyntaxError = true;
            return;
        }

        //O total de operadores e uma unidade menor que o numero de operandos
        for (int i = 0; i < count + 1; i++)
        { //Copy Operandos
            shadowOperandosNode.push_back(operandos.top());
            operandos.pop();
        }

        if (shadowOperandosNode.size() <= shadowOperadores.size())
        {
            hasSyntaxError = true;
            return;
        }

        //Reverse os vector pq ai tirando da pilha vai ser como a precedencia da esquerda pra direita
        //Sem o reverse seria como se efetuassemos os calculos da direita pra esquerda.
        // std::reverse(shadowOperadores.begin(), shadowOperadores.end());
        // std::reverse(shadowOperandosNode.begin(), shadowOperandosNode.end());

        for (auto &v : shadowOperadores)
        {
            orderedOperadores.push_back({v, operadorWeight(v)});
        }

        insertionSort(orderedOperadores);

        if (shadowOperadores.size() == 0)
        {
            if (shadowOperandosNode.size() == 1)
            {
                operandos.push(shadowOperandosNode.at(0));
            }
            else
            {
                hasSyntaxError = true;
            }
            return;
        }

        for (char op : shadowOperadores)
        {
            Node<T> operador(op, getHash());
            shadowOperadoresNode.push_back(operador);
        }

        for (int i = 0; i < static_cast<int>(orderedOperadores.size()); i++)
        {
            for (int pos = 0; shadowOperadoresNode.size(); pos++)
            {
                if (shadowOperadoresNode.at(pos).operador == orderedOperadores.at(i).op)
                {
                    //Send data to stack

                    Node<T> root = shadowOperadoresNode.at(pos);
                    operadoresNode.push(root);
                    // --
                    // operadores.push(shadowOperadores.at(pos));
                    eraseVectorPos(shadowOperadoresNode, pos);

                    Node<T> childRight = shadowOperandosNode.at(pos + 1);
                    operandosNode.push(childRight);
                    // --
                    // operandos.push(shadowOperandos.at(pos+1));
                    eraseVectorPos(shadowOperandosNode, pos + 1);

                    Node<T> childLeft = shadowOperandosNode.at(pos);
                    operandosNode.push(childLeft);
                    // --
                    // operandos.push(shadowOperandos.at(pos));
                    eraseVectorPos(shadowOperandosNode, pos);

                    //Calcula com o que tem na stack
                    //calcula();
                    linkNodes();

                    if (hasSyntaxError)
                        return;

                    //Coloca no `shadowOperandos` pra continuar o processo e remove o resultado de `operandos`
                    insertInPos(shadowOperandosNode, pos, operandos.top()); //Coloca zero
                    if (i != static_cast<int>(orderedOperadores.size()) - 1)
                    { //Se nao e a ultima operacao, entao remove da pilha operandos
                        operandos.pop();
                        // operandosNode.pop();
                    }

                    break;
                }
            }
        }
    }

    void eraseVectorPos(std::vector<Node<T>> &vect, int pos)
    {
        vect.erase(vect.begin() + pos);
    }

    void insertInPos(std::vector<Node<T>> &vect, int pos, Node<T> val)
    {
        vect.insert(vect.begin() + pos, val);
    }

    // finaliza o calculo, retorna false se erro detectado
    bool fim(void)
    {
        while(!operadores.empty() && hasSyntaxError == false){
            if (!hasSyntaxError){
                resolveStack();
            }
        }
        Node<T> root(getHash());
        try{
            if(operandos.empty()){
                hasSyntaxError = true;
                return false;
            }
            root = operandos.top();
            operandos.pop();
            resultadoCalc = calculaTree(root);
        } catch(const std::exception &e){
            reset();
            return false;
        }

        return !hasSyntaxError;
    }

    //Retorna true se a operacao foi realizada com sucesso
    bool calculaStackNormal()
    {
        if (operadores.empty())
            return true;

        char operadorOP = operadores.top();
        operadores.pop();

        if (operandos.size() < 2)
        {
            hasSyntaxError = true;
            return false;
        }

        T operando1 = operandos.top();
        operandos.pop();
        T operando2 = operandos.top();
        operandos.pop();

        T result;

        switch (operadorOP)
        {
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
            if (operando2 != 0)
            {
                result = operando1 / operando2;
            }
            else
            {
                hasSyntaxError = true;
                result = 0;
            }
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

    T calculaTree(Node<T> root)
    {
        T result = 0;
        if(root.isSymbol){
            T operando1 = calculaTree(tree.at(root)[0]);//Filho esquerda
            T operando2 = calculaTree(tree.at(root)[1]);//Filho direita
            switch (root.operador){
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
                    if (operando2 != 0)
                    {
                        result = operando1 / operando2;
                    }
                    else
                    {
                        hasSyntaxError = true;
                        result = 0;
                    }
                    break;
                case '^':
                    result = pow(operando1, operando2);
                    break;
                default:
                    hasSyntaxError = true;
                    return 0;
            }
        } else {
            return root.operando;
        }
        return result;
    }

    // retorna o resultado calculo (topo da pilha de operandos)
    // esse resultado sera parcial se ainda nao foi chamado calc_fim().
    T resultado(void)
    {
        return resultadoCalc;
        //return operandos.top();
    }

    int operadorWeight(char op)
    {
        if (op == '^')
        {
            return 1;
        }
        else if (op == '/')
        {
            return 2;
        }
        else if (op == '*')
        {
            return 3;
        }
        else if (op == '+')
        {
            return 4;
        }
        else if (op == '-')
        {
            return 5;
        }
        else
        {
            hasSyntaxError = true;
            return 6;
        }
    }
};