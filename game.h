#include <vector>
#include <ncurses.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <cmath>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

enum direction{LEFT, RIGHT, UP, DOWN};

bool kbhit(void);

class Timer
{
    // alias our types for simplicity
    using clock             = std::chrono::system_clock;
    using time_point_type   = std::chrono::time_point < clock, std::chrono::milliseconds > ;
public:
    // default constructor that stores the start time
    Timer()
    {
        start_time = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
    }

    void start(){
    	start_time = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
    }

    // gets the time elapsed from construction.
    long /*milliseconds*/ getTimePassed()
    {
        // get the new time
        auto end = clock::now();

        // return the difference of the times
        return (end - start_time).count();
    }

private:
    time_point_type start_time;
};


class Game{
public:
	enum State{MAIN_MENU, READY, PLAYING, GAME_OVER, WIN};

	Game(int width_, int height_);

	void reset();

	void init_colors();

	int get_color_pair(int row, int col, int num);

	void run();

	void run_timed();

	bool check_win();

	void move(direction dir);

	bool tiles_equal(const vector<vector<int> > &a, const vector<vector<int> > &b);

	void move_left(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score);

	void move_right(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score);

	void move_up(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score);

	void move_down(const vector<vector<int> > &tiles, const int &score, vector<vector<int> > &new_tiles, int &new_score);

	// TODO: deal with prev score
	void undo();

	void display();

	void display_score();

	void display_time();

	void display_board();

	void display_tiles();

	vector<vector<int> > add_random(vector<vector<int> > tiles);

	map<int, int> color_lookup;

	int margin_top;
	int margin_left;
	int horz_spacing;
	int vert_spacing;

	bool started;
	Timer timer;
	long move_time;
	long final_time;
	bool quit;
	State state;

	int score;
	vector<vector<int> > tiles;
	vector<vector<int> > prev_tiles;
	int width;
	int height;
};