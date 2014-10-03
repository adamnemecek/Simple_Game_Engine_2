#include "Game_Clock.h"

namespace Timing
{
   Game_Clock &Game_Clock::get_instance()
   {
      static Game_Clock instance;

      return instance;
   }

   bool Game_Clock::initialize()
   {
      m_delta_time_last_frame = 0;
      bool success = m_stopwatch.initialize();
      if (success)
      {
         m_stopwatch.start();
      }

      return success;
   }

   bool Game_Clock::shutdown()
   {
      return m_stopwatch.shutdown();
   }

   // It makes more sense for the user to call new_frame() and then
   // "get delta time last frame" independently than to try to make a
   // single function that does both.
   void Game_Clock::new_frame()
   {
      m_delta_time_last_frame = m_stopwatch.lap();
   }

   float Game_Clock::get_delta_time_last_frame() const
   {
      return m_delta_time_last_frame;
   }

}