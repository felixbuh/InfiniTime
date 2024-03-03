#include "displayapp/screens/Pipe.h"
#include "displayapp/DisplayApp.h"
#include "displayapp/LittleVgl.h"

#include <cstdlib> // for rand()

Pipe::Pipe(int initialX) : pipeX(initialX) {
  Randomize();

  topPipe = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(topPipe, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_radius(topPipe, 0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT);
  lv_obj_set_size(topPipe, pipeW, pipeTop);

  bottomPipe = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(bottomPipe, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_radius(bottomPipe, 0, LV_BTN_PART_MAIN, LV_STATE_DEFAULT);
  lv_obj_set_size(bottomPipe, pipeW, pipeBottom);
}

Pipe::~Pipe() {
  lv_obj_del(topPipe);
  lv_obj_del(bottomPipe);
}

void Pipe::Randomize() {
  spacePosition = rand() % (screenSize - 2 * spaceSize - 10) + spaceSize + 10;
  pipeTop = spacePosition - spaceSize;
  pipeBottom = screenSize - spacePosition - spaceSize;
}

bool Pipe::Hits(uint8_t birdX, int16_t birdY, uint8_t birdSize) {
  if ((birdX > pipeX) && (birdX < (pipeX + pipeW)) &&
      (((birdY < (pipeTop + birdSize)) || (birdY > (screenSize - pipeBottom - birdSize))))) {
    hit = true;
    return true;
  }
  return false;
}

void Pipe::MovePipe() {
  pipeX -= pipeSpeed;
}

void Pipe::UpdatePipe() {
  lv_color_t pipeColor = hit ? LV_COLOR_ORANGE : LV_COLOR_WHITE;
  lv_obj_set_style_local_bg_color(topPipe, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, pipeColor);
  lv_obj_set_style_local_bg_color(bottomPipe, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, pipeColor);

  lv_obj_set_pos(topPipe, pipeX, 0);
  lv_obj_set_pos(bottomPipe, pipeX, screenSize - pipeBottom);
}

bool Pipe::OutOfScreen() {
  return pipeX < -pipeW;
}

void Pipe::Reset(int pos) {
  hit = false;
  pipeX = pos;
  Randomize();
  lv_obj_set_size(topPipe, pipeW, pipeTop);
  lv_obj_set_size(bottomPipe, pipeW, pipeBottom);
}
