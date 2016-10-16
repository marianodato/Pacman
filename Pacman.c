/*!
*	Copyright (c) 2016 Mariano Dato marianodato@gmail.com
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
*	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
*	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
*	IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*	THE SOFTWARE.
*/

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 153  
#define SCREEN_HEIGHT 38
#define PACMAN_WIDTH 14
#define PACMAN_HEIGHT 7

void init_curses();
void init_window(WINDOW *, WINDOW *, WINDOW *, int, int, int, int);
int end_window(WINDOW *, WINDOW *, int, int, int, int, int, char*);
void create_pacman(WINDOW *,int, int, int, int);
int eat_food(int, int, int, int);
void generate_food(int *, int *);
void move_good_pacman(int, int *, int*, int, int *);
void move_bad_pacman(int *, int *, int, int, int, int *);
void keep_inside_screen(int *, int *);

int main()
{
	WINDOW *food = NULL;
	WINDOW *score_good_pacman = NULL;
	WINDOW *score_bad_pacman = NULL;
	WINDOW *finish = NULL;

	int x_good_pacman = 0; 
	int y_good_pacman = 0;
	int x_bad_pacman = SCREEN_WIDTH;
	int y_bad_pacman = SCREEN_HEIGHT;
	int key = 0;
	int x_food = SCREEN_WIDTH/2+5;
	int y_food = SCREEN_HEIGHT/2+2;
	int velocity = 4;
	int score_good = 0;
	int score_bad = 0;
	int direction_good_pacman = 0;
	int direction_bad_pacman = 2;
	int color = 3;
	
	init_curses();

	while (1)
	{
		if((-1 < x_good_pacman && x_good_pacman < (SCREEN_WIDTH-PACMAN_WIDTH)+1  && -1 < y_good_pacman && y_good_pacman < (SCREEN_HEIGHT-PACMAN_HEIGHT)+1) &&
		   (-1 < x_bad_pacman && x_bad_pacman < (SCREEN_WIDTH-PACMAN_WIDTH)+1  && -1 < y_bad_pacman && y_bad_pacman < (SCREEN_HEIGHT-PACMAN_HEIGHT)+1))
		{
			init_window(food, score_good_pacman, score_bad_pacman, x_food, y_food, score_good, score_bad);

			if (score_good == 100)
			{	
				end_window (finish, stdscr,1,30,(SCREEN_HEIGHT/2),(SCREEN_WIDTH/2)-10,3,"           YOU WIN!");
				return 0;
			}

			if (score_bad == 100)
			{	
				end_window (finish,stdscr,1,30,(SCREEN_HEIGHT/2),(SCREEN_WIDTH/2)-10,5,"           YOU LOSE!");
				return 0;
			}

			refresh();
			
			create_pacman(stdscr,x_good_pacman,y_good_pacman, direction_good_pacman, color);
			
			create_pacman(stdscr,x_bad_pacman,y_bad_pacman, direction_bad_pacman, 5);
			
			if (eat_food(x_good_pacman, y_good_pacman, x_food, y_food) == 1)
			{
				generate_food(&x_food, &y_food);
				score_good = score_good + 10;

			}

			key = getch();

			move_good_pacman(key, &x_good_pacman, &y_good_pacman, velocity, &direction_good_pacman);	

			if (eat_food(x_bad_pacman, y_bad_pacman, x_food, y_food) == 1)
			{
				generate_food(&x_food,&y_food);
				score_bad = score_bad + 10;
			}
			
			move_bad_pacman(&x_bad_pacman, &y_bad_pacman,x_food,y_food,velocity,&direction_bad_pacman);

		}else{
			keep_inside_screen(&x_good_pacman,&y_good_pacman);			
			keep_inside_screen(&x_bad_pacman,&y_bad_pacman);
		}
	}

	return 0;
}

void init_curses()
{
    initscr();
    start_color();
    init_pair(1,COLOR_RED,COLOR_BLUE);
    init_pair(2,COLOR_BLUE,COLOR_BLACK);
    init_pair(3,COLOR_BLACK,COLOR_YELLOW);
    init_pair(4,COLOR_BLACK,COLOR_GREEN);
    init_pair(5,COLOR_BLACK,COLOR_WHITE);
    raw();
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);

    return;
}

