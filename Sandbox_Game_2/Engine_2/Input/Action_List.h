#ifndef ENGINE_ACTION_LIST_H
#define ENGINE_ACTION_LIST_H

namespace Input
{
   enum ACTION_LIST
   {
      FORWARD = (1 << 1),
      BACK = (1 << 2),
      YAW_LEFT = (1 << 3),
      YAW_RIGHT = (1 << 4),
      GO_UP = (1 << 5),
      GO_DOWN = (1 << 6),
      ROLL_LEFT = (1 << 7),
      ROLL_RIGHT = (1 << 8),
      LOOK_DOWN = (1 << 9),
      LOOK_UP = (1 << 10),
      STRAFE_LEFT = (1 << 11),
      STRAFE_RIGHT = (1 << 12),
   };
}

#endif
