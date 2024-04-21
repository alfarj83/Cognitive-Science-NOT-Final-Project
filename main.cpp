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

//starting positions
unsigned int START_X = 4, START_Y = 2;
//grid size
int MAX_X = 4, MAX_Y = 4;
//board states
enum GRID_STATUS { GRID_CLEAR, GRID_BLOCKED, CURRENT_POINT};

//point object class
class Point {
public:
  Point(int x0, int y0) : x(x0), y(y0) {}
  int x,y;

  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }
  bool operator!=(const Point &other) const {
    return x != other.x || y != other.y;
  }
    
  void moveUp() { --y; }
  void moveDown() { ++y; }
  void moveLeft() { --x; }
  void moveRight() { ++x; }
};

Point getNewPoint(int x, int y) {
  return Point(x, y);
}

void read_grid(std::vector<std::vector<GRID_STATUS> >& blocked_grid) {
  // Read the x y locations into a list of Points.  Keep track of the
  // max x and max y values so that the size of the grid can be
  // determined.
  // keep track of the max coordinate values
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

  // Make a vector of vectors with all entries marked clear.
  std::vector<GRID_STATUS> one_row_of_ys(MAX_Y+1, GRID_CLEAR);
  std::vector<std::vector<GRID_STATUS> > empty_grid(MAX_X+1, one_row_of_ys);
  blocked_grid = empty_grid;

  // For Point in the list, mark the location in the list as blocked.
  std::list<Point>::iterator p;
  for (p = blocked_points.begin(); p != blocked_points.end(); ++p) {
    blocked_grid[p->x][p->y] = GRID_BLOCKED;
  }
}

