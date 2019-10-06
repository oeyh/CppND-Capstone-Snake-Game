#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <fstream>
#include <sstream>
#include <iostream>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer, std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  // void SetLevelRunning(bool);
  

 private:
  // Level stores level information
  struct Level {
    int level_id;
    int food_cnt;
    int stone_cnt;
    std::vector<SDL_Point> food;
    std::vector<SDL_Point> stone;
    // snake speed, etc.

    // level file format
    // level id
    // food count, then food coordinates
    // stone count, then stone coordinates
    Level(std::string levelfile);
  };

  Snake snake;
  std::vector<SDL_Point> food;
  std::vector<SDL_Point> stone;
  int food_cnt; // TODO: will get this number from config file
  std::vector<bool> food_status;  // whether the food is eaten or not
  bool level_finish;  // flag that indicates a pause is needed
  int total_levels = 3; // total number of levels; when reaches total_levels + 1, game ends
  int m_level;  // game level
  std::size_t m_grid_width;
  std::size_t m_grid_height;

  std::random_device dev;   // use random_device to seed PRNG like mt19937
  std::mt19937 engine;      // Pseudo-random number generator 
  std::uniform_int_distribution<int> random_w;  // define distribution type (uniform here) for random number generation
  std::uniform_int_distribution<int> random_h;

  int score{0};
  

  void PlaceFood(Level &lc);
  void PlaceStone(Level &lc);
  void Update();
  void LevelInit(Controller const &controller, bool &running, Renderer &renderer, int level);
  void LevelWelcomeScreen(Renderer &renderer, int level);
  bool level_running; // 
  
};

#endif