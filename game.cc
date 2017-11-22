#include "game.h"

using namespace std;

const short CSTART = 100;

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


bool kbhit(void)
{
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return true;
    } else {
        return false;
    }
}


Game::Game(int width_, int height_){
	width = width_;
	height = height_;
	quit = false;

	margin_top = 6;
	margin_left = 4;
	horz_spacing = 5;
	vert_spacing = 2;

	init_colors();

	reset();
}

void Game::reset(){
	tiles = vector<vector<int> > (height, vector<int>(width, -1));
	score = 0;
	tiles = add_random(tiles);
	prev_tiles = tiles;
	state = READY;
}

void Game::init_colors(){
	init_color(CSTART+10,1000,1000,1000);
	init_color(CSTART+11,1000,666,666);
	init_color(CSTART+12,1000,333,333);
	init_color(CSTART+13,1000,0,0);
	init_color(CSTART+14,1000,500,0);
	init_color(CSTART+15,1000,1000,0);
	init_color(CSTART+16,500,1000,0);
	init_color(CSTART+17,0,1000,0);
	init_color(CSTART+18,0,1000,600);
	init_color(CSTART+19,0,1000,1000);
	init_color(CSTART+20,0,500,1000);
	init_color(CSTART+21,0,0,1000);

	init_pair(CSTART+1, CSTART+10, COLOR_BLACK);
	init_pair(CSTART+2, CSTART+11, COLOR_BLACK);
	init_pair(CSTART+3, CSTART+12, COLOR_BLACK);
	init_pair(CSTART+4, CSTART+13, COLOR_BLACK);
	init_pair(CSTART+5, CSTART+14, COLOR_BLACK);
	init_pair(CSTART+6, CSTART+15, COLOR_BLACK);
	init_pair(CSTART+7, CSTART+16, COLOR_BLACK);
	init_pair(CSTART+8, CSTART+17, COLOR_BLACK);
	init_pair(CSTART+9, CSTART+18, COLOR_BLACK);
	init_pair(CSTART+10, CSTART+19, COLOR_BLACK);
	init_pair(CSTART+11, CSTART+20, COLOR_BLACK);
	init_pair(CSTART+12, CSTART+21, COLOR_BLACK);
	init_pair(CSTART+13, CSTART+22, COLOR_BLACK);
	max_color_pair = 13;

}

int Game::get_color_pair(int row, int col, int num){
	int color_pair = log2(num);
	if(color_pair > max_color_pair)
		color_pair = max_color_pair;
	return CSTART+log2(num);
}

void Game::run(){
	int ch;
	bool done = false;
	string str;
	while(!quit){
		display();
		run_timed();
	}
	
	// while(!done){
	// 	display();
	// 	ch = getch();
	// 	// cout << ch << endl;
	// 	// str = to_string(ch);
	// 	//mvprintw(2,2,"%d",ch);
	// 	//mvaddch(1,2,ch);
	// 	if(ch == 'q'){
	// 		done = true;
	// 	}
	// 	else if(ch == 'n'){
	// 		tiles = add_random(tiles);
	// 	}
	// 	else if(ch == 'z'){
	// 		undo();
	// 	}
	// 	else if(ch == 'r'){
	// 		reset();
	// 	}
	// 	else if(ch == KEY_UP){
	// 		move(UP);
	// 		//mvprintw(0,2,"KEY_UP");
	// 	}
	// 	else if(ch == KEY_DOWN){
	// 		move(DOWN);
	// 		//mvprintw(0,2,"KEY_DOWN");
	// 	}
	// 	else if(ch == KEY_LEFT){
	// 		move(LEFT);
	// 		//mvprintw(0,2,"KEY_LEFT");
	// 	}
	// 	else if(ch == KEY_RIGHT){
	// 		move(RIGHT);
	// 		//mvprintw(0,2,"KEY_RIGHT");
	// 	}
	// }
}

void Game::run_timed(){
	bool done = false;
	started = false;
	string str;
	while(!done){
		std::this_thread::sleep_for (std::chrono::milliseconds(1));
		display();
		while(kbhit()){
			display();
			
			if(state == READY){
				timer.start();
				state = PLAYING;
			}
			int ch = getch();
			move_time = timer.getTimePassed();

			if(ch == 'q'){
				quit = true;
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
				done = true;
			}
			else if(ch == KEY_UP){
				move(UP);
			}
			else if(ch == KEY_DOWN){
				move(DOWN);
			}
			else if(ch == KEY_LEFT){
				move(LEFT);
			}
			else if(ch == KEY_RIGHT){
				move(RIGHT);
			}
		}

		if(state == PLAYING){
			if(check_win()){
				state = WIN;
				final_time = move_time;
			}else if(check_game_over(tiles)){
				state = GAME_OVER;
			}
		}
	}
}

bool Game::check_win(){
	for(int i = 0; i < tiles.size(); i++){
		for(int j = 0; j < tiles[0].size(); j++){
			if(tiles[i][j] == 2048){
				return true;
			}
		}
	}
	return false;
}

void Game::move(direction dir){
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

bool Game::check_game_over(const vector<vector<int> > tiles){
	vector<vector<int> >new_tiles;
	int new_score;
	move_left(tiles, score, new_tiles, new_score);
	if(!tiles_equal(tiles, new_tiles))
		return false;
	move_right(tiles, score, new_tiles, new_score);
	if(!tiles_equal(tiles, new_tiles))
		return false;
	move_up(tiles, score, new_tiles, new_score);
	if(!tiles_equal(tiles, new_tiles))
		return false;
	move_down(tiles, score, new_tiles, new_score);
	if(!tiles_equal(tiles, new_tiles))
		return false;
	return true;
}

bool Game::tiles_equal(const vector<vector<int> > &a, const vector<vector<int> > &b){
	
	for(int i = 0; i < tiles.size(); i++){
		for(int j = 0; j < tiles[0].size(); j++){
			if(a[i][j] != b[i][j])
				return false;
		}
	}
	return true;
}

void Game::move_left(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score){

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

void Game::move_right(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score){

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

void Game::move_up(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score){

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

void Game::move_down(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score){

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
void Game::undo(){
	tiles = prev_tiles;
}

void Game::display(){

	display_board();
	display_tiles();
	display_score();
	display_time();
	display_game_over();

}


void Game::display_game_over()
{
	if(state == GAME_OVER)
		mvprintw(margin_top + vert_spacing*height + 2, margin_left, "GAME OVER");
	else
		mvprintw(margin_top + vert_spacing*height + 2, margin_left, "         ");

}

void Game::display_score()
{
	mvprintw(margin_top-2, margin_left, "score:               ");
	mvprintw(margin_top-2, margin_left+7, "%d",score);
}

void Game::display_time()
{
	time_t now = time(0);
	mvprintw(margin_top-4, margin_left, " time:              ");
	long ms = 1000000000;
	long t = timer.getTimePassed();
	if(state == PLAYING || state == GAME_OVER){
		mvprintw(margin_top-4, margin_left+7, "%ld:%ld:%ld", (t/ms)/60,(t/ms)%60,(t%ms)/10000000);
	}else if(state == WIN){
		t = final_time;
		mvprintw(margin_top-4, margin_left+7, "%ld:%ld:%ld", (t/ms)/60,(t/ms)%60,(t%ms)/10000000);
	}else{
		mvprintw(margin_top-4, margin_left+7, "0:0:0");
	}
}

void Game::display_board(){
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

void Game::display_tiles(){
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
}

vector<vector<int> > Game::add_random(vector<vector<int> > tiles){
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