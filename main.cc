

#include <vector>
#include <ncurses.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <cmath>

using namespace std;

enum direction{LEFT, RIGHT, UP, DOWN};

bool IncrementVecBase(vector<int>& vals,int amount,int base){
	bool done = false;
	int i = 0;
	while(amount > 0){
		if(i >= vals.size())
			return true;
		if(vals[i]+(amount%base) < base){
			vals[i]+=(amount%base);
			amount = amount/base;
		}else{
			vals[i]+=(amount%base);
			vals[i]%=base;
			amount = 1 + ((amount)/base);
		}
		i++;
	}
	return false;
};

class Game{
public:
	enum states{MAIN_MENU, READY, PLAYING, GAME_OVER};

	Game(int width_, int height_){
		width = width_;
		height = height_;
		tiles = vector<vector<int> > (height, vector<int>(width, -1));

		margin_top = 6;
		margin_left = 4;
		horz_spacing = 4;
		vert_spacing = 2;

		init_colors();

		score = 0;
		tiles = add_random(tiles);
		prev_tiles = tiles;
	};

	void reset(){
		tiles = vector<vector<int> > (height, vector<int>(width, -1));
		score = 0;
		tiles = add_random(tiles);
		prev_tiles = tiles;
	}

	void init_colors(){
		init_color(10,1000,1000,1000);
		init_color(11,1000,666,666);
		init_color(12,1000,333,333);
		init_color(13,1000,0,0);
		init_color(14,1000,500,0);
		init_color(15,1000,1000,0);
		init_color(16,500,1000,0);
		init_color(17,0,1000,0);
		init_color(18,0,1000,600);
		init_color(19,0,1000,1000);
		init_color(20,0,500,1000);
		init_color(21,0,0,1000);
		init_pair(1, 10, COLOR_BLACK);
		init_pair(2, 11, COLOR_BLACK);
		init_pair(3, 12, COLOR_BLACK);
		init_pair(4, 13, COLOR_BLACK);
		init_pair(5, 14, COLOR_BLACK);
		init_pair(6, 15, COLOR_BLACK);
		init_pair(7, 16, COLOR_BLACK);
		init_pair(8, 17, COLOR_BLACK);
		init_pair(9, 18, COLOR_BLACK);
		init_pair(10, 19, COLOR_BLACK);
		init_pair(11, 20, COLOR_BLACK);
		init_pair(11, 21, COLOR_BLACK);
		init_pair(11, 22, COLOR_BLACK);
	}

	int get_color_pair(int row, int col, int num){
		return log2(num);
	}

	void run(){
		int ch;
		bool done = false;
		string str;
		while(!done){
			display();
			ch = getch();
			// cout << ch << endl;
			// str = to_string(ch);
			//mvprintw(2,2,"%d",ch);
			//mvaddch(1,2,ch);
			if(ch == 'q'){
				done = true;
			}
			else if(ch == 'n'){
				tiles = add_random(tiles);
			}
			else if(ch == 'z'){
				undo();
			}
			else if(ch == 'r'){
				reset();
			}
			else if(ch == KEY_UP){
				move(UP);
				//mvprintw(0,2,"KEY_UP");
			}
			else if(ch == KEY_DOWN){
				move(DOWN);
				//mvprintw(0,2,"KEY_DOWN");
			}
			else if(ch == KEY_LEFT){
				move(LEFT);
				//mvprintw(0,2,"KEY_LEFT");
			}
			else if(ch == KEY_RIGHT){
				move(RIGHT);
				//mvprintw(0,2,"KEY_RIGHT");
			}
		}
	};

	void move(direction dir){
		vector<vector<int> >new_tiles;
		int new_score;
		switch(dir){
			case LEFT:
				move_left(tiles, score, new_tiles, new_score);
			break;
			case RIGHT:
				move_right(tiles, score, new_tiles, new_score);
			break;
			case UP:
				move_up(tiles, score, new_tiles, new_score);
			break;
			case DOWN:
				move_down(tiles, score, new_tiles, new_score);
			break;
		}
		if(!tiles_equal(tiles, new_tiles)){
			prev_tiles = tiles;
			tiles = new_tiles;
			score = new_score;
			tiles = add_random(tiles);
		}
	}

