

#include "mconv_converter.h"
#include "mconv_command.h"
#include "mconv_settings.h"
#include "mconv_fbxserializer.h"
#include "mconv_t3dSerializer.h"
#include "mconv_node.h"
#include "mconv_scene.h"
#include "mconv_mesh.h"
#include "mconv_model.h"
#include "mconv_material.h"
#include "mconv_animation.h"


namespace mconv
{
    Converter::Converter()
        : mImporter(nullptr)
        , mExporter(nullptr)
#ifdef _DEBUG
        , mTabCount(0)
#endif
    {

    }

    Converter::~Converter()
    {
        delete mImporter;
        mImporter = nullptr;

        delete mExporter;
        mExporter = nullptr;
    }

    bool Converter::execute(int argc, char *argv[])
    {
        bool result = false;

        Command command;
        result = command.parse(argc, argv, mSettings);

        result = result && importScene();
        result = result && convert();
        result = result && exportScene();

        return result;
    }

    bool Converter::importScene()
    {
        delete mImporter;
        mImporter = nullptr;

        if ((mSettings.mSrcType & E_FILETYPE_FBX)
            || (mSettings.mSrcType & E_FILETYPE_DAE))
        {
            mImporter = new FbxSerializer();
        }
        else if ((mSettings.mSrcType & E_FILETYPE_T3D) == E_FILETYPE_T3D)
        {
            mImporter = new T3DSerializer();
        }
        else if (mSettings.mSrcType & E_FILETYPE_T3DB)
        {
            mImporter = new T3DBinSerializer();
        }
        else if (mSettings.mSrcType & E_FILETYPE_T3DT)
        {
            mImporter = new T3DXMLSerializer();
        }
        else
        {
            T3D_LOG_ERROR("Create importer failed ! Because of invalid source file format !");
        }

        bool result = false;

        if (mImporter != nullptr)
        {
            result = mImporter->load(mSettings.mSrcPath, mSrcData);
        }

        return result;
    }

    bool Converter::exportScene()
    {
        delete mExporter;
        mExporter = nullptr;

        if ((mSettings.mDstType & E_FILETYPE_FBX)
            || (mSettings.mDstType & E_FILETYPE_DAE))
        {
            mExporter = new FbxSerializer();
        }
        else if ((mSettings.mDstType & E_FILETYPE_T3D) == E_FILETYPE_T3D)
        {
            mExporter = new T3DSerializer();
        }
        else if (mSettings.mDstType & E_FILETYPE_T3DB)
        {
            mExporter = new T3DBinSerializer();
        }
        else if (mSettings.mDstType & E_FILETYPE_T3DT)
        {
            mExporter = new T3DXMLSerializer();
        }
        else
        {
            T3D_LOG_ERROR("Create exporter failed ! Because of invalid destination file format !");
            T3D_ASSERT(0);
        }

        bool result = false;
        
        if (mExporter != nullptr)
        {
            result = mExporter->save(mSettings.mDstPath, mDstData);
        }

        return result;
    }

    bool Converter::convert()
    {
        bool result = false;
        if ((mSettings.mDstType & E_FILETYPE_FBX) || (mSettings.mDstType & E_FILETYPE_DAE))
        {
            result = convertToFBX();
        }
        else if (mSettings.mDstType & E_FILETYPE_T3D)
        {
            result = convertToT3D();
        }
        else
        {
            T3D_LOG_ERROR("Convert failed ! Because of invalid destination file format !");
            T3D_ASSERT(0);
        }

        return result;
    }

    bool Converter::convertToFBX()
    {
        

        return true;
    }

    bool Converter::convertToT3D()
    {
        if (mSrcData == nullptr)
        {
            T3D_LOG_ERROR("Convert to T3D failed ! Because of invalid source data !");
            return false;
        }

        FbxScene *pFbxScene = static_cast<FbxScene *>(mSrcData);
        String name = pFbxScene->GetName();
        if (name.empty())
        {
            name = "Scene";
        }
        Scene *pScene = new Scene(name);
        mDstData = pScene;

#ifdef _DEBUG
        mTabCount = 0;
#endif

        processFbxScene(pFbxScene, pScene);

        return true;
    }

