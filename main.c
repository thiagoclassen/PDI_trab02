#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pdi.h"

#define ORIGINAL "./Exemplos/b01 - Original.bmp"
#define JANELA_X 11
#define JANELA_Y 15


void blurIngenuo(Imagem *original);
void fillBorders(Imagem *original);

int main() {

    Imagem *imagem;

    //Abre a imagem a ser borrada
    printf("A carregar a imagem [ %s ]... ", ORIGINAL);
    imagem = abreImagem(ORIGINAL, 3);
    if(!imagem) {
        printf("ERRO: A imagem não pode ser aberta.\n");
        exit(1);
    }
    printf("OK\n");

    blurIngenuo(imagem);


    return 0;
}


void blurIngenuo(Imagem *original){
    int border_x = JANELA_X/2;
    int border_y = JANELA_Y/2;
    float total = 0.0f;
    
    Imagem *buffer;
    buffer = criaImagem(original->largura,original->altura,original->n_canais);

    // canal
    for(int c = 0;c<3;c++){
        // linha
        for(int y = border_y;y<original->altura-border_y;y++){
            // coluna
            for(int x = border_x;x<original->altura-border_x;x++){
                // janela
                total = 0.0f;

                for(int j_y = y-border_y; j_y<y+border_y;j_y++){
                    for(int j_x = x-border_x; j_x<x+border_x;j_x++){
                        total += original->dados[c][j_y][j_x];
                    }   
                }

                buffer->dados[c][y][x] = total/(JANELA_X*JANELA_Y);
            }      
        }
    }

    salvaImagem(buffer,"blurIngenuo");
}

void fillBorders(Imagem *original){
    

}
