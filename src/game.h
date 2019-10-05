#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <fstream>
#include <sstream>

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
    Level(std::string levelfile) {
      std::ifstream infile("levels/" + levelfile);
      // check if open is success
      if (!infile) {
        std::cout << "Cannot open file.\n";
        exit(1);
      }

      // parse level file
      std::string line;
      // first line - level id
      std::getline(infile, line);
      level_id = std::stoi(line);

      // food count
      std::getline(infile, line);
      food_cnt = std::stoi(line);

      // food points
      std::getline(infile, line);
      // split line at whitespaces
      std::istringstream iss(line);
      std::istream_iterator<std::string> start(iss), end;
      std::vector<std::string> results(start, end);
      for (std::string result : results) {
        std::string temp = result.substr(1, result.length() - 2); // example: "7,23"
        std::istringstream ss(temp);
        std::string num_str;
        std::getline(ss, num_str, ','); // get first number
        SDL_Point point;
        point.x = stoi(num_str);
        std::getline(ss, num_str);    // get second number
        point.y = stoi(num_str);

        food.push_back(point);
      }

      // stone count
      std::getline(infile, line);
      stone_cnt = std::stoi(line);

      // stone points
      std::getline(infile, line);
      std::istringstream iss2(line);
      std::istream_iterator<std::string> start2(iss2), end2;
      std::vector<std::string> results2(start2, end2);
      for (std::string result : results2) {
        std::string temp = result.substr(1, result.length() - 2); // example: "7,23"
        std::istringstream ss(temp);
        std::string num_str;
        std::getline(ss, num_str, ','); // get first number
        SDL_Point point;
        point.x = stoi(num_str);
        std::getline(ss, num_str);    // get second number
        point.y = stoi(num_str);

        stone.push_back(point);
      }

      infile.close();
    }
  };

  Snake snake;
  std::vector<SDL_Point> food;
  std::vector<SDL_Point> stone;
  int food_cnt = 3; // TODO: will get this number from config file
  bool level_finish;  // flag that indicates a pause is needed
  int m_level;  // game level
  std::size_t m_grid_width;
  std::size_t m_grid_height;

  std::random_device dev;   // use random_device to seed PRNG like mt19937
  std::mt19937 engine;      // Pseudo-random number generator 
  std::uniform_int_distribution<int> random_w;  // define distribution type (uniform here) for random number generation
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood(int level);
  void PlaceStone(int level);
  void Update();
  void LevelInit(Controller const &controller, bool &running, Renderer &renderer, int level);
  void LevelWelcomeScreen(Renderer &renderer, int level);
  bool level_running; // 
};

#endif