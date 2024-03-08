#pragma once

#include <lvgl/lvgl.h>
#include <cstdint>
#include "displayapp/screens/Screen.h"

class Pipe {
public:
  Pipe(int initialX);
  Pipe() : Pipe(240) {};
  ~Pipe();

  bool Hits(uint8_t birdX, int16_t birdY, uint8_t birdSize);
  void MovePipe();
  void UpdatePipe();
  bool OutOfScreen();
  void Reset(int pos);

private:
  void Randomize();

  bool hit = false;
  uint8_t spacePosition;
  static constexpr uint8_t spaceSize = 70;
  static constexpr uint8_t screenSize = 240;
  int16_t pipeTop;
  int16_t pipeBottom;
  static constexpr uint8_t pipeSpeed = 2;
  int16_t pipeX;
  static constexpr uint8_t pipeW = 20;

  lv_obj_t* topPipe;
  lv_obj_t* bottomPipe;
};