void init_window(WINDOW *food, WINDOW *score_good_pacman, WINDOW *score_bad_pacman, int x_food, int y_food, int score_good, int score_bad)
{

	bkgd(COLOR_PAIR(1));
	food = subwin(stdscr,2,4,y_food,x_food);
	wbkgd(food,COLOR_PAIR(4));
			
	score_good_pacman = subwin(stdscr,1,30,0,(SCREEN_WIDTH/4)-15);
	score_bad_pacman = subwin(stdscr,1,30,0,SCREEN_WIDTH-(SCREEN_WIDTH/4)-10);
	
	wbkgd(score_good_pacman,COLOR_PAIR(3));
	wbkgd(score_bad_pacman,COLOR_PAIR(5));
			
	wprintw(score_good_pacman, "         YOUR SCORE: %d", score_good);
	wprintw(score_bad_pacman, "        ENEMY SCORE: %d", score_bad);

	return;
}

int end_window(WINDOW *finish, WINDOW *ventana, int lineas, int columnas, int x_good_pacman, int y_good_pacman, int color, char* texto)
{
	finish = subwin(stdscr,lineas,columnas,x_good_pacman,y_good_pacman);
	wbkgd(finish,COLOR_PAIR(color));
	wprintw(finish, texto);
	refresh();			
   	getch();			
	endwin();

	return 0;
}

