#include <iostream>
#include <fstream>
#include <string>
#include "structs.h"
#include "Origem.h"

using namespace std;

int carregarIdCarros(caminhosFicheiros* caminho) {
    fstream file;
    file.open(caminho->caminhoListaDeEspera); //Abrir o ficheiro especificado pelo caminho
    string linha;
    int id_Carros; 

    getline(file, linha); //Lê a primeira linha do ficheiro
    id_Carros = stoi(linha);

    file.close(); //Fechar o ficheiro

    return id_Carros; //Retornar o valor do id de carros
}

void gravarListaDeEspera(ListaDeEspera* head, caminhosFicheiros* caminho, int id_Carros) {
    ofstream file;
    file.open(caminho->caminhoListaDeEspera); //Abrir o ficheiro especificado pelo caminho
    
    ListaDeEspera* contarCarro = head; //Apontador para percorrer a lista de espera
    int numListaDeEspera = 0;

    //Conta o número de elementos na lista de espera
    while (contarCarro != nullptr) {
        numListaDeEspera++; //Incrementar número de carros na lista de espera

        contarCarro = contarCarro->next; //Avança para o próximo carro em espera na lista
    }

    file << id_Carros << endl; //Gravar o último id dos carros

    file << numListaDeEspera << endl; //Gravar o número de carros na lista de espera

    ListaDeEspera* carroEmEspera = head; //Apontador para percorrer a lista de espera novamente

    //Grava as informações de cada carro da lista de espera no ficheiro
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

        carroEmEspera = carroEmEspera->next; //Avança para o próximo carro em espera na lista
    }

    file.close(); //Fechar o ficheiro
}

ListaDeEspera* carregarListaDeEspera(ListaDeEspera* head, caminhosFicheiros* caminho) {
    fstream file;
    file.open(caminho->caminhoListaDeEspera); //Abrir o ficheiro especificado pelo caminho

    string linha;
    int numListaDeEspera = 0;

    delete head; //Liberta a memória ocupada pela lista de espera atual
    head = NULL; //Define a cabeça da lista como nula

    getline(file, linha);

    getline(file, linha);
    numListaDeEspera = stoi(linha);

    for (int i = 0; i < numListaDeEspera; i++) {
        ListaDeEspera* listadeespera = new ListaDeEspera; //Cria um novo nó da lista de espera
        umCarro carroEmEspera; //Variável para armazenar as informações do carro

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

        listadeespera->data = carroEmEspera; //Atribui as informações do carro ao nó da lista de espera

        if (i == 0) {
            head = listadeespera; //Se for o primeiro elemento, define-o como cabeça da lista
        }
        else {
            ListaDeEspera* iterador = head;

            while (iterador->next != NULL) {
                iterador = iterador->next; //Avança para o próximo nó da lista
            }

            iterador->next = listadeespera; //Adiciona o nó atual no final da lista
        }

        listadeespera = listadeespera->next; //Avança para o próximo nó
    }

    return head; //Retorna a cabeça da lista atualizada
} 

