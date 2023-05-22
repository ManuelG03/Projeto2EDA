#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <locale.h>
#include "structs.h"
#include "constantes.h"
#include "ficheiros.h"

using namespace std;

string* marcas = new string[NUM_MARCAS]; //Array para guardar marcas dos carros
string* modelos = new string[NUM_MODELOS]; //Array para guardar modelos dos carros
string* marcasET = new string[20]; //Array para guardar marcas que podem ser geradas nas ET�s

BSTNode* insert(BSTNode* root, umCarro data) {
    if (root == nullptr) { //Verifica se a �rvore est� vazia
        BSTNode* newNode = new BSTNode; //Cria��o de um novo nodo
        newNode->data = data;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }

    //Comparar modelos para determinar posi��o na �rvore
    if (data.modelo < root->data.modelo)
        root->left = insert(root->left, data);
    else if (data.modelo > root->data.modelo)
        root->right = insert(root->right, data);
    else {
        //Comparar ID'S caso a marca seja a mesma
        if (data.id < root->data.id)
            root->left = insert(root->left, data);
        else if (data.id > root->data.id)
            root->right = insert(root->right, data);
    }

    return root;
}

void printBST(BSTNode* root) {
    if (root == nullptr) { //Verifica se a �rvore est� vazia
        return;
    }

    printBST(root->left); // Percorre recursivamente a sub�rvore esquerda
    cout << "Carro ID: " << root->data.id << ", Marca: " << root->data.marca << "-" << root->data.modelo << endl; // Imprime os dados do nodo atual
    printBST(root->right); // Percorre recursivamente a sub�rvore direita
}

