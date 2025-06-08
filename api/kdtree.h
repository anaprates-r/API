#ifndef KDTREE_H
#define KDTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

typedef struct _reg{
    float emb[128];
    char id[100];
}treg;
typedef struct _node {
    void *key;
    struct _node *esq;
    struct _node *dir;
} tnode;

typedef struct _arv {
    tnode *raiz;
    double (*cmp)(void *, void *, int);
    double (*dist)(void *, void *);
    int k;
} tarv;

void * aloca_reg(float emb[], char id[]);

double comparador(void *a, void *b, int pos);
double distancia(void *a, void *b);
void kdtree_constroi(tarv * arv, double (*cmp)(void *a, void *b, int ),double (*dist) (void *, void *),int k);
void kdtree_insere(tarv *arv, void *key);
void kdtree_destroi(tarv *arv);
tnode * kdtree_busca(tarv *arv, void * key);
treg buscar_mais_proximo(tarv *arv, treg query);
tarv *get_tree(void);
void inserir_ponto(treg p);
void kdtree_construir();

#endif
