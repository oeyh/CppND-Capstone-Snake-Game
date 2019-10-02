#include "game.h"
#include <iostream>
#include "SDL.h"


// Contructor, init game
Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width) - 1),  // inclusive on both sides, add -1 to avoid placing food just outside screen
      random_h(0, static_cast<int>(grid_height) - 1) {
  // PlaceFood();
  // PlaceStone();
  m_level = 0;
  level_finish = true;
  // level_running = false;

}

// Game loop
void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    // pause game when needed
    if (level_finish) {
      LevelInit(controller, running, renderer, ++m_level);
    }

    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, stone);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

// init next level
void Game::LevelInit(Controller const &controller, bool &running, Renderer &renderer, int level) {
  // level_finish = false;
  PlaceFood(level);
  PlaceStone(level);
  LevelWelcomeScreen(renderer, level);

  while (level_finish) {
    controller.HandlePause(running, level_finish);
  }
}
void Game::SetLevelRunning(bool run_flag) {
  level_finish = !run_flag;
}

// init food location
void Game::PlaceFood(int level) {
  for (int i = 0; i < food_cnt; ++i) {
    SDL_Point point;
    point.x = (i + 3) * 2;
    point.y = i + 5;
    food.push_back(point);
  }
  return;
}

// init stone location
void Game::PlaceStone(int level) {
  for (int i = 0; i < 3; ++i) {
    SDL_Point point;
    point.x = 5 + i * 10;
    point.y = 5 + i * 10;
    stone.push_back(point);
  }
  return;
}

// init level screen
void Game::LevelWelcomeScreen(Renderer &renderer, int level) {
  renderer.Render(snake, food, stone);
  return;
}


void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  bool hit_food = false;
  int food_num = 0;
  for (SDL_Point food_point : food) {
    if (food_point.x == new_x && food_point.y == new_y) {
      score++;
      // PlaceFood();
      hit_food = true;
      // Grow snake and increase speed.
      snake.GrowBody();
      // snake.speed += 0.02;
      // snake.speed += 0.01;  // my slower mode
      break;
    }
    ++food_num;
  }

  // remove the food point that has been consumed
  if (hit_food) {
    food.erase(food.begin() + food_num);
  }

}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }