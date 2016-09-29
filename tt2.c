#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


#define SPEEDTEST 2
#define DRILL 1
#define HSIZE 345
#define LENGTH 75
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

int main()
{	int choice, i;
	char *test_array;
	int ch = KEY_F1;
	int mistakes;
	int x, y;
	time_t start_t, end_t;
	WINDOW *typing_win;
	char string[80];
	
	string[0] = '\0';
						
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	intrflush(stdscr, FALSE);
	
	srandom(time(NULL));
	n_groups = sizeof(groups) / sizeof(char *);
	
	test_array = (char *)calloc(HSIZE + 1, sizeof(char));
	
	while(1)
	{		
		if(ch == KEY_F1)
		{	choice = print_menu();
			choice -= 1;
			if(choice == n_groups)
			{	print_byebye();
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
			x = 1;
			y = 1;
			mvwprintw(typing_win, y, x, "%s", test_array);
			wrefresh(typing_win);
			y += 6;
	
			mistakes = 0;
			i = 0;
			time(&start_t);
			wmove(typing_win, y, x);
			wrefresh(typing_win);
			ch = 0;
			while(ch != KEY_F1 && i < HSIZE + 1)
			{	ch = wgetch(typing_win);
				
				if(ch == test_array[i]) {
					mvwprintw(typing_win, y, x, "%c", ch);
					wrefresh(typing_win);
					++x;
					if ( x == 74 ) {
						x = 1;
						++y;
					}
					++i;
					continue;
				}
				else {
					attron(A_REVERSE);
					mvwprintw(typing_win, y, x, "%c", ch);
					attroff(A_REVERSE);
					wrefresh(typing_win);
					++x;
					if ( x == 74 ) {
						x = 1;
						++y;
					}
					++mistakes;
					++i;
				}
			}
	
			time(&end_t);
			print_time(start_t, end_t, mistakes);
			ch = wgetch(typing_win);
		}
		
		else if ( choice == DRILL ) {
			
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
		for(i = 0;i <= n_groups - 1; ++i)
			printw("\t%3d: \tPractice %s\n", i + 1, groups[i]);
		printw("\t%3d: \tExit\n", i + 1);
	
		printw("\n\n\tChoice: ");
		refresh();
		echo();
		scanw("%d", &choice);
		noecho();
	
		if(choice >= 1 && choice <= n_groups + 1)
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

enum files { test0, test1, test2, test3 };
void create_test_string(char *test_array, int choice)		//use files to create arrays
{	int i, index, length, fdr, x, size = 1024;

	char *arr = (char*)malloc(sizeof(char) * size);
	
	index = (int)(random() % 4);
	
	switch(index) {
		case test0:
			fdr = open("test0.txt", O_RDONLY );
			break;
		case test1:
			fdr = open("test1.txt", O_RDONLY );
			break;
		case test2:
			fdr = open("test2.txt", O_RDONLY );
			break;
		case test3:
			fdr = open("test3.txt", O_RDONLY );
			break;
	}
	
	if ( fdr == -1 ) {
		perror( "Open failed.");
		return errno;
	}
	
	i = 0;
	
	while( (x = read( fdr, &ch, 1 )	) ) {
		arr[i] = ch;
		i++;
		if(i == size) {
			size *= 2;
			arr = (char *)realloc(a, sizeof(char) * size);
			if(arr == NULL) {
				mvprintw(STATUSY, STATUSX, "Failed for %d\n", size);
				exit(1);
			}
	}
	
	arr[i] = '\0';	
	
	if ( choice == SPEEDTEST ) {
		test_array = arr;
	}
	
	else if ( choice == DRILL ) {
		
	
	
	/*length = strlen(groups[choice]);
	for(i = 0;i <= HSIZE - 1; ++i)
	{	if(i%5 == 0)
			test_array[i] = ' ';
		else
		{	index = (int)(random() % length);
			test_array[i] = groups[choice][index];
		}
	}
	test_array[i] = '\0';*/
	
}	
	
void print_byebye()
{	printw("\n");
	print_in_middle(0,0,0,"Thank you for using my typing tutor\n", NULL);
	print_in_middle(0,0,0,"Bye Bye ! ! !\n", NULL);
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
		attron(A_REVERSE);
		mvprintw(STATUSY - 2, STATUSX, "Error Percentage is high. ( %f % )", error);
		attroff(A_REVERSE);
	}
	
	attron(A_REVERSE);
	mvprintw(STATUSY, STATUSX, "MISTAKES: %d  TIME Taken: %d:%d:%d  WPM : %.2f    Press any Key to continue", mistakes, h, m, s, 	wpm);
	attroff(A_REVERSE);
	
	refresh();


}

/* 
 startx = 0 means at present x 									
 starty = 0 means at present y									
 win = NULL means take stdscr 									
 */

void print_in_middle(int startx, int starty, int width, char *string, WINDOW *win)
{	int length, x, y;
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
	temp = (width - length)/ 2;
	x = startx + (int)temp;	
	mvwprintw(win, y, x, "%s", string);
	refresh();
}