void create_pacman (WINDOW *pacman,int x_good_pacman,int y_good_pacman, int direction_good_pacman, int color)
{	
	if (direction_good_pacman == 1)
	{
		WINDOW *A1;
		A1 = subwin(pacman,7,1,4+y_good_pacman,0+x_good_pacman);
		wbkgd(A1,COLOR_PAIR(color));
		wrefresh(A1);
		WINDOW *A2;
		A2 = subwin(pacman,9,1,3+y_good_pacman,1+x_good_pacman);
		wbkgd(A2,COLOR_PAIR(color));
		wrefresh(A2);
		WINDOW *A3;
		A3 = subwin(pacman,5,1,2+y_good_pacman,2+x_good_pacman);
		wbkgd(A3,COLOR_PAIR(color));
		wrefresh(A3);
		WINDOW *A4;
		A4 = subwin(pacman,4,1,8+y_good_pacman,2+x_good_pacman);
		wbkgd(A4,COLOR_PAIR(color));
		wrefresh(A4);
		WINDOW *A5;
		A5 = subwin(pacman,11,1,1+y_good_pacman,3+x_good_pacman);
		wbkgd(A5,COLOR_PAIR(color));
		wrefresh(A5);
		WINDOW *A6;
		A6 = subwin(pacman,10,1,1+y_good_pacman,4+x_good_pacman);
		wbkgd(A6,COLOR_PAIR(color));
		wrefresh(A6);
		WINDOW *A7;
		A7 = subwin(pacman,10,1,0+y_good_pacman,5+x_good_pacman);
		wbkgd(A7,COLOR_PAIR(color));
		wrefresh(A7);
		WINDOW *A8;
		A8 = subwin(pacman,8,1,0+y_good_pacman,6+x_good_pacman);
		wbkgd(A8,COLOR_PAIR(color));
		wrefresh(A8);
		WINDOW *A9;
		A9 = subwin(pacman,7,1,0+y_good_pacman,7+x_good_pacman);
		wbkgd(A9,COLOR_PAIR(color));
		wrefresh(A9);
		WINDOW *A10;
		A10 = subwin(pacman,6,1,0+y_good_pacman,8+x_good_pacman);
		wbkgd(A10,COLOR_PAIR(color));
		wrefresh(A10);
		WINDOW *A11;
		A11 = subwin(pacman,6,1,0+y_good_pacman,9+x_good_pacman);
		wbkgd(A11,COLOR_PAIR(color));
		wrefresh(A11);
		WINDOW *A12;
		A12 = subwin(pacman,7,1,0+y_good_pacman,10+x_good_pacman);
		wbkgd(A12,COLOR_PAIR(color));
		wrefresh(A12);
		WINDOW *A13;
		A13 = subwin(pacman,8,1,0+y_good_pacman,11+x_good_pacman);
		wbkgd(A13,COLOR_PAIR(color));
		wrefresh(A13);
		WINDOW *A14;
		A14 = subwin(pacman,9,1,0+y_good_pacman,12+x_good_pacman);
		wbkgd(A14,COLOR_PAIR(color));
		wrefresh(A14);
		WINDOW *A15;
		A15 = subwin(pacman,10,1,0+y_good_pacman,13+x_good_pacman);
		wbkgd(A15,COLOR_PAIR(color));
		wrefresh(A15);
		WINDOW *A16;
		A16 = subwin(pacman,10,1,1+y_good_pacman,14+x_good_pacman);
		wbkgd(A16,COLOR_PAIR(color));
		wrefresh(A16);
		WINDOW *A17;
		A17 = subwin(pacman,11,1,1+y_good_pacman,15+x_good_pacman);
		wbkgd(A17,COLOR_PAIR(color));
		wrefresh(A17);
		WINDOW *A18;
		A18 = subwin(pacman,10,1,2+y_good_pacman,16+x_good_pacman);
		wbkgd(A18,COLOR_PAIR(color));
		wrefresh(A18);
		WINDOW *A19;
		A19 = subwin(pacman,9,1,3+y_good_pacman,17+x_good_pacman);
		wbkgd(A19,COLOR_PAIR(color));
		wrefresh(A19);
		WINDOW *A20;
		A20 = subwin(pacman,7,1,4+y_good_pacman,18+x_good_pacman);
		wbkgd(A20,COLOR_PAIR(color));
		wrefresh(A20);

	}else if (direction_good_pacman == 3)
	{
		WINDOW *A1;
		A1 = subwin(pacman,7,1,1+y_good_pacman,0+x_good_pacman);
		wbkgd(A1,COLOR_PAIR(color));
		wrefresh(A1);
		WINDOW *A2;
		A2 = subwin(pacman,9,1,0+y_good_pacman,1+x_good_pacman);
		wbkgd(A2,COLOR_PAIR(color));
		wrefresh(A2);
		WINDOW *A3;
		A3 = subwin(pacman,10,1,0+y_good_pacman,2+x_good_pacman);
		wbkgd(A3,COLOR_PAIR(color));
		wrefresh(A3);
		WINDOW *A4;
		A4 = subwin(pacman,11,1,0+y_good_pacman,3+x_good_pacman);
		wbkgd(A4,COLOR_PAIR(color));
		wrefresh(A4);
		WINDOW *A5;
		A5 = subwin(pacman,10,1,1+y_good_pacman,4+x_good_pacman);
		wbkgd(A5,COLOR_PAIR(color));
		wrefresh(A5);
		WINDOW *A6;
		A6 = subwin(pacman,10,1,2+y_good_pacman,5+x_good_pacman);
		wbkgd(A6,COLOR_PAIR(color));
		wrefresh(A6);
		WINDOW *A7;
		A7 = subwin(pacman,9,1,3+y_good_pacman,6+x_good_pacman);
		wbkgd(A7,COLOR_PAIR(color));
		wrefresh(A7);
		WINDOW *A8;
		A8 = subwin(pacman,8,1,4+y_good_pacman,7+x_good_pacman);
		wbkgd(A8,COLOR_PAIR(color));
		wrefresh(A8);
		WINDOW *A9;
		A9 = subwin(pacman,7,1,5+y_good_pacman,8+x_good_pacman);
		wbkgd(A9,COLOR_PAIR(color));
		wrefresh(A9);
		WINDOW *A10;
		A10 = subwin(pacman,6,1,6+y_good_pacman,9+x_good_pacman);
		wbkgd(A10,COLOR_PAIR(color));
		wrefresh(A10);
		WINDOW *A11;
		A11 = subwin(pacman,6,1,6+y_good_pacman,10+x_good_pacman);
		wbkgd(A11,COLOR_PAIR(color));
		wrefresh(A11);
		WINDOW *A12;
		A12 = subwin(pacman,7,1,5+y_good_pacman,11+x_good_pacman);
		wbkgd(A12,COLOR_PAIR(color));
		wrefresh(A12);
		WINDOW *A13;
		A13 = subwin(pacman,8,1,4+y_good_pacman,12+x_good_pacman);
		wbkgd(A13,COLOR_PAIR(color));
		wrefresh(A13);
		WINDOW *A14;
		A14 = subwin(pacman,10,1,2+y_good_pacman,13+x_good_pacman);
		wbkgd(A14,COLOR_PAIR(color));
		wrefresh(A14);
		WINDOW *A15;
		A15 = subwin(pacman,10,1,1+y_good_pacman,14+x_good_pacman);
		wbkgd(A15,COLOR_PAIR(color));
		wrefresh(A15);
		WINDOW *A16;
		A16 = subwin(pacman,11,1,0+y_good_pacman,15+x_good_pacman);
		wbkgd(A16,COLOR_PAIR(color));
		wrefresh(A16);
		WINDOW *A17;
		A17 = subwin(pacman,4,1,0+y_good_pacman,16+x_good_pacman);
		wbkgd(A17,COLOR_PAIR(color));
		wrefresh(A17);
		WINDOW *A18;
		A18 = subwin(pacman,5,1,5+y_good_pacman,16+x_good_pacman);
		wbkgd(A18,COLOR_PAIR(color));
		wrefresh(A18);
		WINDOW *A19;
		A19 = subwin(pacman,9,1,0+y_good_pacman,17+x_good_pacman);
		wbkgd(A19,COLOR_PAIR(color));
		wrefresh(A19);
		WINDOW *A20;
		A20 = subwin(pacman,7,1,1+y_good_pacman,18+x_good_pacman);
		wbkgd(A20,COLOR_PAIR(color));
		wrefresh(A20);
	}else if(direction_good_pacman == 0)
	{
		WINDOW *A1;
		A1 = subwin(pacman,4,1,4+y_good_pacman,0+x_good_pacman);
		wbkgd(A1,COLOR_PAIR(color));
		wrefresh(A1);
		WINDOW *A2;
		A2 = subwin(pacman, 8,1,2+y_good_pacman,1+x_good_pacman);
		wbkgd(A2,COLOR_PAIR(color));
		wrefresh(A2);
		WINDOW *A3;
		A3 = subwin(pacman,8,1,2+y_good_pacman,2+x_good_pacman);
		wbkgd(A3,COLOR_PAIR(color));
		wrefresh(A3);
		WINDOW *A4;
		A4 = subwin(pacman,10,1,1+y_good_pacman,3+x_good_pacman);
		wbkgd(A4,COLOR_PAIR(color));
		wrefresh(A4);
		WINDOW *A5;
		A5 = subwin(pacman,10,1,1+y_good_pacman,4+x_good_pacman);
		wbkgd(A5,COLOR_PAIR(color));
		wrefresh(A5);
		WINDOW *A6;
		A6 = subwin(pacman,12,1,0+y_good_pacman,5+x_good_pacman);
		wbkgd(A6,COLOR_PAIR(color));
		wrefresh(A6);
		WINDOW *A7;
		A7 = subwin(pacman,12,1,0+y_good_pacman,6+x_good_pacman);
		wbkgd(A7,COLOR_PAIR(color));
		wrefresh(A7);
		WINDOW *A8;
		A8 = subwin(pacman,12,1,0+y_good_pacman,7+x_good_pacman);
		wbkgd(A8,COLOR_PAIR(color));
		wrefresh(A8);
		WINDOW *A9;
		A9 = subwin(pacman,12,1,0+y_good_pacman,8+x_good_pacman);
		wbkgd(A9,COLOR_PAIR(color));
		wrefresh(A9);
		WINDOW *A10;
		A10 = subwin(pacman,12,1,0+y_good_pacman,9+x_good_pacman);
		wbkgd(A10,COLOR_PAIR(color));
		wrefresh(A10);
		WINDOW *A11;
		A11 = subwin(pacman,2,1,0+y_good_pacman,10+x_good_pacman);
		wbkgd(A11,COLOR_PAIR(color));
		wrefresh(A11);
		WINDOW *A12;
		A12 = subwin(pacman,2,1,3+y_good_pacman,10+x_good_pacman);
		wbkgd(A12,COLOR_PAIR(color));
		wrefresh(A12);
		WINDOW *A13;
		A13 = subwin(pacman,5,1,7+y_good_pacman,10+x_good_pacman);
		wbkgd(A13,COLOR_PAIR(color));
		wrefresh(A13);
		WINDOW *A14;
		A14 = subwin(pacman,5,1,0+y_good_pacman,11+x_good_pacman);
		wbkgd(A14,COLOR_PAIR(color));
		wrefresh(A14);
		WINDOW *A15;
		A15 = subwin(pacman,5,1,7+y_good_pacman,11+x_good_pacman);
		wbkgd(A15,COLOR_PAIR(color));
		wrefresh(A15);
		WINDOW *A16;
		A16 = subwin(pacman,4,1,0+y_good_pacman,12+x_good_pacman);
		wbkgd(A16,COLOR_PAIR(color));
		wrefresh(A16);
		WINDOW *A17;
		A17 = subwin(pacman,4,1,8+y_good_pacman,12+x_good_pacman);
		wbkgd(A17,COLOR_PAIR(color));
		wrefresh(A17);
		WINDOW *A18;
		A18 = subwin(pacman,4,1,0+y_good_pacman,13+x_good_pacman);
		wbkgd(A18,COLOR_PAIR(color));
		wrefresh(A18);
		WINDOW *A19;
		A19 = subwin(pacman,4,1,8+y_good_pacman,13+x_good_pacman);
		wbkgd(A19,COLOR_PAIR(color));
		wrefresh(A19);
		WINDOW *A20;
		A20 = subwin(pacman,4,1,0+y_good_pacman,14+x_good_pacman);
		wbkgd(A20,COLOR_PAIR(color));
		wrefresh(A20);
		WINDOW *A21;
		A21 = subwin(pacman,4,1,8+y_good_pacman,14+x_good_pacman);
		wbkgd(A21,COLOR_PAIR(color));
		wrefresh(A21);
		WINDOW *A22;
		A22 = subwin(pacman,3,1,1+y_good_pacman,15+x_good_pacman);
		wbkgd(A22,COLOR_PAIR(color));
		wrefresh(A22);
		WINDOW *A23;
		A23 = subwin(pacman,3,1,8+y_good_pacman,15+x_good_pacman);
		wbkgd(A23,COLOR_PAIR(color));
		wrefresh(A23);
		WINDOW *A24;
		A24 = subwin(pacman,3,1,1+y_good_pacman,16+x_good_pacman);
		wbkgd(A24,COLOR_PAIR(color));
		wrefresh(A24);
		WINDOW *A25;
		A25 = subwin(pacman,3,1,8+y_good_pacman,16+x_good_pacman);
		wbkgd(A25,COLOR_PAIR(color));
		wrefresh(A25);
		WINDOW *A26;
		A26 = subwin(pacman,2,1,2+y_good_pacman,17+x_good_pacman);
		wbkgd(A26,COLOR_PAIR(color));
		wrefresh(A26);
		WINDOW *A27;
		A27 = subwin(pacman,2,1,8+y_good_pacman,17+x_good_pacman);
		wbkgd(A27,COLOR_PAIR(color));
		wrefresh(A27);
		WINDOW *A28;
		A28 = subwin(pacman,2,1,2+y_good_pacman,18+x_good_pacman);
		wbkgd(A28,COLOR_PAIR(color));
		wrefresh(A28);
		WINDOW *A29;
		A29 = subwin(pacman,2,1,8+y_good_pacman,18+x_good_pacman);
		wbkgd(A29,COLOR_PAIR(color));
		wrefresh(A29);
	}else
	{
		WINDOW *A1;
		A1 = subwin(pacman,2,1,2+y_good_pacman,0+x_good_pacman);
		wbkgd(A1,COLOR_PAIR(color));
		wrefresh(A1);
		WINDOW *A2;
		A2 = subwin(pacman, 2,1,8+y_good_pacman,0+x_good_pacman);
		wbkgd(A2,COLOR_PAIR(color));
		wrefresh(A2);
		WINDOW *A3;
		A3 = subwin(pacman,2,1,2+y_good_pacman,1+x_good_pacman);
		wbkgd(A3,COLOR_PAIR(color));
		wrefresh(A3);
		WINDOW *A4;
		A4 = subwin(pacman,2,1,8+y_good_pacman,1+x_good_pacman);
		wbkgd(A4,COLOR_PAIR(color));
		wrefresh(A4);
		WINDOW *A5;
		A5 = subwin(pacman,3,1,1+y_good_pacman,2+x_good_pacman);
		wbkgd(A5,COLOR_PAIR(color));
		wrefresh(A5);
		WINDOW *A6;
		A6 = subwin(pacman,3,1,8+y_good_pacman,2+x_good_pacman);
		wbkgd(A6,COLOR_PAIR(color));
		wrefresh(A6);
		WINDOW *A7;
		A7 = subwin(pacman,3,1,1+y_good_pacman,3+x_good_pacman);
		wbkgd(A7,COLOR_PAIR(color));
		wrefresh(A7);
		WINDOW *A8;
		A8 = subwin(pacman,3,1,8+y_good_pacman,3+x_good_pacman);
		wbkgd(A8,COLOR_PAIR(color));
		wrefresh(A8);
		WINDOW *A9;
		A9 = subwin(pacman,4,1,0+y_good_pacman,4+x_good_pacman);
		wbkgd(A9,COLOR_PAIR(color));
		wrefresh(A9);
		WINDOW *A10;
		A10 = subwin(pacman,4,1,8+y_good_pacman,4+x_good_pacman);
		wbkgd(A10,COLOR_PAIR(color));
		wrefresh(A10);
		WINDOW *A11;
		A11 = subwin(pacman,4,1,0+y_good_pacman,5+x_good_pacman);
		wbkgd(A11,COLOR_PAIR(color));
		wrefresh(A11);
		WINDOW *A12;
		A12 = subwin(pacman,4,1,8+y_good_pacman,5+x_good_pacman);
		wbkgd(A12,COLOR_PAIR(color));
		wrefresh(A12);
		WINDOW *A13;
		A13 = subwin(pacman,4,1,0+y_good_pacman,6+x_good_pacman);
		wbkgd(A13,COLOR_PAIR(color));
		wrefresh(A13);
		WINDOW *A14;
		A14 = subwin(pacman,4,1,8+y_good_pacman,6+x_good_pacman);
		wbkgd(A14,COLOR_PAIR(color));
		wrefresh(A14);
		WINDOW *A15;
		A15 = subwin(pacman,5,1,0+y_good_pacman,7+x_good_pacman);
		wbkgd(A15,COLOR_PAIR(color));
		wrefresh(A15);
		WINDOW *A16;
		A16 = subwin(pacman,5,1,7+y_good_pacman,7+x_good_pacman);
		wbkgd(A16,COLOR_PAIR(color));
		wrefresh(A16);
		WINDOW *A17;
		A17 = subwin(pacman,2,1,0+y_good_pacman,8+x_good_pacman);
		wbkgd(A17,COLOR_PAIR(color));
		wrefresh(A17);
		WINDOW *A18;
		A18 = subwin(pacman,2,1,3+y_good_pacman,8+x_good_pacman);
		wbkgd(A18,COLOR_PAIR(color));
		wrefresh(A18);
		WINDOW *A19;
		A19 = subwin(pacman,5,1,7+y_good_pacman,8+x_good_pacman);
		wbkgd(A19,COLOR_PAIR(color));
		wrefresh(A19);
		WINDOW *A20;
		A20 = subwin(pacman,12,1,0+y_good_pacman,9+x_good_pacman);
		wbkgd(A20,COLOR_PAIR(color));
		wrefresh(A20);
		WINDOW *A21;
		A21 = subwin(pacman,12,1,0+y_good_pacman,10+x_good_pacman);
		wbkgd(A21,COLOR_PAIR(color));
		wrefresh(A21);
		WINDOW *A22;
		A22 = subwin(pacman,12,1,0+y_good_pacman,11+x_good_pacman);
		wbkgd(A22,COLOR_PAIR(color));
		wrefresh(A22);
		WINDOW *A23;
		A23 = subwin(pacman,12,1,0+y_good_pacman,12+x_good_pacman);
		wbkgd(A23,COLOR_PAIR(color));
		wrefresh(A23);
		WINDOW *A24;
		A24 = subwin(pacman,12,1,0+y_good_pacman,13+x_good_pacman);
		wbkgd(A24,COLOR_PAIR(color));
		wrefresh(A24);
		WINDOW *A25;
		A25 = subwin(pacman,10,1,1+y_good_pacman,14+x_good_pacman);
		wbkgd(A25,COLOR_PAIR(color));
		wrefresh(A25);  
		WINDOW *A26;
		A26 = subwin(pacman,10,1,1+y_good_pacman,15+x_good_pacman);
		wbkgd(A26,COLOR_PAIR(color));
		wrefresh(A26);
		WINDOW *A27;
		A27 = subwin(pacman,8,1,2+y_good_pacman,16+x_good_pacman);
		wbkgd(A27,COLOR_PAIR(color));
		wrefresh(A27);
		WINDOW *A28;
		A28 = subwin(pacman,8,1,2+y_good_pacman,17+x_good_pacman);
		wbkgd(A28,COLOR_PAIR(color));
		wrefresh(A28);
		WINDOW *A29;
		A29 = subwin(pacman,4,1,4+y_good_pacman,18+x_good_pacman);
		wbkgd(A29,COLOR_PAIR(color));
		wrefresh(A29);
	}

	return;
}

