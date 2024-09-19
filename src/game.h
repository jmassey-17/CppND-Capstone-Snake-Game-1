#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

//JM 
#include <mutex>
#include <thread>
#include <atomic>  // For boolean 
#include <condition_variable>

enum class GameMode {
  CLASSIC,
  HARD,
  GOOD_LUCK, 
  DIRECTED
};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, GameMode mode);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  
  // JM 
  ~Game(); //destructor for threads

 private:
  // JM snake now unique pointer
  std::unique_ptr<Snake> snake;
  SDL_Point food;
  // JM obstacle 
  std::vector<SDL_Point> obstacles;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  
  // JM move obstacle
  std::uniform_int_distribution<int> random_direction;
  std::size_t grid_width;   
  std::size_t grid_height;  
  
  // JM Game mode 
  GameMode mode;
  
  // JM seperating snake and obstacles into different threads
  std::mutex game_mutex;     
  std::condition_variable cv;
  std::thread obstacle_thread;    
  std::thread snake_thread;
  std::atomic<bool> running;       
  
  int score{0};

  void PlaceFood();
  void Update();
  //JM methods
  void PlaceObstacle(std::vector<SDL_Point> &obstacles);
  
  // obstacle thread
  void MoveObstacles(GameMode mode);
  void ObstacleWorker(GameMode mode);       
  
  // snake threads
  void MoveSnake();
  void SnakeWorker();      
  
};

#endif