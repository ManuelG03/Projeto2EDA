#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include "constantes.h";
#include <locale.h>

using namespace std;

string* marcas = new string[NUM_MARCAS];
string* modelos = new string[NUM_MODELOS];
string* marcasET = new string[numETs];

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

struct ET {
    int id;
    int capacidade;
    string mecanico;
    string marca;
    carro* carros;
    carro* regRepCars; 
    int carros_reparados;
    int capacidade_atual;
    int faturacao;
    ET* next; 
};

struct ListaDeEspera {
    carro* data;
    ListaDeEspera* next;
};



ET* inicializaEstacoes() {
    int index = 0;
    string file = "marcas.txt";                                 
    ifstream fileMarcas(file);

    string marca;

    if (fileMarcas.is_open()) {
        int i = 0;
        while (!fileMarcas.eof()) {
            getline(fileMarcas, marcas[i++]);               
        }
    }
    
    srand(time(0)); 
    
    ET* head = nullptr; 
    ET* current = nullptr; 

    for (int i = 0; i < numETs; i++) {
        ET* newET = new ET();

        
        newET->id = id_ETS++;
        newET->capacidade = rand() % 4 + 2; 
        newET->marca = marcas[rand() % NUM_MARCAS];
        newET->carros = nullptr; 
        newET->regRepCars = nullptr;
        newET->carros_reparados = 0;
        newET->capacidade_atual = 0;
        newET->faturacao = 0;
        newET->next = nullptr; 

        marcasET[index++] = newET->marca;
        
        cout << "Introduza o mecanico para a ET " << newET->id << ": ";
        getline(cin, newET->mecanico); 


        if (head == nullptr) {
            head = newET;
            current = newET;
        }
        else {
            current->next = newET;
            current = newET;
        }
    }
    

    return head;
}



void imprimeListaETs(ET* head) {
    ET* current = head;

    while (current != nullptr) {
        cout << "ET " << current->id << ":" << endl;
        cout << "Mecanico: " << current->mecanico << endl;
        cout << "Capacidade: " << current->capacidade << endl;
        cout << "Marca: " << current->marca << endl;
        cout << "-----------------------------" << endl;

        current = current->next;
    }
}

void organizaListaDeEspera(ListaDeEspera*& head) {
    if (head == nullptr || head->next == nullptr) {
        return;
    }

    ListaDeEspera* prioridadeHead = nullptr;
    ListaDeEspera* prioridadeTail = nullptr;
    ListaDeEspera* current = head;
    ListaDeEspera* previous = nullptr;

    while (current != nullptr) {
        if (current->data->prioridade == 1) {
            if (previous == nullptr) {
                head = current->next;
            }
            else {
                previous->next = current->next;
            }

            if (prioridadeHead == nullptr) {
                prioridadeHead = current;
                prioridadeTail = current;
            }
            else {
                prioridadeTail->next = current;
                prioridadeTail = current;
            }

            current = current->next;
            prioridadeTail->next = nullptr;
        }
        else {
            previous = current;
            current = current->next;
        }
    }

    if (prioridadeHead != nullptr) {
        prioridadeTail->next = head;
        head = prioridadeHead;
    }
}



void adicionaListaDeEspera(ListaDeEspera*& head, carro* newCarro) {
    ListaDeEspera* newNode = new ListaDeEspera();
    newNode->data = newCarro;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = newNode;
    }
    else {
        ListaDeEspera* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}
 

void insertCarsIntoETs(ListaDeEspera*& esperaHead, ET* etHead) {
    int totalCarsAdded = 0; 

    ListaDeEspera* esperaCurrent = esperaHead;
    ListaDeEspera* esperaPrevious = nullptr;

    while (esperaCurrent != nullptr && totalCarsAdded < 8) {
        carro* currentCar = esperaCurrent->data;

        ET* currentET = etHead;
        ET* selectedET = nullptr; 

       
        while (currentET != nullptr) {
            if (currentET->capacidade_atual < currentET->capacidade && currentET->marca == currentCar->marca) {
                selectedET = currentET;
                break;
            }
            currentET = currentET->next;
        }

        if (selectedET != nullptr) {
           
            carro* newCar = new carro();
            *newCar = *currentCar;

            
            if (selectedET->carros == nullptr) {
                selectedET->carros = newCar;
            }
            else {
                carro* temp = selectedET->carros;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newCar;
            }

         
            selectedET->capacidade_atual++;

        
            if (esperaPrevious == nullptr) {
                esperaHead = esperaCurrent->next;
            }
            else {
                esperaPrevious->next = esperaCurrent->next;
            }
            ListaDeEspera* temp = esperaCurrent;
            esperaCurrent = esperaCurrent->next;
            delete temp;

            totalCarsAdded++;
        }
        else {
            
            esperaPrevious = esperaCurrent;
            esperaCurrent = esperaCurrent->next;
        }
    }
}




