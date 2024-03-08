#include "displayapp/screens/Bird.h"
#include "displayapp/DisplayApp.h"
#include "displayapp/LittleVgl.h"
#include "Symbols.h"
#include <stdio.h>

using namespace Pinetime::Applications::Screens;

static void event_handler(lv_obj_t* obj, lv_event_t event) {
  Bird* screen = static_cast<Bird*>(lv_obj_get_user_data(obj));
  screen->OnEvent(obj, event);
}

Bird::Bird() {
  background = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_size(background, LV_HOR_RES + 1, LV_VER_RES);
  lv_obj_set_pos(background, -1, 0);
  lv_obj_set_style_local_radius(background, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_bg_color(background, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

  points = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(points, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_42);
  lv_label_set_text(points, "0000");
  lv_obj_align(points, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 10);

  bird = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(bird, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_radius(bird, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_size(bird, birdSize, birdSize);
  lv_obj_set_pos(bird, birdX, birdY);

  pipes[0] = std::make_unique<Pipe>(screenSize);
  pipes[1] = std::make_unique<Pipe>(pipeStartPosition);

  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

Bird::~Bird() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void Bird::OnEvent(lv_obj_t* obj, lv_event_t event) {
  (void) obj;
  (void) event;
  restarted = true;
}

bool Bird::OnTouchEvent(uint16_t x, uint16_t y) {
  (void) x;
  (void) y;
  acceleration -= 1;
  return true;
}

void Bird::Refresh() {
  if (!hit) {
    restarted = false;

    // add gravity every second run
    if (addGravity) {
      acceleration += gravity;
      addGravity = false;
    }
    else {
      addGravity = true;
    }
      
    birdY += velocity;
    velocity += acceleration;
    if (velocity > maxVelocity) {
      velocity = maxVelocity;
    }
    if (velocity < minVelocity) {
      velocity = minVelocity;
    }
    acceleration = 0;

    if (birdY < 0) {
      birdY = 0;
      velocity = 0;
    }
    if (birdY > screenSize) {
      hit = true;
    }

    lv_obj_set_y(bird, birdY);

    for (int i = 0; i < numberOfPipes; i++) {
      if (pipes[i]->Hits(birdX, birdY, birdSize)) {
        hit = true;
      }
      pipes[i]->MovePipe();
      if (pipes[i]->OutOfScreen()) {
        pipes[i]->Reset(screenSize);
        score++;
        lv_label_set_text_fmt(points, "%04d", score);
      } else {
        pipes[i]->UpdatePipe();
      }
    }
  } else {
    WaitForRestart();
  }
}

void Bird::WaitForRestart() {
  if (!restartBtnActive) {
    restartButton = lv_btn_create(background, nullptr);
    lv_obj_set_event_cb(restartButton, event_handler);
    lv_obj_set_size(restartButton, 50, 50);
    lv_obj_align(restartButton, nullptr, LV_ALIGN_CENTER, 0, 0);
    txtRestart = lv_label_create(restartButton, nullptr);
    lv_label_set_text(txtRestart, Symbols::play);
    lv_obj_set_user_data(restartButton, this);
    restartBtnActive = true;
  }
  if (restarted) {
    lv_obj_del(restartButton);
    lv_obj_del(txtRestart);
    restartBtnActive = false;
    score = 0;
    lv_label_set_text_fmt(points, "%04d", score);
    hit = false;
    birdY = 100;
    velocity = 0;
    acceleration = 0;
    pipes[0]->Reset(screenSize);
    pipes[1]->Reset(pipeStartPosition);
  }
}
