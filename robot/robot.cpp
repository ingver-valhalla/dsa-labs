#include <iostream>
#include <cstdlib>

using namespace std;

const int MAP_SIZE = 8;
// original task
char map[MAP_SIZE][MAP_SIZE] = {
	{' ',' ',' ', ' ', ' ', ' ', ' ', ' '},
	{' ',' ',' ', ' ', ' ', '@', ' ', ' '},
	{' ',' ','@', ' ', ' ', ' ', ' ', '@'},
	{' ',' ',' ', '@', ' ', '@', ' ', ' '},
	{' ',' ','@', ' ', ' ', '@', ' ', ' '},
	{' ',' ',' ', ' ', ' ', ' ', ' ', '@'},
	{' ',' ',' ', ' ', ' ', ' ', ' ', ' '},
	{' ',' ',' ', ' ', ' ', '@', '@', ' '},
};

int coins = 0;
int coins_rows[MAP_SIZE]  = { 0 }; // counts coins in rows
int coins_cols[MAP_SIZE] = { 0 };  // counts coins in columns


enum Dir { DOWN, RIGHT };


// robot coords
int robot_x = 0, robot_y = 0;

int passes = 1;

void print_map()
{
	cout << "coins: " << coins << endl;
	cout << passes << " pass\n";
	for (int row = 0; row < MAP_SIZE; ++row) {
		cout << '|';
		for (int col = 0; col < MAP_SIZE; ++col) {
			if (row == robot_y && col == robot_x)
				cout << 'X';
			else
				cout << map[row][col];
			cout << '|';
		}
		cout << coins_rows[row] << endl;
	}
	for (int col = 0; col < MAP_SIZE; ++col) {
		cout << ' ' << coins_cols[col];
	}
	cout << "\nPress Enter to continue\n";
	cin.get();
}

void clean_map()
{
	for (int row = 0; row < MAP_SIZE; ++row)
		for( int col = 0; col < MAP_SIZE; ++col)
			if (map[row][col] != '@' ) {
				map[row][col] = ' ';
			}
}

void count_coins()
{
	coins = 0;
	for (int row = 0; row < MAP_SIZE; ++row)
		coins_rows[row] = 0;
	for (int col = 0; col < MAP_SIZE; ++col)
		coins_cols[col] = 0;
	for (int row = 0; row < MAP_SIZE; ++row)
		for (int col = 0; col < MAP_SIZE; ++col)
			if (map[row][col] == '@') {
				++coins_rows[row];
				++coins_cols[col];
				++coins;
			}
}

Dir choose_dir()
{
	int offset = 1;
	int row = robot_y, col = robot_x;

	if (row >= MAP_SIZE-1)
		return RIGHT;
	else if (col >= MAP_SIZE-1)
		return DOWN;
	else if (coins_rows[row] == coins_cols[col]) {
		//cout << "searching the nearest...\n";
		//cout << "x = " << row << ", col = " << col << endl;
		row += offset;
		while (map[row][col] != '@') {
			//cout << "map[" << row << "][" << col << "]\n";
			if (row >= MAP_SIZE)
				return RIGHT;
			if (col >= MAP_SIZE)
				return DOWN;
			if (row == robot_y) {
				++offset;
				row = robot_y + offset;
				col = robot_x;
				continue;
			}
			--row;
			++col;
		}
		if (row - robot_y > col - robot_x)
			return DOWN;
		else
			return RIGHT;
	}
	else if (coins_rows[row] > coins_cols[col]) {
		return RIGHT;
	}
	return DOWN;
}

void make_turn()
{
	// return to the upper-left corner if faced a wall
	if (robot_x == MAP_SIZE-1 && robot_y == MAP_SIZE-1) {

		print_map();

		robot_x = 0;
		robot_y = 0;
		++passes;
		clean_map();
		// count again
		count_coins();

		print_map();

		return;
	}

	// check map and choose direction
	Dir dir = choose_dir();

	if (dir == DOWN) {
		// forget skipped coins
		for (int col = robot_x+1; col < MAP_SIZE; ++col) {
			if (map[robot_y][col] == '@') {
				--coins_cols[col];
				--coins_rows[robot_y];
			}
		}
		// mark route
		map[robot_y][robot_x] = 'v';
		// move
		++robot_y;
	}
	else if (dir == RIGHT) {
		// forget skipped coins
		for (int row = robot_y+1; row < MAP_SIZE; ++row) {
			if (map[row][robot_x] == '@') {
				--coins_cols[robot_x];
				--coins_rows[row];
			}
		}
		// mark route
		map[robot_y][robot_x] = '>';
		// move
		++robot_x;
	}
	else {
		cout << "Wrong direction!\n";
		exit(1);
	}


	if (robot_y >= MAP_SIZE || robot_x >= MAP_SIZE) {
		cout << "Oops, crashed!\n";
		cout << "Press Enter to exit\n";
		cin.get();
		exit(1);
	}

	if (map[robot_y][robot_x] == '@') {
		// take the coin
		map[robot_y][robot_x] = ' ';
		--coins;
		--coins_rows[robot_y];
		--coins_cols[robot_x];
	}

}

void init_map()
{
	for (int row = 0; row < MAP_SIZE; ++row)
		for (int col = 0; col < MAP_SIZE; ++col)
			map[row][col] = ' ';
}

void generate()
{
	int n = 0;
	cout << "How many coins do you want?\n";
	cin >> n;
	while (!cin || n > MAP_SIZE * MAP_SIZE - 1 || n < 0) {
		cin.clear();
		cout << "Try again\n";
		cin >> n;
	}
	cin.get(); // extract '\n'

	cout << "generating " << n << " coins\n";

	while (n > 0) {
		int row = rand()%MAP_SIZE;
		int col = rand()%MAP_SIZE;
		if (row == 0 && col == 0)
			continue;
		if (map[row][col] != '@') {
			--n;
			map[row][col] = '@';
		}
	}
}

int main(int argc, char *argv[])
{
	// random generation
	srand(time(NULL));
	init_map();
	generate();

	count_coins();
	print_map();
	while (coins) {
		make_turn();
		//print_map();
	}
	print_map();

	cout << "Completed!\n";
	cout << "passes: " << passes << endl;
	return 0;
}
