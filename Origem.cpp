#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

int id_ETS = 1;
int id_Carros = 1;
int numCarrosCriados = 0;

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
    srand(time(0)); 

    int numETs = rand() % 6 + 3; 
    ET* head = nullptr; 
    ET* current = nullptr; 

    for (int i = 0; i < numETs; i++) {
        ET* newET = new ET();

        
        newET->id = id_ETS++;
        newET->capacidade = rand() % 4 + 2; 
        newET->marca = "BMW";
        newET->carros = nullptr; 
        newET->regRepCars = nullptr;
        newET->carros_reparados = 0;
        newET->capacidade_atual = 0;
        newET->faturacao = 0;
        newET->next = nullptr; 

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

#include <cstring> 

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
        newCarro->marca = "BMW";
        newCarro->modelo = "Modelo";
        newCarro->prioridade = "Sim";
        newCarro->tempo_reparacao = 4;
        newCarro->dias_ET = 0;
        newCarro->custo_reparacao = 100;
 

        adicionaListaDeEspera(head, newCarro);
    }
    numCarrosCriados += 10;
}

void printListaDeEspera(const ListaDeEspera* head) {
    const ListaDeEspera* current = head;
    while (current != nullptr) {
        carro* currentCar = current->data;

        cout << "Carro ID: " << currentCar->id;
        cout << " Marca: " << currentCar->marca;
        cout << " Modelo: " << currentCar->modelo;
        // Print other car fields as needed

        cout << endl;

        current = current->next;
    }
}

void printETsAndCars(ET* etHead) {
    ET* currentET = etHead;

    while (currentET != nullptr) {
        cout << "ET " << currentET->id << ":";
        cout << "Mecanico: " << currentET->mecanico;
        cout << "Capacidade: " << currentET->capacidade;
        cout << "Marca: " << currentET->marca;
        cout << "Carros inside:" << endl;

        carro* currentCar = currentET->carros;
        while (currentCar != nullptr) {
            cout << "ID: " << currentCar->id;
            cout << "Marca: " << currentCar->marca;
            cout << "Modelo: " << currentCar->modelo;
            cout << "Tempo de reparacao: " << currentCar->tempo_reparacao;
            cout << "Custo de reparacao: " << currentCar->custo_reparacao;
            cout << "Dias ET: " << currentCar->dias_ET << endl;
            cout << "Prioridade: " << (currentCar->prioridade ? "True" : "False") << endl;
            cout << "-----------------------------" << endl;

            currentCar = currentCar->next;
        }

        cout << "=============================" << endl;

        currentET = currentET->next;
    }
}



int main() {
    ListaDeEspera* ListaDeEspera = nullptr;
    ET* ListaETs = inicializaEstacoes();

    criaCarrosListaDeEspera(ListaDeEspera, 10);
    insertCarsIntoETs(ListaDeEspera, ListaETs);
    insertCarsIntoETs(ListaDeEspera, ListaETs);

    //printListaDeEspera(ListaDeEspera);
    printETsAndCars(ListaETs);
    cout << numCarrosCriados;
    return 0;
}

//MANUEL
//Carros com prioridade devem ficar no inicio da FILA.