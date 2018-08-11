#ifndef ENEMY_H
#define ENEMY_H

#include "AllegroCore.h"
#include "MainMenuLibrary.h"
#include "ClientLibrary.h"

//desenha o nome escolhido no inicio pelo usuario acima de seu respectivo personagem
void writeName(int i, int whereIsX,int whereIsY){
    al_draw_textf(fontepersonagem, jogador[i].cor, whereIsX+10, whereIsY-10, ALLEGRO_ALIGN_CENTRE, 
                              "%s", jogador[i].nome);
}
//checa se o jogador i recebeu dano de algum personagem diretamente
//caso sim, informa o servidor e dá um periodo de imunidade ao jogador ferido
 void didIGetHurt(int i, int conectados){
    for(int j=0; j<8; j++){
        if(jogador[j].comando == 5 && jogador[i].immunity<=0){
            if(jogador[j].set==2){
                if( jogador[j].y - jogador[i].y <= 22 &&
                    jogador[j].y - jogador[i].y >= 8 &&
                    jogador[j].x - jogador[i].x <= 13 &&
                    jogador[j].x - jogador[i].x >= -5)
                    jogador[i].lifeBar--;
                    jogador[i].immunity = 120, sendMsgToServer(&jogador[i], sizeof(character));
            }
            else if(jogador[j].set==0){
                if( jogador[j].y - jogador[i].y >= -30 &&
                    jogador[j].y - jogador[i].y <= -12 &&
                    jogador[i].x + jogador[i].largura - jogador[j].x >= 0  &&
                    jogador[i].x - jogador[j].x <=  13)
                    jogador[i].lifeBar--;
                    jogador[i].immunity = 120, sendMsgToServer(&jogador[i], sizeof(character));
            }
            else if(jogador[j].set==1){
                if( jogador[j].x - jogador[i].x <= 27 &&
                    jogador[j].x - jogador[i].x >=  8 &&
                    jogador[j].y - jogador[i].y <=  8 &&
                    jogador[j].y - jogador[i].y >= -8 )
                    jogador[i].lifeBar--;
                    jogador[i].immunity = 120, sendMsgToServer(&jogador[i], sizeof(character));
            }
            else if(jogador[j].set==3){
                if( jogador[j].x - jogador[i].x >= -27 &&
                    jogador[j].x - jogador[i].x <= -8 &&
                    jogador[j].y - jogador[i].y <=  8 &&
                    jogador[j].y - jogador[i].y >= -8 )
                    jogador[i].lifeBar--;
                    jogador[i].immunity = 120, sendMsgToServer(&jogador[i], sizeof(character));
            }
        }
    }
}
//mostra a barra de vida do jogador abaixo do seu personagem
 void printLifeBar(int i,int posX, int posY){
    if(jogador[i].lifeBar == 4) al_draw_bitmap_region(spriteLifeBar,
             0,0,
             23, 6   ,
             posX,  posY,0);
    if(jogador[i].lifeBar == 3) al_draw_bitmap_region(spriteLifeBar,
             22,0,
             23, 6 ,
             posX,  posY,0);
    if(jogador[i].lifeBar == 2) al_draw_bitmap_region(spriteLifeBar,
             45,0,
             22, 6   ,
             posX,  posY,0);
    if(jogador[i].lifeBar == 1) al_draw_bitmap_region(spriteLifeBar,
             67,0,
             23, 6   ,
             posX,  posY,0);
    if(jogador[i].lifeBar == 0) al_draw_bitmap_region(spriteLifeBar,
             67,0,
             23, 6   ,
             posX,  posY,0);
 }