ET* inicializaEstacoes() { //SARA
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
        ET* newET = new ET;

        newET->id = id_ETS++;
        newET->capacidade = rand() % 4 + 2; 
        newET->marca = marcas[rand() % NUM_MARCAS - 1];
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


void organizaListaDeEspera(ListaDeEspera*& head) {
    if (head == nullptr || head->next == nullptr) { //Verifica se a lista est� vazia ou se tem um s� elemento
        return;
    }
     
    //Inicia apontadores
    ListaDeEspera* prioridadeA = nullptr; //Cabe�a dos nodos c/prioridade
    ListaDeEspera* prioridadeB = nullptr; //Cauda dos nodos c/prioridade
    ListaDeEspera* current = head; //Nodo atual a ser inspecionado
    ListaDeEspera* previous = nullptr; //Nodo seguinte

    while (current != nullptr) { //Percorre a lista de espera
        if (current->data.prioridade == 1) { //Verifica se o nodo atual � priorit�rio
            if (previous == nullptr) { //Remove o nodo da lista
                head = current->next; //Atualiza head para o proximo nodo
            }
            else {
                previous->next = current->next; //Atualiza apontador do nodo anterior
            }
            //Adiciona nodo atual � lista nova
            if (prioridadeA == nullptr) {
                prioridadeA = current;
                prioridadeB = current;
            }
            else {
                prioridadeB->next = current;
                prioridadeB = current;
            }

            current = current->next; //Passa para o proximo nodo
            prioridadeB->next = nullptr;
        }
        else {
            previous = current; //Atualiza nodo anterior
            current = current->next; //Passa ao proximo nodo
        }
    }
    //Liga a cauda da nova lista � cabe�a da original
    if (prioridadeA != nullptr) {
        prioridadeB->next = head;
        head = prioridadeA; //Atualiza cabe�a da lista
    }
}

void adicionaListaDeEspera(ListaDeEspera*& head, carro* newCarro) {
    ListaDeEspera* newNode = new ListaDeEspera();
    newNode->data = newCarro->data;
    newNode->next = nullptr;

    if (head == nullptr) { //Verifica se alista de espera est� vazia
        head = newNode; //Se estiver, o novo carro torn-se o primeiro da lista
    }
    else { //Caso contr�rio encontra o ultimo nodo da lista
        ListaDeEspera* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode; //Define o novo carro como o proximo nodo(do ultimo)
    }
}
 

void insertCarsIntoETs(ListaDeEspera*& esperaHead, ET* etHead) {
    int totalCarsAdded = 0; 

    ListaDeEspera* esperaCurrent = esperaHead;
    ListaDeEspera* esperaPrevious = nullptr;

    while (esperaCurrent != nullptr && totalCarsAdded < 8) {
        umCarro currentCar = esperaCurrent->data;

        ET* currentET = etHead;
        ET* selectedET = nullptr; 

       
        while (currentET != nullptr) {
            if (currentET->capacidade_atual < currentET->capacidade && currentET->marca == currentCar.marca) {
                selectedET = currentET;
                break;
            }
            currentET = currentET->next;
        }

        if (selectedET != nullptr) {
           
            carro* newCar = new carro();
            newCar->data = currentCar;

            
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

void criaCarrosListaDeEspera(ListaDeEspera*& head, int numCarsToAdd) { //Fun��o para criar 10 carros de cada vez que � chamada
    for (int i = 0; i < numCarsToAdd; i++) {
        carro* newCarro = new carro();
        
        newCarro->data.id = id_Carros++;
        newCarro->data.marca = marcasET[rand() % numETs];
        newCarro->data.modelo = modelos[rand() % NUM_MODELOS];
        int probabilidade = rand() % 100;
        if (probabilidade < 5) {
            newCarro->data.prioridade = 1;
        }
        else {
            newCarro->data.prioridade = 0;
        }
        newCarro->data.tempo_reparacao = 2 + (rand() % 4);
        newCarro->data.dias_ET = 0;
        newCarro->data.custo_reparacao = rand() % 120;
 

        adicionaListaDeEspera(head, newCarro); //Cada carro que � criado � passado como parametro em adicionaListaDeEspera e � inserido na Lista de Espera
    }
    numCarrosCriados += 10; //Incrementa numCarrosCriados em 10 unidades
    organizaListaDeEspera(head); //Organiza ListaDeEspera de acordo com a prioridade dos ve�culos de cada vez que s�o adicionados carros
}

void verListaDeEspera(const ListaDeEspera* head) { //Fun��o para dar output de todos os carros presentes na lista de espera
    const ListaDeEspera* current = head;
    cout << "LISTA DE ESPERA: " << endl;
    while (current != nullptr) {
        umCarro currentCar = current->data;

        cout << "Carro: ID: " << currentCar.id << " | ";
        cout << currentCar.marca << "-";
        cout << currentCar.modelo << " | ";
        if (currentCar.prioridade) {
            cout << "Priorit�rio: " << "Sim" << " | ";
        }
        else {
            cout << "Priorit�rio: " << "Nao" << " | ";
        }
        cout << "Tempo repara��o: " << currentCar.tempo_reparacao << endl;
        current = current->next;
    }
}

void master(ET* etHead, ListaDeEspera* head) { //Fun��o para mostrar todas as ET'S + carros Lista de Espera
    ET* currentET = etHead;
    cout << "------------------------------------" << endl;
    while (currentET != nullptr) {
        cout << "ET: " << currentET->id << " | ";
        cout << "Mecanico: " << currentET->mecanico << " | ";
        cout << "Capacidade: " << currentET->capacidade << " | ";
        cout << "Carros: " << currentET->capacidade_atual << " | ";
        cout << "Marca: " << currentET->marca << " | ";
        cout << "Total Faturac�o: " << currentET->faturacao << " $" << endl;

        carro* currentCar = currentET->carros;
        if (currentCar == nullptr) {
            cout << "ET sem carros" << endl;
        }
        while (currentCar != nullptr) {
            cout << "Carro: ID: " << currentCar->data.id << " | ";
            cout << currentCar->data.marca << "-";
            cout << currentCar->data.modelo << " | ";
            if (currentCar->data.prioridade == 1) {
                cout << "Priorit�rio: " << "Sim" << " | ";
            }
            else {
                cout << "Priorit�rio: " << "Nao" << " | ";
            }
            cout << "Tempo repara��o: " << currentCar->data.tempo_reparacao << " | ";
            cout << "Dias na ET: " << currentCar->data.dias_ET << endl;
            

            currentCar = currentCar->next;
        }

        cout << "------------------------------------" << endl;

        currentET = currentET->next; //Passa por todas as Ets..
    }
    cout << endl;
    verListaDeEspera(head); //Mostra os carros da ListaDeEspera no final
}

void incrementaDiasET(ET* head) { //Fun��o para incrementar dias na ET em um unidade (dos carros que est�o em ETS)
    if (numCarrosCriados == 0) {
        return; //Se n�o tiverem sido criado carros, nada acontece
    }

    ET* currentET = head;

    while (currentET != nullptr) {
        carro* currentCar = currentET->carros;

        while (currentCar != nullptr) {
            //Increment dias_ET do carro atual
            currentCar->data.dias_ET = currentCar->data.dias_ET + 1;

            //Passa para o pr�ximo carro
            currentCar = currentCar->next;
        }

        //Passa para a pr�xima ET
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

            if (currentCar->data.tempo_reparacao == currentCar->data.dias_ET || (rand() % 100) < 15) {
                // Carro precisa ser reparado ou foi aleatoriamente selecionado

                // Remove carro da ET
                if (previousCar == nullptr) {
                    currentET->carros = nextCar;
                }
                else {
                    previousCar->next = nextCar;
                }

                currentET->faturacao += currentCar->data.custo_reparacao;

                // Adiciona carro ao BST de carros reparados na ET
                currentCar->next = nullptr;
                currentET->repaired_cars = insert(currentET->repaired_cars, currentCar->data);
                currentET->carros_reparados++;
                if (currentET->capacidade_atual != 0) {
                    currentET->capacidade_atual--;
                }

                // Display
                if (currentCar->data.tempo_reparacao == currentCar->data.dias_ET) {
                    cout << "O carro com ID " << currentCar->data.id << " foi removido da ET " << currentET->id
                        << " - Ultrapassou tempo m�ximo de repara��o." << endl;
                }
                else {
                    cout << "O carro com ID " << currentCar->data.id << " foi reparado na ET " << currentET->id << "." << endl;
                }
            }
            else {
                // Pr�ximo carro
                previousCar = currentCar;
            }
            // Atualiza apontador
            currentCar = nextCar;
        }

        // Pr�xima ET
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
            cout << "Carros reparados da ET " << etId << ":" << endl;
            printBST(root);
            return;
        }
        currentET = currentET->next;
    }

    cout << "ET com ID " << etId << " n�o encontrado." << endl;
}

void atualizaTempoReparacao(ListaDeEspera* lista) { //SARA
    string marca, modelo;
    int newTempoReparacao;

    cout << "Introduza a marca do carro: ";
    cin >> marca;
    cout << "Introduza o modelo do carro: ";
    cin >> modelo;
    cout << "Introduza o novo tempo de repara��o: ";
    cin >> newTempoReparacao;

    ListaDeEspera* current = lista;
    bool carUpdated = false;

    while (current != nullptr) {
        if (current->data.marca == marca && current->data.modelo == modelo) {
            current->data.tempo_reparacao = newTempoReparacao;
            carUpdated = true;
        }
        current = current->next;
    }

    if (carUpdated) {
        cout << "O tempo de repara��o foi atualizado para os seguintes carros:" << endl;
        current = lista;
        while (current != nullptr) {
            if (current->data.marca == marca && current->data.modelo == modelo) {
                cout << "ID: " << current->data.id << ", Marca: " << current->data.marca
                    << ", Modelo: " << current->data.modelo << endl;
            }
            current = current->next;
        }
    }
    else {
        cout << "Nenhum carro teve o tempo de repara��o atualizado." << endl;
    }
}

void adicionarPrioridade(ListaDeEspera* lista) { //SARA
    int id;

    cout << "Introduza o ID do ve�culo: ";
    cin >> id;

    ListaDeEspera* current = lista;
    bool vehicleFound = false;

    while (current != nullptr) {
        if (current->data.id == id) {
            vehicleFound = true;
            if (current->data.prioridade) {
                cout << "Este ve�culo j� � priorit�rio." << endl;
            }
            else {
                current->data.prioridade = true;
                cout << "A prioridade do ve�culo foi alterada com sucesso." << endl;
            }
            break;
        }
        current = current->next;
    }

    if (!vehicleFound) {
        cout << "Ve�culo n�o encontrado." << endl;
    }
}

void adicionarET(ET*& listaET) {
    string mec�nico, marca;

    cout << "Introduza o nome do mec�nico: ";
    cin >> mec�nico;
    cout << "Introduza a marca especializada do mec�nico: ";
    cin >> marca;

    // Cria novo nodo da ET
    ET* newET = new ET;
    newET->id = id_ETS++;
    newET->mecanico = mec�nico;
    newET->marca = marca;
    newET->carros = nullptr;
    newET->repaired_cars = nullptr;
    newET->carros_reparados = 0;
    newET->capacidade_atual = 0;
    newET->capacidade = rand() % 4 + 2;
    newET->faturacao = 0;
    newET->next = nullptr;

    // Verifica se listaET est� vazia
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
    cout << "Introduza o nome do mec�nico a remover: ";
    cin >> nomeMecanico;

    ET* current = listaET;

    //Verifica se o mec�nico existe em alguma ET
    while (current != nullptr && current->mecanico != nomeMecanico) {
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Mec�nico n�o encontrado na lista de ETs." << endl;
        return;
    }

    // Coloca todos os carros da ET do mec�nico na arvore bin�ria da ET
    carro* carros = current->carros;
    while (carros != nullptr) {
        BSTNode* newRepairedCar = new BSTNode;
        newRepairedCar->data = carros->data;
        newRepairedCar->left = nullptr;
        newRepairedCar->right = current->repaired_cars;
        current->repaired_cars = newRepairedCar;

        carros = carros->next;
    }

    current->carros = nullptr; //Remove todos os carros do array carros da ET

    string novaMecanico, novaMarca;
    cout << "Introduza o nome do novo mec�nico: "; //Substitui��o do mec�nico..
    cin >> novaMecanico;

    int contadorMecanicoUnico = 0;
    ET* verificarMarca = listaET;
    while (verificarMarca != nullptr) { //Verifica se a marca do mec�nico � unica na ET
        if (verificarMarca->marca == current->marca) {
            contadorMecanicoUnico++;
        }
        verificarMarca = verificarMarca->next;
    }

    string* novaMarcas = new string[NUM_MARCAS - 1]; //Cria um novo array de marcas
    int index = 0;
    if (contadorMecanicoUnico == 1) { //Cria um novo array de marcas sem a marca do mec�nico que saiu
        for (int m = 0; m < NUM_MARCAS; m++) {
            if (marcas[m] != current->marca) {
                novaMarcas[index++] = marcas[m];
            }
        }
        current->capacidade_atual = 0; //Define a ET como vazia atualmente
        current->mecanico = novaMecanico; //Atualiza nome do mec�nico
        current->marca = novaMarcas[rand() % NUM_MARCAS - 1]; //Seleciona uma marca aleat�ria para nova ET
        marcasET[current->id] = current->marca; //Adiciona nova merca ao array marcasET para poderem ser gerados carros desta marca

        cout << "Mec�nico removido com sucesso." << endl;
        cout << "Novo mec�nico adicionado com sucesso." << endl;

    }
}

void SimulaDia(ListaDeEspera* head, ET* ethead, caminhosFicheiros* caminho);

void PainelDeGestao(ListaDeEspera* head, ET* ethead, caminhosFicheiros* caminho){
    bool sair = false;
    char escolha = ' ';

    do {
        cout << endl;
        cout << "***** Bem Vindo Gestor ***** \n";
        cout << "(1).Atualizar tempo de repara��o \n";
        cout << "(2).Adicionar Prioridade \n";
        cout << "(3).Remover Mec�nico\n";
        cout << "(4).Gravar Oficina \n";
        cout << "(5).Carregar Oficina \n";
        cout << "(6).Adicionar ET \n";
        cout << "(7).Imprimir carros reparados \n";
        cout << "(9). Voltar Atr�s \n";
        cout << "(0).Sair" << endl;
        cout << "Selecione a sua op��o:";
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
            gravarFicheiros(head, ethead, numETs, caminho, id_Carros);
            cout << "A grava��o nos ficheiros foi feita com sucesso." << endl;
            break;
        case '5':
            id_Carros = carregarIdCarros(caminho);
            id_ETS = carregarIdEstacoes(caminho);
            marcasET = carregarMarcasET(caminho);
            numETs = carregarNumETs(caminho);
            head = carregarListaDeEspera(head, caminho);
            ethead = carregarEstacoes(ethead, caminho);
            cout << "O carregamento dos ficheiros foi feito com sucesso." << endl;
            break;
        case '6':
            adicionarET(ethead);
            break;
        case '7':
            printRepairedCarsOfET(ethead);
            break;
        case '9':
            SimulaDia(head,ethead, caminho);
            break;
        case '0': cout << "Selecionou a op��o sair! " << endl;
            exit(0);
            break;
        default: cout << "Escolha Inv�lida!" << endl;
            break;
        }
    } while (!sair);
    cin.ignore();
}

void SimulaDia(ListaDeEspera* head, ET* ethead, caminhosFicheiros* caminho) { //Fun��o utilizada para simular um dia na Oficina EDA
    char opcao = ' ';
    bool sair = false;

    do {
        cout << endl;
        cout << "(s): Simular um dia " << endl;
        cout << "(g): Painel de gest�o" << endl;
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
            PainelDeGestao(head,ethead, caminho);
            break;
        case '0':
            exit(0);
            cout << "At� � proxima!\n";
            break;
        default:
            cout << "Escolha inv�lida!" << endl;
            break;
        }
    } while (!sair);
}

int main(int argc, char* argv[]) {
    locale::global(locale(""));

    string file = "modelos.txt";
    ifstream fileMarcas(file);

    if (fileMarcas.is_open()) {
        int i = 0;
        while (!fileMarcas.eof()) {
            getline(fileMarcas, modelos[i++]); //Coloca todos os modelos de carros do ficheiro no array modelos na inicializa��o
        }
    }

    ListaDeEspera* ListaDeEspera = nullptr; //Cria a Lista De Espera
    ET* ListaETs = inicializaEstacoes(); //Cria um determina n�mero de ET0s e coloca-as numa lista
    
    caminhosFicheiros* caminhos = new caminhosFicheiros;

    if ((argv[1] && argv[2] && argv[3]) == NULL) {
        caminhos->caminhoListaDeEspera = "listaDeEspera.txt";
        caminhos->caminhoEstacoes = "estacoes.txt";
        caminhos->caminhoCarrosReparados = "carrosReparados.txt";
    }
    else {
        caminhos->caminhoListaDeEspera = argv[1];
        caminhos->caminhoEstacoes = argv[2];
        caminhos->caminhoCarrosReparados = argv[3];
    }

    SimulaDia(ListaDeEspera, ListaETs, caminhos);
   
    return 0;
}