	bool tiles_equal(const vector<vector<int> > &a, const vector<vector<int> > &b){
		
		for(int i = 0; i < tiles.size(); i++){
			for(int j = 0; j < tiles[0].size(); j++){
				if(a[i][j] != b[i][j])
					return false;
			}
		}
		return true;
	}

	void move_left(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score){

		vector<int> new_row;
		int prev;
		new_score = score;
		new_tiles = vector<vector<int> > (height, vector<int>(width, -1));
		for(int i = 0; i < tiles.size(); i++){
			new_row.clear();
			prev = -1;
			for(int j = 0; j < tiles[0].size(); j++){
				if(tiles[i][j] == -1)
					continue;
				if(tiles[i][j] == prev){
					new_row.back()*=2;
					new_score += new_row.back();
					prev = -1;
				}else{
					new_row.push_back(tiles[i][j]);
					prev = new_row.back();
				}
			}
			for(int j = 0; j < new_row.size(); j++){
				new_tiles[i][j] = new_row[j];
			}
		}
	}

	void move_right(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score){

		vector<int> new_row;
		int prev;
		new_score = score;
		new_tiles = vector<vector<int> > (height, vector<int>(width, -1));
		for(int i = 0; i < tiles.size(); i++){
			new_row.clear();
			prev = -1;
			for(int j = tiles[0].size()-1; j>=0; j--){
				if(tiles[i][j] == -1)
					continue;
				if(tiles[i][j] == prev){
					new_row.back()*=2;
					new_score += new_row.back();
					prev = -1;
				}else{
					new_row.push_back(tiles[i][j]);
					prev = new_row.back();
				}
			}
			for(int j = 0; j < new_row.size(); j++){
				new_tiles[i][tiles[0].size()-j-1] = new_row[j];
			}

		}
	}

	void move_up(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score){

		vector<int> new_row;
		int prev;
		new_score = score;
		new_tiles = vector<vector<int> > (height, vector<int>(width, -1));
		for(int j = 0; j < tiles[0].size(); j++){
			new_row.clear();
			prev = -1;
			for(int i = 0; i < tiles.size(); i++){
				if(tiles[i][j] == -1)
					continue;
				if(tiles[i][j] == prev){
					new_row.back()*=2;
					new_score += new_row.back();
					prev = -1;
				}else{
					new_row.push_back(tiles[i][j]);
					prev = new_row.back();
				}
			}
			for(int i = 0; i < new_row.size(); i++){
				new_tiles[i][j] = new_row[i];
			}

		}
	}

	void move_down(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score){

		vector<int> new_row;
		int prev;
		new_score = score;
		new_tiles = vector<vector<int> > (height, vector<int>(width, -1));
		for(int j = 0; j < tiles[0].size(); j++){
			new_row.clear();
			prev = -1;
			for(int i = tiles.size()-1; i >= 0; i--){
				if(tiles[i][j] == -1)
					continue;
				if(tiles[i][j] == prev){
					new_row.back()*=2;
					new_score += new_row.back();
					prev = -1;
				}else{
					new_row.push_back(tiles[i][j]);
					prev = new_row.back();
				}
			}
			for(int i = 0; i < new_row.size(); i++){
				new_tiles[tiles.size()-i-1][j] = new_row[i];
			}

		}
	}

	// TODO: deal with prev score
	void undo(){
		tiles = prev_tiles;
	}

	void display(){
		display_board();
		display_tiles();
		display_score();
	};

	void display_score()
	{
		mvprintw(margin_top-2, margin_left, "score:               ");
		mvprintw(margin_top-2, margin_left+7, "%d",score);
	};

	void display_board(){
		int yb = margin_top;
		int xb = margin_left;
		int x,y;
		for(int i = 0; i < tiles.size(); i++){
			y = yb + i*vert_spacing +2;
			x = xb;
			mvaddch(y,x,'|');
		}
		for(int i = 0; i < tiles[0].size(); i++){
			y = yb;
			x = xb + i*horz_spacing +2;
			mvaddch(y,x,'-');
		}
		mvaddch(yb,xb,'.');
	}

