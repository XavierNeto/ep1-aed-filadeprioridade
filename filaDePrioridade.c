/*********************************************************************/
/**   ACH2023 - Algoritmos e Estruturas de Dados I                  **/
/**   EACH-USP - Segundo Semestre de 2020                           **/
/**   <2020204> - Prof. Luciano Antonio Digiampietri                **/
/**                                                                 **/
/**   EP1 - Fila de Prioridade                                      **/
/**                                                                 **/
/**   <Augusto Xavier Neto>                   <11367581>            **/
/**                                                                 **/
/*********************************************************************/

#include "filaDePrioridade.h"

PFILA criarFila(int max)
{
  PFILA res = (PFILA)malloc(sizeof(FILADEPRIORIDADE));
  res->maxElementos = max;
  res->arranjo = (PONT *)malloc(sizeof(PONT) * max);
  int i;
  for (i = 0; i < max; i++)
    res->arranjo[i] = NULL;
  PONT cabeca = (PONT)malloc(sizeof(ELEMENTO));
  res->fila = cabeca;
  cabeca->ant = cabeca;
  cabeca->prox = cabeca;
  cabeca->id = -1;
  cabeca->prioridade = 1000000;
  return res;
}

void exibirLog(PFILA f)
{
  printf("Log [elementos: %i (alem do no cabeca)]\n", tamanho(f));
  PONT atual = f->fila;
  printf("%p[%i;%f;%p]%p ", atual->ant, atual->id, atual->prioridade, atual, atual->prox);
  atual = atual->prox;
  while (atual != f->fila)
  {
    printf("%p[%i;%f;%p]%p ", atual->ant, atual->id, atual->prioridade, atual, atual->prox);
    atual = atual->prox;
  }
  printf("\nElementos validos: ");
  atual = atual->prox;
  while (atual != f->fila)
  {
    printf("[%i;%f;%p] ", atual->id, atual->prioridade, atual);
    atual = atual->prox;
  }

  printf("\nValores do arrajo:\n\[ ");
  int x;
  for (x = 0; x < f->maxElementos; x++)
    printf("%p ", f->arranjo[x]);
  printf("]\n\n");
}

// Função para checar o tamanho da lista
int tamanho(PFILA f)
{
  PONT end = f->fila->prox;
  int tam = 0;

  // Enquanto não o endereço atual não for o do nó cabeça, o tamanho será somado em um
  while (end != f->fila)
  {
    tam++;
    end = end->prox;
  }

  return tam;
}

// Função auxiliar para checar se o id já existe (usando o no cabeça como um sentinela)

PONT buscaSeqExc(PFILA f, int id, PONT *ant)
{
  *ant = f->fila;
  PONT atual = f->fila->prox;
  f->fila->id = id;

  while (atual->id != id)
  {
    *ant = atual;
    atual = atual->prox;
  }

  if (atual != f->fila && atual->id == id)
  {

    return atual;
  }
  return NULL;
}

// Função para inserir elemento

bool inserirElemento(PFILA f, int id, float prioridade)
{
  bool resposta = false;
  PONT i, ant, temp, p, pp, checarid;

  // Checando se o id é valido

  if (id < 0 || id >= f->maxElementos)
  {
    return resposta;
  }

  // Checando se o id já existe

  checarid = buscaSeqExc(f, id, &ant);
  if (checarid != NULL)
  {
    return resposta;
  }

  // Alocando memória para o elemento e inserindo seu id, propriedade e também seu endereço no arranjo
  i = (PONT)malloc(sizeof(ELEMENTO));
  i->id = id;
  i->prioridade = prioridade;
  f->arranjo[id] = i;

  // Caso seja o primeiro da lista, o nó cabeça apontará para ele
  if (f->fila->prox == f->fila)
  {

    f->fila->prox = i;
    f->fila->ant = i;
    i->prox = f->fila;
    i->ant = f->fila;

    return true;
  }
  else
  {

    // Caso não for o primeiro, primeiramente ele vai ser colocado "ao lado" do nó cabeça (na primeira posição da lista)
    temp = f->fila->prox;
    f->fila->prox->ant = i;
    f->fila->prox = i;
    i->prox = temp;
    i->ant = f->fila;

    // e então será movido de acordo com sua prioridade

    while (i->prox->prioridade >= i->prioridade && i->prox != f->fila)
    {

      pp = i->prox->prox;
      p = i->prox;
      i->prox->prox = i;
      i->prox->ant = i->ant;
      i->ant->prox = i->prox;
      i->prox = pp;
      i->ant = p;
    }
    return true;
  }
  return resposta;
}

