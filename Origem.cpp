#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include "constantes.h";
#include <locale.h>
#include "structs.h"
#include "ficheiros.h"

using namespace std;

string* marcas = new string[NUM_MARCAS];
string* modelos = new string[NUM_MODELOS];
string* marcasET = new string[20];

BSTNode* insert(BSTNode* root, carro* data) {
    if (root == nullptr) {
        BSTNode* newNode = new BSTNode;
        newNode->data = *data;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }

    // Compare the "marca" values to determine the placement in the BST
    if (data->modelo < root->data.modelo)
        root->left = insert(root->left, data);
    else if (data->modelo > root->data.modelo)
        root->right = insert(root->right, data);
    else {
        // If the "marca" values are equal, decide based on car ID
        if (data->id < root->data.id)
            root->left = insert(root->left, data);
        else if (data->id > root->data.id)
            root->right = insert(root->right, data);
    }

    return root;
}

void printBST(BSTNode* root) {
    if (root == nullptr) {
        return;
    }

    printBST(root->left);
    cout << "Carro ID: " << root->data.id << ", Marca: " << root->data.marca << "-" << root->data.modelo << endl;
    printBST(root->right);
}

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
        newET->marca = marcas[rand() % NUM_MARCAS -1];
        newET->carros = nullptr; 
        newET->repaired_cars = nullptr;
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
    cout << "LISTA DE ESPERA: " << endl;
    while (current != nullptr) {
        carro* currentCar = current->data;

        cout << "Carro: ID: " << currentCar->id << " | ";
        cout << currentCar->marca << "-";
        cout << currentCar->modelo << " | ";
        if (currentCar->prioridade == 1) {
            cout << "Prioritário: " << "Sim" << " | ";
        }
        else {
            cout << "Prioritário: " << "Nao" << " | ";
        }
        cout << "Tempo reparação: " << currentCar->tempo_reparacao << endl;
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
        cout << "Total Faturacão: " << currentET->faturacao << " $" << endl;

        carro* currentCar = currentET->carros;
        if (currentCar == nullptr) {
            cout << "ET sem carros" << endl;
        }
        while (currentCar != nullptr) {
            cout << "Carro: ID: " << currentCar->id << " | ";
            cout << currentCar->marca << "-";
            cout << currentCar->modelo << " | ";
            if (currentCar->prioridade == 1) {
                cout << "Prioritário: " << "Sim" << " | ";
            }
            else {
                cout << "Prioritário: " << "Nao" << " | ";
            }
            cout << "Tempo reparação: " << currentCar->tempo_reparacao << " | ";
            cout << "Dias na ET: " << currentCar->dias_ET << endl;
            

            currentCar = currentCar->next;
        }

        cout << "------------------------------------" << endl;

        currentET = currentET->next;
    }
    cout << endl;
    verListaDeEspera(head);
}

void incrementaDiasET(ET* head) {
    if (numCarrosCriados == 0) {
        return;
    }

    ET* currentET = head;

    while (currentET != nullptr) {
        carro* currentCar = currentET->carros;

        while (currentCar != nullptr) {
            // Increment dias_ET for the current car
            currentCar->dias_ET = currentCar->dias_ET + 1;

            // Move to the next car
            currentCar = currentCar->next;
        }

        // Move to the next ET
        currentET = currentET->next;
    }
}


void reparaCarros(ET* head) {
    ET* currentET = head;

    while (currentET != nullptr) {
        carro* currentCar = currentET->carros;
        carro* previousCar = nullptr;

        while (currentCar != nullptr) {
            carro* nextCar = currentCar->next;

            if (currentCar->tempo_reparacao == currentCar->dias_ET || (rand() % 100) < 15) {
                // Carro precisa ser reparado ou foi aleatoriamente selecionado

                // Remove carro da ET
                if (previousCar == nullptr) {
                    currentET->carros = nextCar;
                }
                else {
                    previousCar->next = nextCar;
                }

                currentET->faturacao += currentCar->custo_reparacao;

                // Adiciona carro ao BST de carros reparados na ET
                currentCar->next = nullptr;
                currentET->repaired_cars = insert(currentET->repaired_cars, currentCar);

                currentET->capacidade_atual--;

                // Display
                if (currentCar->tempo_reparacao == currentCar->dias_ET) {
                    cout << "O carro com ID " << currentCar->id << " foi removido da ET " << currentET->id
                        << " - Ultrapassou tempo máximo de reparação." << endl;
                }
                else {
                    cout << "O carro com ID " << currentCar->id << " foi reparado na ET " << currentET->id << "." << endl;
                }
            }
            else {
                // Próximo carro
                previousCar = currentCar;
            }
            // Atualiza apontador
            currentCar = nextCar;
        }

        // Próxima ET
        currentET = currentET->next;
    }
}



