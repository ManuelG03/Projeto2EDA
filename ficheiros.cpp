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
        file << carroEmEspera->data.id << endl;
        file << carroEmEspera->data.marca << endl;
        file << carroEmEspera->data.modelo << endl;
        file << carroEmEspera->data.tempo_reparacao << endl;
        file << carroEmEspera->data.custo_reparacao << endl;
        file << carroEmEspera->data.dias_ET << endl;
        if (carroEmEspera->data.prioridade) {
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

    for (int i = 0; i < numListaDeEspera; i++) {
        ListaDeEspera* listadeespera = new ListaDeEspera;
        carro carroEmEspera;

        getline(file, linha);
        carroEmEspera.id = stoi(linha);
        getline(file, carroEmEspera.marca);
        getline(file, carroEmEspera.modelo);
        getline(file, linha);
        carroEmEspera.tempo_reparacao = stoi(linha);
        getline(file, linha);
        carroEmEspera.custo_reparacao = stoi(linha);
        getline(file, linha);
        carroEmEspera.dias_ET = stoi(linha);
        getline(file, linha);
        if (linha == "Sim") {
            carroEmEspera.prioridade = true;
        }
        else if (linha == "Nao") {
            carroEmEspera.prioridade = false;
        }

        listadeespera->data = carroEmEspera;

        if (i == 0) {
            head = listadeespera;
        }
        else {
            ListaDeEspera* iterador = head;

            while (iterador->next != NULL) {
                iterador = iterador->next;
            }

            iterador->next = listadeespera;
        }

        listadeespera = listadeespera->next;
    }

    return head;
} 

void gravarCarrosReparados(BSTNode* carrosReparados, fstream& fileReparados) {
    BSTNode* carroReparado = carrosReparados;

    if (carrosReparados == NULL) {
        return;
    }

    gravarCarrosReparados(carroReparado->left, fileReparados);

    fileReparados << carroReparado->data.id << endl;
    fileReparados << carroReparado->data.marca << endl;
    fileReparados << carroReparado->data.modelo << endl;
    fileReparados << carroReparado->data.tempo_reparacao << endl;
    fileReparados << carroReparado->data.custo_reparacao << endl;
    fileReparados << carroReparado->data.dias_ET << endl;
    if (carroReparado->data.prioridade) {
        fileReparados << "Sim" << endl;
    }
    else {
        fileReparados << "Nao" << endl;
    }

    gravarCarrosReparados(carroReparado->right, fileReparados);

}

void gravarEstacoes(ET* ethead, int numETs) {
    fstream fileReparados;
    fileReparados.open("carrosReparados.txt");

    fstream fileEstacoes;
    fileEstacoes.open("estacoes.txt");

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
            if (carroNaET->prioridade) {
                fileEstacoes << "Sim" << endl;
            }
            else {
                fileEstacoes << "Nao" << endl;
            }

            carroNaET = carroNaET->next;
        }

        gravarCarrosReparados(estacao->repaired_cars, fileReparados);

        estacao = estacao->next;
    }

    fileEstacoes.close();
    fileReparados.close();
}

ET* carregarEstacoes(ET* ethead) {
    fstream fileEstacoes;
    fileEstacoes.open("estacoes.txt");

    fstream fileReparados;
    fileReparados.open("carrosReparados.txt");

    string linha;
    int numETs = 0;

    delete ethead;
    ethead = NULL;

    getline(fileEstacoes, linha);
    numETs = stoi(linha);
    cout << "numETs: " << numETs << endl;

    for (int i = 0; i < numETs; i++) {

        ET* estacoes = new ET;

        getline(fileEstacoes, linha);
        estacoes->id = stoi(linha);
        getline(fileEstacoes, linha);
        estacoes->capacidade = stoi(linha);
        getline(fileEstacoes, estacoes->mecanico);
        getline(fileEstacoes, estacoes->marca);
        getline(fileEstacoes, linha);
        estacoes->carros_reparados = stoi(linha);
        getline(fileEstacoes, linha);
        estacoes->capacidade_atual = stoi(linha);
        getline(fileEstacoes, linha);
        estacoes->faturacao = stoi(linha);

        for (int j = 0; j < estacoes->capacidade_atual; j++) {
            carro* carroNaET = new carro;

            getline(fileEstacoes, linha);
            carroNaET->id = stoi(linha);
            getline(fileEstacoes, carroNaET->marca);
            getline(fileEstacoes, carroNaET->modelo);
            getline(fileEstacoes, linha);
            carroNaET->tempo_reparacao = stoi(linha);
            getline(fileEstacoes, linha);
            carroNaET->custo_reparacao = stoi(linha);
            getline(fileEstacoes, linha);
            carroNaET->dias_ET = stoi(linha);
            getline(fileEstacoes, linha);
            if (linha == "Sim") {
                carroNaET->prioridade = true;
            }
            else if (linha == "Nao") {
                carroNaET->prioridade = false;
            }

            carroNaET = carroNaET->next;
        }

        cout << "Estacao: " << estacoes->id << "--->>> Carros Reparados: " << estacoes->carros_reparados << endl;

        for (int k = 0; k < estacoes->carros_reparados; k++) {
            BSTNode* carroReparado = new BSTNode;

            getline(fileReparados, linha);
            carroReparado->data.id = stoi(linha);
            cout << carroReparado->data.id << endl;
            getline(fileReparados, carroReparado->data.marca);
            getline(fileReparados, carroReparado->data.modelo);
            cout << carroReparado->data.marca << "-" << carroReparado->data.modelo << endl;
            getline(fileReparados, linha);
            carroReparado->data.tempo_reparacao = stoi(linha);
            getline(fileReparados, linha);
            carroReparado->data.custo_reparacao = stoi(linha);
            getline(fileReparados, linha);
            carroReparado->data.dias_ET = stoi(linha);
            getline(fileReparados, linha);
            if (linha == "Sim") {
                carroReparado->data.prioridade = true;
            }
            else if (linha == "Nao") {
                carroReparado->data.prioridade = false;
            }

            carroReparado = carroReparado->right;
        }

        estacoes = estacoes->next;
    }

    fileReparados.close();
    fileEstacoes.close();

    return ethead;
}

void gravarFicheiros(ListaDeEspera* head, ET* ethead, int numETs) {
    gravarListaDeEspera(head);
    gravarEstacoes(ethead, numETs);
}

void carregarFicheiros(ListaDeEspera* head, ET* ethead) {
    carregarListaDeEspera(head);
    carregarEstacoes(ethead);
}