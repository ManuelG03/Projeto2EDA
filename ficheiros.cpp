#include <iostream>
#include <fstream>
#include <string>
#include "structs.h"

using namespace std;

void gravarListaDeEspera(ListaDeEspera* head) {
    fstream file;
    file.open("listaDeEspera.txt");

    ListaDeEspera* contarCarro = head;
    int numListaDeEspera = 0;

    while (contarCarro != nullptr) {
        numListaDeEspera++;

        contarCarro = contarCarro->next;
    }

    file << numListaDeEspera << endl;

    ListaDeEspera* carroEmEspera = head;
    while (carroEmEspera != nullptr) {
        file << carroEmEspera->data->id << endl;
        file << carroEmEspera->data->marca << endl;
        file << carroEmEspera->data->modelo << endl;
        file << carroEmEspera->data->tempo_reparacao << endl;
        file << carroEmEspera->data->custo_reparacao << endl;
        file << carroEmEspera->data->dias_ET << endl;
        if (carroEmEspera->data->prioridade) {
            file << "Sim" << endl;
        }
        else {
            file << "Nao" << endl;
        }

        carroEmEspera = carroEmEspera->next;
    }

    file.close();
}

ListaDeEspera* carregarListaDeEspera(ListaDeEspera* head) {
    fstream file;
    file.open("listaDeEspera.txt");

    string linha;
    int numListaDeEspera = 0;

    delete head;
    head = NULL;

    getline(file, linha);
    numListaDeEspera = stoi(linha);

    ListaDeEspera* listadeespera = nullptr;
    head = listadeespera;

    for (int i = 0; i < numListaDeEspera; i++) {

        getline(file, linha);
        listadeespera->data->id = stoi(linha);
        getline(file, listadeespera->data->marca);
        getline(file, listadeespera->data->modelo);
        getline(file, linha);
        listadeespera->data->tempo_reparacao = stoi(linha);
        getline(file, linha);
        listadeespera->data->custo_reparacao = stoi(linha);
        getline(file, linha);
        listadeespera->data->dias_ET = stoi(linha);
        getline(file, linha);
        if (linha == "Sim") {
            listadeespera->data->prioridade = true;
        }
        else if (linha == "Nao") {
            listadeespera->data->prioridade = false;
        }

        

        listadeespera = listadeespera->next;
    }

    return head;
}

void gravarEstacoes(ET* ethead, int numETs) {
    fstream fileEstacoes;
    fileEstacoes.open("estacoes.txt");

    fstream fileReparados;
    fileReparados.open("carrosReparados.txt");

    fileEstacoes << numETs << endl;

    ET* estacao = ethead;

    while (estacao != nullptr) {
        fileEstacoes << estacao->id << endl;
        fileEstacoes << estacao->capacidade << endl;
        fileEstacoes << estacao->mecanico << endl;
        fileEstacoes << estacao->marca << endl;
        fileEstacoes << estacao->carros_reparados << endl;
        fileEstacoes << estacao->capacidade_atual << endl;
        fileEstacoes << estacao->faturacao << endl;

        carro* carroNaET = estacao->carros;
        while (carroNaET != nullptr) {
            fileEstacoes << carroNaET->id << endl;
            fileEstacoes << carroNaET->marca << endl;
            fileEstacoes << carroNaET->modelo << endl;
            fileEstacoes << carroNaET->tempo_reparacao << endl;
            fileEstacoes << carroNaET->custo_reparacao << endl;
            fileEstacoes << carroNaET->dias_ET << endl;
            fileEstacoes << carroNaET->prioridade << endl;

            carroNaET = carroNaET->next;
        }

        BSTNode* carroReparado = estacao->repaired_cars;
        while (carroReparado != nullptr) {
            fileReparados << carroReparado->data.id << endl;
            fileReparados << carroReparado->data.marca << endl;
            fileReparados << carroReparado->data.modelo << endl;
            fileReparados << carroReparado->data.tempo_reparacao << endl;
            fileReparados << carroReparado->data.custo_reparacao << endl;
            fileReparados << carroReparado->data.dias_ET << endl;
            fileReparados << carroReparado->data.prioridade << endl;

            carroReparado = carroReparado->right;
        }

        estacao = estacao->next;
    }

    fileEstacoes.close();
    fileReparados.close();
}

void gravarFicheiros(ListaDeEspera* head, ET* ethead, int numETs) {
    gravarListaDeEspera(head);
    gravarEstacoes(ethead, numETs);
}

void carregarFicheiros(ListaDeEspera* head) {
    carregarListaDeEspera(head);
}