void printRepairedCarsOfET(ET* head) {
    int etId;
    cout << "Introduza o ID da ET: ";
    cin >> etId;

    ET* currentET = head;

    while (currentET != nullptr) {
        if (currentET->id == etId) {
            BSTNode* root = currentET->repaired_cars;
            cout << "Carros reparados daa ET " << etId << ":" << endl;
            printBST(root);
            return;
        }
        currentET = currentET->next;
    }

    cout << "ET com ID " << etId << " não encontrado." << endl;
}

void atualizaTempoReparacao(ListaDeEspera* lista) {
    string marca, modelo;
    int newTempoReparacao;

    cout << "Introduza a marca do carro: ";
    cin >> marca;
    cout << "Introduza o modelo do carro: ";
    cin >> modelo;
    cout << "Introduza o novo tempo de reparação: ";
    cin >> newTempoReparacao;

    ListaDeEspera* current = lista;
    bool carUpdated = false;

    while (current != nullptr) {
        if (current->data->marca == marca && current->data->modelo == modelo) {
            current->data->tempo_reparacao = newTempoReparacao;
            carUpdated = true;
        }
        current = current->next;
    }

    if (carUpdated) {
        cout << "O tempo de reparação foi atualizado para os seguintes carros:" << endl;
        current = lista;
        while (current != nullptr) {
            if (current->data->marca == marca && current->data->modelo == modelo) {
                cout << "ID: " << current->data->id << ", Marca: " << current->data->marca
                    << ", Modelo: " << current->data->modelo << endl;
            }
            current = current->next;
        }
    }
    else {
        cout << "Nenhum carro teve o tempo de reparação atualizado." << endl;
    }
}

void adicionarPrioridade(ListaDeEspera* lista) {
    int id;

    cout << "Introduza o ID do veículo: ";
    cin >> id;

    ListaDeEspera* current = lista;
    bool vehicleFound = false;

    while (current != nullptr) {
        if (current->data->id == id) {
            vehicleFound = true;
            if (current->data->prioridade) {
                cout << "Este veículo já é prioritário." << endl;
            }
            else {
                current->data->prioridade = true;
                cout << "A prioridade do veículo foi alterada com sucesso." << endl;
            }
            break;
        }
        current = current->next;
    }

    if (!vehicleFound) {
        cout << "Veículo não encontrado." << endl;
    }
}

void adicionarET(ET*& listaET) {
    string mecânico, marca;

    cout << "Introduza o nome do mecânico: ";
    cin >> mecânico;
    cout << "Introduza a marca especializada do mecãnico: ";
    cin >> marca;

    // Cria novo nodo da ET
    ET* newET = new ET;
    newET->id = id_ETS++;
    newET->mecanico = mecânico;
    newET->marca = marca;
    newET->carros = nullptr;
    newET->repaired_cars = nullptr;
    newET->carros_reparados = 0;
    newET->capacidade_atual = 0;
    newET->capacidade = rand() % 4 + 2;
    newET->faturacao = 0;
    newET->next = nullptr;

    // Verifica se listaET está vazia
    if (listaET == nullptr) {
        listaET = newET; // Define a nova ET como primeiro NODO
    }
    else {
        ET* current = listaET;

        // Traverse to the last ET node
        while (current->next != nullptr) {
            current = current->next;
        }

        current->next = newET; // Adiciona ET ao fim da lista
    }

    cout << "Nova ET adicionada com sucesso!" << endl;

    marcasET[numETs] = marca;
    numETs++;
    for (int i = 0; i < numETs; i++) {
        cout << marcasET[i] << endl;
    }
}

