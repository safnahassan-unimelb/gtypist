#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>



#define SPEEDTEST 1
#define DRILL 0
#define HSIZE 345
#define ARR_LENGTH 70
#define LENGTH 85
#define WIDTH 30
#define STARTX 1
#define STARTY 5
#define STATUSX 1
#define STATUSY 40
#define KEY_F1 265
#define DEF_ERR 3

int print_menu();
void print_byebye();
void create_test_string();
void print_time(time_t startt, time_t endt, int mistakes);
void print_in_middle(int startx, int starty, int width, char *string, WINDOW *win);

char *groups[] = {  "Drill" ,"Speed Test"  };
int n_groups;

void print_usage() {
    printf("Usage:\n My typing tutor allows the user pracice typing on the computer keyboard.There are two types of typing exercises: drills and speed tests.In a drill, the typist displays text in every other line on the screen, and waits for the user to correctly type the exact same text in the intermediate lines. Typing errors are indicated by highlighting the mistakes. and at the end of the exe,cise it calculates the real and effective rate in Words Per Minute (WPM).Backward deleting of previously typed characters to correct errors is not allowed.\nIn a speed test, gtypist displays text on the screen, and waits for the user to correctly over-type the exact same text. It indicates typing errors, and at the end of the test it calculates the real and effective rate in WPM.\n General usage: ./try [options, if any] \n \t Availble Options: \n -b : \n\tThetypist will keep track of your personal best typing speeds and tell you when you’ve beaten them.\n -e :\n\t The typist will tell you the default maximum allowed error percentwage.\n -s : \n\t Change the scoring mode. The default scoring mode is WPM.The available modes are wpm( words per minute ) and cpm(characters per minute). \n ");
}

int main( int argc, char *argv[] ) {
	int option = 0;
	while ((option = getopt(argc, argv,"h::")) != -1) {
		switch (option) {
		     case 'h' : 
		     		print_usage();
		     		exit(0);
		     default :
		     		break;
		}
	}

	
	int choice, i, pos;
	char *test_array;
	int ch = KEY_F1;
	int mistakes;
	int x, y;
	time_t start_t, end_t;
	WINDOW *typing_win;
	char string[80], arr[ ARR_LENGTH ];
	
	string[0] = '\0';
						
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	intrflush(stdscr, FALSE);
	
	srandom(time(NULL));
	n_groups = 2;
	
	test_array = (char *)calloc(HSIZE + 1, sizeof(char));
	
	while(1)
	{		
		if(ch == KEY_F1)
		{	choice = print_menu();
			choice -= 1;
			if(choice == 2) {
				print_byebye();
				free(test_array);
				endwin();
				exit(0);
			}
		}
		clear();
		
		strcpy(string, "Typing window");
		print_in_middle(STARTX, STARTY - 2, LENGTH, string, NULL);
		attron(A_REVERSE);
		mvprintw(STATUSY, STATUSX, "Press F1 to Main Menu");
		refresh();
		attroff(A_REVERSE);

		create_test_string(test_array, choice);
		typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
		keypad(typing_win, TRUE);
		intrflush(typing_win, FALSE);
		box(typing_win, 0, 0);
		
		if ( choice == SPEEDTEST ) {  //typing complete para in box
			/* do text wrapping */
			pos = 0;
			for ( i = 0; i < (ARR_LENGTH - 1) ; i++ ) {
				arr[i] = test_array[i];
				pos++;
			}
				
			arr[i] = '\0';
			
			x = 1;
			y = 1;
			mvwprintw(typing_win, y, x, "%s", arr);
			y++;
			while ( pos < HSIZE ) {
			/* 69 characters is the length of one line */
				for( i = 0; i < (ARR_LENGTH - 2) ; i++ ) {
					arr[i] = test_array[pos];
					pos++;
				}
				
				arr[i] = '\0';
				
				mvwprintw(typing_win, y, x, "%s", arr);
				y++;
			}
				
			wrefresh(typing_win);  //time calculation
			
			y++;;
	
			mistakes = 0;
			i = 0;
			time(&start_t);
			wmove(typing_win, y, x);
			wrefresh(typing_win);
			ch = 0;
			while(ch != KEY_F1 && i < HSIZE + 1)
			{	ch = wgetch(typing_win);
			
				if ( ch == KEY_ENTER ) {
					x = 1;
					y++;
					continue;
				}
					
				
				else if (ch == test_array[i]) {
					mvwprintw(typing_win, y, x, "%c", ch);
					wrefresh(typing_win);
					++x;
					++i;
					continue;
				}
				else {
					attron(A_REVERSE);
					mvwprintw(typing_win, y, x, "%c", ch);
					attroff(A_REVERSE);
					wrefresh(typing_win);
					++x;
					++mistakes;
					++i;
				}
			}
	
			time(&end_t);
			print_time(start_t, end_t, mistakes);
			// wait for a key press before quitting
			ch = wgetch(typing_win);
		}
		
		else if ( choice == DRILL ) {    //stuck */
			

	    		int text_length;
	
	    		int i, k, j, max_x, max_y;

	    		// Get text length
	   		text_length = strlen( test_array );
	   		
	    		// Don't show cursor
	    		curs_set(0);
	    		// Get terminal dimensions
	    		//   getmaxyx(stdscr, max_y, max_x);
	    		// Clear the screen
	    		clear();

	    		getmaxyx(stdscr, max_y, max_x);
			box(stdscr, 0, 0);
			wrefresh(typing_win);

		    // Scroll text back across the screen
			j = strlen(test_array);
			mistakes = 0;
			i = 0;
			time(&start_t);
			for (k = 0; k < strlen( test_array ); k++) {
				wmove(typing_win, y, x);
				clrtoeol();  //clearing one line 
				if ( j > max_x )
					mvaddnstr(3, 1, &test_array[i], max_x );
				else
					mvaddnstr(3, 1, &test_array[i], j );
				j--;
				refresh();
				usleep(100000);
				if (ch == test_array[i]) {
					mvwprintw(typing_win, 5, 1, "%c", ch);
					wrefresh(typing_win);
					continue;
				}
				else {
					attron(A_REVERSE);
					mvwprintw(typing_win, 5, 1, "%c", ch);
					attroff(A_REVERSE);
					wrefresh(typing_win);
					++mistakes;
				}
			   	               //time calculation left here
			}
			
			time(&end_t);
			print_time(start_t, end_t, mistakes);
		    // Wait for a keypress before quitting
		    	ch = wgetch(typing_win);
		}
	}
	free(test_array);
	endwin();
	return 0;
}
		

