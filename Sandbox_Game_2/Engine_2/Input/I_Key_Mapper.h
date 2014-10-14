#ifndef ENGINE_I_KEY_MAPPER_H
#define ENGINE_I_KEY_MAPPER_H

namespace Input
{
   // an interface, so it has purely virtual methods
   class I_Key_Mapper
   {
   public:
      virtual int get_key_for(int action) const = 0;
   };
}

#endif