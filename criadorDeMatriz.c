#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
//O MAPA TEM DIMENSOES DE 1789 x 1405

//0 significa onde pode-se andar livremente
//1 significa onde eh impossivel
//2 significa onde pode mas o personagem eh parcialmente coberto por coisas(e.g. plantas)

//O MAPA
ALLEGRO_BITMAP *map = NULL;

//INICIALIZAR
int main(){
    //Checa se houve falha para inicializar a allegro
    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return 0;
    }

    //Checa se  houe falha para inicializar a add-on allegro_image
    if (!al_init_image_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return 0;
    }

    //Carrega o mapa da matriz
    map = al_load_bitmap("mapamatriz.png");
    if (!map){
        fprintf(stderr, "Falha ao carregar o arquivo de map.\n");
        return 0;
    }

    //Define algumas constantes
    const int ALTURA_MAPA   = al_get_bitmap_height(map);
    const int LARGURA_MAPA  = al_get_bitmap_width(map);
    const int ALTURA_TELA   = 900;
    const int LARGURA_TELA  = 1440;

    //Define tamanhos da matriz
    int matriz[ALTURA_MAPA][LARGURA_MAPA];

    //Cria um arquivo para guardar a matriz (como se fosse uma biblioteca)
    FILE * arq;
    arq = fopen("matriz.txt", "wt");
    fprintf(arq, "#ifndef MATRIZ_H\n");
    fprintf(arq, "#define MATRIZ_H\n");
    fprintf(arq, "int matriz[%d][%d] = { ", ALTURA_MAPA, LARGURA_MAPA);

    ALLEGRO_LOCKED_REGION *lockmap;
    lockmap = al_lock_bitmap(map,al_get_bitmap_format(map), ALLEGRO_LOCK_READONLY);

    //Checa se houve erro
    if (lockmap == NULL) { 
        fprintf(stderr,"Erro no lock");
        return 1;
    }

    int x, y, bit;
    unsigned char r, g, b;

    for(y = 0; y < al_get_bitmap_height(map); y++) {
        
        fprintf(arq, "{");

        for(x = 0; x < al_get_bitmap_width(map); x++){

            //Checa qual a cor para saber se pode ou não andar e fazer a matriz
            unsigned char r, g, b;
            ALLEGRO_COLOR cor;
            cor = al_get_pixel(map, x, y);
            al_unmap_rgb(cor, &r, &g, &b);
            if (r == 255 && g == 0 && b == 255) bit = 1;
            else bit = 0;

            //Vai salvando a matriz no arquivo
            if(x != al_get_bitmap_width(map) - 1) fprintf(arq, "%d,", bit);
            else fprintf(arq, "%d", bit);
        }
        if (y != al_get_bitmap_height(map) -1) fprintf(arq, "},\n");
        else fprintf(arq, "}\n");
    }
    fprintf(arq, "};\n");
    fprintf(arq,"#endif\n");
    fclose(arq);

    return 0;
}







