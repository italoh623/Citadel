//Inclui as bibliotecas necessárias para o client
#include "Lib/ClientLibrary.h"
#include "Lib/Matriz.h"
#include "Lib/AllegroCore.h"
#include "Lib/PlayableLibrary.h"
#include "Lib/CharacterLibrary.h"
#include "Lib/MainMenuLibrary.h"

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
int num_jogadores = 0;


int main() {
	//Inicializando
	if(!inicializar()) return -1;

    //Abertura
    int player = 4;

	//Tela de inicio
    waitingScreen();

  
    //Declarando variáveis
    int command, exit = 0, volume = 5;
    int xmain = 0, ymain = 0, onde = 0;

    //Tocar sample do main menu
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP,NULL);

    //Enquanto for diferente de sair
    while(!exit){
 		//Inicializa main menu e retorna o comando do jogador
        command = mainMenu(&xmain,&ymain,&onde);

        //Se o comando for 0, começa conexão com server
        if(command == 0){
        	//A função conectar retorna o id do jogador
        	player = conectar(xmain,ymain);

        	//Jogador escolhe a cor do seu personagem
        	selectYourColorScreen(player,xmain,ymain);

	        //Screen de espera que fica enquanto o jogo não começa
	        int play = 1;
	        while(play)
	        	loadingScreen(&play, player);
	        
	        //Recebendo o número de jogadores conectados
	        recvMsgFromServer(&num_jogadores,WAIT_FOR_IT);

	        //Coloca o nome do jogador na sua struct
	        strcpy(jogador[player].nome,name);
	        corPersonagem(player);  

	        //Começa o jogo
	        playGame(player,num_jogadores);

	        //Faz o zoom do jogo
	        makeZoomedGame(1);
        }

        //Se comando for 1, mostra tela de instruções
        if(command==1)
            mainInstructions(xmain,ymain);

        //Se comando for 2, mostra opção de diminuir volume
        if(command==2)
            mainOptions(&volume,xmain,ymain);

        //Se o comando for 3, sai do jogo
        if(command==3)
            exit=1;

    }

    destruir();
  return 0;
}