/**
 * 转换后的*.t3db (xml) 格式如下：
 *      <?xml version="1.0" encoding="utf-8"?>
 *      <TINY3D xmlns="xxx" version="0.0.1">
 *      <scene>
 *          <model id="wolf">
 *              <mesh id="wolf_skin">
 *                  <attributes count="5">
 *                      <attirbute id="POSITION" size="3" type="float" />
 *                      <attribute id="TEXCOORD" size="2" tpye="float" />
 *                      <attribute id="NORMAL" size="3" type="float" />
 *                      <attribute id="TANGENT" size="3" type="float" />
 *                      <attribute id="BINORMAL" size="3" type="float" />
 *                      <attribute id="COLOR" size="4" type="float" />
 *                      <attribute id="BLEND_WEIGHT" size="4" type="float" />
 *                      <attribute id="BLEND_INDEX" size="4" type="ushort" />
 *                  </attributes>

 *                  <vertices count="12">
 *                      -1.0 1.0 1.0 
 *                  </vertices>

 *                  <parts count="1">
 *                      <part id="wolf" primitive="triangles" count="30" material="wolf">
 *                          <indices count="90" 16bits="true">
 *                              1 2 3 1 2 3 1 2 3
 *                          </indices>
 *                      </part>
 *                  </parts>
 *              </mesh>

 *              <materials id="" count="1">
 *                  <material id="wolf">
 *                      <mode>Lambert</mode>
 *                      <ambient>1.0 1.0 1.0 1.0</ambient>
 *                      <diffuse>1.0 1.0 1.0 1.0</diffuse>
 *                      <specular>1.0 1.0 1.0 1.0</specular>
 *                      <emissive>1.0 1.0 1.0 1.0</emissive>
 *                      <shininess>20.0</shininess>
 *                      <transparency>0.0</transparency>
 *                      <reflection>1.0</reflection>
 *                      <textures count="2">
 *                          <texture id="texture">wolf.png</texture>
 *                          <texture id="lightmap">lightmap.png</texture>
 *                      </textures>
 *                      <effects count="3">
 *                          <effect id="shark" type="HLSL">ss.hlsl</effect>
 *                          <effect id="flash" type="GLSL">ss.glsl</effect>
 *                          <effect id="flash" type="CG">ss.cg</effect>
 *                      </effects>
 *                  </material>
 *              </materials>

 *              <skin id="wolf_skin" count="10">
 *                  <bone id="bone_head">
 *                      <transform>
 *                          1.0 0.0 0.0 0.0
 *                          0.0 1.0 0.0 0.0
 *                          0.0 0.0 1.0 0.0
 *                          0.0 0.0 0.0 1.0
 *                      </transform>
 *                  </bone>
 *              </skin>

 *              <skeleton id="wolf_skeleton">
 *                  <bone id="bone_head">
 *                      <transform>
 *                          1.0 0.0 0.0 0.0
 *                          0.0 1.0 0.0 0.0
 *                          0.0 0.0 1.0 0.0
 *                          0.0 0.0 0.0 1.0
 *                      </transform>
 *                      <bone id="bone_body">
 *                          <transform>
 *                          </transform>
 *                      </bone>
 *                  </bone>
 *              </skeleton>

 *              <animation id="" count="3>
 *                  <action id="idle">
 *                      <keyframe type="translation" bone="head" count="2">
 *                          <frame id="1" time="0.1">1.0 1.0 1.0</frame>
 *                          <frame id="2" time="0.5">0.0 1.0 0.0</frame>
 *                      </keyframe>
 *                      <keyframe type="rotation" bone="head" count="2">
 *                          <frame id="2" time="1.0">1.0 1.0 1.0 1.0</frame>
 *                          <frame id="3" time="1.2">1.0 1.0 1.0 1.0</frame>
 *                      </keyframe>
 *                      <keyframe type="scaling" bone="head" count="2">
 *                          <frame id="4" time="1.5">1.2 1.2 1.2</frame>
 *                          <frame id="5" time="2.0">1.2 1.2 1.2</frame>
 *                      </keyframe>
 *                  </action>
 *                  <action id="walk">
 *                  </action>
 *                  <action id="run">
 *                  </action>
 *              </animation>

 *              <bound type="box">-1.0 -1.0 -1.0 1.0 1.0 1.0</bound>
 *          </model>

 *          <light>
 *          </light>

 *          <camera>
 *          </camera>
 *      </scene>
 */

