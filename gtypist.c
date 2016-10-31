/*Copyright 2016 Safna Hassan 

This program is free software: you can redistribute it and/or modify it.This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.*/

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include "gtypist.h"



#define SPEEDTEST 6
#define DRILL 5
#define QUICK 1
#define QWERTY 2
#define LONG 3
#define DVORAK 4 
#define TOUCH 0

#define HSIZE 345
#define ARR_LENGTH 70
#define LENGTH 85
#define WIDTH 30
#define STARTX 1
#define STARTY 5
#define STATUSX 1
#define STATUSY 40
#define KEY_ESCAPE 27
#define KEY_ENT 10



int DEF_ERR = 3, state = 0; //state 0 means scoring i in wpm
char *groups[] = {  " QWERTY TOUCH typing ", "QUICK QWERTY course" , "QWERTY REVIEW", "LONG QWERTY course",  "DVORAK touch typing", "DRILLS" ,"SPEED TEST"};
  
int n_groups;

void print_usage() {
    printf("\n\nMy typing tutor allows the user practice typing on the computer keyboard. It is a typing tutor with several lessons for different keyboards.\n\nUsage: \t\t./project [ options...(if any) ] \n\n\n Options: \n -e %%(val)\t\tset default maximum error percentage (default 3.0)\n -s c/w\t\t\tset scoring mode (w - words per minute or c - characters per minute)\n\t\t\tvalid values are between 0.0 and 100.0\n -h\t\t\tprint this message\n -v\t\t\toutput version information and exit\n\n");
}

void print_version() {
	printf("\n\nTyping Tutor 1.0\n\nCopyright 2016 Safna Hassan\n\nThis program is free software: you can redistribute it and/or modify it.\n\tThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n");
}

