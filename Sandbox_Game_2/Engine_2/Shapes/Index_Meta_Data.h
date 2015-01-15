#ifndef INDEX_META_DATA_H
#define INDEX_META_DATA_H

// for GL typedefs
#include <Utilities\Include_Helpers\GL_Version.h>

// for my own typedefs
#include <Utilities\Typedefs.h>

struct Index_Meta_Data
{
   GLenum m_render_mode;
   uint m_num_indices_this_mode;
};

#endif