void gravarCarrosReparados(BSTNode* carrosReparados, ofstream& fileReparados) {
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

int carregarIdEstacoes(caminhosFicheiros* caminho) {
    fstream file;
    file.open(caminho->caminhoEstacoes);

    string linha;
    int id_ETS;

    getline(file, linha);
    id_ETS = stoi(linha);

    file.close();

    return id_ETS + 1;
}

void gravarEstacoes(ET* ethead, int numETs, caminhosFicheiros* caminho) {
    ofstream fileReparados;
    fileReparados.open(caminho->caminhoCarrosReparados);

    ofstream fileEstacoes;
    fileEstacoes.open(caminho->caminhoEstacoes);

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
            fileEstacoes << carroNaET->data.id << endl;
            fileEstacoes << carroNaET->data.marca << endl;
            fileEstacoes << carroNaET->data.modelo << endl;
            fileEstacoes << carroNaET->data.tempo_reparacao << endl;
            fileEstacoes << carroNaET->data.custo_reparacao << endl;
            fileEstacoes << carroNaET->data.dias_ET << endl;
            if (carroNaET->data.prioridade) {
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

ET* carregarEstacoes(ET* ethead, caminhosFicheiros* caminho) {
    fstream fileEstacoes;
    fileEstacoes.open(caminho->caminhoEstacoes);

    fstream fileReparados;
    fileReparados.open(caminho->caminhoCarrosReparados);

    string linha;
    int numETs = 0;

    delete ethead;
    ethead = NULL;

    getline(fileEstacoes, linha);
    numETs = stoi(linha);

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
            carroNaET->data.id = stoi(linha);
            getline(fileEstacoes, carroNaET->data.marca);
            getline(fileEstacoes, carroNaET->data.modelo);
            getline(fileEstacoes, linha);
            carroNaET->data.tempo_reparacao = stoi(linha);
            getline(fileEstacoes, linha);
            carroNaET->data.custo_reparacao = stoi(linha);
            getline(fileEstacoes, linha);
            carroNaET->data.dias_ET = stoi(linha);
            getline(fileEstacoes, linha);
            if (linha == "Sim") {
                carroNaET->data.prioridade = true;
            }
            else if (linha == "Nao") {
                carroNaET->data.prioridade = false;
            }

            if (j == 0) {
                estacoes->carros = carroNaET;
            }
            else {
                carro* iterador = estacoes->carros;

                while (iterador->next != NULL) {
                    iterador = iterador->next;
                }

                iterador->next = carroNaET;
            }

            carroNaET = carroNaET->next;
        }

        for (int k = 0; k < estacoes->carros_reparados; k++) {
            BSTNode* carroReparado = new BSTNode;

            getline(fileReparados, linha);
            carroReparado->data.id = stoi(linha);
            getline(fileReparados, carroReparado->data.marca);
            getline(fileReparados, carroReparado->data.modelo);
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
            estacoes->repaired_cars = insert(estacoes->repaired_cars, carroReparado->data);
        }

        if (i == 0) {
            ethead = estacoes;
        }
        else {
            ET* iterador = ethead;

            while (iterador->next != NULL) {
                iterador = iterador->next;
            }

            iterador->next = estacoes;
        }

        estacoes = estacoes->next;
    }

    fileReparados.close();
    fileEstacoes.close();

    return ethead;
}

string* carregarMarcasET(caminhosFicheiros* caminho) {
    fstream fileEstacoes;
    fileEstacoes.open(caminho->caminhoEstacoes);

    int numETs = 0;
    int capacidade_atual = 0;
    string linha;

    getline(fileEstacoes, linha);
    numETs = stoi(linha);

    string* marcasET = new string[20];

    for (int i = 0; i < numETs; i++) {
        getline(fileEstacoes, linha);
        getline(fileEstacoes, linha);
        getline(fileEstacoes, linha);
        getline(fileEstacoes, marcasET[i]);
        getline(fileEstacoes, linha);
        getline(fileEstacoes, linha);
        capacidade_atual = stoi(linha);
        getline(fileEstacoes, linha);

        for (int j = 0; j < capacidade_atual; j++) {
            getline(fileEstacoes, linha);
            getline(fileEstacoes, linha);
            getline(fileEstacoes, linha);
            getline(fileEstacoes, linha);
            getline(fileEstacoes, linha);
            getline(fileEstacoes, linha);
            getline(fileEstacoes, linha);
        }
    }

    fileEstacoes.close();

    return marcasET;
}

int carregarNumETs(caminhosFicheiros* caminho) {
    fstream fileEstacoes;
    fileEstacoes.open(caminho->caminhoEstacoes);

    string linha;

    getline(fileEstacoes, linha);
    int numETs = stoi(linha);

    fileEstacoes.close();

    return numETs;
}

void gravarFicheiros(ListaDeEspera* head, ET* ethead, int numETs, caminhosFicheiros* caminho, int id_Carros) {
    gravarListaDeEspera(head, caminho, id_Carros);
    gravarEstacoes(ethead, numETs, caminho);
}