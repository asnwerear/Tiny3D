

#ifndef __T3D_MATERIAL_MANAGER_H__
#define __T3D_MATERIAL_MANAGER_H__


#include "Resource/T3DResourceManager.h"
#include "T3DPlatform.h"


namespace Tiny3D
{
    class T3D_ENGINE_API MaterialManager 
        : public Singleton<MaterialManager>
        , public ResourceManager
    {

    };
}

#endif  /*__T3D_MATERIAL_MANAGER_H__*/
