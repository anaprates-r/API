#include "kdtree.h"

/*Definições desenvolvedor usuario*/

void * aloca_reg(float emb[], char id[]){
    treg * reg;
    reg = malloc(sizeof(treg));
    memcpy(reg->emb,emb,128 * sizeof(float));
    strcpy(reg->id,id);
    return reg;
}

double comparador(void *a, void *b, int pos){
    return ((treg *)a)->emb[pos] - ((treg *)b)->emb[pos];
    }

double distancia(void * a, void *b){
        int pos;
        float dif;
        double soma = 0.0;
        for(pos = 0;pos<128;pos++){
            dif = ((treg *)a)->emb[pos] - ((treg *)b)->emb[pos];
            soma += dif*dif;
        }
        return soma;
}

void kdtree_constroi(tarv * arv, double (*cmp)(void *a, void *b, int ),double (*dist) (void *, void *),int k){
    arv->raiz = NULL;
    arv->cmp = cmp;
    arv->dist = dist;
    arv->k = k;
}


void _kdtree_insere(tnode **raiz, void * key, double (*cmp)(void *a, void *b, int),int profund, int k){
    if(*raiz == NULL){
        *raiz = malloc(sizeof(tnode));
        (*raiz)->key = key;
        (*raiz)->esq = NULL;
        (*raiz)->dir = NULL;
    }else{
        int pos = profund % k;
        if (cmp( (*(*raiz)).key , key ,pos) <0){
            _kdtree_insere( &((*(*raiz)).dir),key,cmp,profund + 1,k);
        }else{
            _kdtree_insere( &((*raiz)->esq),key,cmp,profund +1,k);
        }
    }
}
void kdtree_insere(tarv *arv, void *key){
    _kdtree_insere(&(arv->raiz),key,arv->cmp,0,arv->k);
}


void _kdtree_destroi(tnode * node){
    if (node!=NULL){
        _kdtree_destroi(node->esq);
        _kdtree_destroi(node->dir);
        free(node->key);
        free(node);
    }
}

void kdtree_destroi(tarv *arv){
    _kdtree_destroi(arv->raiz);
}


void _kdtree_busca(tarv *arv, tnode ** atual, void * key, int profund, double *menor_dist, tnode **menor){
    tnode ** lado_principal; 
    tnode ** lado_oposto;    
    if (*atual != NULL){
        double dist_atual = arv->dist((*atual)->key, key);
        if (dist_atual < *menor_dist){
            *menor_dist = dist_atual;
            *menor = *atual;
        }
        int pos = profund % arv->k;
        double comp = arv->cmp(key, (*atual)->key, pos);

        printf("%s dist %4.3f menor_dist %4.3f comp %f\n", ((treg *)((tnode *)*atual)->key)->id,dist_atual,*menor_dist,comp);

        /* define lado principal para buscar */
        if (comp < 0){
            lado_principal =  &((*atual)->esq);
            lado_oposto    =  &((*atual)->dir); 
        }else{
            lado_principal =  &((*atual)->dir);
            lado_oposto    =  &((*atual)->esq); 
        }

        _kdtree_busca(arv, lado_principal, key, profund + 1, menor_dist, menor);

        /* Verifica se deve buscar também no outro lado*/

        if (comp*comp < *menor_dist) {
            printf("tentando do outro lado %f\n",comp*comp);
            _kdtree_busca(arv, lado_oposto, key, profund + 1, menor_dist, menor);
        }
    }
}


tnode * kdtree_busca(tarv *arv, void * key){
    tnode * menor = NULL;
    double menor_dist = DBL_MAX;
    _kdtree_busca(arv,&(arv->raiz),key,0,&menor_dist,&menor);
    return menor;
}

treg buscar_mais_proximo(tarv *arv, treg query) {
    double menor_dist = 1e20;
    tnode *menor = NULL;
    _kdtree_busca(arv, &(arv->raiz), &query, 0, &menor_dist, &menor);
    return *((treg *)(menor->key));
}


tarv arvore_global;

tarv* get_tree() {
    return &arvore_global;
}

void inserir_ponto(treg p) {
    treg *novo = malloc(sizeof(treg));
    *novo = p;  // cópia de estrutura
    kdtree_insere(&arvore_global,novo);
}
void kdtree_construir() {
    arvore_global.k = 128;
    arvore_global.dist = distancia;
    arvore_global.cmp = comparador;
    arvore_global.raiz = NULL;
}

