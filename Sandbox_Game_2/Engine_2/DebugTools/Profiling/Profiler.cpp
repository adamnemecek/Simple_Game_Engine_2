#include "Profiler.h"

// only include these other files if we are running the profiler
#ifdef PROFILER_ON

// it's ok to use assertions here because this class is only for debugging anyway
#include <cassert>

#include <fstream>
using std::ofstream;
using std::ios;

#include <string>
using std::string;

#include <Utilities\Typedefs.h>
#endif

namespace Profiling
{
   Profiler::Profiler()
   {
      reset();
   }

   Profiler &Profiler::get_instance()
   {
      static Profiler instance;

      return instance;
   }

   // the following are only defined if we are running the compiler, 
   // otherwise the compiler will optimize out the empty definitions 
   // from the header file
#ifdef PROFILER_ON
   void Profiler::reset()
   {
      m_frame_index = -1;
      memset(m_profiler_data, 0, sizeof(m_profiler_category_data) * MAX_PROFILER_CATEGORIES);
   }

   void Profiler::new_frame()
   {
      // the next time log will be added to the next index in the profiler data
      m_frame_index += 1;
   }

   void Profiler::add_category(const string &new_category)
   {
      for (int index = 0; index < MAX_PROFILER_CATEGORIES; index++)
      {
         m_profiler_category_data &data_ref = m_profiler_data[index];
         if (data_ref.category_name.empty())
         {
            // not assigned yet, so make a copy of the string name
            data_ref.category_name = new_category;
         }
         else
         {
            // check that we didn't run out of categories
            assert((index + 1) != MAX_PROFILER_CATEGORIES);
         }
      }
   }

   void Profiler::add_category_time_log(const string &category_name, float time)
   {
      // check that the frame index is valid
      assert(m_frame_index >= 0);

      for (int index = 0; index < MAX_PROFILER_CATEGORIES; index++)
      {
         m_profiler_category_data &data_ref = m_profiler_data[index];
         if (data_ref.category_name == category_name)
         {
            // found the category to add data to, so break
            data_ref.samples[m_frame_index % MAX_FRAME_SAMPLES] = time;
         }
         else
         {
            // check that we didn't run out of categories
            assert((index + 1) != MAX_PROFILER_CATEGORIES);
         }
      }
   }

   void Profiler::flush_to_file(const string &file_path, const char delim)
   {
      // open the file; empty it out if it already exists
      ofstream out_stream(file_path, ios::trunc);

      // write the category names to the top of the file
      for (int index = 0; index < MAX_PROFILER_CATEGORIES; index++)
      {
         string &category_name_ref = m_profiler_data[index].category_name;

         if (!category_name_ref.empty())
         {
            out_stream << category_name_ref << delim;
         }
      }
      out_stream << "\n";

      // write the time data to the file row by row
      for (uint frame_index_counter = 0; frame_index_counter < (m_frame_index % MAX_FRAME_SAMPLES); frame_index_counter++)
      {
         for (uint category_index_counter = 0; category_index_counter < MAX_PROFILER_CATEGORIES; category_index_counter++)
         {
            m_profiler_category_data &data_ref = m_profiler_data[category_index_counter];
            if (data_ref.category_name.empty())
            {
               // gone through all the categories, so break out of the inner loop
               break;
            }

            // category exists, so write the sample data to the file
            out_stream << data_ref.samples[frame_index_counter] << delim;
         }

         // end the row with a new line
         out_stream << "\n";
      }

      // close the file
      out_stream.close();
   }

#endif   // PROFILER_ON
}