int print_menu() {
	int choice, i;
	
	choice = 0;
	while(1) {
		clear();
		printw("\n\n");
		print_in_middle(1, 1, 0, " Welcome to gtypist ", NULL);
		printw("\n\n\n");
		for(i = 0;i <= (n_groups - 1); ++i)
			printw("\t%3d: \tPractice %s\n", i + 1, groups[i]);
		printw("\t%3d: \tExit\n", i + 1);
	
		printw("\n\n\tChoice: ");
		refresh();
		echo();
		scanw("%d", &choice);
		noecho();
	
		if(choice >= 1 && choice <= 3)
			break;
		else {
			attron(A_REVERSE);
			mvprintw(STATUSY, STATUSX, "Wrong choice\tPress any key to continue");
			attroff(A_REVERSE);
			getch();
		}
	}
	return choice;
}


void create_test_string(char *test_array, int choice) {		//use files to create arrays
	int i, index, length, fdr, x, size = 346;

	char ch;

	char *array = (char*)malloc(sizeof(char) * size);
	
	index = (int)(random() % 4);
	
	switch (index) {
		case 0:
			fdr = open("test0.txt", O_RDONLY );
			break;
		case 1:
			fdr = open("test1.txt", O_RDONLY );
			break;
		case 2:
			fdr = open("test2.txt", O_RDONLY );
			break;
		case 3:
			fdr = open("test3.txt", O_RDONLY );
			break;
	}
	
	if ( fdr == -1 ) {
		perror( "Open failed.");
		return;
	}
	
	i = 0;
	
	while( i < HSIZE )  {
		x = read( fdr, &ch, 1 );
		array[i] = ch;
		i++;
		if(i == size) {
			size *= 2;
			array = (char *)realloc(array, sizeof(char) * size);
			if(array == NULL) {
				mvprintw(STATUSY, STATUSX, "Failed for %d\n", size);
				exit(1);
			}
			
		}
	}
	
	
	array[i] = '\0';
	
	test_array = array;
	
	close( fdr );
	
	
}	
	
void print_byebye()
{	printw("\n");
	print_in_middle(0,0,0,"Thank you for using my typing tutor\n", NULL);
	print_in_middle(0,0,0,"Bye.\n", NULL);
	refresh();
}
		
void print_time(time_t start_t, time_t end_t, int mistakes)
{	long int diff;
	int h,m,s;
	float wpm, error;

	diff = end_t - start_t;
	wpm = ((HSIZE / 5)/(double)diff)*60;

	h = (int)(diff / 3600);
    	diff -= h * 3600;
	m = (int)(diff / 60);
	diff -= m * 60;
	s = (int)diff; 
	
	error = ( mistakes / HSIZE ) * 100;
	
	if ( error > DEF_ERR ) {
		attron(A_BOLD);
		attron(A_REVERSE);
		mvprintw(STATUSY - 2, STATUSX, "Error Percentage is high. ( %f % )", error);
		attroff(A_REVERSE);
		attroff(A_BOLD);
	}
	
	attron(A_BOLD);
	attron(A_REVERSE);
	mvprintw(STATUSY, STATUSX, "MISTAKES: %d  TIME Taken: %d:%d:%d  WPM : %.2f    Press any Key to continue", mistakes, h, m, s, 	wpm);
	attroff(A_REVERSE);
	attroff(A_BOLD);
	
	refresh();


}

/* 
 startx = 0 means at present x 									
 starty = 0 means at present y									
 win = NULL means take stdscr 									
 */

void print_in_middle(int startx, int starty, int width, char *string, WINDOW *win) {
	int length, x, y;
	float temp;
	
	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length) / 2;
	x = startx + (int)temp;	
	mvwprintw(win, y, x, "%s", string);
	refresh();
}
