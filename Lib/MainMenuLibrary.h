#ifndef MAINMENU_H
#define MAINMENU_H

#include "AllegroCore.h"
#include "CharacterLibrary.h"
// essa funcao esta indicando a cor do mersonagem (amarelo, verde, vermelho ou azul)
// a cor do personagem sera escolhida pelo jogador
void corPersonagem(int i){
    if(jogador[i].color == 0) jogador[i].cor = al_map_rgb(0,255,0);
    if(jogador[i].color == 1) jogador[i].cor = al_map_rgb(0,0,255);
    if(jogador[i].color == 2) jogador[i].cor = al_map_rgb(255,0,0);
    if(jogador[i].color == 3) jogador[i].cor = al_map_rgb(255,255,0);
}

void putMainBackground(int dx,int dy){

            al_draw_bitmap(mainmap, dx, dy, 0);
            // Dizemos que vamos tratar os eventos vindos do mouse
            al_draw_text(fontetitulo, al_map_rgb(50, 50, 50), LARGURA_TELA/2, 70, ALLEGRO_ALIGN_CENTRE, "Citadel");
}
// essa funcao ajusta a cor e o tamanho das frases que vao aparecer no main menu:
// start game, instructions, options e exit
void destacar(int corA,int corB,int corC,int corD){

    al_draw_text(fontesecundaria, al_map_rgb(corA, corA, corA), LARGURA_TELA/2, 250, ALLEGRO_ALIGN_CENTRE, "Start Game");
    al_draw_text(fontesecundaria, al_map_rgb(corB, corB, corB), LARGURA_TELA/2, 350, ALLEGRO_ALIGN_CENTRE, "Instructions");
    al_draw_text(fontesecundaria, al_map_rgb(corC, corC, corC), LARGURA_TELA/2, 450, ALLEGRO_ALIGN_CENTRE, "Options");
    al_draw_text(fontesecundaria, al_map_rgb(corD, corD, corD), LARGURA_TELA/2, 550, ALLEGRO_ALIGN_CENTRE, "Exit");  
}
// essa funcao ajusta a cor e o tamanho das frases que vao aparecer quando o personagem selecionar instructions no main menu
// as instructions irao explicar um pouco dos movimentos do jogo
//as teclas do jogo sao as seguintes:
//w,a,s,d ou cima baixo direita esquerda para mover
// espaco ou enter para ativar uma acao(bomba)
//mudar de arma: tab
// alem disso existe uma tecla que volta para o menu principal(return)
void instructions(){
        al_draw_text(fontesecundaria, al_map_rgb(20, 20, 20), LARGURA_TELA/2, 250, ALLEGRO_ALIGN_CENTRE, "Movement: [W A S D] or [UP LEFT DOWN RIGHT]");
        al_draw_text(fontesecundaria, al_map_rgb(20, 20, 20), LARGURA_TELA/2, 350, ALLEGRO_ALIGN_CENTRE, "Action: [space] or [ENTER]");
        al_draw_text(fontesecundaria, al_map_rgb(20, 20, 20), LARGURA_TELA/2, 450, ALLEGRO_ALIGN_CENTRE, "Change weapon: [TAB]");
        al_draw_text(fontesecundaria, al_map_rgb(150, 150, 150), LARGURA_TELA/2, 650, ALLEGRO_ALIGN_CENTRE, "Return");  
}

//essa funcao checa os eventos(toques no teclado) do main menu
void checarEventoMain(int *command,int *i,int *fechar){
    ALLEGRO_EVENT evento;
    ALLEGRO_TIMEOUT timeout;
    al_init_timeout(&timeout, 0.01);
    int tem_eventos = al_wait_for_event_until(fila_eventos, &evento, &timeout);
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
        //salva no ponteiro comand qual foi a tecla q foi apertada
        *command = qualComando(evento.keyboard.keycode);
    // dependendo da tecla apertada, existe uma consequencia diferente
    if(*command==1||*command==2)
        keyConsequece(*command,&*i);
    if(*command==5) *fechar=1;
    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        *fechar = 1;
    }
}

