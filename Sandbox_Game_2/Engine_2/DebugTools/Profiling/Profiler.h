#ifndef ENGINE_DEBUG_TOOLS_PROFILER_H
#define ENGINE_DEBUG_TOOLS_PROFILER_H

#include <string>
#include <Misc\Typedefs.h>

namespace Profiling
{
   class __declspec(dllexport) Profiler
   {
   public:
      static Profiler &get_instance();

#ifdef PROFILER_ON
      void reset();
      void new_frame();
      void add_category(const std::string &new_category);
      void add_category_time_log(const std::string &category_name, float time);
      void flush_to_file(const std::string &file_path, const char delim);

      static const uint MAX_FRAME_SAMPLES = 500;
      static const uint MAX_PROFILER_CATEGORIES = 20;
#else
      // if the flag is off, make these functions empty so that the compiler
      // will ignore any calls to these functions, saving the program from 
      // spending unnecessary CPU time performing debugging features
      void reset() {}
      void new_frame() {}
      void add_category(const std::string &new_category) {}
      void add_category_time_log(const std::string &category_name, float time) {}
      void flush_to_file(const std::string &file_path, const char delim) {}
      
      // don't take up memory that isn't needed
      static const uint MAX_FRAME_SAMPLES = 1;
      static const uint MAX_PROFILER_CATEGORIES = 1;
#endif


   private:
      // make these private to force access through get_instance()
      Profiler();
      Profiler(const Profiler&);
      Profiler& operator=(const Profiler&);

      // maintain a static instance of this class internally
      // Note: As a static data member, it is not bound to any instance of the
      // class, but as a result, the declaration here is not considered a 
      // definition.  That is, the declaration here says that the static value
      // exists, but it doesn't say where.  We will define it (that is, bring 
      // it into existance) in the source file for this class.  If PROFILER_ON
      // is false though, then this value will not be defined, which is ok 
      // because it will therefore not be accessed.
      static Profiler m_the_instance;

      int m_frame_index;

      // this defines the profiler data for each frame of each category
      // Note: The memory for it is declared up front to avoid having any 
      // memory allocation mechanisms interfere with the profiling.
      struct m_profiler_category_data
      {
         std::string category_name;
         float samples[MAX_FRAME_SAMPLES];
      } m_profiler_data[MAX_PROFILER_CATEGORIES];
   };
}


#endif