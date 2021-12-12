#pragma once

#include <lvgl/lvgl.h>
#include <cstdint>
#include "displayapp/screens/Screen.h"

class Pipe {
public:
  Pipe(int pos) : pipeX(pos) {};
  Pipe() : Pipe(240) {};
  ~Pipe();

  void Show();
  void Randomize();
  bool Hits(uint8_t birdX, int16_t birdY, uint8_t birdSize);
  void UpdatePipe();
  bool OutOfScreen();
  void Reset(int pos);

private:
  bool hit = false;
  uint8_t spacePosition;
  uint8_t spaceSize = 70;
  int16_t pipeTop;
  int16_t pipeBottom;
  uint8_t pipeSpeed = 2;
  int16_t pipeX;
  uint8_t pipeW = 20;

  lv_obj_t* topPipe;
  lv_obj_t* bottomPipe;
};