void mainInstructions(int dx, int dy){
    int exit = 0,command=0;
    while(!exit){
        putMainBackground(dx,dy);
        instructions();
        al_flip_display();
        command = checkEvent5();
        if(command==5) exit=1;
    }
}
// essa funcao da as caracteristicas da tela options na qual ajusta o som da musica do jogo
void mainOptions(int *volume,int dx, int dy){
    int command=0,i=0,exit=0,cor[]={150,30};


    while(!exit){
        putMainBackground(dx,dy);
        command = checkEvent5();
        
        cor[i]=30;
        if(command==1)  i--;    if(command==2)  i++;
        if(i<0)         i=1;    if(i>1)         i=0;

        if(i==0 && command==3)  if(*volume>0)  *volume=*volume-1;
        if(i==0 && command==4)  if(*volume<5)  *volume=*volume+1;
        if(*volume==0) al_draw_text(fontemusica,al_map_rgb(cor[0], cor[0], cor[0]), LARGURA_TELA/2, 450, ALLEGRO_ALIGN_CENTRE, "x x x x x");
        if(*volume==1) al_draw_text(fontemusica,al_map_rgb(cor[0], cor[0], cor[0]), LARGURA_TELA/2, 450, ALLEGRO_ALIGN_CENTRE, "n x x x x");
        if(*volume==2) al_draw_text(fontemusica,al_map_rgb(cor[0], cor[0], cor[0]), LARGURA_TELA/2, 450, ALLEGRO_ALIGN_CENTRE, "nn x x x");
        if(*volume==3) al_draw_text(fontemusica,al_map_rgb(cor[0], cor[0], cor[0]), LARGURA_TELA/2, 450, ALLEGRO_ALIGN_CENTRE, "nnn x x");
        if(*volume==4) al_draw_text(fontemusica,al_map_rgb(cor[0], cor[0], cor[0]), LARGURA_TELA/2, 450, ALLEGRO_ALIGN_CENTRE, "nnnn x");
        if(*volume==5) al_draw_text(fontemusica,al_map_rgb(cor[0], cor[0], cor[0]), LARGURA_TELA/2, 450, ALLEGRO_ALIGN_CENTRE, "nnnnn");
        
        cor[i]=150;
        al_draw_text(fontesecundaria, al_map_rgb(cor[0], cor[0], cor[0]), LARGURA_TELA/2, 350, ALLEGRO_ALIGN_CENTRE, "Change Volume");
        al_draw_text(fontesecundaria, al_map_rgb(cor[1], cor[1], cor[1]), LARGURA_TELA/2, 650, ALLEGRO_ALIGN_CENTRE, "Return");
        al_flip_display();
        if(command==5 && i==1) exit=1;
    }

}    
// essa funcao eh utilizada para pedir o nome do jogador
void writeYourNameScreen(int dx,int dy){
    int finished=0,exit=0;
    strcpy(name, "");
    while (!finished){
        //plano de fundo
        putMainBackground(dx,dy);
        while (!al_is_event_queue_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
            if (!finished){
                readNameString(evento);
                if (evento.type == ALLEGRO_EVENT_KEY_DOWN       &&
                    evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    finished=1;
            }
        }
        
        al_draw_text(fontesecundaria, al_map_rgb(30,30,30), LARGURA_TELA / 2,300,
                     ALLEGRO_ALIGN_CENTRE, "Write your Name:");
        if (strlen(name) > 0)
            // ele vai mostrar na tela o nome da pessoa enquanto ela escreve no teclado
            al_draw_text(fontesecundaria, al_map_rgb(30,30,30), LARGURA_TELA / 2,450,
                         ALLEGRO_ALIGN_CENTRE, name);
 
        al_flip_display();
    }

}
//funcao que ler o IP do jogador
void readIPString(ALLEGRO_EVENT evento){
    
    if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
        // tamanho da ip deve ser menor que 39
        if (strlen(ip) < 39){
            char temp[] = {evento.keyboard.unichar, '\0'};
            // checa se alguma tecla foi pressionada
            if (evento.keyboard.unichar == ' '
                
                ||
                
                (evento.keyboard.unichar >= '0'     &&
                 evento.keyboard.unichar <= '9')
                ||
                
                (evento.keyboard.unichar >= 'A'     &&
                 evento.keyboard.unichar<= 'Z')
                ||
                
                (evento.keyboard.unichar >= 'a'     &&
                 evento.keyboard.unichar <= 'z')
                ||
                 evento.keyboard.unichar == '.'		)	
                strcat(ip, temp);
        }
 
        if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(ip) != 0)
           // conta o tamanho do ip
            ip[strlen(ip) - 1] = '\0';
    }  
}


