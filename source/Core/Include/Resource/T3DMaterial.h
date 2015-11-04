

#ifndef __T3D_MATERIAL_H__
#define __T3D_MATERIAL_H__


#include "Resource/T3DResource.h"
#include "Misc/T3DColor4.h"


namespace Tiny3D
{
    class T3D_ENGINE_API Material : public Resource
    {
    public:
        Material();
        virtual ~Material();

    private:
        String  mName;

        Color4  mAmbientColor;
        Color4  mDiffuseColor;
        Color4  mSpecularColor;
        Color4  mEmissiveColor;

        Real    mShininess;

        
    };
}


#endif  /*__T3D_MATERIAL_H__*/
