#pragma once

#include <lvgl/lvgl.h>
#include <cstdint>
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Pipe.h"

namespace Pinetime {
  namespace Components {
    class LittleVgl;
  }
  namespace Applications {
    namespace Screens {
      class Bird : public Screen {
      public:
        Bird(DisplayApp* app, Pinetime::Components::LittleVgl& lvgl);
        ~Bird() override;

        void OnEvent(lv_obj_t* obj, lv_event_t event);
        bool OnTouchEvent(uint16_t x, uint16_t y);
        void Refresh() override;
        void WaitForRestart();

      private:
        Pinetime::Components::LittleVgl& lvgl;

        uint16_t score = 0;
        int16_t birdY = 0; // Bird center
        const uint8_t birdX = 30;
        const uint8_t birdSize = 16;

        Pipe* pipes[2];

        float velocity = 0;
        float acceleration = 0;
        float gravity = 0.5;

        bool hit = false;
        bool restarted = false;
        bool restartBtnActive = false;

        lv_obj_t* points;
        lv_obj_t* bird;
        lv_obj_t* background;
        lv_obj_t *restartButton, *txtRestart;

        lv_task_t* taskRefresh;
      };
    }
  }
}
