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
	const char *pipe_server1 = "FIFO_CLIENT1_SERVER";
	int rd_ch_1 = open(pipe_client1, O_CREAT | O_RDONLY);
	int wr_ch_1 = open(pipe_server1, O_CREAT | O_WRONLY);
	int points [2];
	int points_old, first, second, end_game=0;
	int turn=0;
	char board[52];
	guess try;
	if( rd_ch_1 < 0 || (wr_ch_1 < 0)){
		i=0;
		printf("errore pipe\n");
		write(wr_ch_1, &i, sizeof(int));
		exit(EXIT_FAILURE);	
	}else{
		i=1;
		printf("ok pipe\n");
		write(wr_ch_1, &i, sizeof(int));
	}
	//receive points
	read(rd_ch_1, &points, sizeof(points));
	printf("points received\n");
	read(rd_ch_1, board, sizeof(board));
	printf("board received!\n");
	while (1) {
		//receive turns
		read(rd_ch_1, &turn, sizeof(int));
		for(i=0; i<52; i++){
			printf("%c", board[i]);
		}
		printf("\nturn:%d\n", turn+1);
		if(turn==0){
			printf("it's your turn!\n");
			do {
				printf("Insert the decimal of your first guess between 0 and 5:\n");
				scanf("%d", &first);
			} while(first>5 || first < 0);
			do {
				printf("Insert the unit of your first guess between 0 and 9:\n");
				scanf("%d", &second);
			} while(second > 9 || second < 0 );
			try.first= first*10 + second;
			do {
				printf("Insert the decimal of your second guess between 0 and 5:\n");
				scanf("%d", &first);
			} while(first>5 || first < 0);
			do {
				printf("Insert the unit of your second guess between 0 and 9:\n");
				scanf("%d", &second);
			} while(second > 9 || second < 0 );
			try.second=first*10 + second;
			write(wr_ch_1, &try, sizeof(guess));	
		} 
		read(rd_ch_1, board, sizeof(board));
		points_old=points[turn];
		read(rd_ch_1, points, sizeof(points));
		if ( points[turn] == points_old ) {
			printf("client_%d guessed wrong !\n", turn+1);
			for(i=0; i<52; i++){
				printf("%c", board[i]);
			}
			printf("\n");
			sleep(10);
			read(rd_ch_1, board, sizeof(board));
		} else {
			printf("client %d guessed right!\n", turn+1);		
		}		
		
		//check points
		if( points[0]==13 && points[1] == 13){
			read(rd_ch_1, &end_game, sizeof(int));
			if ( end_game == -3){
				printf("Game over!\nMatch ended with a tie!\nServer correctly shutted down!\n");
				printf("Thanks for playing!\n");
				exit(0);
			} else {
				printf("Something went wrong while closing the server!\n");
				exit(EXIT_FAILURE);
			}
		
		}
		if( points[0]==14 || points[1]==14){
			read(rd_ch_1, &end_game, sizeof(int));
			if ( end_game == -3){
				printf("Game over!\nMatch ended with client_%d winning!\nServer correctly shutted down!\n", turn+1);
				printf("Thanks for playing!\n");
				exit(0);
			} else {
				printf("Something went wrong while closing the server!\n");
				exit(EXIT_FAILURE);
			}
		
		}
		system("clear");
	}

	return 0;
	
}




