#ifndef CLIENT_H
#define CLIENT_H

#include "client.h"
#include "AllegroCore.h"
#include "MainMenuLibrary.h"
#include <ctype.h>

// simple chat client
#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define HIST_MAX_SIZE 200

void aleatorizePowerUps(){
    int forget;
    for(int i = 0; i < 5; i++){
        forget = 1;
        while(forget==1){
            forget = 0;
            int var = rand() % 10;
            for(int j=0 ; j < i; j++)if(powerup[j].tipo == var) forget = 1;
            if (forget == 0) powerup[i].tipo = var;
        }
        if(powerup[i].tipo==0) powerup[i].x = 110, powerup[i].y = 240;
        if(powerup[i].tipo==1) powerup[i].x = 570, powerup[i].y = 425;
        if(powerup[i].tipo==2) powerup[i].x = 750, powerup[i].y = 620;
        if(powerup[i].tipo==3) powerup[i].x =1200, powerup[i].y = 250;
        if(powerup[i].tipo==4) powerup[i].x =1400, powerup[i].y = 700;
        if(powerup[i].tipo==5) powerup[i].x =  80, powerup[i].y = 860;
        if(powerup[i].tipo==6) powerup[i].x =1700, powerup[i].y = 950;
        if(powerup[i].tipo==7) powerup[i].x = 625, powerup[i].y = 725;
        if(powerup[i].tipo==8) powerup[i].x =1400, powerup[i].y = 350;
        if(powerup[i].tipo==9) powerup[i].x = 375, powerup[i].y = 270;
        powerup[i].aindatem=1;
    }
    
}

int my_id; // ID do proprio jogador
character send_struct; 

void assertConnection() {
  enum conn_ret_t ans = connectToServer(ip);
  // temos a resposta do tryconnect() no ans.A resposta esperada eh SERVER_UP
  // o while eh pra resolver os problemas ate recebermos SERVER_UP como resposta 
  while (ans != SERVER_UP) {
    if (ans == SERVER_DOWN) {
      puts("Server is down!\n");
    } else if (ans == SERVER_FULL) {
      puts("Server is full!\n");
    } else if (ans == SERVER_CLOSED) {
      puts("Server is closed for new connections!\n");
    } else {
      puts("Server didn't respond to connection!\n");
    }
    printf("Want to try again? [Y/n]\n");
    int res;
    while (res = tolower(getchar()), res != 'n' && res != 'y' && res != '\n'){
      puts("anh???\n");
    }
    if (res == 'n') {
      exit(EXIT_SUCCESS);
    }
    ans = connectToServer(ip);
  }
}


//FUNÇÃO QUE FAZ A CONEXÃO DO CLIENT COM O SERIDOR
int conectar(int xmain, int ymain){
	my_id = 0;
  // IP
  writeYourIPScreen(xmain,ymain);
  assertConnection(ip);

  // NOME
  writeYourNameScreen(xmain,ymain);

  sendMsgToServer(name, sizeof(char)*strlen(name));
  recvMsgFromServer(&my_id, WAIT_FOR_IT);

  powerUP aux;
  for (int j= 0; j < 5; j++) {
    recvMsgFromServer(&aux, WAIT_FOR_IT);
    printf("x = %d y = %d\n", aux.x, aux.y);
    jogador[my_id].power[j].x = aux.x;
    jogador[my_id].power[j].y = aux.y;
    jogador[my_id].power[j].aindatem = 1;
  }


  return my_id;
}

#endif