    bool Converter::processFbxScene(FbxScene *pFbxScene, Node *pRoot)
    {
        FbxNode *pFbxRoot = pFbxScene->GetRootNode();

        bool result = processFbxNode(pFbxRoot, pRoot);
        result = result && optimize(pRoot);
        return result;
    }

    bool Converter::processFbxNode(FbxNode *pFbxNode, Node *pParent)
    {
        bool result = false;
        Node *pNode = nullptr;

#ifdef _DEBUG
        std::stringstream ssTab;
        for (int t = 0; t < mTabCount; ++t)
        {
            ssTab<<"\t";
        }
        T3D_LOG_INFO("%sNode : %s", ssTab.str().c_str(), pFbxNode->GetName());
#endif

        int nAttribCount = pFbxNode->GetNodeAttributeCount();
        int i = 0;
        for (i = 0; i < nAttribCount; ++i)
        {
            if (pFbxNode->GetNodeAttribute() != nullptr)
            {
                FbxNodeAttribute::EType attribType = pFbxNode->GetNodeAttribute()->GetAttributeType();
                switch (attribType)
                {
                case FbxNodeAttribute::eMesh:
                    {
                        result = processFbxMesh(pFbxNode, pParent, pNode);
                        result = result && processFbxSkin(pFbxNode, pParent);
                        result = result && processFbxMaterial(pFbxNode, pParent);
                    }
                    break;
                case FbxNodeAttribute::eSkeleton:
                    {
                        result = processFbxSkeleton(pFbxNode, pParent, pNode);
                    }
                    break;
                case FbxNodeAttribute::eCamera:
                    {
                        result = processFbxCamera(pFbxNode, pParent, pNode);
                    }
                    break;
                case FbxNodeAttribute::eLight:
                    {
                        result = processFbxLight(pFbxNode, pParent, pNode);
                    }
                    break;
                }
            }
        }

        if (pNode == nullptr)
        {
            pNode = pParent;
        }

#ifdef _DEBUG
        mTabCount++;
#endif

        for (i = 0; i < pFbxNode->GetChildCount(); ++i)
        {
            processFbxNode(pFbxNode->GetChild(i), pNode);
        }

#ifdef _DEBUG
        mTabCount--;
#endif

        return result;
    }

    bool Converter::processFbxMesh(FbxNode *pFbxNode, Node *pParent, Node *&pNewNode)
    {
        FbxMesh *pFbxMesh = pFbxNode->GetMesh();

        if (pFbxMesh == nullptr)
        {
            T3D_LOG_ERROR("FBX mesh is invalid !");
            return false;
        }

#ifdef _DEBUG
        std::stringstream ssTab;
        for (int t = 0; t < mTabCount; ++t)
        {
            ssTab<<"\t";
        }
        T3D_LOG_INFO("%sMesh : %s", ssTab.str().c_str(), pFbxNode->GetName());
#endif

        String name = pFbxNode->GetName();
        if (name == "")
        {
            name = "Model";
        }
        Model *pModel = new Model(name);
        pParent->addChild(pModel);
        pNewNode = pModel;

        Mesh *pMesh = new Mesh(name);
        pModel->addChild(pMesh);

        pMesh->mWorldMatrix = pFbxNode->EvaluateGlobalTransform();

        int nTriangleCount = pFbxMesh->GetPolygonCount();
        int nVertexCount = 0;
        int i = 0, j = 0;

        // 生成顶点属性
        processVertexAttribute(pFbxMesh, pMesh);

        // 构建顶点数据
        for (i = 0; i < nTriangleCount; ++i)
        {
            for (j = 0; j < 3; ++j)
            {
                int nControlPointIdx = pFbxMesh->GetPolygonVertex(i, j);

                Vertex vertex;

                // 读取顶点位置信息
                readPosition(pFbxMesh, nControlPointIdx, vertex.mPosition);

                // 读取顶点颜色信息
                FbxVector4 color;
                int k = 0;
                bool ret = false;
                do 
                {
                    ret = readColor(pFbxMesh, nControlPointIdx, nVertexCount, k++, color);
                    if (ret)
                    {
                        vertex.mColorElements.push_back(color);
                    }
                } while (ret);

                // 读取纹理UV坐标
                FbxVector2 uv;
                int nTexCount = pFbxMesh->GetTextureUVCount();
                for (k = 0; k < nTexCount; ++k)
                {
                    if (readUV(pFbxMesh, nControlPointIdx, pFbxMesh->GetTextureUVIndex(i, j), k, uv))
                    {
                        vertex.mTexElements.push_back(uv);
                    }
                }

                // 读取顶点法线
                FbxVector3 normal;
                k = 0;
                do 
                {
                    ret = readNormal(pFbxMesh, nControlPointIdx, nVertexCount, k++, normal);
                    if (ret)
                    {
                        vertex.mNormalElements.push_back(normal);
                    }
                } while (ret);

                // 读取副法线
                FbxVector3 binormal;
                k = 0;
                do
                {
                    ret = readBinormal(pFbxMesh, nControlPointIdx, nVertexCount, k++, binormal);
                    if (ret)
                    {
                        vertex.mBinormalElements.push_back(binormal);
                    }
                } while (ret);

                // 读取切线
                FbxVector3 tangent;
                k = 0;
                do 
                {
                    ret = readTangent(pFbxMesh, nControlPointIdx, nVertexCount, k++, tangent);
                    if (ret)
                    {
                        vertex.mTangentElements.push_back(tangent);
                    }
                } while (ret);

                int nMaterialIdx = 0;
                if (ret = readMaterial(pFbxMesh, i, nMaterialIdx))
                {
                    vertex.mMaterialIdx = nMaterialIdx;
                }

                pMesh->mVertices.push_back(vertex);

                ++nVertexCount;
            }
        }

        return true;
    }

