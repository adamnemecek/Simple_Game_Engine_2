#include "Stopwatch.h"

// this is a big header, but necessary to get access to LARGE_INTEGER
// Note: We can't just include winnt.h, in which LARGE_INTEGER is defined,
// because there are some macros that this header file needs that are defined
// further up in the header hierarchy.  So just include Windows.h and be done
// with it.
#include <Utilities\Include_Helper_Windows_Light.h>

// make an empty namespace to keep these values in this file only
// Note: I do this instead of making them class members because their declaration
// requires including Windows.h, which is a large header file and I want to avoid
// bloat if I can, and this is an easy way to avoid it.
namespace
{
   LARGE_INTEGER g_cpu_timer_frequency;
   LARGE_INTEGER g_start_counter;
   LARGE_INTEGER g_last_lap_counter;

   inline float counter_to_seconds(LARGE_INTEGER counter)
   {
      return ((float)counter.QuadPart / g_cpu_timer_frequency.QuadPart);
   }
}

namespace Timing
{

   bool Stopwatch::initialize()
   {
      // the "performance frequency only changes on system reset, so it's ok
      // to do it only during initialization
      // Note: If it succeeds, it returns non-zero, not a bool as C++ knows it.
      // Rather, it returns a BOOL a typedef of an int.
      // http://msdn.microsoft.com/en-us/library/windows/desktop/ms644905(v=vs.85).aspx
      bool success = true;
      success = (0 != QueryPerformanceFrequency(&g_cpu_timer_frequency));

      return true;
   }

   bool Stopwatch::shutdown()
   {
      // nothing happens in shutdown, but return true to keep up the interface 
      // expectations of boolean return values
      return true;
   }

   void Stopwatch::start()
   {
      // give the counters their first values
      // Note: "On systems that run Windows XP or later, the function will always succeed and will thus never return zero."
      // http://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx
      QueryPerformanceCounter(&g_start_counter);
      g_last_lap_counter.QuadPart = g_start_counter.QuadPart;
   }
   
   float Stopwatch::lap()
   {
      // get the current time and convert it to a second
      LARGE_INTEGER now;
      QueryPerformanceCounter(&now);
      
      g_last_lap_counter.QuadPart = now.QuadPart;
      
      float delta_time = counter_to_seconds(now);
      return delta_time;
   }
   
   float Stopwatch::total_time()
   {
      LARGE_INTEGER now;
      QueryPerformanceCounter(&now);

      float delta_time = counter_to_seconds(now);
      return delta_time;
   }

   void Stopwatch::reset()
   {
      // reset the values by giving them new start values
      this->start();
   }
}
