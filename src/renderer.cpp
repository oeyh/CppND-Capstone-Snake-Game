#include "renderer.h"
#include <iostream>
#include <string>

// Renderer constructor
Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

// Destructor
Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}


// Main render function
void Renderer::Render(Snake const snake, std::vector<SDL_Point> const &food, std::vector<bool> const &food_status, std::vector<SDL_Point> const &stone, Snake::LevelStatus level_status, int level) {
  SDL_Rect block;
  block.w = screen_width / grid_width;      // grid_width refers to number of grids in x; so w is number of pixels in x
  block.h = screen_height / grid_height;    // grid_height refers to number of grids in y; so h is number of pixels in y

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);   // black grey
  SDL_RenderClear(sdl_renderer);

  // Render food
  // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);   // Yellow
  SDL_Texture* food_image = IMG_LoadTexture(sdl_renderer, "../images/pacman-yellow2.png");
  for (int i = 0; i < food.size(); ++i) {
    if (food_status[i]) {   // only render food that's still there
      block.x = food[i].x * block.w;
      block.y = food[i].y * block.h;
      // SDL_RenderFillRect(sdl_renderer, &block);
      SDL_RenderCopy(sdl_renderer, food_image, nullptr, &block);
    }
  }

  // Render stone
  // SDL_SetRenderDrawColor(sdl_renderer, 0x80, 0xFF, 0x00, 0xFF);   // Green
  SDL_Texture* stone_image = IMG_LoadTexture(sdl_renderer, "../images/wall.png");
  for (SDL_Point const &point : stone) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    // SDL_RenderFillRect(sdl_renderer, &block);
    SDL_RenderCopy(sdl_renderer, stone_image, nullptr, &block);
  }

  // Render snake's body
  // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);   // White
  SDL_Texture* body_image = IMG_LoadTexture(sdl_renderer, "../images/greytinytank.png");
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    // SDL_RenderFillRect(sdl_renderer, &block);
    SDL_RenderCopy(sdl_renderer, body_image, nullptr, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;

  SDL_Texture* head_image;
  if (snake.alive) {
    // SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);   // Blue
    head_image = IMG_LoadTexture(sdl_renderer, "../images/greentank.png");
  } else {
    head_image = IMG_LoadTexture(sdl_renderer, "../images/blast3.png");
    // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);   // Red
    // Also show game over screen
    SDL_Texture* level_image = IMG_LoadTexture(sdl_renderer, "../images/gameover.png");
    SDL_Rect block;
    block.w = screen_width / grid_width * 18;      // This is how a grid's width and height are defined
    block.h = screen_height / grid_height * 6;
    block.x = screen_width / grid_width * 7;
    block.y = screen_height / grid_height * 1;
    SDL_RenderCopy(sdl_renderer, level_image, nullptr, &block);
  }
  // SDL_RenderFillRect(sdl_renderer, &block);
  SDL_RenderCopy(sdl_renderer, head_image, nullptr, &block);


  // Render level screen based on level_status
  if (level_status == Snake::LevelStatus::LEVEL_END) {
    std::string filename = "../images/level" + std::to_string(level) + ".png";
    SDL_Texture* level_image = IMG_LoadTexture(sdl_renderer, filename.c_str());
    SDL_Rect block;
    block.w = screen_width / grid_width * 18;      // This is how a grid's width and height are defined
    block.h = screen_height / grid_height * 6;
    block.x = screen_width / grid_width * 7;
    block.y = screen_height / grid_height * 1;
    SDL_RenderCopy(sdl_renderer, level_image, nullptr, &block);
  } else if (level_status == Snake::LevelStatus::GAME_END) {
    SDL_Texture* level_image = IMG_LoadTexture(sdl_renderer, "../images/success.png");
    SDL_Rect block;
    block.w = screen_width / grid_width * 18;      // This is how a grid's width and height are defined
    block.h = screen_height / grid_height * 6;
    block.x = screen_width / grid_width * 7;
    block.y = screen_height / grid_height * 1;
    SDL_RenderCopy(sdl_renderer, level_image, nullptr, &block);
  } 


  // Update Screen
  // Up until now everything was drawn behind the scenes.
  // This will show the contents of the window.
  SDL_RenderPresent(sdl_renderer);
}

// Update window title
void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