    bool Converter::processVertexAttribute(FbxMesh *pFbxMesh, Mesh *pMesh)
    {
        int i = 0;

        // 位置
        VertexAttribute attribute;
        attribute.mVertexType = VertexAttribute::E_VT_POSITION;
        attribute.mSize = 3;
        attribute.mDataType = VertexAttribute::E_VT_FLOAT;
        pMesh->mAttributes.push_back(attribute);

        int nControlPointsCount = pFbxMesh->GetControlPointsCount();
        int nUVCount = pFbxMesh->GetElementUVCount();
        int nNormalCount = pFbxMesh->GetElementNormalCount();
        int nBinormalCount = pFbxMesh->GetElementBinormalCount();
        int nTangentCount = pFbxMesh->GetElementTangentCount();

        // UV
        for (i = 0; i < pFbxMesh->GetElementUVCount(); ++i)
        {
            attribute.mVertexType = VertexAttribute::E_VT_TEXCOORD;
            attribute.mSize = 2;
            attribute.mDataType = VertexAttribute::E_VT_FLOAT;
            pMesh->mAttributes.push_back(attribute);
        }

        // 法线
        for (i = 0; i < pFbxMesh->GetElementNormalCount(); ++i)
        {
            attribute.mVertexType = VertexAttribute::E_VT_NORMAL;
            attribute.mSize = 3;
            attribute.mDataType = VertexAttribute::E_VT_FLOAT;
            pMesh->mAttributes.push_back(attribute);
        }

        // 副法线
        for (i = 0; i < pFbxMesh->GetElementBinormalCount(); ++i)
        {
            attribute.mVertexType = VertexAttribute::E_VT_BINORMAL;
            attribute.mSize = 3;
            attribute.mDataType = VertexAttribute::E_VT_FLOAT;
            pMesh->mAttributes.push_back(attribute);
        }

        // 切线
        for (i = 0; i < pFbxMesh->GetElementTangentCount(); ++i)
        {
            attribute.mVertexType = VertexAttribute::E_VT_TANGENT;
            attribute.mSize = 3;
            attribute.mDataType = VertexAttribute::E_VT_FLOAT;
            pMesh->mAttributes.push_back(attribute);
        }

        // 颜色
        for (i = 0; i < pFbxMesh->GetElementVertexColorCount(); ++i)
        {
            attribute.mVertexType = VertexAttribute::E_VT_COLOR;
            attribute.mSize = 4;
            attribute.mDataType = VertexAttribute::E_VT_FLOAT;
            pMesh->mAttributes.push_back(attribute);
        }

        return true;
    }

    bool Converter::readPosition(FbxMesh *pFbxMesh, int nControlPointIdx, FbxVector3 &pos)
    {
        FbxVector4 *pControlPoint = pFbxMesh->GetControlPoints();
        pos[0] = pControlPoint[nControlPointIdx][0];
        pos[1] = pControlPoint[nControlPointIdx][1];
        pos[2] = pControlPoint[nControlPointIdx][2];
        return true;
    }

