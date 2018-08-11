#ifndef GUARD_H
#define GUARD_H

#include "AllegroCore.h"
#include "CharacterLibrary.h"
#include "MainMenuLibrary.h"

void playGame(int i,int conectados){
jogador[i].id = i;
int mortos = 0;
int vivo = 0;
int dead;
character person;
int lifecont = 0;
int zoom = 3;
int outrocont = 0;
for(int ka=0; ka < conectados; ka++) jogador[ka].lifeBar = 4, jogador[ka].imDead = 0;
    makeZoomedGame(zoom);
    int sair = 0, comando =0;
    double vel=zoom;
    //mostrar o tamanho do sprite do personagem
    if(i%2==0)//Guard
        declareFirstAtributes(i,15,20,LARGURA_MAPA/2 + 30+i*10, ALTURA_MAPA/2 + 30);
    else//Enemy
        declareFirstAtributes(i,16,24,   20, 1360);
    
    int wait,cont;

    jogador[i].bomb.cont=800;
    jogador[i].bomb.isOn=0,jogador[i].bomb.set = 0;
    int px, py;
    //onde ele vai comecar(centralizado na tela zoomificada);
    int whereIsX=LARGURA_TELA/2/zoom;
    int whereIsY=ALTURA_TELA/2/zoom ;
    //esses dois de baixo sao as posicoes finais dos personagens, de (0,0) a (larguramapa,alturamapa)
    jogador[i].x = (LARGURA_TELA/2+jogador[i].posX)/zoom;
    jogador[i].y = (ALTURA_TELA/2+jogador[i].posY)/zoom;
    int j;
    //Manda a struct do personagem para o server logo no início do game
    sendMsgToServer(&jogador[i], sizeof(character)); 

    for(int w=0;w<conectados;w++){
    	//Recebe a struct de cada jogador
        int resp = recvMsgFromServer(&person, DONT_WAIT);
        //Checa se existe mensagem
        if(resp != NO_MESSAGE){
        	//Checa se a struct não pertence ao próprio jogador para não haver conflitos
            if(jogador[i].id != person.id){
            	//Iguala a struct antiga com a recebida
                jogador[person.id] = person;
                for (int j = 0; j < 4; j++) {
                	//Atualiza as bombas no mapa
                    if (person.power[j].aindatem == 0) jogador[i].power[j].aindatem = 0;
                }
            }
        }
    }
    while(!sair){
        outrocont++;
        cont=0;
        wait=0;
        //checar qual eh o comando
        comando = checkEvent();
        jogador[i].comando=comando;


        while(wait!=10+comando && comando>0&&comando<10 && !sair){
            //enquanto nao soltar a tecla ele continua andando para frente
            wait = checkEvent();

            if(wait>0&&wait <10){
                comando = wait;
                jogador[i].comando = wait;
                wait = checkEvent();
            }
            
            cont++;

            //se ele checar a barreira e nao tiver barreira, ele continue esse if
            if(!checkIfBarrer(comando, vel, jogador[i].x, jogador[i].y,1)){
                //ai o personagem anda pra frente
                
                whichCommandPlayer(comando, vel, i);
                

                px = (-jogador[i].posX)/zoom;
                py = (- jogador[i].posY)/zoom;

                jogador[i].x = (LARGURA_TELA/2+jogador[i].posX)/zoom;
                jogador[i].y = (ALTURA_TELA/2+jogador[i].posY)/zoom;
            
                //e a tela acompanha seu movimento
                whereIs(map,&whereIsX, &whereIsY, comando, zoom, px, py, vel, i);


                
                
            }
            else{ //Se o personagem encontrar um bareira ele para de andar 
                whichCommandPlayer(comando, 0, i);

                whereIs(map,&whereIsX, &whereIsY, 0, zoom, px, py, vel, i);
            }
            //Manda a struct para o servidor logo apos checar se axiste barreir
            sendMsgToServer(&jogador[i], sizeof(character));

            for(int w=0;w<conectados;w++){
    			//Recebe a struct de cada jogador
        		int resp = recvMsgFromServer(&person, DONT_WAIT);
        		//Checa se existe mensagem
        		if(resp != NO_MESSAGE){
        			//Checa se a struct não pertence ao próprio jogador para não haver conflitos
            		if(jogador[i].id != person.id){
            			//Iguala a struct antiga com a recebida
                		jogador[person.id] = person;
                		for (int j = 0; j < 4; j++) {
                			//Atualiza as bombas no mapa
                    		if (person.power[j].aindatem == 0) jogador[i].power[j].aindatem = 0;
                		}
            		}
        		}
    		}
            
            //Verifica se existem outros players na visão do jogador
            isThereAnotherPlayer(conectados,i,whereIsX,whereIsY);

            if (i%2 == 0) makeAnimation(i,comando,&sair,cont,2); //Animação do Guard
            else makeAnimation(i,comando,&sair,cont,4); //Animação do Enemy
            
            for (int x = 0; x < conectados; x++) { 
                if(jogador[x].bomb.isOn) //Verifica se a bomba foi solta
                    drawBomb(i,whereIsX,whereIsY,conectados); //Solta a bomba
            }


            if(jogador[i].immunity>0) jogador[i].immunity--; //Deixa o player que foi atacado inalvejável durante certo tempo
            
            printPowerUps(i,whereIsX,whereIsY); //Mostra as bombas no mapa

            didIGotPowerUp(i,whereIsX,whereIsY); //Verifica se alguem pegou uma bomba

            didIGetHurt(i, conectados); //Verifica se alguem levou dano físico

            didIGetBurn(i); //Verifica se alguem foi queimado pela explosão da bomba
            
            lifecont++;
            if(lifecont==1800){ //Recupera a vida de todos os players que estiverem com a vida baixa (ocasionalmente)
                lifecont = 0;
                if(jogador[i].lifeBar < 4) jogador[i].lifeBar++;
            }
            
            if(jogador[i].bomb.cont<420)    jogador[i].bomb.cont+=4;
            if(jogador[i].bomb.cont==420) jogador[i].ihaveweapon=1,jogador[i].bomb.cont+=4,jogador[i].bomb.isOn=0;
            

            //MODIFIQUEI
            if(comando == 5 ){//Verifica se o player apertou espaço e começa o processo de ataque
                if (jogador[i].weapon == 0){ //Sem bomba
                    if(i%2==0) //Animação de ataque do Guard
                        drawSpriteGuardHit(i,whereIsX,whereIsY);
                    else //Animação de ataque do Enemy
                    	drawSpriteEnemyHit(i,whereIsX,whereIsY);

                }
                
                else if (jogador[i].weapon == 1 ){ //Com bomba
                    if(i%2==0)
                        drawSpriteGuard(i,whereIsX,whereIsY);
                    else 
                        drawSpriteEnemy(i,whereIsX,whereIsY);
                    //Estoura a bomba no local onde o player estiver
                    jogador[i].bomb.cont=0;
                    jogador[i].bomb.x = jogador[i].x, jogador[i].bomb.y = jogador[i].y;
                    jogador[i].bomb.isOn=1;
                    wait=comando+10;
                }
                
                
            }

            else{ //começa o processo de movimentação
                if(i%2==0)drawSpriteGuard(i,whereIsX,whereIsY); //Animação de movimento do Guard
                
                else drawSpriteEnemy(i,whereIsX,whereIsY); //Animação de movimento do Enemy
            }
            whereIs(foremap,&whereIsX, &whereIsY, 0, zoom, px, py, vel, i); //Localiza o player na matriz do mapa
            
            writeName(i,whereIsX,whereIsY); //Escreve o nome do player em cima de sua cabeça
            printLifeBar(i,whereIsX - 4,whereIsY + 24); //Printa a barra de vida 

            mortos = 0;
            vivo = 0;
            for(int k=0;k<conectados;k++){ //Comtabiliza a quantidade de players mortos e vivos
                if(i != k){ 
                    if(jogador[k].imDead) mortos++;
                    if(!jogador[k].imDead) vivo++;
                }
            }

            //Manda a struct do personagem avisando que ele está morto
            if(jogador[i].lifeBar<=0) sair=1, jogador[i].imDead = 1, sendMsgToServer(&jogador[i], sizeof(character));
            
            al_flip_display(); //Rola a tela
            if (mortos >= (conectados - 2) && vivo == 0) sair = 1;

        }
        //FIM DO WHILE
        if(comando == wait - 10|| comando == wait){

        	//Se a bomba estiver sido acionada é mandado uma mensagem pro server com a struct atualizada  
            if(jogador[i].bomb.isOn && comando == 0) sendMsgToServer(&jogador[i], sizeof(character));
            
            //Se a tab for precionado a arma é trocado para bomba ou virse versa
            if (comando == 6 && !jogador[i].bomb.isOn && jogador[i].ihaveweapon){
                if (jogador[i].weapon == 0) jogador[i].weapon = 1;
                else jogador[i].weapon = 0;
            }

            //Localiza o personagem na matriz do mapa
            whereIs(map,&whereIsX, &whereIsY, 0, zoom, px, py, vel, i);
            


            for(int w=0;w<conectados;w++){
    			//Recebe a struct de cada jogador
        		int resp = recvMsgFromServer(&person, DONT_WAIT);
        		//Checa se existe mensagem
        		if(resp != NO_MESSAGE){
        			//Checa se a struct não pertence ao próprio jogador para não haver conflitos
            		if(jogador[i].id != person.id){
            			//Iguala a struct antiga com a recebida
                		jogador[person.id] = person;
                		for (int j = 0; j < 4; j++) {
                			//Atualiza as bombas no mapa
                    		if (person.power[j].aindatem == 0) jogador[i].power[j].aindatem = 0;
                		}
            		}
        		}
    		}

            if(jogador[i].bomb.cont<420)  jogador[i].weapon=0,jogador[i].ihaveweapon=0,jogador[i].bomb.cont+=4;

            if(jogador[i].bomb.cont==420) jogador[i].ihaveweapon=1,jogador[i].bomb.cont+=4,jogador[i].bomb.isOn=0;

            if(jogador[i].immunity>0) jogador[i].immunity--; //Tira a imunidade do player

            printThings(i,whereIsX,whereIsY,conectados);

            lifecont++;
            if(lifecont==1800 && !jogador[i].imDead){ //Recupera a vida dos personagens que estiverem com a vida baixa (ocasionalmente)
                lifecont = 0;
                if(jogador[i].lifeBar < 4) jogador[i].lifeBar++;
            }

            mortos = 0;
            vivo = 0;
            for(int k=0;k<conectados;k++){ //Comtabiliza a quantidade de players mortos e vivos
                if(i != k){ 
                    if(jogador[k].imDead) mortos++;
                    if(!jogador[k].imDead) vivo++;
                }
            }

            whereIs(foremap,&whereIsX, &whereIsY, 0, zoom, px, py, vel, i); //Localiza o player na matriz do mapa

            didIGotPowerUp(i,whereIsX,whereIsY); //Verifica se alguma bomba foi pega

            didIGetHurt(i, conectados); //Verifica se alguem levou dano físico

            didIGetBurn(i); //Verifica se alguem foi queimado

            //Manda a struct do personagem avisando que ele está morto
            if(jogador[i].lifeBar<=0) sair=1, jogador[i].imDead = 1, sendMsgToServer(&jogador[i], sizeof(character));

            al_flip_display();
            if (mortos >= (conectados - 2) && vivo == 0) sair = 1;
        }

        

    }
    if(mortos >= (conectados - 2) && vivo == 0){ //Função que checa se o jogo terminou e printa na tela se o player ganhou
        while(dead != -1){
            dead = checkEvent(); 
            whereIs(map,&whereIsX, &whereIsY, 0, zoom, px, py, vel, i); //
            isThereAnotherPlayer(conectados,i,whereIsX,whereIsY);
            if(i%2==0)
                drawSpriteGuard(i,whereIsX,whereIsY);
            else
                drawSpriteEnemy(i,whereIsX,whereIsY);
            if(i%2 == 0){
                al_draw_text(fontesecundaria, jogador[i].cor, LARGURA_TELA/2/3, 250/3, ALLEGRO_ALIGN_CENTRE, "Congrats!");
                al_draw_text(fontesecundaria, jogador[i].cor, LARGURA_TELA/2/3, 350/3, ALLEGRO_ALIGN_CENTRE, "You Won!");
            }
            else{
                al_draw_text(fontesecundaria, jogador[i].cor, LARGURA_TELA/2/3, 250/3, ALLEGRO_ALIGN_CENTRE, "Goddamn...");
                al_draw_text(fontesecundaria, jogador[i].cor, LARGURA_TELA/2/3, 350/3, ALLEGRO_ALIGN_CENTRE, "You Won.");
            }

            al_flip_display();
        }
    }
    else {
        while(dead!=-1) {//Função que checa se o jogo terminou e printa na tela se o player perdeu
            dead = checkEvent(); //Checa o comando
            jogador[i].imDead = 1;
            if(dead!=-1)
            	//Mnada struct se o player pro server
                sendMsgToServer(&jogador[i], sizeof(character));
            for(int w=0;w<conectados;w++){
            	//Recebe a struct dos outros players 
                int resp = recvMsgFromServer(&person, DONT_WAIT);
                if(resp != NO_MESSAGE){
                    if(jogador[i].id != person.id)
                    	//Iguala a struct antiga a nova recebida
                        jogador[person.id] = person;
                }
            }

           whereIs(map,&whereIsX, &whereIsY, 0, zoom, px, py, vel, i); //Localiza o player na matriz do mapa

           drawDeadSprite(i,whereIsX,whereIsY); //Printa

           isThereAnotherPlayer(conectados,i,whereIsX,whereIsY); //Verifica se existe outro player por perto e printa na tela

           whereIs(foremap,&whereIsX, &whereIsY, 0, zoom, px, py, vel, i); //Localiza o player na matriz do mapa

           
            if(i%2 == 0) al_draw_text(fontesecundaria, jogador[i].cor, LARGURA_TELA/2/3, 250/3, ALLEGRO_ALIGN_CENTRE, "You Died...");
            else         al_draw_text(fontesecundaria, jogador[i].cor, LARGURA_TELA/2/3, 250/3, ALLEGRO_ALIGN_CENTRE, "Busted.");
            
           al_flip_display();

        }
    }

}


#endif
