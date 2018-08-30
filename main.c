#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pdi.h"

#define ORIGINAL "./Exemplos/b01 - Original.bmp"
#define JANELA_X 15
#define JANELA_Y 15

void blurIngenuo(Imagem *imagem);
void separavel(Imagem *imagem);
void integral(Imagem *imagem);
void fillBorders(Imagem *imagem);

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

    printf("BlurIngenuo....");
    blurIngenuo(imagem);
    printf("OK\n");

    printf("Separavel....");
    separavel(imagem);
    printf("OK\n");

    printf("Integral....");
    integral(imagem);
    printf("OK\n");

    destroiImagem(imagem);

    return 0;
}


void blurIngenuo(Imagem *imagem){
    
    float total = 0.0f;
    int border_x = JANELA_X/2, 
        border_y =JANELA_Y/2,
        total_janela = JANELA_X * JANELA_Y;
    
    Imagem *buffer;
    buffer = criaImagem(imagem->largura,imagem->altura,imagem->n_canais);

    // canal
    for(int c = 0;c<3;c++){
        // linha
        for(int y = border_y;y<imagem->altura-border_y;y++){
            // coluna
            for(int x = border_x;x<imagem->largura-border_x;x++){
                // janela
                total = 0.0f;

                for(int j_y = y-border_y; j_y<y+border_y;j_y++){
                    for(int j_x = x-border_x; j_x<x+border_x;j_x++){
                        total += imagem->dados[c][j_y][j_x];
                    }   
                }

                buffer->dados[c][y][x] = total/total_janela;
            }      
        }
    }

    fillBorders(buffer);

    salvaImagem(buffer,"01blurIngenuo");
    destroiImagem(buffer);    
}

void fillBorders(Imagem *imagem){

    int border_x = JANELA_X/2, 
        border_y =JANELA_Y/2;

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
    
    float total;
    int border_x = JANELA_X/2, 
        border_y =JANELA_Y/2;
    
    Imagem *buffer, *final;

    buffer = criaImagem(imagem->largura,imagem->altura,imagem->n_canais);
    final = criaImagem(imagem->largura,imagem->altura,imagem->n_canais);

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

                buffer->dados[c][y][x] = total/JANELA_X;
            }      
        }
    }   

    // filtro vertical
    // canal
    for(int c = 0;c<3;c++){
        // linha
        for(int y = border_y; y < buffer->altura-border_y; y++){
            // coluna
            for(int x = 0; x < buffer->largura; x++){
                // janela
                total = 0.0f;

                for(int j_y = y-border_y; j_y<y+border_y;j_y++){
                    total += buffer->dados[c][j_y][x];
                }  

                final->dados[c][y][x] = total/JANELA_Y;
            }      
        }
    }

    fillBorders(final);

    salvaImagem(final,"02separavel");

    destroiImagem(buffer);
    destroiImagem(final);
}

void integral(Imagem *imagem){

    int border_x = JANELA_X/2, 
        border_y =JANELA_Y/2,
        total_janela = JANELA_X * JANELA_Y;
    float total;

    Imagem *integral, *final;

    integral = criaImagem(imagem->largura,imagem->altura,imagem->n_canais);
    final = criaImagem(imagem->largura,imagem->altura,imagem->n_canais);

    // gera imagem integral
    printf("gerando imagem soma integral....");
    for(int c = 0; c < 3; c++){
        for(int y = 0; y < imagem->altura; y++){
            for(int x = 0; x < imagem->largura; x++){

                // somando todos acima e esquerda do ponto
                for(int i = y; i >= 0; i--){
                    for(int j = x; j >= 0; j--){
                        integral->dados[c][y][x] += imagem->dados[c][i][j];
                    }
                }
            }
        }
    }

    printf("OK....gerando imagem final....");

    for(int c = 0;c<3;c++){
        // linha
        for(int y = border_y;y<integral->altura-border_y;y++){
            // coluna
            for(int x = border_x;x<integral->largura-border_x;x++){
                // janela
                total = 0.0f;
                
                total = integral->dados[c][y+border_y][x+border_x]; // valor total
                if(y>border_y)
                    total -= integral->dados[c][y-border_y-1][x+border_x]; // remove superior da janela
                if(x>border_x)
                    total -= integral->dados[c][y+border_y][x-border_x-1]; // remove esquerda da janela
                if(x>border_x && y>border_y)
                    total += integral->dados[c][y-border_y-1][x-border_x-1]; // soma valores esquerda e acima da janela

                final->dados[c][y][x] = total/total_janela;
            }      
        }

        //borda esquerda
        for(int x = 0; x < border_x; x++){
            for(int y = border_y; y < imagem->altura-border_y; y++){
                total = 0.0f;
                
                total = integral->dados[c][y+border_y][x+border_x]; // valor total
                if(y > border_y)
                    total -= integral->dados[c][y-border_y-1][x+border_x]; // remove superior da janela
                final->dados[c][y][x] = total/(JANELA_Y*2);
            }
        }

        // borda direita
        for(int x = imagem->largura - border_x; x < imagem->largura; x++){
            for(int y = border_y; y < imagem->altura-border_y; y++){
                total = 0.0f;
                
                total = integral->dados[c][y+border_y][x]; // valor total
                if(y > border_y)
                    total -= integral->dados[c][y+border_y][x-border_x-1]; // remove superior da janela
                final->dados[c][y][x] = total/(JANELA_Y*2);
            }
        }

        // borda cima
        for(int x = border_x; x < imagem->largura - border_x; x++){
            for(int y = 0; y < border_y; y++){
                total = 0.0f;
                total += integral->dados[c][y+border_y][x+border_x];
                
                if(x > border_x)
                    total -= integral->dados[c][y+border_y][x-border_x-1];
                final->dados[c][y][x] = total/(JANELA_X*2);
            }
        }

        // borda baixo
        for(int x = border_x; x < imagem->largura - border_x; x++){
            for(int y = imagem->altura - border_y; y < imagem->altura; y++){
                total = 0.0f;
                total += integral->dados[c][y][x+border_x];
                
                total -= integral->dados[c][y-border_y-1][x+border_x];
                if(x > border_x){
                    total -= integral->dados[c][y][x-border_x-1];
                    total += integral->dados[c][y-border_y-1][x-border_x-1];
                }
                final->dados[c][y][x] = total/(JANELA_X*2);
            }
        }
    }

    //fillBorders(final);
    
    salvaImagem(final, "integral");

    destroiImagem(integral);
    destroiImagem(final);
}
