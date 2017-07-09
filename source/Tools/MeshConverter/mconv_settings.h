

#ifndef __MCONV_SETTING_H__
#define __MCONV_SETTING_H__


#include "mconv_prerequisites.h"


namespace mconv
{
    class Settings
    {
    public:
        Settings()
            : mSrcType(E_FILETYPE_FBX)
            , mDstType(E_FILETYPE_T3D)
            , mBoundType(E_BT_AABB)
            , mFileMode(E_FM_SHARE_VERTEX)
            , mVerbose(true)
        {

        }

        String  mSrcPath;       /// 输入文件路径和文件名，当不是绝对路径时，默认是exe同级目录的文件
        String  mDstPath;       /// 输出文件路径和文件名，当为空时，则默认用mSrcPath的路径和文件名

        String  mExtraPath;     /// 附加路径，在OGRE的mesh时，这个是相应的材质文件

        FileType    mSrcType;   /// 输入文件类型，
        FileType    mDstType;

        BoundType   mBoundType;
        FileMode    mFileMode;

        bool    mVerbose;
    };
}


#endif  /*__MCONV_SETTING_H__*/