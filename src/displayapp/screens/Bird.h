#pragma once

#include "displayapp/apps/Apps.h"
#include "displayapp/screens/Pipe.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/Controllers.h"
#include "Symbols.h"

#include <lvgl/lvgl.h>
#include <cstdint>
#include <memory>

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

      private:
        void WaitForRestart();

        uint16_t score = 0;
        int16_t birdY = 0; // Bird center
        static constexpr uint8_t birdX = 30;
        static constexpr uint8_t birdSize = 16;
        static constexpr uint8_t numberOfPipes = 2;
        static constexpr uint8_t screenSize = 240;
        static constexpr uint16_t pipeStartPosition = 370;
        static constexpr uint8_t gravity = 1;
        static constexpr uint8_t maxVelocity = 4;
        static constexpr int8_t minVelocity = -6;
        int8_t velocity = 0;
        int8_t acceleration = 0;
        bool addGravity = true;
        bool hit = false;
        bool restarted = false;
        bool restartBtnActive = false;

        std::unique_ptr<Pipe> pipes[numberOfPipes];

        lv_obj_t* points;
        lv_obj_t* bird;
        lv_obj_t* background;
        lv_obj_t *restartButton, *txtRestart;

        lv_task_t* taskRefresh;
      };
    }
    
    template <>
    struct AppTraits<Apps::Bird> {
      static constexpr Apps app = Apps::Bird;
      static constexpr const char* icon = Screens::Symbols::bird;
      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::Bird();
      };
    };
  }
}
