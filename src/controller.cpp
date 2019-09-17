#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

// change direction based on input direction
// opposite direction is also fed manually (may be not the best way to do it)
void Controller::ChangeDirection(Snake &snake, Snake::Direction input) const {
  if (static_cast<int>(snake.direction) + static_cast<int>(input) != 3 || snake.size == 1) snake.direction = input;
  return;
}

// handles quit event and key stroke of direction keys
void Controller::HandleInput(bool &running, Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight);
          break;
      }
    }
  }
}