void removerMecanico(ET*& listaET) {
    string nomeMecanico;
    cout << "Introduza o nome do mecânico a remover: ";
    cin >> nomeMecanico;



    ET* current = listaET;

    // Traverse the ET list to find the mechanic to be removed
    while (current != nullptr && current->mecanico != nomeMecanico) {
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Mecânico não encontrado na lista de ETs." << endl;
        return;
    }

    // Move all cars in the mechanic's carros array to the repaired cars BST
    carro* carros = current->carros;
    while (carros != nullptr) {
        BSTNode* newRepairedCar = new BSTNode;
        newRepairedCar->data = *carros;
        newRepairedCar->left = nullptr;
        newRepairedCar->right = current->repaired_cars;
        current->repaired_cars = newRepairedCar;

        carros = carros->next;
    }

    // Remove the cars from the carros array
    current->carros = nullptr;

    // Replace the mechanic with a new one
    string novaMecanico, novaMarca;
    cout << "Introduza o nome do novo mecânico: ";
    cin >> novaMecanico;

    int contadorMecanicoUnico = 0;
    ET* verificarMarca = listaET;
    while (verificarMarca != nullptr) {
        if (verificarMarca->marca == current->marca) {
            contadorMecanicoUnico++;
        }
        verificarMarca = verificarMarca->next;
    }

    string* novaMarcas = new string[NUM_MARCAS - 1];
    int index = 0;
    if (contadorMecanicoUnico == 1) {
        for (int m = 0; m < NUM_MARCAS; m++) {
            if (marcas[m] != current->marca) {
                novaMarcas[index++] = marcas[m];
            }
        }
        current->capacidade_atual = 0;
        current->mecanico = novaMecanico;
        current->marca = novaMarcas[rand() % NUM_MARCAS - 1];
        marcasET[current->id] = current->marca;

        cout << "Mecânico removido com sucesso." << endl;
        cout << "Novo mecânico adicionado com sucesso." << endl;

    }
}



void SimulaDia(ListaDeEspera* head, ET* ethead);
void PainelDeGestao(ListaDeEspera* head, ET* ethead){
    bool sair = false;
    char escolha = ' ';
    carro* CARRO = head->data;

    do {
        cout << endl;
        cout << "***** Bem Vindo Gestor ***** \n";
        cout << "(1).Atualizar tempo de reparação \n";
        cout << "(2).Adicionar Prioridade \n";
        cout << "(3).Remover Mecânico\n";
        cout << "(4).Gravar Oficina \n";
        cout << "(5).Carregar Oficina \n";
        cout << "(6).Adicionar ET \n";
        cout << "(7).Imprimir carros reparados \n";
        cout << "(9). Voltar Atrás \n";
        cout << "(0).Sair" << endl;
        cout << "Selecione a sua opção:";
        cin >> escolha;
        switch (escolha)
        {
        case '1':
            atualizaTempoReparacao(head);
            break;
        case '2':
            adicionarPrioridade(head);
            break;
        case '3':
            removerMecanico(ethead);
            break;
        case '4':
            gravarFicheiros(head, ethead, numETs);
            cout << "A gravação nos ficheiros foi feita com sucesso." << endl;
            break;
        case '5':
            carregarFicheiros(head);
            break;
        case '6':
            adicionarET(ethead);
            break;
        case '7':
            printRepairedCarsOfET(ethead);
            break;
        case '9':
            SimulaDia(head,ethead);
            break;
        case '0': cout << "Selecionou a opção sair! " << endl;
            exit(0);
            break;
        default: cout << "Escolha Inválida!" << endl;
            break;
        }
    } while (!sair);
    cin.ignore();
}




void SimulaDia(ListaDeEspera* head, ET* ethead) {
    char opcao = ' ';
    bool sair = false;

    do {
        cout << endl;
        cout << "(s): Simular um dia " << endl;
        cout << "(g): Painel de gestão" << endl;
        cout << "(0): SAIR" << endl;
        cin >> opcao;
        switch (opcao) {
        case 's':
        case 'S':
            cout << endl;
            cout << "Dia simulado com sucesso!\n";
            incrementaDiasET(ethead);
            reparaCarros(ethead);
            criaCarrosListaDeEspera(head, 10);
            insertCarsIntoETs(head, ethead);
            master(ethead, head);
            break;
        case 'g':
        case 'G':
            PainelDeGestao(head,ethead);
            break;
        case '0':
            exit(0);
            cout << "Até à proxima!\n";
            break;
        default:
            cout << "Escolha inválida!" << endl;
            break;
        }
    } while (!sair);
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

    SimulaDia(ListaDeEspera, ListaETs);
    
   
   
    return 0;
}

