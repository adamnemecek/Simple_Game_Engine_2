#ifndef ENGINE_ACTION_LIST_H
#define ENGINE_ACTION_LIST_H

namespace Input
{
   enum ACTION_LIST
   {
      ACCELERTATE    = 0x00000001,
      ROTATE_LEFT    = 0x00000002,
      ROTATE_RIGHT   = 0x00000004,
      MAX            = 0x00000008,
   };
}

#endif
