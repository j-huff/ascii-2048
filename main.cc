
#include <ncurses.h>
#include <iostream>
#include <utility>

#include <cxxopts.hpp>
#include "game.h"
#include <limits.h>

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

vector<pair<short, short> > pairs;
vector<vector<short> > colors;

void save_colors(){
	short r,g,b, p1, p2;
	pair<short,short> p;
	vector<short> c(3);
	for(int i = 0; i < 100; i++){
		color_content(i, &r, &g, &b);
		c[0] = r;
		c[1] = g;
		c[2] = b;
		colors.push_back(c);

		pair_content(i, &p1, &p2);
		pairs.push_back(pair<short,short>(p1,p2));
	}
}
void restore_colors(){
	for(int i = 0; i < 100; i++){
		init_color(i, colors[i][0], colors[i][1], colors[i][2]);
		init_pair(i, pairs[i].first, pairs[i].second);
	}
}



int main(int argc, char** argv){
	//atexit(restore_colors);
	
	cxxopts::Options options("ascii2048", "Terminal implementation of the popular 2048 game");
	options
      .positional_help("[optional args]")
      .show_positional_help();


	options.add_options()
  ("s,size", "Grid size", cxxopts::value<int>()->default_value("4"))
  ("h,help", "Print help")

  ;

	auto result = options.parse(argc, argv);

	if (result.count("help"))
    {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

	initscr();
	start_color();
	//save_colors();

	noecho();
	raw();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	scrollok(stdscr, TRUE);
	curs_set(0);
	
	while(kbhit()){
		getch();
	}


	int grid_size = result["size"].as<int>();

	Game game(grid_size,grid_size);
	game.run();

	// Solver solver(true);
	// solver.run();
	endwin();
	return 0;

}
