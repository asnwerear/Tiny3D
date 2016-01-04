

namespace Tiny3D
{
    inline uint32_t Bound::getID() const
    {
        return mID;
    }

    inline void Bound::setGroupID(uint32_t groupID)
    {
        mGroupID = groupID;
    }

    inline uint32_t Bound::getGroupID() const
    {
        return mGroupID;
    }

    inline const Sphere &Bound::getSphere() const
    {
        return mSphere;
    }

    inline const SGVisual *Bound::getNode() const
    {
        return mNode;
    }

    inline SGVisual *Bound::getNode()
    {
        return mNode;
    }

    inline void Bound::setCollisionSource(bool isSource)
    {
        mCollisionSource = isSource;
    }

    inline bool Bound::isCollisionSource() const
    {
        return mCollisionSource;
    }

    inline void Bound::setEnable(bool enable)
    {
        mEnable = enable;
    }

    inline bool Bound::isEnable() const
    {
        return mEnable;
    }
}