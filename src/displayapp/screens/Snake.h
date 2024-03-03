#pragma once

#include <lvgl/lvgl.h>
#include <cstdint>
#include <deque>
#include "displayapp/screens/Screen.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Snake : public Screen {
      public:
        Snake();
        ~Snake() override;

        void OnEvent(lv_obj_t* obj, lv_event_t event);
        bool OnTouchEvent(Pinetime::Applications::TouchEvents event);
        void Refresh() override;
        void Show();
        void Clear();
        void AddElement();
        bool Contains(uint8_t X, uint8_t Y);
        void SpawnFood();
        bool Eat();
        bool Hits();
        void WaitForRestart();

      private:
        uint16_t score = 0;
        const uint8_t scale = 15;
        int16_t snakeX = 120;
        int16_t snakeY = 120;
        int8_t speedX = 1;
        int8_t speedY = 0;
        uint8_t direction = 0;
        uint8_t tickCounter = 0;
        std::deque<std::pair<int, int>> history;

        uint8_t foodX;
        uint8_t foodY;

        bool hit = false;
        bool restarted = false;
        bool restartBtnActive = false;

        std::deque<lv_obj_t*> snake;
        lv_obj_t* points;
        lv_obj_t* tail;
        lv_obj_t* food;
        lv_obj_t* background;
        lv_obj_t *restartButton, *txtRestart;

        lv_task_t* taskRefresh;
      };
    }
  }
}
