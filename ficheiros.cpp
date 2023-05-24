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

    getline(file, linha); //L� a primeira linha do ficheiro
    id_Carros = stoi(linha);

    file.close(); //Fechar o ficheiro

    return id_Carros; //Retornar o valor do id de carros
}

void gravarListaDeEspera(ListaDeEspera* head, caminhosFicheiros* caminho, int id_Carros) {
    ofstream file;
    file.open(caminho->caminhoListaDeEspera); //Abrir o ficheiro especificado pelo caminho
    
    ListaDeEspera* contarCarro = head; //Apontador para percorrer a lista de espera
    int numListaDeEspera = 0;

    //Conta o n�mero de elementos na lista de espera
    while (contarCarro != nullptr) {
        numListaDeEspera++; //Incrementar n�mero de carros na lista de espera

        contarCarro = contarCarro->next; //Avan�a para o pr�ximo carro em espera na lista
    }

    file << id_Carros << endl; //Gravar o �ltimo id dos carros

    file << numListaDeEspera << endl; //Gravar o n�mero de carros na lista de espera

    ListaDeEspera* carroEmEspera = head; //Apontador para percorrer a lista de espera novamente

    //Grava as informa��es de cada carro da lista de espera no ficheiro
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

        carroEmEspera = carroEmEspera->next; //Avan�a para o pr�ximo carro em espera na lista
    }

    file.close(); //Fechar o ficheiro
}

ListaDeEspera* carregarListaDeEspera(ListaDeEspera* head, caminhosFicheiros* caminho) {
    fstream file;
    file.open(caminho->caminhoListaDeEspera); //Abrir o ficheiro especificado pelo caminho

    string linha;
    int numListaDeEspera = 0;

    delete head; //Liberta a mem�ria ocupada pela lista de espera atual
    head = NULL; //Define a cabe�a da lista como nula

    getline(file, linha);

    getline(file, linha);
    numListaDeEspera = stoi(linha);

    for (int i = 0; i < numListaDeEspera; i++) {
        ListaDeEspera* listadeespera = new ListaDeEspera; //Cria um novo n� da lista de espera
        umCarro carroEmEspera; //Vari�vel para armazenar as informa��es do carro

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

        listadeespera->data = carroEmEspera; //Atribui as informa��es do carro ao n� da lista de espera

        if (i == 0) {
            head = listadeespera; //Se for o primeiro elemento, define-o como cabe�a da lista
        }
        else {
            ListaDeEspera* iterador = head;

            while (iterador->next != NULL) {
                iterador = iterador->next; //Avan�a para o pr�ximo n� da lista
            }

            iterador->next = listadeespera; //Adiciona o n� atual no final da lista
        }

        listadeespera = listadeespera->next; //Avan�a para o pr�ximo n�
    }

    return head; //Retorna a cabe�a da lista atualizada
} 

void gravarCarrosReparados(BSTNode* carrosReparados, ofstream& fileReparados) { //Recebe como par�metro o ficheiro
    BSTNode* carroReparado = carrosReparados;

    if (carrosReparados == NULL) { //Verifica se a �rvore est� vazia
        return; 
    }

    gravarCarrosReparados(carroReparado->left, fileReparados); //Chama recursivamente a fun��o para o n� esquerdo

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

    gravarCarrosReparados(carroReparado->right, fileReparados); //Chama recursivamente a fun��o para o n� direito

}

int carregarIdEstacoes(caminhosFicheiros* caminho) {
    fstream file;
    file.open(caminho->caminhoEstacoes); //Abrir o ficheiro especificado pelo caminho

    string linha;
    int id_ETS;

    getline(file, linha);
    id_ETS = stoi(linha); //Converte a string para int

    file.close(); //Fechar o ficheiro

    return id_ETS + 1; //Retorna o id das esta��es + 1 para adicionar esta��o posteriormente
}

