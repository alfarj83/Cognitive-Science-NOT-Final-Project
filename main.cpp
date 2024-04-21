#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>


// A simple class to represent a point location.  It only has a
// constructor and a two public member variables.  This is one of the
// few times that you are allowed to use non-private member variables.

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


// NOTE: We could use a boolean (true/false) to represent whether each
// cell of the grid was blocked or open.  This would be the minimal
// representation for memory.

// However, debuggers (both traditional and memory debuggers) might
// not be able to help debug errors with vectors of booleans, if they
// are efficiently packed by a clever STL implementation.  So instead
// we use an enum, to improve readability, and to ensure that the
// status of each grid cell is stored as an integer avoiding debugger
// confusion during development.


enum GRID_STATUS { GRID_CLEAR, GRID_BLOCKED };


// Input the grid and the start location.  The input is a sequence of
// x y locations, terminated by x==0 and y==0.  The last input, which
// follows 0 0 input, is the start location.
//
// The grid is represented as a 2d vector of GRID_STATUS, with each location
// that is blocked --- meaning that no path can go through --- being
// represented by the value GRID_BLOCKED.  The grid is large enough to
// include all blocked points and include the starting location.  The
// first coordinate of the vector of vectors is the x coordinate, and
// the second is the y coordinate.  The format of the input is
// specified in the lab handout.

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


// Output the grid to cout.  The form of the output is explained in
// the cout statement below.

void print_grid(const std::vector<std::vector<GRID_STATUS> > & blocked_grid,
                unsigned int start_x, unsigned int start_y,
                const std::list<Point>& path = std::list<Point>()) {

    std::cout << "Here is the grid with the origin in the upper left corner, x increasing \n"
            << "horizontally and y increasing down the screen.  An 'X' represents a blocked\n"
            << "location and the 'S' represents the starting location. A '$' represents the path.\n\n";

    for (unsigned int y=0; y<blocked_grid[0].size(); ++y) {
        for (unsigned int x=0; x<blocked_grid.size(); ++x) {
            if (std::find(path.begin(), path.end(), Point(x,y)) != path.end()) {
                std::cout << " $";
            } else if (x == start_x && y == start_y) {
                std::cout << " S";
            } else if (blocked_grid[x][y] == GRID_BLOCKED) {
                std::cout << " X";
            } else {
                std::cout << " .";
            }
        }
        std::cout << std::endl;
    }
}


int countToOrigin(int x, int y, std::vector<std::vector<GRID_STATUS> > blocked_grid){
    if(x==0 && y==0){
        return 1;
    } else if(x < 0 || y < 0 || blocked_grid[x][y]==GRID_BLOCKED){
        return 0;
    } else{
        return (countToOrigin(x-1, y, blocked_grid) + countToOrigin(x, y-1, blocked_grid));
    }
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

void addPointToPath(std::list<Point>& path, const Point& newPoint) {
    int position;
    std::cout << "Current path:\n";
    int index = 0;
    for (const Point& p : path) {
        std::cout << index++ << ": (" << p.x << ", " << p.y << ") ";
    }
    std::cout << "\nChoose the position in the path to insert the new point (0 for the start): ";
    std::cin >> position;

    std::list<Point>::iterator it = path.begin();
    std::advance(it, position); // Move the iterator to the desired position
    path.insert(it, newPoint);
}

// Consider renaming this as it now includes logic for a simple game.
void handleIncorrectAnswer(int& score, Point& currentPosition, const std::vector<std::vector<GRID_STATUS>>& blocked_grid) {
    score -= 1;  // Decrement score
    Point previousPosition = currentPosition.moveBackward();
    if (previousPosition.x >= 0 && previousPosition.y >= 0 &&
        blocked_grid[previousPosition.x][previousPosition.y] == GRID_CLEAR) {
        currentPosition = previousPosition;
        std::cout << "Moved back to (" << currentPosition.x << ", " << currentPosition.y << ").\n";
    } else {
        std::cout << "Cannot move back, edge or blocked path!\n";
    }
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
}


