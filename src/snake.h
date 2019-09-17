#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kLeft, kRight, kDown };   // scoped enumeration called Direction

  // constructor
  // grid_width and grid_height are defined same as in main(), this is for snake to know when to wrap around when hitting boundaries
  // head_x and head_y defines the coordinates (in unit of grids) of the head of the snake 
  Snake(int grid_width, int grid_height)    
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  void Update();    // update head and body

  void GrowBody();  // set growing flag to true
  bool SnakeCell(int x, int y);   // check if a cell is part of the snake

  Direction direction = Direction::kUp;   // public direction attribute

  // float speed{0.1f};    // speed of the snake, starting at 0.1, what does 0.1 mean? 0.1 grid per loop? 
  float speed{0.02f};
  int size{1};          // size, starting at 1
  bool alive{true};     // alive or not
  float head_x;         // head x coordinate, float
  float head_y;         // head y coordinate, float
  std::vector<SDL_Point> body;    // a vector of all body points; the begin of the vector is tail, the end is head 

 private:
  void UpdateHead();    // update head
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);   // update body

  bool growing{false};  // use growing as a flag to mark if the snake is touching the food
  int grid_width;     // ???
  int grid_height;    // ???
};

#endif