#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  std::vector<SDL_Point> food;
  std::vector<SDL_Point> stone;
  int food_cnt = 3; // TODO: will get this number from config file

  std::random_device dev;   // use random_device to seed PRNG like mt19937
  std::mt19937 engine;      // Pseudo-random number generator 
  std::uniform_int_distribution<int> random_w;  // define distribution type (uniform here) for random number generation
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void PlaceStone();
  void Update();
};

#endif