#ifndef __MCONV_T3DSERIALIZER_H__
#define __MCONV_T3DSERIALIZER_H__


#include "mconv_serializer.h"
#include "tinyxml2/tinyxml2.h"


namespace mconv
{
    using namespace tinyxml2;

    class Node;

    class T3DBinSerializer : public Serializer
    {
    public:
        T3DBinSerializer();
        virtual ~T3DBinSerializer();

        virtual bool load(const String &path, void *&pData) override;
        virtual bool save(const String &path, void *pData) override;
    };

    class T3DXMLSerializer : public Serializer
    {
    public:
        static const char * const TAG_VERSION;
        static const char * const TAG_SCENE;
        static const char * const TAG_MODEL;
        static const char * const TAG_MESH;
        static const char * const TAG_ATTRIBUTES;
        static const char * const TAG_ATTRIBUTE;
        static const char * const TAG_VERTICES;
        static const char * const TAG_PARTS;
        static const char * const TAG_PART;
        static const char * const TAG_INDICES;
        static const char * const TAG_MATERIALS;
        static const char * const TAG_MATERIAL;
        static const char * const TAG_MODE;
        static const char * const TAG_AMBIENT;
        static const char * const TAG_DIFFUSE;
        static const char * const TAG_SPECULAR;
        static const char * const TAG_EMISSIVE;
        static const char * const TAG_SHININESS;
        static const char * const TAG_TRANSPARENCY;
        static const char * const TAG_REFLECTION;
        static const char * const TAG_TEXTURES;
        static const char * const TAG_TEXTURE;
        static const char * const TAG_EFFECTS;
        static const char * const TAG_EFFECT;
        static const char * const TAG_SKIN;
        static const char * const TAG_SKELETON;
        static const char * const TAG_BONE;
        static const char * const TAG_TRANSFORM;
        static const char * const TAG_ANIMATION;
        static const char * const TAG_ACTION;
        static const char * const TAG_KEYFRAME;
        static const char * const TAG_FRAME;
        static const char * const TAG_BOUND;
        static const char * const TAG_LIGHT;
        static const char * const TAG_CAMERA;

        static const char * const ATTRIB_ID;
        static const char * const ATTRIB_COUNT;
        static const char * const ATTRIB_SIZE;
        static const char * const ATTRIB_TYPE;
        static const char * const ATTRIB_PRIMITIVE;
        static const char * const ATTRIB_MATERIAL;
        static const char * const ATTRIB_TIME;
        static const char * const ATTRIB_16BITS;
        static const char * const ATTRIB_SPAN;
        static const char * const ATTRIB_BONE;
        static const char * const ATTRIB_WRAP_U;
        static const char * const ATTRIB_WRAP_V;
        static const char * const ATTRIB_SHARED;
        static const char * const ATTRIB_DURATION;

        T3DXMLSerializer();
        virtual ~T3DXMLSerializer();

        virtual bool load(const String &path, void *&pData) override;
        virtual bool save(const String &path, void *pData) override;

    protected:
        XMLElement *populateXMLNode(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);

        XMLElement *buildXMLModel(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLMesh(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLSubMeshes(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLSubMesh(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLVertexAttributes(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLAnimation(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLAction(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
//         XMLElement *buildXMLMaterials(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
//         XMLElement *buildXMLMaterial(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLTextures(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLTexture(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLSkeleton(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLSkin(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLBone(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLAlignAxisBound(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLSphereBound(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLLight(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);
        XMLElement *buildXMLCamera(XMLDocument *pDoc, XMLElement *pParentElem, Node *pNode);

        bool loadMaterial(Node *&pNode);
        bool saveMaterial(Node *pNode);

        String  mSavePath;
        size_t  mTabCount;
    };

    class T3DSerializer : public Serializer
    {
    public:
        T3DSerializer();
        virtual ~T3DSerializer();

        virtual bool load(const String &path, void *&pData) override;
        virtual bool save(const String &path, void *pData) override;

    protected:
        T3DBinSerializer    *mBinSerializer;
        T3DXMLSerializer    *mXMLSerializer;
    };
}


#endif  /*__MCONV_T3DSERIALIZER_H__*/
