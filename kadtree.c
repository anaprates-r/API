#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<string.h>
#include<assert.h>

/*Definições desenvolvedor usuario*/
typedef struct _reg{
    float emb[128];
    char id[100];
}treg;

void * aloca_reg(float emb[], char id[]){
    treg * reg;
    reg = malloc(sizeof(treg));
    memcpy(reg->emb,emb,128 * sizeof(float));
    strcpy(reg->id,id);
    return reg;
}

int comparador(void *a, void *b, int pos){
    if (((treg *)a)->emb[pos] < ((treg *)b)->emb[pos])
        return -1;
    else if(((treg *)a)->emb[pos] > ((treg *)b)->emb[pos])
        return 1;
    else
        return 0;  
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


/*Definições desenvolvedor da biblioteca*/
typedef struct _node{
    void * key;
    struct _node * esq;
    struct _node * dir;
}tnode;

typedef struct _arv{
    tnode * raiz;
    int (*cmp)(void *, void *, int);
    double (*dist) (void *, void *);
    int k;
}tarv;



/*funções desenvolvedor da biblioteca*/

void kdtree_constroi(tarv * arv, int (*cmp)(void *a, void *b, int ),double (*dist) (void *, void *),int k){
    arv->raiz = NULL;
    arv->cmp = cmp;
    arv->dist = dist;
    arv->k = k;
}

/*teste*/
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
    assert(arv.cmp(node1.key,node2.key,0) < 0);
    assert(strcpy(((treg *)node1.key)->id,"Ana"));
    assert(strcpy(((treg *)node2.key)->id,"Carol"));
    free(node1.key);
    free(node2.key);
}

void _kdtree_insere(tnode **raiz, void * key, int (*cmp)(void *a, void *b, int),int profund, int k){
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
        int comp = arv->cmp(key, (*atual)->key, pos);

        printf("%s dist %4.3f menor_dist %4.3f comp %d\n", ((treg *)((tnode *)*atual)->key)->id,dist_atual,*menor_dist,comp);

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

int main(void){
    test_constroi();
    test_busca();
    printf("SUCCESS!!\n");
    return EXIT_SUCCESS;
}
