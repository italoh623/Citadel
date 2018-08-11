//Incluindo bibliotecas necessárias para o server
#include "Lib/AllegroCore.h"
#include "Lib/ServerLibrary.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 450
#define MAX_CLIENTS 8

//Função que faz o aleatório da localidade das bombas
void aleatorizePowerUps(){
    int forget;
    int var;
    for(int i = 0; i < 5; i++){
        forget = 1;
        while(forget==1) {
            forget = 0;
            var = rand() % 10;
            //Se var já for a localidade de alguma outra bomba então ele vai aleatorizar denovo
			//para que duas bombas não tenham a mesma localização
            for (int j = 0 ; j < i; j++) {
            	if(powerup[j].tipo == var) forget = 1;
            }
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

//whatIs recebe o id do cliente e checa se é par(guarda) ou ímpar(ladrão) e retorna o que ele é
int whatIs (int x) {
	if (x%2 == 0) return 0;
	return 1;
}

int main() {
	
	//Iniciando o allegro e checando se há falha	
	if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return 0;
    }
    
    //Iniciando eventos do allegro
    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return 0;
    }

	char str_buffer[BUFFER_SIZE]; //String para receber mensagens dos jogadores
	int quant_connect = 0, conectados=0; //Guardando a quantidade de jogadores totais no jogo
	int i;
	int id = 0; //Guardando o id de cada jogador
	char start = 'N'; //Maneira de começar o jogo

	//Chamando a função para aleatorizar as bombas
	aleatorizePowerUps();

	//Iniciando o server
	serverInit(MAX_CLIENTS);
	fprintf(stderr,"Server is running!!\n");

	//Começando a rodar o server
	while (1) {	
	  	//Recebendo conexão enquanto o jogo não começa
	    id = acceptConnection();

	    //Se houver conexão:
	    if (id != NO_CONNECTION) {
	    	//Recebe o nome do cliente
	    	recvMsgFromClient(&jogador[id].nome, id, WAIT_FOR_IT);
	    	strcpy(str_buffer, jogador[id].nome);

	    	//Manda o id para o cliente
	    	jogador[id].id = id;
	    	sendMsgToClient(&id, sizeof(int), id);
	    	
	    	//Manda a localizacao das bombas para o cliente
	    	for (int j = 0; j < 5; j++){
	    		sendMsgToClient(&powerup[j], sizeof(powerUP), id);
	    	}

	    	//Checa se o cliente é guarda(0) ou ladrão(1)
	    	jogador[id].whatis = whatIs(id);

	    	//Concatena na string o que o jogador é
	    	if (jogador[id].whatis == 0)
	    		strcat(str_buffer, " entrou no jogo e é um Guarda\n");
	    	
	    	else if (jogador[id].whatis == 1)
	      		strcat(str_buffer, " entrou no jogo e é um Ladrão\n");
	      	
	      	//Manda uma mensagem para todos os jogadores com informacoes dos novos jogadores que se conectarem
	      	broadcast(&str_buffer, strlen(str_buffer) + 1);

	      	//Printa no terminal do server o nome do jogador e seu id
	      	fprintf(stderr,"%s connected, with id = %d\n", jogador[id].nome, id);

	      	//Aumenta a quantidade de jogadores conectados
	      	quant_connect++;
	    }
	    
	    //Recebe comando do jogador 0 para saber se o jogo comecou
	    recvMsgFromClient(&start, 0, DONT_WAIT);

	    //Se receber 'q' o jogo começou, se a quantidade de jogadores for 8, começou também
		if (start == 'q' || quant_connect == 8) {
			start = 'q';
			//Avisa que o jogo comecou para os jogadores
			broadcast(&start, sizeof(char));

			//Manda o numero de jogadores conectados para os jogadores
			broadcast(&quant_connect, sizeof(int));
			conectados = quant_connect;

			//Printa, no terminal, quantos jogadores estão no jogo 
			fprintf(stderr,"jogo comecou, com %d jogadores", quant_connect);
		}
	    
	    
	    int b;
	    character comando;

	    //Deve acontecer quando o jogo começar
	    while(start == 'q'){

			//Recebe as structs dos jogadores que mandarem structs atualizadas
			struct msg_ret_t person = recvMsg(&comando);

			//Se houver mensagem:
			if(person.status == MESSAGE_OK){
				//A variável b será o id do jogador
		    	b = person.client_id;

		    	//Atualiza a struct do jogador[b] com a struct que ele mandou atualizada
		    	jogador[b] = comando;

		    	//Manda a struct do jogador b para os outros jogadores se o camando for diferente de 0(parado)
		    	if(jogador[b].comando != 0) broadcast(&jogador[b], sizeof(character));   
			    
			}

			//Se a mensagem for um aviso de desconectar:    	
		    if (person.status == DISCONNECT_MSG) {
		    	sprintf(str_buffer, "%s disconnected\n", jogador[person.client_id].nome);

		    	//Printa no terminal quem se desconectou e qual id está livre
		      	fprintf(stderr,"%s disconnected, id = %d is free\n", jogador[person.client_id].nome, person.client_id);

		      	//Avisa para os jogadores quem se desconectou
		      	broadcast(&str_buffer, strlen(str_buffer) + 1);
			}
		}
	}
}
