#include "Geometry_Loader.h"


namespace Utility
{
   Geometry_Loader &Geometry_Loader::get_instance()
   {
      static Geometry_Loader instance;

      return instance;
   }


}