//inicializa as variaveis da estrutura do jogador
void declareFirstAtributes(int i,int largura, int altura,int x, int y){
    jogador[i].largura = largura;
    jogador[i].altura  = altura;
    jogador[i].posX = x;
    jogador[i].posY = y;
    jogador[i].weapon = 0,jogador[i].ihaveweapon=0;
    jogador[i].immunity = 0;
    jogador[i].act=0,jogador[i].set=0;
    jogador[i].lifeBar = 4;
}
//desenha o guard dependo do seu set (que é para onde ele está virado)
void drawSpriteGuard(int i, double posX,double posY){
    if(jogador[i].set==2)
        al_draw_bitmap_region(spriteGuard[jogador[i].color],
            (jogador[i].largura+1)*jogador[i].act, jogador[i].altura+2,
             jogador[i].largura       ,    jogador[i].altura          ,
             posX                      ,    posY                      ,0);
    if(jogador[i].set==0)
        al_draw_bitmap_region(spriteGuard[jogador[i].color],
            (jogador[i].largura+1)*jogador[i].act,    0      ,
             jogador[i].largura       ,    jogador[i].altura ,
             posX                     ,    posY              ,0);
    if(jogador[i].set==1)
        al_draw_bitmap_region(spriteGuard[jogador[i].color],
            (jogador[i].largura+1)*(2+jogador[i].act)+jogador[i].act*1, 0,
             jogador[i].largura                 ,   jogador[i].altura    ,
             posX                               ,   posY                 ,0);
    if(jogador[i].set==3)
        al_draw_bitmap_region(spriteGuard[jogador[i].color],
             66 + jogador[i].largura*jogador[i].act,    0           ,
             jogador[i].largura         ,    jogador[i].altura      ,
             posX                       ,    posY                   ,0);
}
//printa as bombas que ainda não foram pegas por outros jogadores
void printPowerUps(int p,int whereIsX,int whereIsY){
    for(int i = 0; i < 4; i ++) if(jogador[p].power[i].aindatem!=0){
        
        al_draw_bitmap_region(spriteBomb,
             0 ,   0                      ,
             16,  16                      ,
             jogador[p].power[i].x - jogador[p].x + whereIsX,jogador[p].power[i].y - jogador[p].y + whereIsY
                                          ,0);
    }
}
//checa se o jogador p pegou uma bomba
//caso sim essa bomba não estará mais disponivel e o jogador p não poderá pegar mais bombas
void didIGotPowerUp(int p, int whereIsX, int whereIsY){
    for(int i = 0; i < 4; i++){
        if( jogador[p].x+7 >=jogador[p].power[i].x   &&
            jogador[p].x+7 <=jogador[p].power[i].x+16&&
            jogador[p].y+17>=jogador[p].power[i].y   &&
            jogador[p].y+17<=jogador[p].power[i].y+16&&
            jogador[p].power[i].aindatem    == 1     &&
            jogador[p].ihaveweapon == 0     )
                jogador[p].power[i].aindatem=0,jogador[p].ihaveweapon=1;
    }
}
//desenha o guard dando seu ataque, de novo, dependendo de seu sentido
void drawSpriteGuardHit(int i,int posX,int posY){
    if(jogador[i].set==2)//p cima
        al_draw_bitmap_region(spriteGuardHit[jogador[i].color],
             34 + 16*jogador[i].act         ,    1     ,
             15                     ,    24    ,
             posX                   ,    posY - 4,0);
    if(jogador[i].set==0)//p baixo
        al_draw_bitmap_region(spriteGuardHit[jogador[i].color],
             15*jogador[i].act              ,    5     ,
             15                     ,    32    ,
             posX -jogador[i].act              ,    posY  ,0);
    if(jogador[i].set==1)//p esquerda
        al_draw_bitmap_region(spriteGuardHit[jogador[i].color],
             25*jogador[i].act                  ,   61     ,
             25                     ,   19     ,
             posX - 9 - 6*jogador[i].act       ,   posY   ,0);
    if(jogador[i].set==3)//p direita
        al_draw_bitmap_region(spriteGuardHit[jogador[i].color],
             2 + 28*jogador[i].act          ,   41     ,
             26                     ,   19     ,
             posX +3*jogador[i].act            ,   posY+1 ,0);
}
//desenha o enemy
void drawSpriteEnemy(int i, double posX,double posY){
    al_draw_bitmap_region(spriteEnemy[jogador[i].color],
        (jogador[i].largura+1)*jogador[i].act,   (jogador[i].altura+1)*jogador[i].set,
         jogador[i].largura                  ,    jogador[i].altura                  ,
         posX                                ,    posY                               ,0);
}
//desenha o ataque do enemy, dependendo de seu sentido
void drawSpriteEnemyHit(int i,int posX,int posY){  
    if(jogador[i].color == 0 || jogador[i].color == 1){
        if(jogador[i].set==2){//p cima
        	if(jogador[i].act == 0)
        		al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
        	     0                      ,    64    ,
        	     26                     ,    31    ,
        	     posX -11               ,    posY+3,0);

        	if(jogador[i].act == 1)
        		al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
        	     26                     ,    69    ,
        	     25                     ,    28    ,
        	     posX -8                ,    posY-4,0);

        	if(jogador[i].act == 2 || jogador[i].act == 3)
        		al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
        	     60                     ,    64    ,
        	     21                     ,    25    ,
        	     posX - 6               ,    posY-3,0);
    	}
    	if(jogador[i].set==0){//p baixo
    	    if(jogador[i].act ==0 )
    	    	al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
    	         61                     ,    99    ,
    	         20                     ,    23    ,
    	         posX - 5               ,  posY - 2,0);

    	    if(jogador[i].act == 1)
    	    	al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
    	         25                     ,    100   ,
    	         25                     ,    26    ,
    	         posX - 10              ,    posY-3,0);

    	    if(jogador[i].act == 2 || jogador[i].act == 3)
    	   		al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
    	         0                      ,    95    ,
    	         25                     ,    32    ,
    	         posX   - 10            ,    posY  ,0);
    	}
	}
    else{
        if(jogador[i].set==2){//p cima
        	if(jogador[i].act ==0 )
        		al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
        	     4                      ,    64    ,
        	     26                     ,    31    ,
        	     posX -11               ,    posY+3,0);

        	if(jogador[i].act == 1)
        		al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
        	     30                     ,    70    ,
        	     25                     ,    28    ,
        	     posX -8                ,    posY-4,0);

        if(jogador[i].color == 3)
            if(jogador[i].act == 2 || jogador[i].act == 3)
            	al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
                 63                     ,    68    ,
                 21                     ,    25    ,
                 posX - 6               ,    posY-3,0);
        if(jogador[i].color == 2)
            if(jogador[i].act == 2 || jogador[i].act == 3)
            al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
                 63                     ,    63    ,
                 21                     ,    25    ,
                 posX - 6               ,    posY-3,0);
    }
    if(jogador[i].set==0){//p baixo

        if(jogador[i].act == 0 )
        al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
             61                     ,    94    ,
             20                     ,    23    ,
             posX - 5               ,  posY - 2,0);
        if(jogador[i].color == 3)
            if(jogador[i].act == 1)
            al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
                 27                     ,    102   ,
                 25                     ,    26    ,
                 posX - 10              ,    posY-3,0);

        if(jogador[i].color == 2)
            if(jogador[i].act == 1)
            al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
                 27                     ,    96   ,
                 25                     ,    26    ,
                 posX - 10              ,    posY-3,0);

        if(jogador[i].act == 2 || jogador[i].act == 3)
        al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
             0                      ,    94    ,
             25                     ,    32    ,
             posX   - 10            ,    posY  ,0);
    }
	}

    if(jogador[i].set==1){//p esquerda

        if(jogador[i].act ==0 )
        al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
             57                     ,    1     ,
             25                     ,    32    ,
             posX -1                ,    posY - 8,0);

        if(jogador[i].act == 1)
        al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
             33                     ,    0     ,
             18                     ,    33    ,
             posX -1                ,    posY - 9,0);

        if(jogador[i].act == 2 || jogador[i].act == 3)
        al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
             0                      ,    3     ,
             33                     ,    29    ,
             posX   -17             ,    posY - 5,0);
    }
    if(jogador[i].set==3){//p direita

        if(jogador[i].act ==0 )
        al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
             83                     ,    33    ,
             25                     ,    32    ,
             posX - 9               ,  posY - 8,0);

        if(jogador[i].act == 1)
        al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
             114                    ,    96    ,
             18                     ,    32    ,
             posX +1                ,    posY - 9,0);

        if(jogador[i].act == 2 || jogador[i].act == 3)
        al_draw_bitmap_region(spriteEnemyHit[jogador[i].color],
             132                    ,    35    ,
             33                     ,    29    ,
             posX                   ,    posY - 5,0);
    }
}
//desenha as animações da bomba explodindo
void drawBomb(int i,int whereIsX, int whereIsY, int conectados){
    int bomba = 1;
    for (int x = 0; x < conectados; x++) {
        if(jogador[x].bomb.cont==300) jogador[x].bomb.set = -1;
        if(jogador[x].bomb.cont<300){
            if (jogador[x].bomb.cont%15==0) jogador[x].bomb.set++;
            if(jogador[x].bomb.set>=4) jogador[x].bomb.set=0;
            al_draw_bitmap_region(spriteBomb,
                 17*jogador[x].bomb.set , 0,
                 16             ,16,
                 jogador[x].bomb.x - jogador[i].x + whereIsX, jogador[x].bomb.y -jogador[i].y + whereIsY, 0);
        }
        else if(jogador[x].bomb.cont<=420){
            if((jogador[x].bomb.cont-300)%24 == 0) jogador[x].bomb.set++;
            if(jogador[x].bomb.set>=5) jogador[i].bomb.set=0;
            al_draw_bitmap_region(spriteExplosion,
                 80*jogador[x].bomb.set   , 0,
                 80   , 80        ,
                 -32 + jogador[x].bomb.x - jogador[i].x + whereIsX, -32 + jogador[x].bomb.y -jogador[i].y + whereIsY, 0);
        }
    }
}
//checa se o jogador j está dentro o sprite bomba, mas fora da área de dano
//caso não, ele recebe o dano
void didIGetBurn(int j){
    int i;
    for(i=0; i<8; i++){
        if(jogador[i].bomb.isOn&&jogador[i].bomb.cont>=300 && jogador[j].immunity <= 0){
            if( jogador[j].y - jogador[i].bomb.y >= -32 &&  //checa se esta dentro de todo sprite bomba
                jogador[j].y - jogador[i].bomb.y <=  40 &&
                jogador[j].x - jogador[i].bomb.x >= -32 &&
                jogador[j].x - jogador[i].bomb.x <=  48){
                if( jogador[j].x + jogador[j].largura - jogador[i].bomb.x <= 4      && //checa o primeiro quadrante
                    jogador[j].y + jogador[j].altura - jogador[i].bomb.y <= 4       )
                    break;
                else if( jogador[j].x - jogador[i].bomb.x >= 16                      && //segundo quadrante
                         jogador[j].y + jogador[j].altura - jogador[i].bomb.y <= 4   )
                    break;
                else if( jogador[j].x + jogador[j].largura - jogador[i].bomb.x < 0  && //terceiro quadrante
                         jogador[j].y - jogador[i].bomb.y > 16                      )
                    break;
                else if( jogador[j].x - jogador[i].bomb.x > 16                      && // quarto quadrante
                         jogador[j].y - jogador[i].bomb.y > 16                      )
                    break;
                
                else jogador[j].lifeBar--,jogador[j].immunity = 120, sendMsgToServer(&jogador[i], sizeof(character));
            }        
        }
    }
}
//Esse comando vai checar  qual foi o comando que recebeu do teclado
void whichCommandPlayer(int comando,int vel,int i){
    //1=cima,2=baixo,3=esquerda,4=direita
    //jogador[i].set é qual jogador[i].set de sprites eu devo usar
    if(comando==1)
        jogador[i].posY-=vel,jogador[i].set=2;

    if(comando==2)
        jogador[i].posY+=vel,jogador[i].set=0;
    
    if(comando==3)
        jogador[i].posX-=vel,jogador[i].set=1;
    
    if(comando==4)
        jogador[i].posX+=vel,jogador[i].set=3;
}

