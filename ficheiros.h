#pragma once
#include "structs.h"

void gravarFicheiros(ListaDeEspera* head, ET* ethead, int numETs, caminhosFicheiros* caminho, int id_Carros);

ET* carregarEstacoes(ET* ethead, caminhosFicheiros* caminho);

ListaDeEspera* carregarListaDeEspera(ListaDeEspera* head, caminhosFicheiros* caminho);

int carregarIdCarros(caminhosFicheiros* caminho);

int carregarIdEstacoes(caminhosFicheiros* caminho);

string* carregarMarcasET( caminhosFicheiros* caminho);

int carregarNumETs(caminhosFicheiros* caminho);