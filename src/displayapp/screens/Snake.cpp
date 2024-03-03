#include "displayapp/screens/Snake.h"
#include "displayapp/DisplayApp.h"
#include "displayapp/LittleVgl.h"
#include "Symbols.h"
#include <cstdlib> // for rand()

using namespace Pinetime::Applications::Screens;

static void event_handler(lv_obj_t* obj, lv_event_t event) {
  Snake* screen = static_cast<Snake*>(obj->user_data);
  screen->OnEvent(obj, event);
}

Snake::Snake() {
  background = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_size(background, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_pos(background, 0, 0);
  lv_obj_set_style_local_radius(background, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_bg_color(background, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_obj_set_style_local_border_color(background, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_border_width(background, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 1);

  history.push_back({snakeX, snakeY});
  AddElement();

  food = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_radius(food, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_style_local_bg_color(food, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_size(food, scale, scale);

  SpawnFood();

  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

Snake::~Snake() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void Snake::OnEvent(lv_obj_t* obj, lv_event_t event) {
  (void)event;
  if (obj == restartButton) {
    restarted = true;
  }
}

bool Snake::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  switch (event) {
    case TouchEvents::SwipeLeft:
      if (direction != 1) {
        speedX = -1;
        speedY = 0;
        direction = 0;
      }
      return true;
    case TouchEvents::SwipeRight:
      if (direction != 0) {
        speedX = 1;
        speedY = 0;
        direction = 1;
      }
      return true;
    case TouchEvents::SwipeUp:
      if (direction != 3) {
        speedX = 0;
        speedY = -1;
        direction = 2;
      }
      return true;
    case TouchEvents::SwipeDown:
      if (direction != 2) {
        speedX = 0;
        speedY = 1;
        direction = 3;
      }
      return true;
    default:
      return false;
  }
  return false;
}

void Snake::Refresh() {
  if (hit == true) {
    WaitForRestart();
  } else {
    restarted = false;
    tickCounter++;
    if (tickCounter > 15) {
      tickCounter = 0;
      snakeX = snakeX + speedX * scale;
      snakeY = snakeY + speedY * scale;
      if (Hits()) {
        hit = true;
      }
      if (Eat()) {
        history.push_back({snakeX, snakeY});
        AddElement();
        score++;
      }
      history.pop_front();
      history.push_back({snakeX, snakeY});
      Show();
    }
  }
}

void Snake::Show() {
  tail = snake.front();
  snake.pop_front();
  lv_obj_set_pos(tail, history.back().first, history.back().second);
  snake.push_back(tail);
}

void Snake::Clear() {
  for (uint8_t i = 0; i <= snake.size() - 1; i++) {
    lv_obj_del(snake[i]);
  }
  history.clear();
  snake.clear();
}

void Snake::AddElement() {
  lv_obj_t* new_element = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_radius(new_element, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_bg_color(new_element, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_size(new_element, scale, scale);
  lv_obj_set_style_local_border_color(new_element, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_obj_set_style_local_border_width(new_element, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 1);
  lv_obj_set_pos(new_element, snakeX, snakeY);
  snake.push_back(new_element);
}

bool Snake::Contains(uint8_t X, uint8_t Y) {
  for (uint8_t i = 0; i < history.size(); i++) {
    if ((X == history[i].first) && (Y == history[i].second)) {
      return true;
    }
  }
  return false;
}

void Snake::SpawnFood() {
  do {
    foodX = rand() % (240 / scale) * scale;
    foodY = rand() % (240 / scale) * scale;
  } while (Contains(foodX, foodY));
  lv_obj_set_pos(food, foodX, foodY);
}

bool Snake::Eat() {
  if ((snakeX == foodX) and (snakeY == foodY)) {
    SpawnFood();
    return true;
  }
  return false;
}

bool Snake::Hits() {
  if ((snakeX < 0) || (snakeX >= 240) || (snakeY < 0) || (snakeY >= 240)) {
    return true;
  }
  return Contains(snakeX, snakeY);
}

void Snake::WaitForRestart() {
  if (restartBtnActive == false) {
    points = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_set_style_local_text_font(points, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_42);
    lv_label_set_text_fmt(points, "%04d", score);
    lv_obj_align(points, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 10);
    restartButton = lv_btn_create(background, nullptr);
    restartButton->user_data = this;
    lv_obj_set_event_cb(restartButton, event_handler);
    lv_obj_set_size(restartButton, 50, 50);
    lv_obj_align(restartButton, nullptr, LV_ALIGN_CENTER, 0, 0);
    txtRestart = lv_label_create(restartButton, nullptr);
    lv_label_set_text(txtRestart, Symbols::play);
    restartBtnActive = true;
  }
  if (restarted == true) {
    lv_obj_del(points);
    lv_obj_del(restartButton);
    lv_obj_del(txtRestart);
    SpawnFood();
    restartBtnActive = false;
    hit = false;
    tickCounter = 0;
    snakeX = 120;
    snakeY = 120;
    speedX = 1;
    speedY = 0;
    direction = 0;
    Clear();
    history.push_back({snakeX, snakeY});
    AddElement();
  }
}
