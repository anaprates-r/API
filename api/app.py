# app.py
from fastapi import FastAPI, Query
from kdtree_wrapper import lib, Tarv, TReg
from ctypes import POINTER,c_double,c_float
from pydantic import BaseModel

app = FastAPI()

class PontoEntrada(BaseModel):
    emb: list[float]
    id: str

@app.post("/construir-arvore")
def constroi_arvore():
    lib.kdtree_construir()
    return {"mensagem": "Árvore KD inicializada com sucesso."}

@app.post("/inserir")
def inserir(ponto: PontoEntrada):
    id_bytes = ponto.id.encode('utf-8')[:99]  # Trunca se necessário
    emb_array = (c_float * 128)(*ponto.emb)
    novo_ponto = TReg(emb=emb_array, id=id_bytes)
    
    return {"mensagem": f"Ponto '{ponto.id}' inserido com sucesso."}

@app.get("/buscar",response_model=PontoEntrada)
def buscar(emb: list[float] = Query(..., min_items=128, max_items=128)):
    emb_array = (c_double * 128)(*emb)
    query = TReg(emb = emb_array)

    arv = lib.get_tree() 
    resultado = lib.buscar_mais_proximo(arv, query)
    id_str = resultado.id.decode('utf-8').rstrip('\x00') 
    emb_result = list(resultado.emb)
    return {
        "emb": emb_result,
        "id": id_str
    }