    bool Converter::readColor(FbxMesh *pFbxMesh, int nControlPointIdx, int nVertexIndex, int nLayer, FbxVector4 &color)
    {
        if (pFbxMesh->GetElementVertexColorCount() < 1)
        {
            return false;
        }

        bool result = false;

        FbxGeometryElementVertexColor *pVertexColor = pFbxMesh->GetElementVertexColor(nLayer);
        if (pVertexColor != nullptr)
        {
            result = true;

            switch (pVertexColor->GetMappingMode())
            {
            case FbxGeometryElement::eByControlPoint:
                {
                    switch (pVertexColor->GetReferenceMode())
                    {
                    case FbxGeometryElement::eDirect:
                        {
                            color[0] = pVertexColor->GetDirectArray().GetAt(nControlPointIdx).mBlue;
                            color[1] = pVertexColor->GetDirectArray().GetAt(nControlPointIdx).mGreen;
                            color[2] = pVertexColor->GetDirectArray().GetAt(nControlPointIdx).mRed;
                            color[3] = pVertexColor->GetDirectArray().GetAt(nControlPointIdx).mAlpha;
                        }
                        break;
                    case FbxGeometryElement::eIndex:
                    case FbxGeometryElement::eIndexToDirect:
                        {
                            int idx = pVertexColor->GetIndexArray().GetAt(nControlPointIdx);
                            color[0] = pVertexColor->GetDirectArray().GetAt(idx).mBlue;
                            color[1] = pVertexColor->GetDirectArray().GetAt(idx).mGreen;
                            color[2] = pVertexColor->GetDirectArray().GetAt(idx).mRed;
                            color[3] = pVertexColor->GetDirectArray().GetAt(idx).mAlpha;
                        }
                        break;
                    }
                }
                break;
            case FbxGeometryElement::eByPolygonVertex:
                {
                    switch (pVertexColor->GetReferenceMode())
                    {
                    case FbxGeometryElement::eDirect:
                        {
                            color[0] = pVertexColor->GetDirectArray().GetAt(nVertexIndex).mBlue;
                            color[1] = pVertexColor->GetDirectArray().GetAt(nVertexIndex).mGreen;
                            color[2] = pVertexColor->GetDirectArray().GetAt(nVertexIndex).mRed;
                            color[3] = pVertexColor->GetDirectArray().GetAt(nVertexIndex).mAlpha;
                        }
                        break;
                    case FbxGeometryElement::eIndex:
                    case FbxGeometryElement::eIndexToDirect:
                        {
                            int idx = pVertexColor->GetIndexArray().GetAt(nVertexIndex);
                            color[0] = pVertexColor->GetDirectArray().GetAt(idx).mBlue;
                            color[1] = pVertexColor->GetDirectArray().GetAt(idx).mGreen;
                            color[2] = pVertexColor->GetDirectArray().GetAt(idx).mRed;
                            color[3] = pVertexColor->GetDirectArray().GetAt(idx).mAlpha;
                        }
                        break;
                    }
                }
                break;
            }
        }

        return result;
    }

    bool Converter::readUV(FbxMesh *pFbxMesh, int nControlPointIdx, int nUVIndex, int nLayer, FbxVector2 &uv)
    {
        if (pFbxMesh->GetElementUVCount() < 1)
        {
            return false;
        }

        bool result = false;
        FbxGeometryElementUV *pVertexUV = pFbxMesh->GetElementUV(nLayer);

        if (pVertexUV != nullptr)
        {
            result = true;

            switch (pVertexUV->GetMappingMode())
            {
            case FbxGeometryElement::eByControlPoint:
                {
                    switch (pVertexUV->GetReferenceMode())
                    {
                    case FbxGeometryElement::eDirect:
                        {
                            uv[0] = pVertexUV->GetDirectArray().GetAt(nControlPointIdx)[0];
                            uv[1] = pVertexUV->GetDirectArray().GetAt(nControlPointIdx)[1];
                        }
                        break;
                    case FbxGeometryElement::eIndex:
                    case FbxGeometryElement::eIndexToDirect:
                        {
                            int idx = pVertexUV->GetIndexArray().GetAt(nControlPointIdx);
                            uv[0] = pVertexUV->GetDirectArray().GetAt(idx)[0];
                            uv[1] = pVertexUV->GetDirectArray().GetAt(idx)[1];
                        }
                        break;
                    }
                }
                break;
            case FbxGeometryElement::eByPolygonVertex:
                {
                    switch (pVertexUV->GetReferenceMode())
                    {
                    case FbxGeometryElement::eDirect:
                    case FbxGeometryElement::eIndex:
                    case FbxGeometryElement::eIndexToDirect:
                        {
                            uv[0] = pVertexUV->GetDirectArray().GetAt(nUVIndex)[0];
                            uv[1] = pVertexUV->GetDirectArray().GetAt(nUVIndex)[1];
                        }
                        break;
                    }
                }
                break;
            }
        }

        return result;
    }

