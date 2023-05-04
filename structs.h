#pragma once

using namespace std;

struct carro {
    int id;
    string marca;
    string modelo;
    int tempo_reparacao;
    int custo_reparacao;
    int dias_ET;
    bool prioridade;
    carro* next; // Apontador para o prox. carro
};

struct BSTNode {
    carro data;
    BSTNode* left;
    BSTNode* right;
};

struct ET {
    int id;
    int capacidade;
    string mecanico;
    string marca;
    carro* carros;
    BSTNode* repaired_cars;
    int carros_reparados = 0;
    int capacidade_atual;
    int faturacao;
    ET* next;
};

struct ListaDeEspera {
    carro data;
    ListaDeEspera* next = nullptr;
};

struct caminhosFicheiros {
    string caminhoCarros;
    string caminhoListaDeEspera;
    string caminhoEstacoes;
    string caminhoCarrosReparados;
};