#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

// JM select game mode
GameMode SelectGameMode() {
  int choice;
  std::cout << "Select Game Mode:\n";
  std::cout << "1. Classic Snake with no obstacles?\n";
  std::cout << "2. Classic Snake With Obstacles?\n";
  std::cout << "3. Classic Snake with random moving blocks?\n";
  std::cout << "4. Classic Snake with moving blocks that are out to get you?\n";
  
  std::cin >> choice;

  switch (choice) {
    case 1:
      return GameMode::CLASSIC;
    case 2:
      return GameMode::HARD;
    case 3:
      return GameMode::GOOD_LUCK;
    case 4:
      return GameMode::DIRECTED;
    default:
      std::cout << "Invalid selection. Defaulting to the hardest mode. Choose properly next time...\n";
      return GameMode::GOOD_LUCK;
  }
}

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  
  //JM add game mode selection
  GameMode mode = SelectGameMode();

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight, mode);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}