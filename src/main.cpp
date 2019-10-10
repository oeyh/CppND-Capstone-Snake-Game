#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};   // constexpr makes sure it is a compile-time constant; frames per second
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};   // ms per frame = 1000 / frame per second
  constexpr std::size_t kScreenWidth{800};    // screen width
  constexpr std::size_t kScreenHeight{800};   // screen height
  constexpr std::size_t kGridWidth{32};       // number of grids in horizontal direction
  constexpr std::size_t kGridHeight{32};      // number of grids in vertical direction

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);  // create Renderer object
  Controller controller;    // create controller object, used to interface with game control input
  Game game(kGridWidth, kGridHeight);   // create game object
  game.Run(controller, renderer, kMsPerFrame);    // run game, start game loop
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}