void writeYourIPScreen(int dx,int dy){
    //mostrar na tela as teclas do ip que foram pressionadas
    
    int finished=0,exit=0;
    strcpy(ip, "");
    // enquanto finish==0
    while (!finished){
        putMainBackground(dx,dy);
        while (!al_is_event_queue_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
            if (!finished){
                readIPString(evento);
                // o computador so vai parar de ler as teclas quando o usuario apertar e soltar enter
                if (evento.type == ALLEGRO_EVENT_KEY_DOWN       &&
                    evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    finished=1;
                // sai do while
            }
        }
        al_draw_text(fontesecundaria, al_map_rgb(30,30,30), LARGURA_TELA / 2,300,
                     ALLEGRO_ALIGN_CENTRE, "Write your IP:");
        if (strlen(ip) > 0)
            al_draw_text(fontesecundaria, al_map_rgb(30,30,30), LARGURA_TELA / 2,450,
                         ALLEGRO_ALIGN_CENTRE, ip);
 
        al_flip_display();
    }

}

void selectColor(int player,int corA,int corB,int corC,int corD){
    char tipo[10];
    if(player %2 == 0) strcpy(tipo, "Guard");
    else strcpy(tipo, "Thief");
    al_draw_textf(fontesecundaria, al_map_rgb(20, 20, 20), LARGURA_TELA/2, 250, ALLEGRO_ALIGN_CENTRE, "You are a %s!",tipo);
    al_draw_textf(fontesecundaria, al_map_rgb(20, 20, 20), LARGURA_TELA/2, 350, ALLEGRO_ALIGN_CENTRE, "Choose a color:");
    al_draw_text(fontesecundaria, al_map_rgb(corA, corA, corA), LARGURA_TELA/2, 450, ALLEGRO_ALIGN_CENTRE, "Green");
    al_draw_text(fontesecundaria, al_map_rgb(corB, corB, corB), LARGURA_TELA/2, 510, ALLEGRO_ALIGN_CENTRE, "Blue");
    al_draw_text(fontesecundaria, al_map_rgb(corC, corC, corC), LARGURA_TELA/2, 570, ALLEGRO_ALIGN_CENTRE, "Red");
    al_draw_text(fontesecundaria, al_map_rgb(corD, corD, corD), LARGURA_TELA/2, 630, ALLEGRO_ALIGN_CENTRE, "Yellow");  
}
//essa funcao diz se voce é um guarda ou um ladrao
//e seleciona a cor escolhida:
//verde,azul,vermelho ou amarelo

void selectYourColorScreen(int player,int dx, int dy){
    int fechar = 0;
    int i=0;
    int corA = 30,corB = 30, corC = 30, corD = 40;
    int comando = 0;
    while(!fechar){
    	//entra no while
        putMainBackground(dx, dy);
        selectColor(player, corA, corB, corC, corD);
        checarEventoMain(&comando,&i,&fechar);
        if(i==0) corA = 150, corB = 30, corC = 30, corD = 30;
        if(i==1) corA =  30, corB =150, corC = 30, corD = 30;
        if(i==2) corA =  30, corB = 30, corC =150, corD = 30;
        if(i==3) corA =  30, corB = 30, corC = 30, corD =150;
        al_flip_display();
    }
    jogador[player].color = i;
  //mostra a tela com as opcoes de cores
}


void waitForServerScreen(int dx,int dy){
    int finished = 0,comando;
    int cont = 0,act;
    while(!finished){
        comando = checkEvent();
        putMainBackground(dx,dy);
        al_draw_text(fontesecundaria, al_map_rgb(30,30,30), LARGURA_TELA / 2,300,
                     ALLEGRO_ALIGN_CENTRE, "Wait for all players to join");
        //entra no while
        //mostra a tela de espera até todos os jogadores entrarem
        cont++;
        act++;
        if(act==2) act = 0;
        for(int i = 1; i < 8; i+=2){
                al_draw_bitmap_region(spriteEnemy[p[i]],
                17 * act,   25 * 3,
                16      ,    24   ,
                cont + i*16 + 2   ,  650, 0);
        }

        
        for(int i = 0; i < 8; i+=2){
            al_draw_bitmap_region(spriteGuard[p[i]],
                    16 * (2 + act) + act * 1,     0,
                    16                      ,    20,
                    (cont + 7*16+ i*15 + 2) ,   650, 0);

        }
        if(cont+7*15+2 == LARGURA_TELA) cont = 0;
        al_flip_display();
        if(comando == -1) finished=1;
        //mostra para os jogadores que sao guardas e para os ladroes telas diferentes
    }
}

int preparationForGame(int *player,int xmain,int ymain,int volume){
            writeYourIPScreen(xmain,ymain);
            waitForServerScreen(xmain,ymain);
        //essa funçao manda o ip para o server e pega o nome digitado do jogador    
            writeYourNameScreen(xmain,ymain);
            al_stop_samples();
            al_play_sample(sample2, volume/5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP,NULL);
            strcpy(jogador[*player].nome,name);
            corPersonagem(*player);
    return 1;
}

int mainMenu(int *dx,int *dy,int *where){

    int i=0;
    int cor[]={150,30,30,30};
    int fechar=0;
    int command=0;

    while(!fechar){
        while(*dx>LARGURA_TELA-LARGURA_MAPA&&!fechar&&*where==0){
            putMainBackground(*dx,*dy);
            //mostra o background
            destacar(cor[0],cor[1],cor[2],cor[3]);       
            
            cor[i]=30;
            checarEventoMain(&command, &i, &fechar);
            cor[i]=150;
            
            al_flip_display();
            *dx=*dx-1;
            //essa funcao mostra a tela do main menu
        }
        if(*dx<=LARGURA_TELA-LARGURA_MAPA) *where=1;
        while(*dy>ALTURA_TELA-ALTURA_MAPA&&!fechar&&*where==1){
            putMainBackground(*dx,*dy);

            destacar(cor[0],cor[1],cor[2],cor[3]);

            cor[i]=30;
            checarEventoMain(&command, &i, &fechar);
            cor[i]=150;

            al_flip_display();
            *dy=*dy-1;
        }
        if(*dy<=ALTURA_TELA-ALTURA_MAPA) *where=2;

        while(*dx<0&&!fechar&&*where==2){
            putMainBackground(*dx,*dy);

            destacar(cor[0],cor[1],cor[2],cor[3]);

            cor[i]=30;
            checarEventoMain(&command, &i, &fechar);
            cor[i]=150;

            al_flip_display();
            *dx=*dx+1;
        }
        if(*dx>=0) *where=3;
        while(*dy<0&&!fechar&&*where==3){
            putMainBackground(*dx,*dy);

            destacar(cor[0],cor[1],cor[2],cor[3]);

            cor[i]=30;
            checarEventoMain(&command, &i, &fechar);
            cor[i]=150;

            al_flip_display();
            *dy=*dy+1;
        }
        if(*dy>=0) *where=0;

    }
    return i;
}

void waitingScreen(){
    int inicio = 0;
    while(inicio<400){
        ALLEGRO_EVENT evento;
        ALLEGRO_TIMEOUT timeout;
        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_bitmap(logocin, 640/2, 480/2, 0);
        inicio++;
        al_init_timeout(&timeout, 0.01);
        int tem_eventos = al_wait_for_event_until(fila_eventos, &evento, &timeout);
        al_flip_display();
    }
}

//FUNÇÃO QUE ESPERA OS JOGADORES ENTRAREM NA SALA ATÉ O INÍCIO DO JOGO
void loadingScreen(int *play, int my_id){
  enum conn_ret_t resp;
  char msg[100],start;
  resp = recvMsgFromServer(msg, DONT_WAIT);
  if(resp == SERVER_DISCONNECTED){
  //tentar conectar de novo
  }
  else if(resp != NO_MESSAGE){
  //passar pro allegro
    if(resp == SERVER_FULL){
      printf("SERVER IS FULL\n");
      printf("GAME IS BEGINNING...\n");
      *play = 0;
    }
    else if(msg[0] == 'q'){ //Start
      printf("GAME IS BEGINNING...\n");
      *play = 0;
    }
    else{
      printf("%s", msg); //terminal
    }
  }
  if(my_id == 0){
    int comando = checkEvent();
    if(comando == 5){
      start = 'q';
      //manda msg para o server comecar o jogo
      if(start == 'q') sendMsgToServer(&start, sizeof(char));
    }
  }
}

#endif