    bool Converter::readNormal(FbxMesh *pFbxMesh, int nControlPointIdx, int nVertexIndex, int nLayer, FbxVector3 &normal)
    {
        if (pFbxMesh->GetElementNormalCount() < 1)
        {
            return false;
        }

        bool result = false;
        FbxGeometryElementNormal *pNormal = pFbxMesh->GetElementNormal(nLayer);

        if (pNormal != nullptr)
        {
            result = true;

            switch (pNormal->GetMappingMode())
            {
            case FbxGeometryElement::eByControlPoint:
                {
                    switch (pNormal->GetReferenceMode())
                    {
                    case FbxGeometryElement::eDirect:
                        {
                            normal[0] = pNormal->GetDirectArray().GetAt(nControlPointIdx)[0];
                            normal[1] = pNormal->GetDirectArray().GetAt(nControlPointIdx)[1];
                            normal[2] = pNormal->GetDirectArray().GetAt(nControlPointIdx)[2];
                        }
                        break;
                    case FbxGeometryElement::eIndex:
                    case FbxGeometryElement::eIndexToDirect:
                        {
                            int idx = pNormal->GetIndexArray().GetAt(nControlPointIdx);
                            normal[0] = pNormal->GetDirectArray().GetAt(idx)[0];
                            normal[1] = pNormal->GetDirectArray().GetAt(idx)[1];
                            normal[2] = pNormal->GetDirectArray().GetAt(idx)[2];
                        }
                        break;
                    }
                }
                break;
            case FbxGeometryElement::eByPolygonVertex:
                {
                    switch (pNormal->GetReferenceMode())
                    {
                    case FbxGeometryElement::eDirect:
                        {
                            normal[0] = pNormal->GetDirectArray().GetAt(nVertexIndex)[0];
                            normal[1] = pNormal->GetDirectArray().GetAt(nVertexIndex)[1];
                            normal[2] = pNormal->GetDirectArray().GetAt(nVertexIndex)[2];
                        }
                        break;
                    case FbxGeometryElement::eIndex:
                    case FbxGeometryElement::eIndexToDirect:
                        {
                            int idx = pNormal->GetIndexArray().GetAt(nVertexIndex);
                            normal[0] = pNormal->GetDirectArray().GetAt(idx)[0];
                            normal[1] = pNormal->GetDirectArray().GetAt(idx)[1];
                            normal[2] = pNormal->GetDirectArray().GetAt(idx)[2];
                        }
                        break;
                    }
                }
                break;
            }
        }

        return result;
    }