int main( int argc, char *argv[] ) {
	int option = 0;
	while ((option = getopt(argc, argv,"hvs:e:")) != -1) {
		switch (option) {
			case 'h' : 
				print_usage();
				exit(0);
			case 'v' :
				print_version();
				exit(0);
			case 'e' :
				if ( atoi(optarg) > 0 && atoi(optarg) < 100 )
					DEF_ERR = atoi(optarg);
				else
					printf( "Invalid error max. value\n" );
				break;
			case 's' :
				if (strcmp( optarg, "c" ) == 0 ) {
					state = 1;
					break;
				}
				
				else if (strcmp( optarg, "w" ) == 0 ) {
					state = 0;
					break;
				}
				
				else {
					printf( "Invalid scoring mode\n" );
					break;
				}
			default :
				break;
		}
	}

	
	int choice, i, pos;
	char *test_array;
	int ch = KEY_ESCAPE;
	int mistakes;
	int x, y;
	time_t start_t, end_t;
	WINDOW *typing_win;
	char string[80], arr[ ARR_LENGTH ];
	
	string[0] = '\0';
						
	initscr();
	
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_CYAN);
	init_pair(2, COLOR_WHITE, COLOR_MAGENTA);
	
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	intrflush(stdscr, FALSE);
	
	srandom(time(NULL));
	n_groups = sizeof(groups) / sizeof(char *);
	
	//test_array = (char *)calloc(HSIZE + 1, sizeof(char));
	
	while(1)
	{		
		if(ch == KEY_ESCAPE)
		{	choice = print_menu();
			choice -= 1;
			if(choice ==  n_groups) {
				print_byebye();
				//free(test_array);
				endwin();
				exit(0);
			}
		}
		clear();
		
		if ( choice == QUICK ) {
			char *lessons[] = { "Lesson Q1     a s d f j k l ; e i" , "Lesson Q2     h g o u n . t" , "Lesson Q3     y r c , ? : p" , "Lesson Q4     m w v z x b q" , "Lesson Q5     ' -" };
			int les, i;
	
			les = 0;
			while(1) {
				clear();
				printw("\n\n");
				
				attron(COLOR_PAIR(1));
				print_in_middle(1, 1, 0, "The Q series contains the following 5 lessons ", NULL);
				attroff(COLOR_PAIR(1));
				
				printw("\n\n\n");
				for(i = 0;i <= (4); ++i)
					printw("\t%3d: \t %s\n", i + 1, lessons[i]);
				printw("\t%3d: \tExit\n", i + 1);
	
				printw("\n\n\tChoice: ");
				refresh();
				echo();
				scanw("%d", &les);
				noecho();
	
				if(les >= 1 && les <= 6)
					break;
				else {
					attron(A_REVERSE);
					mvprintw(STATUSY, STATUSX, "Wrong choice\tPress any key to continue");
					attroff(A_REVERSE);
					ch = getch();
				}
			}
			
			clear();
		
			strcpy(string, "Typing window");
			attron(COLOR_PAIR(1));
			print_in_middle(STARTX, STARTY - 2, LENGTH, string, NULL);
			attroff(COLOR_PAIR(1));
			attron(A_REVERSE);
			mvprintw(STATUSY, STATUSX, "Press ESC to Main Menu");
			refresh();
			attroff(A_REVERSE);

			
			typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			box(typing_win, 0, 0);
			if ( les == 1 ) {
			
				mvprintw(STARTY - 2, STARTX, "Try this:");
				refresh();
				test_array = "asdf ;lkj asdf ;lkj asdf ;lkj asdf ;lkj asdf ;lkj asdf ;lkj";
			}
			
			if ( les == 2 ) {
			
				mvprintw(STARTY - 2, STARTX, "Try this:");
				refresh();
				test_array = "asdefghk lokijujhjn asdefghk lokijujhjn asdefghk houas fghk";
			}
			
			if ( les == 3 ) {
			
				mvprintw(STARTY - 2, STARTX, "Try this:");
				refresh();
				test_array = "deki frju dck, dcl. frju ftjy deki frju dck, dcl. frju ftjy";
			}
			
			if ( les == 4 ) {
			
				mvprintw(STARTY - 2, STARTX, "Try this:");
				refresh();
				test_array = "aqsw az;p sxl. fvjm fvjn fbjn aqsw az;p sxl. fvjm fvjn fbjn";
			}
			
			if ( les == 5 ) {
			
				mvprintw(STARTY - 2, STARTX, "Try this:");
				refresh();
				test_array = "frfk- fvf'b jujy j'mjn aqsw a'zsw azsx- ;plo ;p;- kik, ;p;-";
			}
			
			if ( les == 6 ) {
				ch = KEY_ESCAPE;
				continue;
			}
			typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			box(typing_win, 0, 0);
			
			x = 1;
			y = 1;
			mvwprintw(typing_win, y, x, "%s", test_array);
			wrefresh(typing_win);
			y += 1;

			mistakes = 0;
			i = 0;
			time(&start_t);
			wmove(typing_win, y, x);
			wrefresh(typing_win);
			ch = 0;
			while(ch != KEY_ESCAPE && i != 59)
			{	ch = wgetch(typing_win);
				
				if(ch == test_array[i])
				{	++i;
					mvwprintw(typing_win, y, x, "%c", ch);
					wrefresh(typing_win);
					++x;
					continue;
				}
				else
				{	
					wattron(typing_win, A_REVERSE);
					mvwprintw(typing_win, y, x, "%c", ch);
					wattroff(typing_win, A_REVERSE);
					wrefresh(typing_win);
					++x;
					++mistakes;
					++i;
				}
			}

			time(&end_t);
			print_time(start_t, end_t, mistakes, 59);
			
			// wait for a key press before quitting
			ch = wgetch(typing_win);
		}
		
		
		if ( choice == TOUCH ) {
			char *lessons[] = {"Lesson 1 a s d f g h j k l ;", "Lesson 2     e g h ,", "Lesson 3     i r . shift" ,"Lesson 4     o p shift ? " , "Lesson 5     t n :" ,"Lesson 6     b u /", "Lesson 7     c m -", "Lesson 8    1 2 3 4 5 6 7 8 9 0 " };
			int les, i;
	
			les = 0;
			while(1) {
				clear();
				printw("\n\n");
				attron(COLOR_PAIR(1));
				print_in_middle(1, 1, 0, "The T series contains the following 8 lessons ", NULL);
				attroff(COLOR_PAIR(1));
				
				printw("\n\n\n");
				for(i = 0;i <= (7); ++i)
					printw("\t%3d: \t %s\n", i + 1, lessons[i]);
				printw("\t%3d: \tExit\n", i + 1);
	
				printw("\n\n\tChoice: ");
				refresh();
				echo();
				scanw("%d", &les);
				noecho();
	
				if(les >= 1 && les <= 9)
					break;
				else {
					attron(A_REVERSE);
					mvprintw(STATUSY, STATUSX, "Wrong choice\tPress any key to continue");
					attroff(A_REVERSE);
					ch = getch();
				}
			}
			
			clear(); 
			
			strcpy(string, "Typing window");
			attron(COLOR_PAIR(1));
			print_in_middle(STARTX, STARTY - 2, LENGTH, string, NULL);
			attroff(COLOR_PAIR(1));
			
			attron(A_REVERSE);
			mvprintw(STATUSY, STATUSX, "Press ESC to Main Menu");
			refresh();
			attroff(A_REVERSE);

			
			typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			box(typing_win, 0, 0);
			if ( les == 1 ) {
			
				mvprintw(STARTY - 2, STARTX, "Don't look at your hands!");
				refresh();
				test_array = "f j d k s l a ; ff jj dd ;; fff jjj ddd kkk sss lll aaa ;;;";
			}
			
			if ( les == 2 ) {
			
				mvprintw(STARTY - 2, STARTX, "Don't look at your hands!");
				refresh();
				test_array = "e g h , ee gg hh ,, eee ggg hhh ,,, aaee ssgg kk,, jjeel fg";
			}
			
			if ( les == 3 ) {
			
				mvprintw(STARTY - 2, STARTX, "Don't look at your hands!");
				refresh();
				test_array = ";; i r  ;A; ;;; I ; R  ;F; ;;; ;D; ;;; ;S; ii rr DDD ; KSL;";
			}
			
			if ( les == 4 ) {
			
				mvprintw(STARTY - 2, STARTX, "Don't look at your hands!");
				refresh();
				test_array = "o p oo pp poke Pi Lord Jill Joe Kip Leif Peg Olaf Ira Heid,";
			}
			
			if ( les == 5 ) {
			
				mvprintw(STARTY - 2, STARTX, "Don't look at your hands!");
				refresh();
				test_array = "tt nn :: The talk is that those three are threatening her..";
			}
			
			if ( les == 6 ) {
			
				mvprintw(STARTY - 2, STARTX, "Don't look at your hands!");
				refresh();
				test_array = "bb nn // n/a Bb Nn / In baseball, bunting is a kind of hit.";
			}
			
			if ( les == 7 ) {
			
				mvprintw(STARTY - 2, STARTX, "Don't look at your hands!");
				refresh();
				test_array = "cc mm -- cM Cm- one-third ;;; one-half ;;; three-fifths ;;;";
			}
			
			if ( les == 8 ) {
			
				mvprintw(STARTY - 2, STARTX, "Don't look at your hands!");
				refresh();
				test_array = "123 456 789  Jr. 99 log 57-9 old 7 hours 87/ 76 log123 45 /";
			}
			
			if ( les == 9 ) {
				ch = KEY_ESCAPE;
				continue;
			}
			
			typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			box(typing_win, 0, 0);
			
			x = 1;
			y = 1;
			mvwprintw(typing_win, y, x, "%s", test_array);
			wrefresh(typing_win);
			y += 1;

			mistakes = 0;
			i = 0;
			time(&start_t);
			wmove(typing_win, y, x);
			wrefresh(typing_win);
			ch = 0;
			while(ch != KEY_ESCAPE && i != 59)
			{	ch = wgetch(typing_win);
				
				if(ch == test_array[i])
				{	++i;
					mvwprintw(typing_win, y, x, "%c", ch);
					wrefresh(typing_win);
					++x;
					continue;
				}
				else
				{	
					wattron(typing_win, A_REVERSE);
					mvwprintw(typing_win, y, x, "%c", ch);
					wattroff(typing_win, A_REVERSE);
					wrefresh(typing_win);
					++x;
					++mistakes;
					++i;
				}
			}

			time(&end_t);
			print_time(start_t, end_t, mistakes, 59);
			
			// wait for a key press before quitting
			ch = wgetch(typing_win);
		}
		
		if ( choice == QWERTY ) {
		
			char *lessons[] = {"Lesson 1     Home row Combinations", "Lesson 2     Other letters" , "Lesson 3     Shift numerals figs" ,"Lesson 4     Drill on S Combinations" ,"Lesson 5     Drill on R Combinations","Lesson 6     Drill on L Combinations", "Lesson 7    Drill on D-T" ,"Lesson 8     Drill on M-N Combinations" , "Lesson 9    Drill on com-con Combinations" , "Lesson 10    Drill on ter, ther, tor, ture, ster, der" };
			int les, i;
	
			les = 0;
			while(1) {
				clear();
				printw("\n\n");
				attron(COLOR_PAIR(1));
				print_in_middle(1, 1, 0, "The U series contains the following 10 lessons ", NULL);
				attroff(COLOR_PAIR(1));
				
				printw("\n\n\n");
				for(i = 0;i <= (9); ++i)
					printw("\t%3d: \t %s\n", i + 1, lessons[i]);
				printw("\t%3d: \tExit\n", i + 1);
	
				printw("\n\n\tChoice: ");
				refresh();
				echo();
				scanw("%d", &les);
				noecho();
	
				if(les >= 1 && les <= 11)
					break;
				else {
					attron(A_REVERSE);
					mvprintw(STATUSY, STATUSX, "Wrong choice\tPress any key to continue");
					attroff(A_REVERSE);
					ch = getch();
				}
			}
			
			clear(); 
			
			strcpy(string, "Typing window");
			attron(COLOR_PAIR(1));
			print_in_middle(STARTX, STARTY - 2, LENGTH, string, NULL);
			attroff(COLOR_PAIR(1));
			
			attron(A_REVERSE);
			mvprintw(STATUSY, STATUSX, "Press ESC to Main Menu");
			refresh();
			attroff(A_REVERSE);

			
			typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			box(typing_win, 0, 0);
			
			if ( les == 1 ) {
			
				mvprintw(STARTY - 2, STARTX, "Let's drill, starting from the home row.");
				refresh();
				test_array = "hash half hag gad gash glass sash slash shall hall gas gaff";
			}
			
			if ( les == 2 ) {
			
				mvprintw(STARTY - 2, STARTX, "Some words :");
				refresh();
				test_array = "jug part ye aqua their hearty dry great why whose youth sun";
			}
			
			if ( les == 3 ) {
			
				mvprintw(STARTY - 2, STARTX, "Practice! ");
				refresh();
				test_array = "Violet, Helen, Dora, Jenny, and Elsa are staying at the Inn";
			}
			
			if ( les == 4 ) {
			
				mvprintw(STARTY - 2, STARTX, " S combinations:");
				refresh();
				test_array = "sas ses sis sos sus sc scr sh sk sl sm sn sp spl spr st str";
			}
			
			if ( les == 5 ) {
			
				mvprintw(STARTY - 2, STARTX, " R combinations:");
				refresh();
				test_array = "reef rare rural proper rear trader drier roar freer cracker";
			}
			
			if ( les == 6 ) {
			
				mvprintw(STARTY - 2, STARTX, " L combinations:");
				refresh();
				test_array = "low loll play blow lisle glass flesh slow fully silly sleep";
			}
			
			if ( les == 7 ) {
			
				mvprintw(STARTY - 2, STARTX, " Drill on D-T :");
				refresh();
				test_array = "deed lend toot tutor this kept melted handed doubted treats";
			}
			
			if ( les == 8 ) {
			
				mvprintw(STARTY - 2, STARTX, " M-N combinations:");
				refresh();
				test_array = "many amount damper only names payment moment immense emblem";
			}
			
			if ( les == 9 ) {
			
				mvprintw(STARTY - 2, STARTX, " Drill on com-con :");
				refresh();
				test_array = "com recom decom discom incom comply comment comfort connect";
			}
			
			if ( les == 10 ) {
			
				mvprintw(STARTY - 2, STARTX, "Practice :");
				refresh();
				test_array = "ater etor rather other eture ister feature fixture register";
			}
			
			if ( les == 11 ) {
				ch = KEY_ESCAPE;
				continue;
			}
				
			
			typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			box(typing_win, 0, 0);
			
			x = 1;
			y = 1;
			mvwprintw(typing_win, y, x, "%s", test_array);
			wrefresh(typing_win);
			y += 1;

			mistakes = 0;
			i = 0;
			time(&start_t);
			wmove(typing_win, y, x);
			wrefresh(typing_win);
			ch = 0;
			while(ch != KEY_ESCAPE && i != 59)
			{	ch = wgetch(typing_win);
				
				if(ch == test_array[i])
				{	++i;
					mvwprintw(typing_win, y, x, "%c", ch);
					wrefresh(typing_win);
					++x;
					continue;
				}
				else
				{	
					wattron(typing_win, A_REVERSE);
					mvwprintw(typing_win, y, x, "%c", ch);
					wattroff(typing_win, A_REVERSE);
					wrefresh(typing_win);
					++x;
					++mistakes;
					++i;
				}
			}

			time(&end_t);
			print_time(start_t, end_t, mistakes, 59);
			
			// wait for a key press before quitting
			ch = wgetch(typing_win);
		}
		
		if ( choice == LONG ) {
			char *lessons[] = { "Lesson 1     Practise", "Lesson 2     Practise", "Lesson 3     Practise", "Lesson 4     Practise" , "Lesson 5     Practise" , "Lesson 6     Practise" , "Lesson 7     Practise" , "Lesson 8     Practise" , "Lesson 9     Practise" , "Lesson 10     Practise" };
			int les, i;
	
			les = 0;
			while(1) {
				clear();
				printw("\n\n");
				attron(COLOR_PAIR(1));
				print_in_middle(1, 1, 0, "The R series contains the following 10 lessons ", NULL);
				attroff(COLOR_PAIR(1));
				
				printw("\n\n\n");
				for(i = 0;i <= (9); ++i)
					printw("\t%3d: \t %s\n", i + 1, lessons[i]);
				printw("\t%3d: \tExit\n", i + 1);
	
				printw("\n\n\tChoice: ");
				refresh();
				echo();
				scanw("%d", &les);
				noecho();
	
				if(les >= 1 && les <= 11)
					break;
				else {
					attron(A_REVERSE);
					mvprintw(STATUSY, STATUSX, "Wrong choice\tPress any key to continue");
					attroff(A_REVERSE);
					ch = getch();
				}
			}
			
			clear(); 
			
			strcpy(string, "Typing window");
			attron(COLOR_PAIR(1));
			print_in_middle(STARTX, STARTY - 2, LENGTH, string, NULL);
			attroff(COLOR_PAIR(1));
			
			attron(A_REVERSE);
			mvprintw(STATUSY, STATUSX, "Press ESC to Main Menu");
			refresh();
			attroff(A_REVERSE);

			
			typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			box(typing_win, 0, 0);
			
			if ( les == 1 ) {
			
				mvprintw(STARTY - 2, STARTX, "Work on your speed and accuracy.");
				refresh();
				test_array = "Up to date I can't see that the car is as good as this one.";
			}
			
			if ( les == 2 ) {
			
				mvprintw(STARTY - 2, STARTX, "Warming up Drill :");
				refresh();
				test_array = "It's a good deed to use good habits each time you practice.";
			}
			
			if ( les == 3 ) {
			
				mvprintw(STARTY - 2, STARTX, "Warming up Drill :");
				refresh();
				test_array = "As a rule call earlier on all orders as long as these ones.";
			}
			
			if ( les == 4 ) {
			
				mvprintw(STARTY - 2, STARTX, "Warming up Drill :");
				refresh();
				test_array = "After that date I believe he can't say that it isnt a fact.";
			}
			
			if ( les == 5 ) {
			
				mvprintw(STARTY - 2, STARTX, "Warming up Drill :");
				refresh();
				test_array = "In any case there is only this order that is long past due.";
			}
			
			if ( les == 6 ) {
			
				mvprintw(STARTY - 2, STARTX, "Focus on the special characters!");
				refresh();
				test_array = "ki9 de3 ju8 fr4 ju7 fr5 jy6 ft6 lo0 sw2 ju7 fr5 jy6 ft6 ;p-";
			}
			
			if ( les == 7 ) {
			
				mvprintw(STARTY - 2, STARTX, "Focus on the special characters!");
				refresh();
				test_array = "ki9 de3 ju8 fr4 ju7 fr5 jy6 ft6 lo0 sw2 ju7 fr5 jy6 ft6 ;p-";
			}
			
			if ( les == 8 ) {
			
				mvprintw(STARTY - 2, STARTX, "Continuous copy");
				refresh();
				test_array = "To buy an air-mail stamp I save 1, 2, 4 and mostly 6 cents.";
			}
			
			if ( les == 9 ) {
			
				mvprintw(STARTY - 2, STARTX, "Focus on the special characters!");
				refresh();
				test_array = "k( d# j* f$ j& f% j^ f^ l) s@ ;_ s@ ;{ d# ;@ f% ;? f^ j^ s@";
			}
			if ( les == 10 ) {
			
				mvprintw(STARTY - 2, STARTX, "Focus on the special characters!");
				refresh();
				test_array = "'A 'S 'O *W :E :C ;P &F ^I %M $E #B e5@ t4@ u3! h2+ u1' n6'";
			}
			
			if ( les == 11 ) {
				ch = KEY_ESCAPE;
				continue;
			}
			
			typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			box(typing_win, 0, 0);
			
			x = 1;
			y = 1;
			mvwprintw(typing_win, y, x, "%s", test_array);
			wrefresh(typing_win);
			y += 1;

			mistakes = 0;
			i = 0;
			time(&start_t);
			wmove(typing_win, y, x);
			wrefresh(typing_win);
			ch = 0;
			while(ch != KEY_ESCAPE && i != 59)
			{	ch = wgetch(typing_win);
				
				if(ch == test_array[i])
				{	++i;
					mvwprintw(typing_win, y, x, "%c", ch);
					wrefresh(typing_win);
					++x;
					continue;
				}
				else
				{	
					wattron(typing_win, A_REVERSE);
					mvwprintw(typing_win, y, x, "%c", ch);
					wattroff(typing_win, A_REVERSE);
					wrefresh(typing_win);
					++x;
					++mistakes;
					++i;
				}
			}

			time(&end_t);
			print_time(start_t, end_t, mistakes, 59);
			
			// wait for a key press before quitting
			ch = wgetch(typing_win);
		}
		
		if ( choice == DVORAK ) {
			char *lessons[] = {"Lesson D1     The home row" , "Lesson D2     Learning the Shift Key" , "Lesson D3     Home Row and the Period" , "Lesson D4     Upper Row and Essential Punctuation" , "Lesson D5     Review" , "Lesson D6     Learning the Lower Row" , "Lesson D7     Practise" }; 
			int les, i;
	
			les = 0;
			while(1) {
				clear();
				printw("\n\n");
				attron(COLOR_PAIR(1));
				print_in_middle(1, 1, 0, "The D series contains the following 7 lessons ", NULL);
				attroff(COLOR_PAIR(1));
				
				printw("\n\n\n");
				for(i = 0;i <= (6); ++i)
					printw("\t%3d: \t %s\n", i + 1, lessons[i]);
				printw("\t%3d: \tExit\n", i + 1);
	
				printw("\n\n\tChoice: ");
				refresh();
				echo();
				scanw("%d", &les);
				noecho();
	
				if(les >= 1 && les <= 8)
					break;
				else {
					attron(A_REVERSE);
					mvprintw(STATUSY, STATUSX, "Wrong choice\tPress any key to continue");
					attroff(A_REVERSE);
					ch = getch();
				}
			}
			
			clear(); 
			
			strcpy(string, "Typing window");
			attron(COLOR_PAIR(1));
			print_in_middle(STARTX, STARTY - 2, LENGTH, string, NULL);
			attroff(COLOR_PAIR(1));
			attron(A_REVERSE);
			mvprintw(STATUSY, STATUSX, "Press ESC to Main Menu");
			refresh();
			attroff(A_REVERSE);

			
			typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			box(typing_win, 0, 0);
			
			if ( les == 1 ) {
			
				mvprintw(STARTY - 2, STARTX, "Home row exercises");
				refresh();
				test_array = "aoeu ueoa aoeui snth htnsa asonetuhid dihutenosa dihutenosa";
			} 
			
			if ( les == 2 ) {
			
				mvprintw(STARTY - 2, STARTX, "Using the shift key.");
				refresh();
				test_array = "On his date Ethan had a thin suit and no tint on his shoes.";
			}   
			
			if ( les == 3 ) {
			
				mvprintw(STARTY - 2, STARTX, "Home row and period.");
				refresh();
				test_array = "Adana.  Andy.  Annie.  Aona.  Audie.  Denis.  Diana.  Dina.";
			}
			
			if ( les == 4 ) {
			
				mvprintw(STARTY - 2, STARTX, "Essential Puntuation.");
				refresh();
				test_array = "For atop fee, her act is to hop in, lie on the rug and sit.";
			}   
			
			                        
                        if ( les == 5 ) {
			
				mvprintw(STARTY - 2, STARTX, "Review :");
				refresh();
				test_array = "He paid in cash for the cap, tomatoes, ales, rice and fats.";
			}                
                              
                        if ( les == 6 ) {
			
				mvprintw(STARTY - 2, STARTX, "Learning the lower row.");
				refresh();
				test_array = "At the zoo, the wee, bad boa in a box bit Tom, Boby and me.";
			}                                   
                                           
                	if ( les == 7 ) {
			
				mvprintw(STARTY - 2, STARTX, "Practice :");
				refresh();
				test_array = "Six quizzical sheep kept their jaws dry in a farm bungalow.";
			}
			
			if ( les == 8 ) {
				ch = KEY_ESCAPE;
				continue;
			}
			
			typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			box(typing_win, 0, 0);
			
			x = 1;
			y = 1;
			mvwprintw(typing_win, y, x, "%s", test_array);
			wrefresh(typing_win);
			y += 1;

			mistakes = 0;
			i = 0;
			time(&start_t);
			wmove(typing_win, y, x);
			wrefresh(typing_win);
			ch = 0;
			while(ch != KEY_ESCAPE && i != 59)
			{	ch = wgetch(typing_win);
				
				if(ch == test_array[i])
				{	++i;
					mvwprintw(typing_win, y, x, "%c", ch);
					wrefresh(typing_win);
					++x;
					continue;
				}
				else
				{	
					wattron(typing_win, A_REVERSE);
					mvwprintw(typing_win, y, x, "%c", ch);
					wattroff(typing_win, A_REVERSE);
					wrefresh(typing_win);
					++x;
					++mistakes;
					++i;
				}
			}

			time(&end_t);
			print_time(start_t, end_t, mistakes, 59);
			
			// wait for a key press before quitting
			ch = wgetch(typing_win);
		}

		if ( choice == SPEEDTEST ) {  //typing complete para in box
			/* do text wrapping */
			
			int index;
			strcpy(string, "Typing window");
			attron(COLOR_PAIR(1));
			print_in_middle(STARTX, STARTY - 2, LENGTH, string, NULL);
			attroff(COLOR_PAIR(1));
			
			attron(A_REVERSE);
			mvprintw(STATUSY, STATUSX, "Press ESC to Main Menu");
			refresh();
			attroff(A_REVERSE);

		
			srand(time(NULL));
	
			index = rand() % 4;
	
			switch (index) {
				case 0:
					test_array = "We are social creatures to the inmost centre of our being. The notion that a man can begin anything at all from scratch, free from the past, or unindebted to others, could not conceivably be more wrong. Never forget what you are, for surely the world will not. Make it your strength. Armor yourself in it, and it will never be used to hurt you.";
			
					break;
				case 1:
					test_array = "You must go back with me to 1827.My father, as you know, was a sort of gentleman farmer in -shire; and I, by his express desire, succeeded him in the same quiet occupation, not very willingly, for ambition urged me to higher aims, and self-conceit assured me that, I was burying my talent in the earth, and hiding my light, deep under a bushel.";
			
					break;
				case 2:
					test_array = "Coming in from the light, I am all out at the eyes. Such white doves were paddling in the sunshine and the trees were as bright as broken glass.Out there, in the sparkling air, the sun and the rain clash together like the cymbals clashing when David did his dance. I've an April blindness. You are hidden in a cloud of crimson catherine-wheels.";
			
					break;
				case 3:
					test_array = "As you know, our organization has made a commitment to continual training of all personnel. We've maintained our edge against the increasing numbers of competitors by staying ahead technologically. This has contributed greatly to our success. Keeping up with technology also means that on the job training is very vital to a successful company.";
			
					break;
			}
			
			typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			box(typing_win, 0, 0);
			
			
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
				for( i = 0; i < (ARR_LENGTH - 1) ; i++ ) {
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
			while(ch != KEY_ESCAPE && i < HSIZE + 1)
			{	ch = wgetch(typing_win);
			
				if ( ch == KEY_ENT ) {
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
					wattron(typing_win, A_REVERSE);
					mvwprintw(typing_win, y, x, "%c", ch);
					wattroff(typing_win, A_REVERSE);
					wrefresh(typing_win);
					++x;
					++mistakes;
					++i;
				}
			}
	
			time(&end_t);
			print_time(start_t, end_t, mistakes, HSIZE);
			// wait for a key press before quitting
			ch = wgetch(typing_win);
		}
		
		 if ( choice == DRILL ) {    
			

	    		int text_length;
	
	    		int k, j, max_x, max_y, index;
	    		
	    		strcpy(string, "Typing window");
	    		attron(COLOR_PAIR(1));
			print_in_middle(STARTX, STARTY - 2, LENGTH, string, NULL);
			attroff(COLOR_PAIR(1));
			attron(A_REVERSE);
			mvprintw(STATUSY, STATUSX, "Press ESC to Main Menu");
			refresh();
			attroff(A_REVERSE);

			
	    		srand(time(NULL));
	
			index = rand() % 4;
	
			switch (index) {
				case 0:
					test_array = "We are social creatures to the inmost centre of our being. The notion that a man can begin anything at all from scratch, free from the past, or unindebted to others, could not conceivably be more wrong. Never forget what you are, for surely the world will not. Make it your strength. Armor yourself in it, and it will never be used to hurt you.";
			
					break;
				case 1:
					test_array = "You must go back with me to 1827.My father, as you know, was a sort of gentleman farmer in -shire; and I, by his express desire, succeeded him in the same quiet occupation, not very willingly, for ambition urged me to higher aims, and self-conceit assured me that, I was burying my talent in the earth, and hiding my light, deep under a bushel.";
			
					break;
				case 2:
					test_array = "Coming in from the light, I am all out at the eyes. Such white doves were paddling in the sunshine and the trees were as bright as broken glass.Out there, in the sparkling air, the sun and the rain clash together like the cymbals clashing when David did his dance. I've an April blindness. You are hidden in a cloud of crimson catherine-wheels.";
			
					break;
				case 3:
					test_array = "As you know, our organization has made a commitment to continual training of all personnel. We've maintained our edge against the increasing numbers of competitors by staying ahead technologically. This has contributed greatly to our success. Keeping up with technology also means that on the job training is very vital to a successful company.";
			
					break;
			}
	    		
	   		text_length = strlen( test_array );
	   		
	    		typing_win = newwin(WIDTH, LENGTH, STARTY, STARTX);
			keypad(typing_win, TRUE);
			intrflush(typing_win, FALSE);
			curs_set(0);
			box(typing_win, 0, 0);

	    		getmaxyx( typing_win, max_y, max_x);
	    		
	    		max_y *= 1;
			
			wrefresh(typing_win);

		    // Scroll text back across the screen
			j = strlen(test_array);
			mistakes = 0;
		
			time(&start_t);
			for (k = 0; k < text_length; k++) {
				wmove( typing_win, 3, 1 );
				
				wclrtoeol(typing_win);  //clearing one line
				 
				if ( j > max_x )
					mvwaddnstr( typing_win, 3, 1, &test_array[k], max_x - 1 );
				else
					mvwaddnstr(typing_win, 3, 1, &test_array[k], j );
				j--;
				wrefresh(typing_win);
				usleep(90000);
				ch = wgetch(typing_win);
				if (ch == test_array[k]) {
					mvwprintw(typing_win, 5, 1, "%c", ch);
					wrefresh(typing_win);
					continue;
				}
				else {
					wattron(typing_win, A_REVERSE);
					mvwprintw(typing_win, 5, 1, "%c", ch);
					wattroff(typing_win, A_REVERSE);
					wrefresh(typing_win);
					++mistakes;
				}
			   	               
			}
			
			time(&end_t);
			print_time(start_t, end_t, mistakes, HSIZE);
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
		attron(COLOR_PAIR(1));
		print_in_middle(1, 1, 0, " Welcome to gtypist ", NULL);
		attroff(COLOR_PAIR(1));
		printw("\n\n\n");
		for(i = 0;i <= (n_groups - 1); ++i)
			printw("\t%3d: \tPractice %s\n\n", i + 1, groups[i]);
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




	
void print_byebye()
{	printw("\n");
	print_in_middle(0,0,0,"Thank you for using my typing tutor\n", NULL);
	print_in_middle(0,0,0,"Bye.\n", NULL);
	refresh();
}
		
void print_time(time_t start_t, time_t end_t, int mistakes, int letters)
{	long int diff;
	int h,m,s;
	float pm, error;

	diff = end_t - start_t;
	
	if ( state == 0 )
		pm = (( letters / 5)/((double)diff))*60;
	if ( state == 1 )
		pm = (( letters )/((double)diff))*60;
	

	h = (int)(diff / 3600);
    	diff -= h * 3600;
	m = (int)(diff / 60);
	diff -= m * 60;
	s = (int)diff; 
	
	error = ( mistakes / letters ) * 100;
	
	if ( error > DEF_ERR ) {
		attron(A_BOLD);
		attron(COLOR_PAIR(2));
		mvprintw(STATUSY - 2, STATUSX, "Error Percentage is high. ( %f % )", error);
		attroff(COLOR_PAIR(2));
		attroff(A_BOLD);
	}
	
	attron(A_BOLD);
	
	attron(COLOR_PAIR(2));
	if ( state == 0 )
		mvprintw(STATUSY, STATUSX, "MISTAKES: %d  TIME Taken: %d:%d:%d  WPM : %.2f    Press any Key to continue", mistakes, h, m, s, 	pm);
	else if ( state == 1 )
		mvprintw(STATUSY, STATUSX, "MISTAKES: %d  TIME Taken: %d:%d:%d  CPM : %.2f    Press any Key to continue", mistakes, h, m, s, 	pm);
	attroff(COLOR_PAIR(2));
	
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