void gravarEstacoes(ET* ethead, int numETs, caminhosFicheiros* caminho) {
    ofstream fileReparados;
    fileReparados.open(caminho->caminhoCarrosReparados); //Abrir o ficheiro especificado pelo caminho

    ofstream fileEstacoes;
    fileEstacoes.open(caminho->caminhoEstacoes); //Abrir o ficheiro especificado pelo caminho

    fileEstacoes << numETs << endl; //Guarda o n�mero de ETs

    ET* estacao = ethead; //Apontador para percorrer a lista de esta��es

    while (estacao != nullptr) { //Percorre a lista de esta��es
        fileEstacoes << estacao->id << endl;
        fileEstacoes << estacao->capacidade << endl;
        fileEstacoes << estacao->mecanico << endl;
        fileEstacoes << estacao->marca << endl;
        fileEstacoes << estacao->carros_reparados << endl;
        fileEstacoes << estacao->capacidade_atual << endl;
        fileEstacoes << estacao->faturacao << endl;

        carro* carroNaET = estacao->carros; //Apontador para percorrer a lista de carros da esta��o
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

            carroNaET = carroNaET->next; //Avan�a para o pr�ximo carro na esta��o
        }

        gravarCarrosReparados(estacao->repaired_cars, fileReparados); //Chama a fun��o gravarCarrosReparados para gravar os carros reparados

        estacao = estacao->next; //Avan�a para a pr�xima esta��o
    }

    fileEstacoes.close(); //Fecha o ficheiro
    fileReparados.close(); //Fecha o ficheiro
}

ET* carregarEstacoes(ET* ethead, caminhosFicheiros* caminho) {
    fstream fileEstacoes;
    fileEstacoes.open(caminho->caminhoEstacoes); //Abrir o ficheiro especificado pelo caminho

    fstream fileReparados;
    fileReparados.open(caminho->caminhoCarrosReparados); //Abrir o ficheiro especificado pelo caminho

    string linha;
    int numETs = 0;

    delete ethead; //Liberta a mem�ria ocupada pela lista de esta��es
    ethead = NULL; //Define como NULL

    getline(fileEstacoes, linha);
    numETs = stoi(linha);

    for (int i = 0; i < numETs; i++) {

        ET* estacoes = new ET; //Cria um novo n� para armazenar uma esta��o

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
            carro* carroNaET = new carro; //Cria um novo n� para armazenar um carro da ET

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

            if (j == 0) { //Se for o primeiro carro da esta��o, atribui ao array carros da ET
                estacoes->carros = carroNaET;
            }
            else { //Caso contr�rio, percorre a lista de carros da esta��o e adiciona o carro no final
                carro* iterador = estacoes->carros; 

                while (iterador->next != NULL) {
                    iterador = iterador->next;
                }

                iterador->next = carroNaET;
            }

            carroNaET = carroNaET->next; //Avan�a para o pr�ximo carro
        }

        for (int k = 0; k < estacoes->carros_reparados; k++) {
            BSTNode* carroReparado = new BSTNode; //Cria um novo n� para armazenar um carro reparado

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
            estacoes->repaired_cars = insert(estacoes->repaired_cars, carroReparado->data); //Insere o carro reparado na �rvore
        }

        if (i == 0) { //Se for a primeira esta��o, atribui ao apontador ethead
            ethead = estacoes;
        }
        else {
            ET* iterador = ethead;

            while (iterador->next != NULL) {
                iterador = iterador->next;
            }

            iterador->next = estacoes;
        }

        estacoes = estacoes->next; //Avan�a para a pr�xima esta��o
    }

    fileReparados.close(); //Fecha o ficheiro
    fileEstacoes.close(); //Fecha o ficheiro

    return ethead; //Retorna o apontador ethead
}

string* carregarMarcasET(caminhosFicheiros* caminho) {
    fstream fileEstacoes;
    fileEstacoes.open(caminho->caminhoEstacoes); //Abrir o ficheiro especificado pelo caminho

    int numETs = 0;
    int capacidade_atual = 0;
    string linha;

    getline(fileEstacoes, linha);
    numETs = stoi(linha);

    string* marcasET = new string[20]; //Cria um array de strings com tamanho 20 para armazenar as marcas dos ETs

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

    fileEstacoes.close(); //Fechar o ficheiro

    return marcasET; //Retorna o array de marcas das ETs
}

int carregarNumETs(caminhosFicheiros* caminho) {
    fstream fileEstacoes;
    fileEstacoes.open(caminho->caminhoEstacoes); //Abrir o ficheiro especificado pelo caminho

    string linha;

    getline(fileEstacoes, linha);
    int numETs = stoi(linha); //Converte a linha num n�mero inteiro e atribui a numETs

    fileEstacoes.close(); //Fechar o ficheiro

    return numETs; //Retornar o array de marcas dos ETs
}

void gravarFicheiros(ListaDeEspera* head, ET* ethead, int numETs, caminhosFicheiros* caminho, int id_Carros) {
    gravarListaDeEspera(head, caminho, id_Carros); //Chama a fun��o gravarListaDeEspera
    gravarEstacoes(ethead, numETs, caminho); //Chama a fun��o gravarEstacoes
}