    bool Converter::readTangent(FbxMesh *pFbxMesh, int nControlPointIdx, int nVertexIndex, int nLayer, FbxVector3 &tangent)
    {
        if (pFbxMesh->GetElementTangentCount() < 1)
        {
            return false;
        }

        bool result = false;
        FbxGeometryElementTangent *pTangent = pFbxMesh->GetElementTangent(nLayer);

        if (pTangent != nullptr)
        {
            result = true;

            switch (pTangent->GetMappingMode())
            {
            case FbxGeometryElement::eByControlPoint:
                {
                    switch (pTangent->GetReferenceMode())
                    {
                    case FbxGeometryElement::eDirect:
                        {
                            tangent[0] = pTangent->GetDirectArray().GetAt(nControlPointIdx)[0];
                            tangent[1] = pTangent->GetDirectArray().GetAt(nControlPointIdx)[1];
                            tangent[2] = pTangent->GetDirectArray().GetAt(nControlPointIdx)[2];
                        }
                        break;
                    case FbxGeometryElement::eIndex:
                    case FbxGeometryElement::eIndexToDirect:
                        {
                            int idx = pTangent->GetIndexArray().GetAt(nControlPointIdx);
                            tangent[0] = pTangent->GetDirectArray().GetAt(idx)[0];
                            tangent[1] = pTangent->GetDirectArray().GetAt(idx)[1];
                            tangent[2] = pTangent->GetDirectArray().GetAt(idx)[2];
                        }
                        break;
                    }
                }
                break;
            case FbxGeometryElement::eByPolygonVertex:
                {
                    switch (pTangent->GetReferenceMode())
                    {
                    case FbxGeometryElement::eDirect:
                        {
                            tangent[0] = pTangent->GetDirectArray().GetAt(nVertexIndex)[0];
                            tangent[1] = pTangent->GetDirectArray().GetAt(nVertexIndex)[1];
                            tangent[2] = pTangent->GetDirectArray().GetAt(nVertexIndex)[2];
                        }
                        break;
                    case FbxGeometryElement::eIndex:
                    case FbxGeometryElement::eIndexToDirect:
                        {
                            int idx = pTangent->GetIndexArray().GetAt(nVertexIndex);
                            tangent[0] = pTangent->GetDirectArray().GetAt(idx)[0];
                            tangent[1] = pTangent->GetDirectArray().GetAt(idx)[1];
                            tangent[2] = pTangent->GetDirectArray().GetAt(idx)[2];
                        }
                        break;
                    }
                }
                break;
            }
        }

        return result;
    }

    bool Converter::readBinormal(FbxMesh *pFbxMesh, int nControlPointIdx, int nVertexIndex, int nLayer, FbxVector3 &binormal)
    {
        if (pFbxMesh->GetElementBinormalCount() < 1)
        {
            return false;
        }

        bool result = false;
        FbxGeometryElementBinormal *pBinormal = pFbxMesh->GetElementBinormal(nLayer);

        if (pBinormal != nullptr)
        {
            result = true;

            switch (pBinormal->GetMappingMode())
            {
            case FbxGeometryElement::eByControlPoint:
                {
                    switch (pBinormal->GetReferenceMode())
                    {
                    case FbxGeometryElement::eDirect:
                        {
                            binormal[0] = pBinormal->GetDirectArray().GetAt(nControlPointIdx)[0];
                            binormal[1] = pBinormal->GetDirectArray().GetAt(nControlPointIdx)[1];
                            binormal[2] = pBinormal->GetDirectArray().GetAt(nControlPointIdx)[2];
                        }
                        break;
                    case FbxGeometryElement::eIndex:
                    case FbxGeometryElement::eIndexToDirect:
                        {
                            int idx = pBinormal->GetIndexArray().GetAt(nControlPointIdx);
                            binormal[0] = pBinormal->GetDirectArray().GetAt(idx)[0];
                            binormal[1] = pBinormal->GetDirectArray().GetAt(idx)[1];
                            binormal[2] = pBinormal->GetDirectArray().GetAt(idx)[2];
                        }
                        break;
                    }
                }
                break;
            case FbxGeometryElement::eByPolygonVertex:
                {
                    switch (pBinormal->GetReferenceMode())
                    {
                    case FbxGeometryElement::eDirect:
                        {
                            binormal[0] = pBinormal->GetDirectArray().GetAt(nVertexIndex)[0];
                            binormal[1] = pBinormal->GetDirectArray().GetAt(nVertexIndex)[1];
                            binormal[2] = pBinormal->GetDirectArray().GetAt(nVertexIndex)[2];
                        }
                        break;
                    case FbxGeometryElement::eIndex:
                    case FbxGeometryElement::eIndexToDirect:
                        {
                            int idx = pBinormal->GetIndexArray().GetAt(nVertexIndex);
                            binormal[0] = pBinormal->GetDirectArray().GetAt(idx)[0];
                            binormal[1] = pBinormal->GetDirectArray().GetAt(idx)[1];
                            binormal[2] = pBinormal->GetDirectArray().GetAt(idx)[2];
                        }
                        break;
                    }
                }
                break;
            }
        }

        return result;
    }

