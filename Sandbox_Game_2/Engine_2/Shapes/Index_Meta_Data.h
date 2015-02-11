#ifndef INDEX_META_DATA_H
#define INDEX_META_DATA_H

// for GL typedefs
#include <Utilities\Include_Helpers\GL_Version.h>

// for my own typedefs
#include <Utilities\Typedefs.h>

namespace Shapes
{
   struct Index_Meta_Data
   {
      Index_Meta_Data(GLenum render_mode, uint num_indices_for_this_mode) :
         m_render_mode(render_mode),
         m_num_indices_this_mode(num_indices_for_this_mode)
      {
      }

      GLenum m_render_mode;
      uint m_num_indices_this_mode;
   };
}

#endif