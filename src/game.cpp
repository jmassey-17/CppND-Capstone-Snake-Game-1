#include "game.h"
#include <iostream>
#include "SDL.h"

//JM 
#include <mutex>
#include <thread>
#include <atomic>  // For boolean 

Game::Game(std::size_t grid_width, std::size_t grid_height, GameMode mode)
    : snake(std::make_unique<Snake>(grid_width, grid_height)),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)), 
	  random_direction(0,3), //JM for random obstacle movement
	  grid_width(grid_width),   // JM for obstacle movement
      grid_height(grid_height),  // JM for obstacle movement
	  mode(mode), // JM 
	  running(true) // JM threads
{
  PlaceFood();
  
  // JM thread obstacle initiation
  	obstacle_thread = std::thread(&Game::ObstacleWorker, this, mode);
      
  // JM snake obstacle initiation
     snake_thread = std::thread(&Game::SnakeWorker, this);
}

// JM destructor for therads
Game::~Game() {
  // stop thread loop
	running = false;
  //inform threads
  cv.notify_all();
  // destroy thrads
  if (obstacle_thread.joinable()) {
    obstacle_thread.join();
  }
  if (snake_thread.joinable()) {
    snake_thread.join();
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, *snake);
    Update();
    //JM lock the renderer for the threads
    {
      std::lock_guard<std::mutex> lock(game_mutex);
      renderer.Render(*snake, food, obstacles); //JM
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

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

// JM - adding in obstacles
// method adapted from PlaceFood
void Game::PlaceObstacle(std::vector<SDL_Point> &obstacles) {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    
    // Check that the location is not occupied by a snake item or food
    if (!snake->SnakeCell(x, y) && !(x == food.x && y == food.y)) {
      bool locationFree = true;
      
      // Check that the location is not occupied by another obstacle
      for (const auto &point: obstacles) {
        if (x == point.x && y == point.y) {
          locationFree = false;
          break;
        }
      }
      
      // if free, place the obstacle and exit the function
      if (locationFree) {
        SDL_Point newObstacle = {x, y};
        obstacles.push_back(newObstacle);
        return; 
      }
    }
  }
}

void Game::Update() {
  if (!snake->alive) {
    // kill game and all threads if snake is dead
    running = false;
    cv.notify_all();
    return;
  
  }

  snake->Update();
  
  
  int new_x = static_cast<int>(snake->head_x);
  int new_y = static_cast<int>(snake->head_y);
  
  // JM - Check if the snake has hit an obstacle
  // need to check if thsi kills properly the game or not...
  for (auto const &ob : obstacles) {
    if (new_x == ob.x && new_y == ob.y) {
      snake->alive = false;
      running = false;
      cv.notify_all();
      return;
    }
  }

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // JM game mode options
    if (mode == GameMode::HARD || mode == GameMode::GOOD_LUCK || mode == GameMode::DIRECTED) {
    	PlaceObstacle(obstacles);
    }
    MoveObstacles(mode);
    
    // Grow snake and increase speed.
    snake->GrowBody();
    snake->speed += 0.02;
  }
}

// moves obstacles acording to the game mode
void Game::MoveObstacles(GameMode mode) {
  if (mode == GameMode::GOOD_LUCK) {
    for (auto &ob : obstacles) {
      // Randomly generate number corresponding to directional shift
      int directionCase = random_direction(engine);

      // Define cases for movement
      switch (directionCase) {
        case 0:  // Move up
          ob.y = fmod(ob.y -1 + grid_width, grid_width);
          break;
        case 1:  // Move down
          ob.y = fmod(ob.y +1 + grid_width, grid_width);
          break;
        case 2:  // Move left
          ob.x =  fmod(ob.x -1 + grid_height, grid_height);
          break;
        case 3:  // Move right
          ob.x = fmod(ob.x + 1 + grid_height, grid_height);
          break;
      }
    }
  }
  else if (mode==GameMode::DIRECTED) {
  for (auto &ob : obstacles) {
    // Find the distance between obstacle and snake head 
    int distance_x = ob.x - static_cast<int>(snake->head_x);
    int distance_y = ob.y - static_cast<int>(snake->head_y);
    
    // Move the obstacle towards the snake
    if (abs(distance_x) > abs(distance_y)) {
      // Move horizontally
      if (distance_x > 0) {
        // Move left
        ob.x =  fmod(ob.x -1 + grid_height, grid_height);
      } else {
        // Move right
        ob.x =  fmod(ob.x +1 + grid_height, grid_height);
      }
    } else {
      // Move vertically
      if (distance_y > 0) {
        // Move up
        ob.y = fmod(ob.y +1 + grid_width, grid_width);
      } else {
        // Move down
        ob.y = fmod(ob.y +1 + grid_width, grid_width);
      }
    }
  }
  }
  else {return;}
}

//obtscale worker function for the threads
void Game::ObstacleWorker(GameMode mode) {
	while (running) {
    	std::unique_lock<std::mutex> lock(game_mutex);
      	cv.wait(lock);
      	MoveObstacles(mode);
      std::this_thread::sleep_for(std::chrono::milliseconds(1)); // prevent burnup
    }
  
}
//thred worker function for snake
void Game::SnakeWorker() {
	while (running) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // determines game challenge...
      {
    	std::lock_guard<std::mutex> lock(game_mutex);
    	Update();
    }
      	cv.notify_one();
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // determines game challenge...
    }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake->size; }