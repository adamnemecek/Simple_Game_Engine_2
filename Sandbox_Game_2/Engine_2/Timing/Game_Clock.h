#ifndef ENGINE_GAME_CLOCK_H
#define ENGINE_GAME_CLOCK_H

#include <Timing\Stopwatch.h>

namespace Timing
{
   // this is a wrapper for the generic stopwatch and is used by the 
   // game engine to track frame times

   class __declspec(dllexport) Game_Clock
   {
   public:
      static Game_Clock &get_instance();

      bool initialize();
      bool shutdown();

      void new_frame();
      double get_delta_time_last_frame() const;

   private:
      // make these private to force access through get_instance()
      Game_Clock() {}
      Game_Clock(const Game_Clock&);
      Game_Clock &operator=(const Game_Clock&);

      Stopwatch m_stopwatch;
      double m_delta_time_last_frame;
   };
}

#endif
