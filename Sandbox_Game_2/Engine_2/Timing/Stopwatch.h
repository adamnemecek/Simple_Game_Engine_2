#ifndef ENGINE_STOPWATCH
#define ENGINE_STOPWATCH


namespace Timing
{
   class __declspec(dllexport) Stopwatch
   {
   public:
      bool initialize();
      bool shutdown();

      // delta time is in seconds
      void start();
      double lap();
      double total_time();
      void reset();
   };
}

#endif