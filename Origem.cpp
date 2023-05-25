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
        BSTNode* novoNodo = new BSTNode; //Cria��o de um novo nodo
        novoNodo->data = data;
        novoNodo->left = nullptr;
        novoNodo->right = nullptr;
        return novoNodo;
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

ET* inicializaEstacoes() {
    int index = 0;
    string file = "marcas.txt";
    ifstream fileMarcas(file);

    string marca;

    if (fileMarcas.is_open()) {                                     //array que guarda as marcas do ficheiro .txt
        int i = 0;
        while (!fileMarcas.eof()) {
            getline(fileMarcas, marcas[i++]);
        }
    }

    srand(time(0));

    ET* head = nullptr;                                         //apontador do 1�elemento da lista (cabe�alho)
    ET* current = nullptr;                                      //apontador do �ltimo elemento da lista (ignorando o vazio)

    for (int i = 0; i < numETs; i++) {                         //preencher cada caracter�stica das ETs:
        ET* newET = new ET;                                    //apontador para uma nova ET

        newET->id = id_ETS++;                                  //id da ET
        newET->capacidade = rand() % 4 + 2;                    //capacidade total da ET (valor aleat�rio entre 2 e 5)
        newET->marca = marcas[rand() % NUM_MARCAS - 1];        //marca atribu�da � ET
        newET->carros = nullptr;                               //carros presentes na ET 
        newET->repaired_cars = nullptr;                        //carros reparados
        newET->carros_reparados = 0;                           //n+ de carros reparados
        newET->capacidade_atual = 0;                           //n� de carros atualmente na ET
        newET->faturacao = 0;                                  //valor total de fatura��o da ET
        newET->next = nullptr;                                 //pr�ximo elemento da lista � nulo

        marcasET[index++] = newET->marca;                      //guardar no array as marcas que est�o a ser usadas na simula��o

        cout << "Introduza o mecanico para a ET " << newET->id << ": ";
        getline(cin, newET->mecanico);                                      //atribuir um mec�nico � ET


        if (head == nullptr) {        //se a lista estiver vazia
            head = newET;             //1� elemento da lista passa a ser a ET criada
            current = newET;          //aponta para a nova ET 
        }
        else {                        //caso a lista tenha 1 ou mais elementos(ETs)
            current->next = newET;    //o pr�ximo elemento da lista passa a ser a ET criada
            current = newET;          //aponta para o novo elemento
        }
    }


    return head;
}


