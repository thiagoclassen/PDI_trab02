#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pdi.h"

#define ORIGINAL "./Exemplos/b01 - Original.bmp"
#define JANELA_X 11
#define JANELA_Y 15


void blurIngenuo(Imagem *original);
void fillBorders(Imagem *imagem);
void separavel(Imagem *imagem);
void integral(Imagem *imagem);

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

    separavel(imagem);

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
            for(int x = border_x;x<original->largura-border_x;x++){
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

    fillBorders(buffer);

    salvaImagem(buffer,"blurIngenuo");
}

void fillBorders(Imagem *imagem){

    int border_x = JANELA_X/2;
    int border_y = JANELA_Y/2;
    
    for(int c = 0; c < 3;c++){
        //borda esquerda
        for(int x = 0; x < border_x; x++){
            for(int y = 0; y < imagem->altura; y++){
                imagem->dados[c][y][x] = 0.0f;
            }
        }

        // borda direita
        for(int x = imagem->largura - border_x; x < imagem->largura; x++){
            for(int y = 0; y < imagem->altura; y++){
                imagem->dados[c][y][x] = 0.0f;
            }
        }

        // borda cima
        for(int x = border_x; x < imagem->largura - border_x; x++){
            for(int y = 0; y < border_y; y++){
                imagem->dados[c][y][x] = 0.0f;
            }
        }

        // borda baixo
        for(int x = border_x; x < imagem->largura - border_x; x++){
            for(int y = imagem->altura - border_y; y < imagem->altura; y++){
                imagem->dados[c][y][x] = 0.0f;
            }
        }
    }

}

void separavel(Imagem *imagem){
    
    int border_x = JANELA_X/2;
    int border_y = JANELA_Y/2;
    float total;
    
    Imagem *buffer, *final;

    copiaConteudo(imagem, buffer);

    // filtro horizontal

    // canal
    for(int c = 0;c<3;c++){
        // linha
        for(int y = 0;y<imagem->altura;y++){
            // coluna
            for(int x = border_x;x<imagem->largura-border_x;x++){
                // janela
                total = 0.0f;

                for(int j_x = x-border_x; j_x<x+border_x;j_x++){
                    total += imagem->dados[c][y][j_x];
                }   

                buffer->dados[c][y][x] = total/(JANELA_X);
            }      
        }
    }

    //salvaImagem(buffer,"separavelHorizontal");

    // filtro vertical
    // canal
    for(int c = 0;c<3;c++){
        // linha
        for(int y = border_y; y < imagem->altura-border_y; y++){
            // coluna
            for(int x = 0; x < imagem->largura; x++){
                // janela
                total = 0.0f;

                for(int j_y = y-border_y; j_y<y+border_y;j_y++){
                    total += imagem->dados[c][j_y][x];
                }  

                buffer->dados[c][y][x] = total/(JANELA_Y);
            }      
        }
    }

    fillBorders(buffer);

    salvaImagem(buffer,"separavel");
}