int checkIfBarrer(int comando,int vel,int x,int y,int type){
    int i,j,a,b,c;
    if(type==0) a=x+10,b=y+20,c=y+17;
    if(type==1) a=x+7,b=y+15,c=y+8;

            for(i=vel;i>1;i--)
                for(j=vel;j>1;j--){

            if(comando==1&&matriz[b-j][a]==1) { 
                jogador[i].set=2;return 1;
            }
            
            if(comando==2&&matriz[b+j][a]==1) { 
                jogador[i].set=0;return 1;
            }

            if(comando==3&&matriz[b][a-i]==1) { 
                jogador[i].set=1;return 1;
            }
            
            if(comando==4&&matriz[b][a+i]==1) { 
                jogador[i].set=3;return 1;
            }

        }
    return 0;
}

//esse é para checar qual vai ser a animacao, totalmente de fundo estetico
void makeAnimation(int i,int comando, int *sair, int cont,int type){
    if(comando!=0&&cont%8==0) jogador[i].act++;
    
    if(jogador[i].act==type)      jogador[i].act = 0;

    if(comando==9||comando==-1) *sair = 1;
}
//funcao que checa onde esta o personagem. ele normalmente estaria centralizado na tela,
//mas se chegar nas pontas ou lados, antes ele acompanharia e ficava uma parte toda bugada.
//entao serve para checar onde vai estar o personagem na tela.
// se ele chegar nos limites, a tela para e o personagem continua
void whereIs(ALLEGRO_BITMAP *mapa ,int *whereIsX, int *whereIsY,int comando, int zoom, int px, int py,int vel,int i){
    if(px>=0 && py>=0){
        al_draw_bitmap(mapa, 0, 0, 0);
        if(comando==1)
            *whereIsY-=vel/zoom;
        if(comando==2)
            *whereIsY+=vel/zoom;
    
        if(comando==3)
            *whereIsX-=vel/zoom;
    
        if(comando==4)
            *whereIsX+=vel/zoom;
    }

    else if(px<=-LARGURA_MAPA+LARGURA_TELA/zoom && py>=0){
        al_draw_bitmap(mapa, -LARGURA_MAPA+LARGURA_TELA/zoom, 0, 0);
        if(comando==1)
            *whereIsY-=vel/zoom;
        if(comando==2)
            *whereIsY+=vel/zoom;
    
        if(comando==3)
            *whereIsX-=vel/zoom;
    
        if(comando==4)
            *whereIsX+=vel/zoom;
    
    }


    else if(px>=0 && py<=-ALTURA_MAPA+ALTURA_TELA/zoom){
        al_draw_bitmap(mapa, 0, -ALTURA_MAPA+ALTURA_TELA/zoom, 0);
        if(comando==1)
            *whereIsY-=vel/zoom;
        if(comando==2)
            *whereIsY+=vel/zoom;
    
        if(comando==3)
            *whereIsX-=vel/zoom;
    
        if(comando==4)
            *whereIsX+=vel/zoom;
    
    }

    else if(px<=-LARGURA_MAPA+LARGURA_TELA/zoom &&
            py<=-ALTURA_MAPA +ALTURA_TELA/zoom){
        al_draw_bitmap(mapa,-LARGURA_MAPA+LARGURA_TELA/zoom,
                           -ALTURA_MAPA+ALTURA_TELA/zoom, 0);
        if(comando==1)
            *whereIsY-=vel/zoom;
        if(comando==2)
            *whereIsY+=vel/zoom;
    
        if(comando==3)
            *whereIsX-=vel/zoom;
    
        if(comando==4)
            *whereIsX+=vel/zoom;
    
    }

    else if(px>=0 && py<0){
        al_draw_bitmap(mapa, 0, py, 0);

        if(comando==3)
            *whereIsX-=vel/zoom;
    
        if(comando==4)
            *whereIsX+=vel/zoom;
    }

    

    else if(px<0 && py>=0){
        al_draw_bitmap(mapa, px, 0, 0);
        if(comando==1)
            *whereIsY-=vel/zoom;
        if(comando==2)
            *whereIsY+=vel/zoom;
    
    }

    else if(px>-LARGURA_MAPA+LARGURA_TELA/zoom && 
            py<=-ALTURA_MAPA+ALTURA_TELA/zoom){
        al_draw_bitmap(mapa, px, -ALTURA_MAPA+ALTURA_TELA/zoom, 0);
        if(comando==1)
            *whereIsY-=vel/zoom;
        if(comando==2)
            *whereIsY+=vel/zoom;

    }

    else if(px<=-LARGURA_MAPA+LARGURA_TELA/zoom &&
            py>-ALTURA_MAPA+ALTURA_TELA/zoom){
        al_draw_bitmap(mapa, -LARGURA_MAPA+LARGURA_TELA/zoom, py, 0);
    
        if(comando==3)
            *whereIsX-=vel/zoom;
    
        if(comando==4)
            *whereIsX+=vel/zoom;
    }

    else
        al_draw_bitmap(mapa, px, py, 0);
}
//desenha o enemy morto
void drawDeadSprite(int i,int posX,int posY){
    if(i%2==0){
int set;
    if(jogador[i].color == 0) set = 3;
if(jogador[i].color == 1) set = 2;
    if(jogador[i].color == 2) set = 1;
    if(jogador[i].color == 3) set = 0;
        al_draw_bitmap_region(guardDead,
        set*17,   0,
        16  , 16,
        posX    ,    posY,0);
}
    else
al_draw_bitmap_region(enemyDead,
         69 + jogador[i].color*17,   0,
         16  , 16,
         posX    ,    posY,0);

}

