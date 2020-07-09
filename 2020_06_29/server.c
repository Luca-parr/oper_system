#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
typedef struct {
	int first;
	int second;
} guess;

int main () {
	int i;
	char var,start = 'a';
	const char *pipe_client1 = "FIFO_SERVER_CLIENT1";
	const char *pipe_client2 = "FIFO_SERVER_CLIENT2";
	const char *pipe_server1 = "FIFO_CLIENT1_SERVER";
	const char *pipe_server2 = "FIFO_CLIENT2_SERVER";
	mkfifo(pipe_client1, 0666 );
	mkfifo(pipe_client2, 0666 );
	mkfifo(pipe_server1, 0666 );
	mkfifo(pipe_server2, 0666 );
	int wr_ch_1 = open(pipe_client1, O_CREAT | O_WRONLY);
        int wr_ch_2 = open (pipe_client2, O_CREAT | O_WRONLY);
	int rd_ch_1 = open (pipe_server1, O_CREAT | O_RDONLY);
	int rd_ch_2 = open (pipe_server2, O_CREAT | O_RDONLY);
	int points [2];
	int pointer;
	int turn=0;
	int end_game=-3;
	guess try;
	points[0]=0;
	points[1]=0;
	if(  rd_ch_1 < 0 || rd_ch_2 < 0 || wr_ch_1 < 0 ||  wr_ch_2 < 0){
		printf("errore pipe\n");
		exit(EXIT_FAILURE);	
	}
	//creating random board
	char board[52];
	char board_to_send [52];
	for (i=0; i<51; i++) {
		board[i]=start;
		board[i+1]=start;
		//printf("board at pos: %d is %c \nboard at pos %d is %c \n", i, board[i], i+1, board [i+1]);
		i++;
		start++;
	}
	for (i=0; i<51; i++) {
		srand(time(NULL));
		pointer=rand()%(51-i);
		var=board[i+pointer];
		board[i+pointer]=board[i];
		board[i]=var;	
	}
	//end_creating board
	for(i=0; i<52; i++){
		printf("%c %d ", board[i], i);
		board_to_send[i]= '.';	
	}
	printf("\n");
	//read_client1 ready
	read(rd_ch_1, &i, sizeof(int));
	//printf("test_1\n");
	if(i==1){
		printf("client_1 joined!\n");
	} else {
		printf("client_1 can't join\n");
		exit(-1);
	}
	//read_client2 ready
	read(rd_ch_2, &i, sizeof(int));
	if(i==1){
		printf("client_2 joined!\n");
	} else {
		printf("client_2 can't join\n");
		exit(-1);
	}
	//send points
	write(wr_ch_1, &points, sizeof(points));
	write(wr_ch_2, &points, sizeof(points));
	printf("points sent\n");
	//send board
	write(wr_ch_1, board_to_send, sizeof(board_to_send));
	write(wr_ch_2, board_to_send, sizeof(board_to_send));
	while (1) {
		//send turns
		write(wr_ch_1, &turn, sizeof(int));
		write(wr_ch_2, &turn, sizeof(int));
		//read guess
		if(turn==0){
			read(rd_ch_1, &try, sizeof(guess));	
		} else {
			read(rd_ch_2,&try,sizeof(guess));
		}
		printf("first: %d second: %d\n", try.first, try.second);
		if(board[try.first] == board[try.second]){
			printf("client_%d guessed right!\n", turn+1);
			board_to_send[try.first]=board[try.first];
			board_to_send[try.second]=board[try.second];
			points[turn]++;
			write(wr_ch_1,board_to_send,sizeof(board_to_send));
			write(wr_ch_2, board_to_send, sizeof(board_to_send));
			write(wr_ch_1, points, sizeof(points));
			write(wr_ch_2, points, sizeof(points));
		}else {	
			printf("client_%d guessed wrong!\n", turn+1);
			board_to_send[try.first]=board[try.first];
			board_to_send[try.second]=board[try.second];
			write(wr_ch_1,board_to_send,sizeof(board_to_send));
			write(wr_ch_2, board_to_send, sizeof(board_to_send));
			write(wr_ch_1, points, sizeof(points));
			write(wr_ch_2, points, sizeof(points));
			sleep(5);
			board_to_send[try.first]='.';
			board_to_send[try.second]='.';
			write(wr_ch_1,board_to_send,sizeof(board_to_send));
			write(wr_ch_2, board_to_send, sizeof(board_to_send));
		
		}
		//check points
		if( points[0]==13 && points[1] == 13){
			printf("Game over!\nMatch ended with tie!\nShutting server down!\n");
			write(wr_ch_1, &end_game, sizeof(int));
			write(wr_ch_2, &end_game, sizeof(int));
			sleep(1);
			unlink(pipe_client1);
			unlink(pipe_server1);
			unlink(pipe_server2);
			unlink(pipe_client2);
			exit(0);
		
		}
		if( points[0]==14 || points[1]==14){
			printf("Game over!\nMatch ended with client_%d winning!\nShutting server down!\n", turn+1);
			write(wr_ch_1, &end_game, sizeof(int));
			write(wr_ch_2, &end_game, sizeof(int));
			//sleep(1);
			unlink(pipe_client1);
			unlink(pipe_server1);
			unlink(pipe_server2);
			unlink(pipe_client2);
			exit(0);
		
		}
		//change turn
		if(turn==0){
			turn=1;
		}else{
			turn=0;
		}
	}

	return 0;
	
}