	void display_tiles(){
		int yb = margin_top+2;
		int xb = margin_left+2;
		int x,y;
		for(int i = 0; i < tiles.size(); i++){
			for(int j = 0; j < tiles[0].size(); j++){

				y = yb + i*vert_spacing;
				x = xb + j*horz_spacing;
				for(int k = 0; k < horz_spacing; k++){
					mvaddch(y,x+k,' ');
				}


				if(tiles[i][j] != -1){
					attron(COLOR_PAIR(get_color_pair(i,j,tiles[i][j])));
					mvprintw(y,x,"%d",tiles[i][j]);
					attroff(COLOR_PAIR(get_color_pair(i,j,tiles[i][j])));
				}
			}
		}
	};

	vector<vector<int> > add_random(vector<vector<int> > tiles){
		int num_empty = 0;
		for(int i = 0; i < tiles.size(); i++){
			for(int j = 0; j < tiles[0].size(); j++){
				if(tiles[i][j] == -1)
					num_empty++;
			}
		}
		if(num_empty == 0){
			return tiles;
		}
		int place = rand()%num_empty;
		int num;
		int r = rand()%10;
		if(r == 0)
			num = 4;
		else
			num = 2;

		int count = 0;
		for(int i = 0; i < tiles.size(); i++){
			for(int j = 0; j < tiles[0].size(); j++){			
				if(tiles[i][j] == -1){
					if(count == place){
						tiles[i][j] = num;
						return tiles;
					}
					count++;
				}
			}
		}
		return tiles;
	}

	map<int, int> color_lookup;

	int margin_top;
	int margin_left;
	int horz_spacing;
	int vert_spacing;

	int score;
	vector<vector<int> > tiles;
	vector<vector<int> > prev_tiles;
	int width;
	int height;
};

// class Solver(){
// 	Solver(bool display_on_){
// 		display_on = display_on_;
// 		game = Game(4,4);
// 	}

// 	void run(){
// 		vector<
// 	}

// 	direction step(int depth, vector<vector<int> > tiles, int score){
// 		vector<vector<int > > new_tiles;
// 	}

// 	double step_score(int depth, vector<vector<int> > tiles, int score, direction dir){
// 		if(depth)
// 	}

// 	// direction step(int depth, vector<vector<int> > tiles, int score){
		
// 	// 	vector<vector<vector<int> > > new_tiles(4);
// 	// 	vector<double> scores(4);
// 	// 	vector<int> new_scores(4); 
// 	// 	direction dirs[4] = {LEFT,RIGHT,UP,DOWN};
// 	// 	max_dir = LEFT;
// 	// 	max_score = -1;
// 	// 	for(int i = 0; i < 4; i++){
// 	// 		scores[i] = score_move(tiles, score, new_tiles[i], new_scores[i], dirs[i]);
// 	// 		if(scores[i] > max_score){
// 	// 			max_score = scores[i];
// 	// 			max_dir = dirs[i];
// 	// 		}
// 	// 	}
// 	// 	return max_dir;
// 	// }

// 	double score_move(const vector<vector<int> > &tiles,const int &score, vector<vector<int> > &new_tiles, int &new_score, direction dir){

// 		switch(dir){
// 			case LEFT:
// 				move_left(tiles, score, new_tiles, new_score);
// 			break;
// 			case RIGHT:
// 				move_right(tiles, score, new_tiles, new_score);
// 			break;
// 			case UP:
// 				move_up(tiles, score, new_tiles, new_score);
// 			break;
// 			case DOWN:
// 				move_down(tiles, score, new_tiles, new_score);
// 			break;
// 		}
// 		if(tiles_equal(tiles, new_tiles)){
// 			return -1;
// 		}

// 		return new_score;
// 	}

// 	bool display_on;
// 	Game game;
// }

int main(){
	initscr();
	start_color();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	Game game(4,4);
	game.run();

	// Solver solver(true);
	// solver.run();

	endwin();
	return 0;

}