    bool Converter::readMaterial(FbxMesh *pFbxMesh, int nTriangleIndex, int &nMaterialIndex)
    {
        bool result = false;

        if (pFbxMesh->GetElementMaterial() != nullptr)
        {
            FbxLayerElementArrayTemplate<int> &indices = pFbxMesh->GetElementMaterial()->GetIndexArray();
            FbxGeometryElement::EMappingMode eMappingMode = pFbxMesh->GetElementMaterial()->GetMappingMode();

            if (indices.GetCount() > 0)
            {
                result = true;

                switch (eMappingMode)
                {
                case FbxGeometryElement::eByPolygon:
                    {
                        nMaterialIndex = indices.GetAt(nTriangleIndex);
                    }
                    break;
                case FbxGeometryElement::eAllSame:
                    {
                        nMaterialIndex = indices.GetAt(0);
                    }
                    break;
                }
            }
        }
        
        return result;
    }

    bool Converter::processFbxSkeleton(FbxNode *pFbxNode, Node *pParent, Node *&pNewNode)
    {
#ifdef _DEBUG
        std::stringstream ssTab;
        for (int t = 0; t < mTabCount; ++t)
        {
            ssTab<<"\t";
        }
        T3D_LOG_INFO("%sSkeleton : %s, 0x%p", ssTab.str().c_str(), pFbxNode->GetName(), pFbxNode);
#endif

        FbxSkeleton *pFbxSkel = pFbxNode->GetSkeleton();

        return true;
    }

    bool Converter::processFbxCamera(FbxNode *pFbxNode, Node *pParent, Node *&pNewNode)
    {
#ifdef _DEBUG
        std::stringstream ssTab;
        for (int t = 0; t < mTabCount; ++t)
        {
            ssTab<<"\t";
        }
        T3D_LOG_INFO("%sCamera : %s", ssTab.str().c_str(), pFbxNode->GetName());
#endif

        return true;
    }

    bool Converter::processFbxLight(FbxNode *pFbxNode, Node *pParent, Node *&pNewNode)
    {
#ifdef _DEBUG
        std::stringstream ssTab;
        for (int t = 0; t < mTabCount; ++t)
        {
            ssTab<<"\t";
        }
        T3D_LOG_INFO("%sLight : %s", ssTab.str().c_str(), pFbxNode->GetName());
#endif

        return true;
    }

    bool Converter::processFbxMaterial(FbxNode *pFbxNode, Node *pParent)
    {
        int nMaterialCount = pFbxNode->GetMaterialCount();
        int i = 0;

#ifdef _DEBUG
        std::stringstream ssTab;
        for (i = 0; i < mTabCount; ++i)
        {
            ssTab<<"\t";
        }
#endif

        for (i = 0; i < nMaterialCount; ++i)
        {
            FbxSurfaceMaterial *pFbxMaterial = pFbxNode->GetMaterial(i);
//             Material *pMaterial = new Material(pFbxMaterial->GetName());
//             pParent->addChild(pMaterial);
#ifdef _DEBUG
            T3D_LOG_INFO("%sMaterial : %s", ssTab.str().c_str(), pFbxMaterial->GetName());
#endif
        }

        return true;
    }

