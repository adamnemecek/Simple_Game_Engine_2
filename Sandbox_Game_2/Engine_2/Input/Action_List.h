#ifndef ENGINE_ACTION_LIST_H
#define ENGINE_ACTION_LIST_H

namespace Input
{
   enum ACTION_LIST
   {
      FORWARD = (1 << 1),
      BACK = (1 << 2),
      ROTATE_LEFT = (1 << 3),
      ROTATE_RIGHT = (1 << 4),
      GO_UP = (1 << 5),
      GO_DOWN = (1 << 6),
      TILT_LEFT = (1 << 7),
      TILT_RIGHT = (1 << 8),
      TILT_FORWARD = (1 << 9),
      TILT_BACK = (1 << 10),
      STRAFE_LEFT = (1 << 11),
      STRAFE_RIGHT = (1 << 12),
   };
}

#endif
