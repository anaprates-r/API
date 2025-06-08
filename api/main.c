#include "kdtree.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void test_busca(){
    tarv arv;
    kdtree_constroi(&arv,comparador,distancia,128);
    float emb1[128] = {0};
    float emb2[128] = {0};
    float emb3[128] = {0};
    float emb4[128] = {0};
    float emb5[128] = {0};
    float embt[128] = {0};
    
    emb1[0] = 5.0;
    emb2[0] = 2.0;
    emb3[0] = 1.0;
    emb4[0] = 9.0;
    emb5[0] = 7.0;
    embt[0] = 3.5;

    kdtree_insere(&arv,aloca_reg(emb1,"a"));
    kdtree_insere(&arv,aloca_reg(emb2,"b"));
    kdtree_insere(&arv,aloca_reg(emb3,"c"));
    kdtree_insere(&arv,aloca_reg(emb4,"d"));
    kdtree_insere(&arv,aloca_reg(emb5,"e"));



    tnode * raiz = arv.raiz;
    assert(strcmp(((treg *)raiz->dir->key)->id, "d")==0);
    assert(strcmp(((treg *)raiz->esq->key)->id, "b")==0);
    assert(strcmp(((treg *)raiz->esq->esq->key)->id, "c")==0);
    assert(strcmp(((treg *)raiz->dir->esq->key)->id, "e")==0);

    printf("\n");
    treg  * atual = aloca_reg(embt,"x");
    tnode * mais_proximo = kdtree_busca(&arv,atual);
    assert(strcmp(((treg *)mais_proximo->key)->id,"a") == 0);

    printf("\n");
    atual->emb[0] = 8.0;    
    mais_proximo = kdtree_busca(&arv,atual);
    assert(strcmp(((treg *)mais_proximo->key)->id,"d") == 0);

    printf("\n");
    atual->emb[0] = 1.0;    
    mais_proximo = kdtree_busca(&arv,atual);
    assert(strcmp(((treg *)mais_proximo->key)->id,"c") == 0);

    free(atual);
    kdtree_destroi(&arv);
}
void test_constroi(){
    /* declaracao de variaveis */
    tarv arv;
    tnode node1;
    tnode node2;
    float emb1[128] = {0};
    float emb2[128] = {0};
    
    emb1[0] = 1.0;
    emb2[0] = 2.0;

    node1.key = aloca_reg(emb1,"Ana");
    node2.key = aloca_reg(emb2,"Carol");


    /* chamada de funções */
    kdtree_constroi(&arv,comparador,distancia,128 );
    
    /* testes */
    assert(arv.raiz == NULL);
    assert(arv.k == 128);
    assert(arv.cmp(node1.key,node2.key,0) < 0.0);
    assert(strcpy(((treg *)node1.key)->id,"Ana"));
    assert(strcpy(((treg *)node2.key)->id,"Carol"));
    free(node1.key);
    free(node2.key);
}
int main(void){
    test_constroi();
    test_busca();
    printf("SUCCESS!!\n");
    return EXIT_SUCCESS;
}
