#ifndef ENGINE_ACTION_LIST_H
#define ENGINE_ACTION_LIST_H

namespace Input
{
   enum ACTION_LIST
   {
      ACCELERTATE    = (1 << 1),
      ROTATE_LEFT    = (1 << 2),
      ROTATE_RIGHT   = (1 << 3),
      ROTATE_OBJECT_CW_X_AXIS = (1 << 4),
      ROTATE_OBJECT_CCW_X_AXIS = (1 << 5),
      ROTATE_OBJECT_CW_Y_AXIS = (1 << 6),
      ROTATE_OBJECT_CCW_Y_AXIS = (1 << 7),
      MAX            = (1 << 8),
   };
}

#endif