//mostra os personagens diferentes do seu e suas respectivas ações em sua tela
void isThereAnotherPlayer(int conectados,int i,int whereIsX, int whereIsY){
    for(int j=0;j<conectados;j++){
	if(i!=j)
        if(!jogador[j].imDead){
            if(j%2==0){
                if(jogador[j].comando!=5)
                    drawSpriteGuard(j,jogador[j].x - jogador[i].x + whereIsX,jogador[j].y - jogador[i].y + whereIsY);
                if(jogador[j].comando==5)
                    drawSpriteGuardHit(j,jogador[j].x - jogador[i].x + whereIsX,jogador[j].y - jogador[i].y + whereIsY);
            }
            else{
                if(jogador[j].comando!=5)
                    drawSpriteEnemy(j,jogador[j].x - jogador[i].x + whereIsX,jogador[j].y - jogador[i].y + whereIsY);
                if(jogador[j].comando==5)
                drawSpriteEnemyHit(j,jogador[j].x-jogador[i].x +whereIsX,jogador[j].y-jogador[i].y +whereIsX);
            }

        writeName   (j,jogador[j].x - jogador[i].x + whereIsX,jogador[j].y - jogador[i].y + whereIsY);
        printLifeBar(j,jogador[j].x - jogador[i].x + whereIsX - 4 ,jogador[j].y - jogador[i].y + whereIsY + 24);
        }
        else drawDeadSprite(j,jogador[j].x - jogador[i].x + whereIsX + 10,jogador[j].y - jogador[i].y + whereIsY + 10);
    }
}

//mostra na tela todos os desenhos possiveis, dos guards, enemys, seus respectivos nomes e barras de vida além das bombas
void printThings(int i,int whereIsX,int whereIsY,int conectados){
            
    isThereAnotherPlayer(conectados,i,whereIsX,whereIsY);

    if(i%2==0)drawSpriteGuard(i,whereIsX,whereIsY);

       else drawSpriteEnemy(i,whereIsX,whereIsY);
       for (int x = 0; x < conectados; x++) {
          if(jogador[x].bomb.isOn)
                drawBomb(i,whereIsX,whereIsY,conectados);
          }


    printPowerUps(i,whereIsX,whereIsY);
    
    writeName(i,whereIsX,whereIsY);
    printLifeBar(i,whereIsX - 4,whereIsY + 24);
}



#endif