int eat_food(int x_good_pacman, int y_good_pacman, int x_food, int y_food)
{
	if(x_good_pacman < x_food && (y_good_pacman + (PACMAN_HEIGHT-2) >= y_food) && y_good_pacman < y_food && (x_good_pacman + (PACMAN_WIDTH-2) >= x_food))
		return 1;
	else
		return 0;
}

void generate_food(int *x_food, int *y_food)
{
	srand(time(NULL));
	
	*x_food= rand() %(SCREEN_WIDTH-2);
	
	while(*x_food < 12)
		*x_food= rand() %(SCREEN_WIDTH-2);
	
	*y_food= rand() %(SCREEN_HEIGHT-2);
	
	while(*y_food < 1)
		*y_food= rand() %(SCREEN_HEIGHT-2);

	return;
}

void move_good_pacman(int key, int *x_good_pacman, int*y_good_pacman, int velocity, int *direction_good_pacman)
{
	if (key == KEY_UP)
	{		
		*y_good_pacman -= velocity;
		*direction_good_pacman = 3;
	}
	else if (key == KEY_DOWN)
	{
		*y_good_pacman += velocity;
		*direction_good_pacman = 1;
	}
	else if (key == KEY_LEFT)
	{
		*x_good_pacman -= velocity;
		*direction_good_pacman = 2;
	}
	else if (key == KEY_RIGHT)
	{
		*x_good_pacman += velocity;
		*direction_good_pacman = 0;
	}else{}

	return;
}