// Função para aumentar a prioridade de um elemento
bool aumentarPrioridade(PFILA f, int id, float novaPrioridade)
{
  bool resposta = false;

  PONT checarid, ant, p, pp, i;

  i = f->arranjo[id];

  // Validando o id
  if (id < 0 || id >= f->maxElementos)
  {
    return resposta;
  }

  // Checando se o id existe
  checarid = buscaSeqExc(f, id, &ant);
  if (checarid == NULL)
  {
    return resposta;
  }

  // Checando se a prioridade é maior ou igual a nova prioridade
  if (f->arranjo[id]->prioridade >= novaPrioridade)
  {
    return resposta;
  }

  // Atribuindo a nova prioridade
  i->prioridade = novaPrioridade;

  // Deslocando de acordo com a nova prioridade
  while (i->ant->prioridade < i->prioridade && i->ant != f->fila)
  {
    p = i->prox;
    pp = i->ant;
    i->ant->ant->prox = i;
    i->ant = i->ant->ant;
    i->prox = pp;
    i->prox->ant = i;
    i->prox->prox = p;
  }
  return true;
}

// Função para reduzir a prioridade de um elemento
bool reduzirPrioridade(PFILA f, int id, float novaPrioridade)
{
  bool resposta = false;

  PONT checarid, ant, p, pp, i;

  i = f->arranjo[id];

  // Validando o id
  if (id < 0 || id >= f->maxElementos)
  {
    return resposta;
  }

  // Checando se o id existe
  checarid = buscaSeqExc(f, id, &ant);
  if (checarid == NULL)
  {
    return resposta;
  }

  // Checando se a prioridade é menor ou igual a nova prioridade
  if (f->arranjo[id]->prioridade <= novaPrioridade)
  {
    return resposta;
  }

  // Atribuindo a nova prioridade
  i->prioridade = novaPrioridade;

  // Deslocando de acordo com a nova prioridade
  while (i->prox->prioridade >= i->prioridade && i->prox != f->fila)
  {

    pp = i->prox->prox;
    p = i->prox;
    i->prox->prox = i;
    i->prox->ant = i->ant;
    i->ant->prox = i->prox;
    i->prox = pp;
    i->ant = p;
  }
  return true;
}

// Função para remover um elemento da lista
PONT removerElemento(PFILA f)
{
  PONT resposta = NULL;
  PONT p, pp, i;

  // Verificando se a lista está vazia
  if (f->fila->prox == f->fila)
  {
    return resposta;
  }

  i = f->fila->prox;

  // Atribuindo NULL no arranjo
  f->arranjo[i->id] = NULL;

  // Acertando os ponteiros
  p = i->prox;
  f->fila->prox = p;
  p->ant = f->fila->prox;

  return i;
}

// Função para consultar a prioridade de um elemento
bool consultarPrioridade(PFILA f, int id, float *resposta)
{
  bool resp = false;

  PONT checarid, ant;

  // Validando o id
  if (id < 0 || id >= f->maxElementos)
  {
    return resp;
  }

  // Checando se o id existe
  checarid = buscaSeqExc(f, id, &ant);
  if (checarid == NULL)
  {
    return resp;
  }

  // Usa o ponteiro para resposta para guardar o valor da prioridade do elemento
  *resposta = f->arranjo[id]->prioridade;

  return true;
}