void criaCarrosListaDeEspera(ListaDeEspera*& head, int numCarsToAdd) {
    for (int i = 0; i < numCarsToAdd; i++) {
        carro* newCarro = new carro(); 

        
        newCarro->id = id_Carros++;
        newCarro->marca = marcasET[rand() % numETs];
        newCarro->modelo = modelos[rand() % NUM_MODELOS];
        int probabilidade = rand() % 100;
        if (probabilidade < 25) {
            newCarro->prioridade = 1;
        }
        else {
            newCarro->prioridade = 0;
        }
        newCarro->tempo_reparacao = 4;
        newCarro->dias_ET = 0;
        newCarro->custo_reparacao = rand() % 120;
 

        adicionaListaDeEspera(head, newCarro);
    }
    numCarrosCriados += 10;
    organizaListaDeEspera(head);
}

void verListaDeEspera(const ListaDeEspera* head) {
    const ListaDeEspera* current = head;
    while (current != nullptr) {
        carro* currentCar = current->data;

        cout << "Carro: ID: " << currentCar->id << " | ";
        cout << currentCar->marca << "-";
        cout << currentCar->modelo << " | ";
        if (currentCar->prioridade == 1) {
            cout << "Prioritario: " << "Sim" << endl;
        }
        else {
            cout << "Prioritario: " << "Nao" << endl;
        }
        current = current->next;
    }
}

void master(ET* etHead, ListaDeEspera* head) {
    ET* currentET = etHead;
    cout << "------------------------------------" << endl;
    while (currentET != nullptr) {
        cout << "ET: " << currentET->id << " | ";
        cout << "Mecanico: " << currentET->mecanico << " | ";
        cout << "Capacidade: " << currentET->capacidade << " | ";
        cout << "Carros: " << currentET->capacidade_atual << " | ";
        cout << "Marca: " << currentET->marca << " | ";
        cout << "Total Faturacao: " << currentET->faturacao << " $" << endl;

        carro* currentCar = currentET->carros;
        while (currentCar != nullptr) {
            cout << "Carro: ID: " << currentCar->id << " | ";
            cout << currentCar->marca << "-";
            cout << currentCar->modelo << " | ";
            if (currentCar->prioridade == 1) {
                cout << "Prioritario: " << "Sim" << " | ";
            }
            else {
                cout << "Prioritario: " << "Nao" << " | ";
            }
            cout << "Tempo reparação: " << currentCar->tempo_reparacao << " | ";
            cout << "Dias na ET: " << currentCar->dias_ET << endl;
            

            currentCar = currentCar->next;
        }

        cout << "------------------------------------" << endl;

        currentET = currentET->next;
    }
    verListaDeEspera(head);
}



int main() {
    {
        setlocale(LC_ALL, "Portuguese");
    }

    string file = "modelos.txt";
    ifstream fileMarcas(file);

    if (fileMarcas.is_open()) {
        int i = 0;
        while (!fileMarcas.eof()) {
            getline(fileMarcas, modelos[i++]);
        }
    }

    ListaDeEspera* ListaDeEspera = nullptr;
    ET* ListaETs = inicializaEstacoes();
    
    criaCarrosListaDeEspera(ListaDeEspera, 10);
    criaCarrosListaDeEspera(ListaDeEspera, 10);
    criaCarrosListaDeEspera(ListaDeEspera, 10);
    criaCarrosListaDeEspera(ListaDeEspera, 10);
    criaCarrosListaDeEspera(ListaDeEspera, 10);

    insertCarsIntoETs(ListaDeEspera, ListaETs);
    

    
    master(ListaETs, ListaDeEspera);
   
    return 0;
}

//MANUEL
//Carros com prioridade devem ficar no inicio da FILA.