void move_bad_pacman(int *x_bad_pacman, int *y_bad_pacman, int x_food, int y_food,int velocity,int *direction_bad_pacman)
{
	if ((*x_bad_pacman + (PACMAN_WIDTH-2)) != x_food)
	{	
		if((*x_bad_pacman + (PACMAN_WIDTH-2)) < x_food)
		{
			if(abs((*x_bad_pacman + (PACMAN_WIDTH-2)-x_food)) < velocity)
			{
				*x_bad_pacman += abs(*x_bad_pacman + (PACMAN_WIDTH-2) - x_food);
			}else
			{
				*x_bad_pacman += velocity;
				*direction_bad_pacman = 0;
			}
		}else
		{
			if(abs((*x_bad_pacman + (PACMAN_WIDTH-2)-x_food)) < velocity)
			{
				*x_bad_pacman -= abs(*x_bad_pacman + (PACMAN_WIDTH-2) - x_food);
			}else
			{
				*x_bad_pacman -= velocity;
				*direction_bad_pacman = 2;
			}
		}
	}else
	{			
		if((*y_bad_pacman + (PACMAN_HEIGHT-2)) < y_food)
		{
			if(abs((*y_bad_pacman + (PACMAN_HEIGHT-2))-y_food) < velocity)
			{
				*y_bad_pacman += abs(*y_bad_pacman + (PACMAN_HEIGHT-2) - y_food);
			}else
			{
				*y_bad_pacman += velocity;
				*direction_bad_pacman = 1;
			}
		}else
		{
			if(abs((*y_bad_pacman + (PACMAN_HEIGHT-2))-y_food) < velocity)
			{
				*y_bad_pacman -= abs(*y_bad_pacman + (PACMAN_HEIGHT-2) - y_food);
			}else
			{
				*y_bad_pacman -= velocity;
				*direction_bad_pacman = 3;
			}
		}			
	}

	return;
}

void keep_inside_screen (int *x_good_pacman, int *y_good_pacman)
{
	if (*x_good_pacman < 0)
		*x_good_pacman = 0;	
	else if (*x_good_pacman > (SCREEN_WIDTH-PACMAN_WIDTH))
		*x_good_pacman = (SCREEN_WIDTH-PACMAN_WIDTH);		
	
	if (*y_good_pacman < 0)
		*y_good_pacman = 0;	
	else if (*y_good_pacman > (SCREEN_HEIGHT-PACMAN_HEIGHT))
		*y_good_pacman = (SCREEN_HEIGHT-PACMAN_HEIGHT);

	return;
}