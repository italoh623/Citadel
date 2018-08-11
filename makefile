serverFile=server
clientFile=client
serverName=Server
clientName=Game

all: compServer compClient

server: compServer runServer

client: compClient runClient

compClient:
	gcc -o $(clientName) $(clientFile).c  $(pkg-config –cflags –libs allegro-5) -lallegro_ttf -lallegro_font -lallegro_image -lallegro_audio -lallegro_acodec -lallegro -lallegro_main

compServer:
	gcc -o $(serverName) $(serverFile).c $(pkg-config –cflags –libs allegro-5) -lallegro_ttf -lallegro_font -lallegro_image -lallegro_audio -lallegro_acodec -lallegro -lallegro_main

runClient:
	./$(clientName)

runServer:
	./$(serverName)