void print_grid(std::vector<std::vector<GRID_STATUS> > & blocked_grid) {
  for (unsigned int y=0; y<blocked_grid[0].size(); ++y) {
    for (unsigned int x=0; x<blocked_grid.size(); ++x) {
      if (x == START_X && y == START_Y)
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

//puts all words (real words and pseudowords alike) into map
std::vector<std::pair<std::string, bool> > parseWords() {
  std::vector<std::pair<std::string, bool> > allWords;
  std::ifstream wordsFile("allWords.txt");
  std::string word, currentWord, validity;
  while(wordsFile >> currentWord >> validity) {
    if (validity == "TRUE") {
      allWords.push_back(std::pair<std::string, bool>(currentWord, true));
    } else {
      allWords.push_back(std::pair<std::string, bool>(currentWord, false));
    }
  }
  wordsFile.close();
  return allWords;
}

bool canMoveTo(Point& pos, std::vector<std::vector<GRID_STATUS> >& blocked_grid) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= blocked_grid.size() || pos.y >= blocked_grid[0].size()) return false;
    return blocked_grid[pos.x][pos.y] == GRID_CLEAR;
}

//in the case of a correct answer;
void correctAnswer(Point& pos, std::vector<std::vector<GRID_STATUS> > & blocked_grid) {
  std::cout << "Pick where you wanna go! (Write up, down, left, or right) ";
  std::string direction;
  std::cin >> direction;

  //turns all characters into lowercase
  for (char &c : direction) {
    c = std::tolower(c);
  }

  //checks if input is valid
  if (direction == "up") {
    Point possiblePos = pos;
    possiblePos.moveUp();
    //moves up
    if (canMoveTo(possiblePos, blocked_grid)) {
      blocked_grid[pos.x][pos.y] = GRID_CLEAR;
      pos.moveUp();
      blocked_grid[pos.x][pos.y] = CURRENT_POINT;
    }
  } else if (direction == "down") {
    Point possiblePos = pos;
    possiblePos.moveDown();
    //moves down
    if (canMoveTo(possiblePos, blocked_grid)) {
      blocked_grid[pos.x][pos.y] = GRID_CLEAR;
      pos.moveDown();
      blocked_grid[pos.x][pos.y] = CURRENT_POINT;
    }
  } else if (direction == "left") {
    Point possiblePos = pos;
    possiblePos.moveLeft();
    //moves left
    if (canMoveTo(possiblePos, blocked_grid)) {
      blocked_grid[pos.x][pos.y] = GRID_CLEAR;
      pos.moveLeft();
      blocked_grid[pos.x][pos.y] = CURRENT_POINT;
    }
  } else if (direction == "right") {
    Point possiblePos = pos;
    possiblePos.moveRight();
    //moves right
    if (canMoveTo(possiblePos, blocked_grid)) {
      blocked_grid[pos.x][pos.y] = GRID_CLEAR;
      pos.moveRight();
      blocked_grid[pos.x][pos.y] = CURRENT_POINT;
    }
  } else {
    std::cout << "Not valid direction. Please try again." << std::endl;
    correctAnswer(pos, blocked_grid);
  }

  std::cout << "\nCurrent grid:\n";
  print_grid(blocked_grid);
}

//if they get a wrong answer, return to start
void wrongAnswer(Point& currentPos, std::vector<std::vector<GRID_STATUS> > & blocked_grid) {
  std::cout << "Wrong answer! You've gone back to the start." << std::endl;
  blocked_grid[currentPos.x][currentPos.y] = GRID_CLEAR;
  blocked_grid[START_X][START_Y] = CURRENT_POINT;
}

double getResponseTime(std::string& answer) {
  using Clock = std::chrono::high_resolution_clock;

  std::cout << "Is this a word (1 for yes, 0 for no): ";
  auto start = Clock::now();
  std::cin >> answer;

  auto end = Clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  return ms / 1000.0; // Convert to seconds
}
bool isValidAnswer(std::string& ans) {
  if (ans == "1" || ans == "0") {
    return true;
  }
  return false;
}

int main() {
  //starting description
  std::cout << "Welcome to our NOT final project!\nWe\'ve designed a little game to test your lexical abilities.\n";
  std::cout << "We\'ll give you a word and you\'ll write \'T\' if it\'s a real word, and \'F\' if it\'s a fake one.\n";
  std::cout << "The correct answer will let you progress in the maze, while the wrong answer makes your restart, so be careful.\n";
  std::cout << "You'll have [] minutes to get to the end of the maze, so good luck!\n";

  std::vector<std::vector<GRID_STATUS> > blocked_grid;
  read_grid(blocked_grid);
  std::vector<std::pair<std::string, bool> > allWords = parseWords();

  //prints starting grid
  std::cout << "Here is the grid with the origin in the upper left corner, x increasing \n"
          << "horizontally and y increasing down the screen.  An 'X' represents a blocked\n"
          << "location and the 'S' represents the starting location. 'C' represents your current position.\n\n";
  print_grid(blocked_grid);
  
  //gets starting point and ending point
  Point currentPos = getNewPoint(START_X, START_Y);
  Point endPos = getNewPoint(0, 0);

  while (currentPos != endPos) {
    //Generates random number
    std::mt19937 rng(std::random_device{}());
    int min = 0, max = allWords.size() - 1;
    std::uniform_int_distribution<int> dist(min, max);
    int random_int = dist(rng);

    //pulls word from vector based on random number
    std::pair<std::string, bool> testWord = allWords[random_int];
    //answer that the player gives
    std::string answer;
    std::cout << testWord.first << std::endl;
    double responseTime = getResponseTime(answer);
    std::cout << "Response time: " << responseTime << " seconds\n";

    //checks if answer is valid
    if (isValidAnswer(answer)) {
      //if the player answers that the word is real
      if (answer == "1") {
        if (testWord.second == true) {
          correctAnswer(currentPos, blocked_grid);
        } else { wrongAnswer(currentPos, blocked_grid); }
      //if the player answers that the word is fake
      } else if (answer == "0") {
        if (testWord.second == false) {
          correctAnswer(currentPos, blocked_grid);
        } else { wrongAnswer(currentPos, blocked_grid); }
      }
      //if the player answers with a non-valid answer
    } else {
      std::cout << "Answer not valid. Try again.\n";
    }
  }
  return 0;
}