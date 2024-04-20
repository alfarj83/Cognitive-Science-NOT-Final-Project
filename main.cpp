#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cctype>

//point object class
class Point {
public:
  Point(int x0, int y0) : x(x0), y(y0) {}
  int x,y;
};

enum GRID_STATUS { GRID_CLEAR, GRID_BLOCKED, CURRENT_POINT};

void read_grid(std::vector<std::vector<GRID_STATUS> >& blocked_grid,
	       int& start_x, int& start_y) {

  // Read the x y locations into a list of Points.  Keep track of the
  // max x and max y values so that the size of the grid can be
  // determined.
  int max_x = 4, max_y = 4;  // keep track of the max coordinate values
  std::list<Point> blocked_points;
  blocked_points.push_back(Point(2,1));
  blocked_points.push_back(Point(3,1));
  blocked_points.push_back(Point(1,2));
  blocked_points.push_back(Point(3,2));
  blocked_points.push_back(Point(0,3));
  blocked_points.push_back(Point(1,3));
  blocked_points.push_back(Point(0,4));
  blocked_points.push_back(Point(1,4));
  blocked_points.push_back(Point(3,4));
  blocked_points.push_back(Point(4,4));

  // Now that a 0 0 location has been read, read the start location.
  // If this is beyond the max x or y value then update these values.
  start_x = 2;
  start_y = 4;

  // Make a vector of vectors with all entries marked clear.
  std::vector<GRID_STATUS> one_row_of_ys(max_y+1, GRID_CLEAR);
  std::vector<std::vector<GRID_STATUS> > empty_grid(max_x+1, one_row_of_ys);
  blocked_grid = empty_grid;

  // For Point in the list, mark the location in the list as blocked.
  std::list<Point>::iterator p;
  for (p = blocked_points.begin(); p != blocked_points.end(); ++p) {
    blocked_grid[p->x][p->y] = GRID_BLOCKED;
  }
}

void print_grid(const std::vector<std::vector<GRID_STATUS> > & blocked_grid,
                unsigned int start_x, unsigned int start_y) {

  std::cout << "Here is the grid with the origin in the upper left corner, x increasing \n"
            << "horizontally and y increasing down the screen.  An 'X' represents a blocked\n"
            << "location and the 'S' represents the starting location.\n\n";
  
  for (unsigned int y=0; y<blocked_grid[0].size(); ++y) {
    for (unsigned int x=0; x<blocked_grid.size(); ++x) {
      if (x == start_x && y == start_y)
        std::cout << " S";
      else if (blocked_grid[x][y] == CURRENT_POINT)
        std::cout << " C";
      else if (blocked_grid[x][y] == GRID_BLOCKED)
        std::cout << " X";
      else
        std::cout << " .";
    }
    std::cout << std::endl;
  }
}

void correctAnswer(int x, int y, std::vector<std::vector<GRID_STATUS> > & blocked_grid) {
  std::cout << "Pick where you wanna go! (Write up, down, left, or right) ";
  std::string direction;
  std::cin >> direction;

  //turns all characters into lowercase
  for (char &c : direction) {
    c = std::tolower(c);
  }

  //checks if input is valid
  if (direction == "up") {
    int newY = y - 1;
    if (newY < 5 && newY >= 0 && blocked_grid[x][newY] != GRID_BLOCKED) {
      blocked_grid[x][y] = GRID_CLEAR;
      blocked_grid[x][newY] = CURRENT_POINT;
    }
  } else if (direction == "down") {
    int newY = y + 1;
    if (newY < 5 && newY >= 0 && blocked_grid[x][newY] != GRID_BLOCKED) {
      blocked_grid[x][y] = GRID_CLEAR;
      blocked_grid[x][newY] = CURRENT_POINT;
    }
  } else if (direction == "left") {
    int newX = x - 1;
    if (newX < 5 && newX >= 0 && blocked_grid[newX][y] != GRID_BLOCKED) {
      blocked_grid[x][y] = GRID_CLEAR;
      blocked_grid[newX][y] = CURRENT_POINT;
    }
  } else if (direction == "right") {
    int newX = x + 1;
    if (newX < 5 && newX >= 0 && blocked_grid[newX][y] != GRID_BLOCKED) {
      blocked_grid[x][y] = GRID_CLEAR;
      blocked_grid[newX][y] = CURRENT_POINT;
    }
  } else {
    std::cout << "Not valid direction. Please try again." << std::endl;
    correctAnswer(x, y, blocked_grid);
  }
}

//if they get a wrong answer, return to start
bool wrongAnswer(int x, int y, unsigned int start_x, unsigned int start_y, std::vector<std::vector<GRID_STATUS> > & blocked_grid) {
  std::cout << "Wrong answer! You've gone back to the start." << std::endl;
  blocked_grid[x][y] = GRID_CLEAR;
  blocked_grid[start_x][start_y] = CURRENT_POINT;
}

int main() {
  using Clock = std::chrono::high_resolution_clock;

  std::cout << "Enter your name: ";
  std::string name;
  auto start = Clock::now();
  std::cin >> name;

  auto end = Clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  std::cout << "Your input took " << ms << " milliseconds" << std::endl;
}