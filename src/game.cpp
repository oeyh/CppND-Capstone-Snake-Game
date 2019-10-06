#include "game.h"
#include <iostream>
#include "SDL.h"
#include <algorithm> // for std::count


// Contructor, init game
Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width) - 1),  // inclusive on both sides, add -1 to avoid placing food just outside screen
      random_h(0, static_cast<int>(grid_height) - 1) {
  // PlaceFood();
  // PlaceStone();
  m_level = 1;
  level_finish = true;
  m_grid_width = grid_width;
  m_grid_height = grid_height;
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

      // std::cout << "Before Level Init\n";
      LevelInit(controller, running, renderer, m_level);
      m_level++;
      // LevelInit(controller, running, renderer, m_level);  // for testing, level does not increment
      // std::cout << "After Level Init\n";
    }

    // if running flag becomes false, quit game loop
    if (!running) {
      break;
    }

    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, food_status, stone);

    // for debug
    // std::cout << "After Update and Render\n";
    // std::cout << "Food Count: " << food_cnt << "\n";

    // level completes when food_cnt becomes 0; food_cnt is updated in Game::Update()
    if (food_cnt == 0) {
      level_finish = true;
    }

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
  if (level <= total_levels) {
    // read level config file and init level
    std::string level_file {"level_" + std::to_string(level) + ".dat"};
    std::cout << level_file << "\n";
    Level levelconfig(level_file);

    // level_finish = false;
    // std::cout << "Before Place Food\n";
    PlaceFood(levelconfig);
    // std::cout << "Before Place Stone\n";
    PlaceStone(levelconfig);
    // std::cout << "Before LevelWelcome screen\n";
    LevelWelcomeScreen(renderer, levelconfig.level_id);

    // Also init snake
    snake = Snake(m_grid_width, m_grid_height);
  }

  while (level_finish) {
    // std::cout << "Before HandlePause\n";
    // if running flag becomes false, quit loop
    if (!running) {  
      break;
    }
    controller.HandlePause(running, level_finish); // click cross on window would set running to false and quit the game loop
    // SDL_Delay(100);
  
    // std::cout << "After HandlePause\n";
  }
  // std::cout << "After HandlePause while loop\n";
}

// void Game::SetLevelRunning(bool run_flag) {
//   level_finish = !run_flag;
// }

// init food location
void Game::PlaceFood(Level &lc) {
  food = lc.food;
  food_cnt = lc.food_cnt;
  food_status = std::vector<bool>(food_cnt, true);  // init to all true
}

// init stone location
void Game::PlaceStone(Level &lc) {
  stone = lc.stone;
}

// init level screen
void Game::LevelWelcomeScreen(Renderer &renderer, int level) {
  renderer.Render(snake, food, food_status, stone);
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

  // // record the food point that has been consumed
  if (hit_food) {
    food_status[food_num] = false;
    food_cnt = std::count(food_status.begin(), food_status.end(), true);   // update food count, level completes when food count = 0
  }

  // Check if snake hit stone; if so, the snake dies
  for (SDL_Point stone_point : stone) {
    if (stone_point.x == new_x && stone_point.y == new_y) {
      snake.alive = false;
      break;
    }
  }

}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

// Nested class constructor
Game::Level::Level(std::string levelfile) {
  std::ifstream infile("../levels/" + levelfile);
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