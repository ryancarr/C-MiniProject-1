#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::abs;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::pow;
using std::sort;
using std::sqrt;
using std::string;
using std::vector;

// Possible node states
enum class State {kEmpty, kObstacle, kClosed, kPath};

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};



vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
      if (n == 0) {
        row.push_back(State::kEmpty);
      } else {
        row.push_back(State::kObstacle);
      }
    }
    return row;
}

vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

int Heuristic(int originX, int originY, int goalX, int goalY)
{

  return (abs(goalX) - abs(originX)) + (abs(goalY) - abs(originY));
}

float Heuristic(vector<int> origin, vector<int> goal)
{
  return sqrt(pow(goal[0] - origin[0], 2) + 
              pow(goal[1] - origin[1], 2));
}

bool Compare(const vector<int> firstNode, const vector<int> secondNode)
{
  float firstF  = firstNode[2] + firstNode[3];
  float secondF = secondNode[2] + secondNode[3];
  return firstF > secondF;
}

/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v) 
{
  sort(v->begin(), v->end(), Compare);
}

void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &open, vector<vector<State>> &board)
{
  vector<int> node = {x, y, g, h};
  open.push_back(node);
  board[x][y] = State::kClosed;
}

bool CheckValidCell(int x, int y, const vector<vector<State>> board)
{
  // TODO: Check out of bound conditions x or y < 0 or x or y > size()
  if((x >= 0 && x < board.size()) &&
     (y >= 0 && y < board[0].size()))
    return board[x][y] == State::kEmpty;
  else
    return false;
}

/** 
 * Expand current nodes's neighbors and add them to the open list.
 */
void ExpandNeighbors(vector<int> &current, int goal[2], vector<vector<int>> &open, vector<vector<State>> &board)
{
  int x = current[0];
  int y = current[1];
  int g = current[2];

  for(int i = 0; i < 4; i++)
  {
    int neighborX = x + delta[i][0];
    int neighborY = y + delta[i][1];
  
    if(CheckValidCell(neighborX, neighborY, board))
    {
      int g2 = g + 1;
      int h = Heuristic(neighborX, neighborY, goal[0], goal[1]);
      AddToOpen(neighborX, neighborY, g2, h, open, board);
    }
  }  
}

/** 
 * Implementation of A* search algorithm
 */
vector<vector<State>> Search(vector<vector<State>> board, int init[2], int goal[2])
{
  vector<vector<int>> open{};
  int x = init[0];
  int y = init[1];
  int g = 0;
  int h = Heuristic(init[0], init[1], goal[0], goal[1]);
  AddToOpen(x, y, g, h, open, board);

  while(open.size() > 0)
  {
    CellSort(&open);

    auto current = open.back();
    open.pop_back();
    x = current[0];
    y = current[1];
    board[x][y] = State::kPath;

    if(x == goal[0] && y == goal[1])
      return board;
    else
      ExpandNeighbors(current, goal, open, board);
  
  }

  cout << "No path found!" << std::endl;
  return vector<vector<State>>{};
}


string CellString(State cell)
{
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    case State::kPath: return "🚗   ";
    default: return "0   "; 
  }
}


void PrintBoard(const vector<vector<State>> board)
{
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}


int main()
{
  int init[2]{0, 0};
  int goal[2]{4, 5};
  auto board = ReadBoardFile("1.board");
  auto solution = Search(board, init, goal);
  PrintBoard(solution);
}