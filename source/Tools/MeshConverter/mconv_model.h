

#ifndef __MCONV_MODEL_H__
#define __MCONV_MODEL_H__


#include "mconv_node.h"


namespace mconv
{
    class Model : public Node
    {
    public:
        Model(const String &ID)
            : Node(ID)
        {

        }

        virtual ~Model()
        {

        }

        virtual Type getNodeType() const override
        {
            return E_TYPE_MODEL;
        }
    };
}


#endif  /*__MCONV_MODEL_H__*/