void organizaListaDeEspera(ListaDeEspera*& head) { //Fun��o para organizar lista de espera de acordo com a prioridade
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

void adicionaListaDeEspera(ListaDeEspera*& head, carro* newCarro) { //Adiciona carros � lista de espera
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
 

void adicionaCarrosET(ListaDeEspera*& esperaHead, ET* etHead) { //Fun��o para adicionar carros a ETs
    int totalCarsAdded = 0;  //Inicializa vari�vel a zero

    ListaDeEspera* esperaAtual = esperaHead; //Apontador para o nodo atual
    ListaDeEspera* esperaAnterior = nullptr;

    while (esperaAtual != nullptr && totalCarsAdded < 8) { //Ciclo vair parar ap�s percorrer todas as ETs ou depois de terem sido adicionados 8 carros
        umCarro currentCar = esperaAtual->data; //Obtem INFO do carro no nodo atual

        ET* currentET = etHead; //Apontador para a ET selecionada
        ET* selectedET = nullptr;  //Apontador para ET na qual vai ser adicionado o carro

       
        while (currentET != nullptr) { //Percorre todas as ETs... (que tenha a mesma marca e capacidade dispon�vel)
            if (currentET->capacidade_atual < currentET->capacidade && currentET->marca == currentCar.marca) {
                selectedET = currentET; //Seleciona a ET
                break;
            }
            currentET = currentET->next; //Verifica a pr�xima ET
        }

        if (selectedET != nullptr) {
           
            carro* newCar = new carro(); //Criado um novo nodo para o carro
            newCar->data = currentCar; //Atribui INFO do carro ao nodo

            
            if (selectedET->carros == nullptr) { //Adicionar carro � lista de carros da ET..
                selectedET->carros = newCar; //Se n�o tiver carros ainda, o primeiro carro � definido como primeiro nodo
            }
            else {
                carro* temp = selectedET->carros; 
                while (temp->next != nullptr) { //Caso contr�rio, adiciona o carro ao fim da Lista dos Carros
                    temp = temp->next;
                }
                temp->next = newCar;
            }

         
            selectedET->capacidade_atual++; //Incrementa capacidade atual da ET

            //Remove o carro adicionado (nodo atual) da Lista De Espera
            if (esperaAnterior == nullptr) {
                esperaHead = esperaAtual->next;
            }
            else {
                esperaAnterior->next = esperaAtual->next;
            }
            ListaDeEspera* temp = esperaAtual;
            esperaAtual = esperaAtual->next;
            delete temp; //Liberta espa�o da mem�ria anteriormente ocupado

            totalCarsAdded++; //Incrementa numero de carros adicionados
        }
        else {
            
            esperaAnterior = esperaAtual; //Atualiza o nodo
            esperaAtual = esperaAnterior->next; //Passa para o proximo nodo
        }
    }
}

void criaCarrosListaDeEspera(ListaDeEspera*& head, int numCarsToAdd) { //Fun��o para criar 10 carros de cada vez que � chamada
    for (int i = 0; i < numCarsToAdd; i++) {
        carro* newCarro = new carro();
        
        newCarro->data.id = id_Carros++; //Atribui diversos atributos ao carro..
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


void reparaCarros(ET* head) { //Fun��o utilizada para reparar carros
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

                // Output do que aconteceu ao utilizador...
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
    cout << "Introduza o ID da ET: "; //Introdu��o do id da ET a ver os carros reparados
    cin >> etId;

    ET* currentET = head;

    while (currentET != nullptr) { //Enquanto n�o chegar ao fim das ETs
        if (currentET->id == etId) { //Para encontrar o id
            BSTNode* root = currentET->repaired_cars;
            cout << "Carros reparados da ET " << etId << ":" << endl;
            printBST(root); //Utilizada a fun��o de imprimir a �rvore bin�ria
            return; 
        }
        currentET = currentET->next; //Passar � pr�xima ET at� encontrar o id
    }

    cout << "ET com ID " << etId << " n�o encontrado." << endl; //Caso o ciclo de encontrar o id termine e n�o seja encontrado o id � mostrada a mensagem
}

void atualizaTempoReparacao(ListaDeEspera* lista) {
    string marca, modelo;
    int newTempoReparacao;

    cout << "Introduza a marca do carro: ";
    cin >> marca;                                                                 //utilizador insere a marca
    cout << "Introduza o modelo do carro: ";
    cin >> modelo;                                                                //utilizador insere o modelo
    cout << "Introduza o novo tempo de repara��o: ";
    cin >> newTempoReparacao;                                                     //utilizador insere o novo tempo de repara��o

    ListaDeEspera* current = lista;                                               //apontador da lista de espera
    bool carUpdated = false;                                                      //o carro ainda n�o foi encontrado

    while (current != nullptr) {                                                 //ciclo vai decorrer at� percorrer a lista toda
        if (current->data.marca == marca && current->data.modelo == modelo) {    //caso as caracter�sticas inseridas pelo utilizador sejam encontradas
            current->data.tempo_reparacao = newTempoReparacao;                   //tempo de repara��o atualizado
            carUpdated = true;                                                   //carro encontrado
        }
        current = current->next;                                                 //passar para o pr�ximo elemento da lita
    }

    if (carUpdated) {                                                                      //caso o carro seja encontrado
        cout << "O tempo de repara��o foi atualizado para os seguintes carros:" << endl;
        current = lista;
        while (current != nullptr) {
            if (current->data.marca == marca && current->data.modelo == modelo) {
                cout << "ID: " << current->data.id << ", Marca: " << current->data.marca
                    << ", Modelo: " << current->data.modelo << endl;                        //caracteristicas dos ve�culos que sofreram atualiza��es no tempo de repara��o
            }
            current = current->next;
        }
    }
    else {                                                                                 //caso o carro n�o seja encontrado
        cout << "Nenhum carro teve o tempo de repara��o atualizado." << endl;
    }
}

void adicionarPrioridade(ListaDeEspera* lista) {
    int id;

    cout << "Introduza o ID do ve�culo: ";
    cin >> id;                                                                          //utilizador insere id

    ListaDeEspera* current = lista;                                                     //apontador da lista de espera
    bool vehicleFound = false;                                                          //ve�culo na� encontrado

    while (current != nullptr) {                                                        //ciclo que percorre a lista de espera
        if (current->data.id == id) {                                                   //caso o id de um ve�culo seja igual ao que foi inserido pelo utilizador
            vehicleFound = true;                                                        //ve�culo encontrado
            if (current->data.prioridade) {                                             //caso o ve�culo j� tenha prioridade atribu�da
                cout << "Este ve�culo j� � priorit�rio." << endl;
            }
            else {                                                                      //caso ainda n�o tenha prioridade
                current->data.prioridade = true;                                        //adicionar prioridade
                cout << "A prioridade do ve�culo foi alterada com sucesso." << endl;
            }
            break;
        }
        current = current->next;                                                        //pr�ximo elemento da lista de espera � analisado
    }

    if (!vehicleFound) {                                                                //caso n�o exista um ve�culo com o id inserido 
        cout << "Ve�culo n�o encontrado." << endl;
    }
}

void adicionarET(ET*& listaET) { //Fun��o para adicionar uma nova ET
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

void PainelDeGestao(ListaDeEspera* head, ET* ethead, caminhosFicheiros* caminho){ //Fun��o que controla menu de gest�o
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
           adicionaCarrosET(head, ethead);
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

