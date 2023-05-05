#pragma once

using namespace std;

struct umCarro {
    int id = 0;
    string marca = "";
    string modelo = "";
    int tempo_reparacao = 0;
    int custo_reparacao = 0;
    int dias_ET = 0;
    bool prioridade = "false";
};

struct carro {
    umCarro data;
    carro* next = nullptr;
};

struct BSTNode {
    umCarro data;
    BSTNode* left = nullptr;
    BSTNode* right = nullptr;
};

struct ET {
    int id = 0;
    int capacidade = 0;
    string mecanico = "";
    string marca = "";
    carro* carros = new carro;
    BSTNode* repaired_cars = nullptr;
    int carros_reparados = 0;
    int capacidade_atual = 0;
    int faturacao = 0;
    ET* next = nullptr;
};

struct ListaDeEspera {
    umCarro data;
    ListaDeEspera* next = nullptr;
};

struct caminhosFicheiros {
    string caminhoListaDeEspera = "";
    string caminhoEstacoes = "";
    string caminhoCarrosReparados = "";
};