    bool Converter::processFbxAnimation(FbxNode *pFbxNode, Node *pParent)
    {
#ifdef _DEBUG
        std::stringstream ssTab;
        for (int t = 0; t < mTabCount; ++t)
        {
            ssTab<<"\t";
        }
        T3D_LOG_INFO("%sAnimation : %s 0x%p", ssTab.str().c_str(), pFbxNode->GetName(), pFbxNode);
#endif

        FbxScene *pFbxScene = (FbxScene *)mSrcData;
        int nAnimStackCount = pFbxScene->GetSrcObjectCount(FbxAnimStack::ClassId);
        int i = 0;
        if (nAnimStackCount > 0)
        {
            ssTab<<"\t";
        }
        for (i = 0; i < nAnimStackCount; ++i)
        {
            FbxAnimStack *pFbxAnimStack = (FbxAnimStack *)pFbxScene->GetSrcObject(FbxAnimStack::ClassId, i);

            String name = pFbxAnimStack->GetName();
            if (name.empty())
            {
                name = "Animation";
            }
#ifdef _DEBUG
            T3D_LOG_INFO("%sAction : %s", ssTab.str().c_str(), name.c_str());
#endif
            Animation *pAnimation = new Animation(name);
//             pParent->addChild(pAnimation);

            int nAnimLayerCount = pFbxAnimStack->GetMemberCount();
#ifdef _DEBUG
            std::stringstream ss;
            if (nAnimLayerCount > 0)
            {
                ss<<ssTab.str()<<"\t";
            }
#endif
            
            int j = 0;
            for (j = 0; j < nAnimLayerCount; ++j)
            {
                FbxAnimLayer *pFbxAnimLayer = (FbxAnimLayer *)pFbxAnimStack->GetMember(j);
                String name = pFbxAnimLayer->GetName();
                if (name.empty())
                {
                    name = "Action";
                }

#ifdef _DEBUG
                T3D_LOG_INFO("%sLayer : %s", ss.str().c_str(), name.c_str());
#endif

                Action *pAction = new Action(name);

                FbxAnimCurve *pFbxTransCurve = pFbxNode->LclTranslation.GetCurve(pFbxAnimLayer);
                FbxAnimCurve *pFbxRotationCurve = pFbxNode->LclRotation.GetCurve(pFbxAnimLayer);
                FbxAnimCurve *pFbxScaleCurve = pFbxNode->LclScaling.GetCurve(pFbxAnimLayer);

                if (pFbxTransCurve != nullptr)
                {
                    int nKeyframeCount = pFbxTransCurve->KeyGetCount();

                    int k = 0;
                    for (k = 0; k < nKeyframeCount; ++k)
                    {
                        FbxTime frameTime = pFbxTransCurve->KeyGetTime(k);
                        FbxVector4 translate = pFbxNode->EvaluateLocalTranslation(frameTime);
                        float x = translate[0];
                        float y = translate[1];
                        float z = translate[2];
                        frameTime.GetMilliSeconds();
                    }
                }
                else
                {

                }

                if (pFbxRotationCurve != nullptr)
                {
                    int nKeyframeCount = pFbxRotationCurve->KeyGetCount();

                    int k = 0;
                    for (k = 0; k < nKeyframeCount; ++k)
                    {
                        FbxTime frameTime = pFbxRotationCurve->KeyGetTime(k);
                        FbxVector4 rotation = pFbxNode->EvaluateLocalRotation(frameTime);
                    }
                }
                else
                {

                }
            }
        }

        return true;
    }

    bool Converter::processFbxSkin(FbxNode *pFbxNode, Node *pParent)
    {
#ifdef _DEBUG
        std::stringstream ssTab;
        for (int t = 0; t < mTabCount; ++t)
        {
            ssTab<<"\t";
        }
        T3D_LOG_INFO("%sSkin : %s", ssTab.str().c_str(), pFbxNode->GetName());
#endif

        FbxMesh *pFbxMesh = pFbxNode->GetMesh();
        int nDeformerCount = pFbxMesh->GetDeformerCount();

        int i = 0;
        for (i = 0; i < nDeformerCount; ++i)
        {
            FbxDeformer *pFbxDeformer = pFbxMesh->GetDeformer(i);
            if (pFbxDeformer == nullptr)
            {
                continue;
            }

            if (pFbxDeformer->GetDeformerType() != FbxDeformer::eSkin)
            {
                continue;
            }

            FbxSkin *pFbxSkin = (FbxSkin *)pFbxDeformer;
            if (pFbxSkin == nullptr)
            {
                continue;
            }

            int j = 0;
            int nBoneCount = pFbxSkin->GetClusterCount();

#ifdef _DEBUG
            std::stringstream ss;
            if (nBoneCount > 0)
            {
                ss<<ssTab.str()<<"\t";
            }
#endif

            for (j = 0; j < nBoneCount; ++j)
            {
                FbxCluster *pFbxCluster = pFbxSkin->GetCluster(j);
                FbxNode *pFbxLinkNode = pFbxCluster->GetLink();
#ifdef _DEBUG
                T3D_LOG_INFO("%sLink node : %s", ss.str().c_str(), pFbxLinkNode->GetName());
#endif
            }
        }

        return true;
    }

    bool Converter::optimize(Node *pNode)
    {
        if (pNode->getNodeType() == Node::E_TYPE_MESH)
        {
            Mesh *pMesh = (Mesh *)pNode;
            pMesh->split();
        }

        int i = 0;
        for (i = 0; i < pNode->getChildrenCount(); ++i)
        {
            Node *pChild = pNode->getChild(i);
            optimize(pChild);
        }

        return true;
    }
}
