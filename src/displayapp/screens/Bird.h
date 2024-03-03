#pragma once

#include <lvgl/lvgl.h>
#include <cstdint>
#include <memory>
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Pipe.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Bird : public Screen {
      public:
        Bird();
        ~Bird() override;

        void OnEvent(lv_obj_t* obj, lv_event_t event);
        bool OnTouchEvent(uint16_t x, uint16_t y);
        void Refresh() override;
        void WaitForRestart();

      private:
        uint16_t score = 0;
        int16_t birdY = 0; // Bird center
        static constexpr uint8_t birdX = 30;
        static constexpr uint8_t birdSize = 16;
        static constexpr uint8_t numberOfPipes = 2;
        static constexpr int screenSize = 240;
        static constexpr int pipeStartPosition = 370;
        static constexpr float gravity = 0.5;
        static constexpr float maxVelocity = 4;
        static constexpr float minVelocity = -6;
        float velocity = 0;
        float acceleration = 0;

        std::unique_ptr<Pipe> pipes[numberOfPipes];

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
