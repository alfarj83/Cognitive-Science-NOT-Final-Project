#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cctype>
#include <algorithm>
#include <random>

//point object class
class Point {
public:
  Point(int x0, int y0) : x(x0), y(y0) {}
  int x,y;

  bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
    
    Point& moveBackward() {
        if (x > 0) --x;
        if (y > 0) --y;
        return *this;
    }
};

enum GRID_STATUS { GRID_CLEAR, GRID_BLOCKED, CURRENT_POINT};

void read_grid(std::vector<std::vector<GRID_STATUS> >& blocked_grid,
           int& start_x, int& start_y) {

  // Read the x y locations into a list of Points.  Keep track of the
  // max x and max y values so that the size of the grid can be
  // determined.
  int x, y;
  int max_x = 4, max_y = 4;  // keep track of the max coordinate values
  std::list<Point> blocked_points = {};
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
  // istr >> start_x >> start_y;
  start_x = 2;
  start_y = 4;
  if (start_x > max_x) max_x = start_x;
  if (start_y > max_y) max_y = start_y;

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
                unsigned int start_x, unsigned int start_y,
                const std::list<Point>& path = std::list<Point>()) {

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

bool findPath(const std::vector<std::vector<GRID_STATUS> >& blocked_grid, int x, int y, std::list<Point>& path) {
    // Base Case: If we're at the origin, add this point to the path
    if (x == 0 && y == 0) {
        path.push_front(Point(x, y));
        return true;
    }

    // If the current cell is blocked, no paths will pass through it
    if (blocked_grid[x][y] == GRID_BLOCKED) {
        return false;
    }

    // Try moving left
    if (x > 0 && findPath(blocked_grid, x-1, y, path)) {
        path.push_front(Point(x, y));
        return true;
    }

    // Try moving down
    if (y > 0 && findPath(blocked_grid, x, y-1, path)) {
        path.push_front(Point(x, y));
        return true;
    }

    return false;
}

bool confirmPathValidity(const std::list<Point>& path) {
    // Display the path
    std::cout << "Proposed path:\n";
    for (const Point& point : path) {
        std::cout << "(" << point.x << ", " << point.y << ") ";
    }
    std::cout << "\nIs this path valid? (yes/no): ";

    // Get user confirmation
    std::string response;
    std::cin >> response;
    std::transform(response.begin(), response.end(), response.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return (response == "yes");
}


double getResponseTime() {
    using namespace std::chrono;

    std::cout << "Press any key to start the timer...";
    std::cin.ignore(); // Wait for user input
    time_point<high_resolution_clock> start = high_resolution_clock::now();

    std::cout << "Is 2+2=4? (1 for yes, 0 for no): ";
    bool answer;
    std::cin >> answer;

    time_point<high_resolution_clock> stop = high_resolution_clock::now();
    milliseconds duration = duration_cast<milliseconds>(stop - start);

    return duration.count() / 1000.0; // Convert to seconds
}


Point getNewPoint() {
    int x, y;
    std::cout << "Enter the x coordinate of the new point: ";
    std::cin >> x;
    std::cout << "Enter the y coordinate of the new point: ";
    std::cin >> y;
    return Point(x, y);
}


bool canMoveTo(const Point& pos, const std::vector<std::vector<GRID_STATUS>>& blocked_grid) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= blocked_grid.size() || pos.y >= blocked_grid[0].size()) return false;
    return blocked_grid[pos.x][pos.y] == GRID_CLEAR;
}

int main() {
    std::vector<std::vector<GRID_STATUS>> blocked_grid;
    int start_x, start_y;
    start_x = 2;
    start_y = 4;
    read_grid(blocked_grid, start_x, start_y);
    print_grid(blocked_grid, start_x, start_y);

    std::list<Point> path;
    if (findPath(blocked_grid, start_x, start_y, path)) {
        print_grid(blocked_grid, start_x, start_y, path);
        if (confirmPathValidity(path)) {
            std::cout << "The path is confirmed as valid.\n";
            Point newPoint = getNewPoint();
            addPointToPath(path, newPoint);
            std::cout << "Updated path with the new point:\n";
            for (const Point& p : path) {
                std::cout << "(" << p.x << ", " << p.y << ") ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "The path is not valid.\n";
        }
    } else {
        std::cout << "No path found from (" << start_x << ", " << start_y << ") to the origin.\n";
    }
    
    double responseTime;
    responseTime = getResponseTime();
    std::cout << "Response time: " << responseTime << " seconds\n";

    return 0;
