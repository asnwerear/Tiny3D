

#ifndef __T3D_BONE_H__
#define __T3D_BONE_H__


#include "T3DPrerequisitesInternal.h"
#include "T3DTypedefInternal.h"
#include "Misc/T3DNode.h"
#include "Math/T3DMatrix4.h"
#include "Math/T3DTransform.h"


namespace Tiny3D
{
    class Bone : public Node
    {
    public:
        virtual ~Bone();

        static BonePtr create(const String &name, const Matrix4 &m);

        virtual Type getNodeType() const override;
        virtual NodePtr clone() const override;

        void updateBone();

        void setDirty(bool isDirty, bool recursive = false);
        bool isDirty() const { return mIsDirty; }

        const Transform &getCombineTransform();

    protected:
        Bone();
        Bone(const String &name, const Matrix4 &m);

        virtual void cloneProperties(const NodePtr &node) const override;

        virtual void onAttachParent(const NodePtr &parent) override;

    protected:
        Vector3     mTranslation;
        Vector3     mScaling;
        Quaternion  mOrientation;

        Transform   mCombineTransform;

        bool        mIsDirty;
    };
}

#endif  /*__T3D_BONE_H__*/