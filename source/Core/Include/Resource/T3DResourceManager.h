/*******************************************************************************
 * This file is part of Tiny3D (Tiny 3D Graphic Rendering Engine)
 * Copyright (C) 2015-2017  Answer Wong
 * For latest info, see https://github.com/asnwerear/Tiny3D
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef __T3D_RESOURCE_MANAGER_H__
#define __T3D_RESOURCE_MANAGER_H__


#include "T3DPrerequisites.h"
#include "T3DResource.h"


namespace Tiny3D
{
    class T3D_ENGINE_API ResourceManager
    {
    public:
        ResourceManager();
        virtual ~ResourceManager();

        static uint32_t toID(const String &name);

        /**
         * @brief Load resource from file.
         */
        virtual ResourcePtr load(const String &name, int32_t argc, ...);

        /**
         * @brief Unload resource in memory.
         */
        virtual void unload(ResourcePtr &res);

        /**
         * @brief Unload all resources unused from memory.
         */
        virtual void unloadUnused();

        /**
         * @brief Clone another new resource from source resource.
         * @param [in] pSrcRes
         *      Source resource used to clone.
         * @return retrieve a pointer to new instance of resource.
         * @remarks
         *      All Resource subclass should implement clone() method to
         *      new instance of resource. This method will call 
         *      Resource::clone() to implement this function.
         */
        ResourcePtr clone(const ResourcePtr &src);

        /**
         * @brief Get resource by name.
         * @param [in] strName
         *      The name of resource.
         * @param [in] unCloneID
         *      0 if the resource wasn't created by clone. Default is 0.
         * @return retrieve a pointer to instance of resource.
         */
        ResourcePtr getResource(const String &name, uint32_t cloneID = 0) const;

        /**
         * @brief Get resources by name.
         * @remarks
         *      This method will return all resource in list
         *      whether the resource created or cloned.
         */
        bool getResources(const String &name, std::list<ResourcePtr> &rList) const;

    protected:
        virtual ResourcePtr create(const String &strName, int32_t argc, va_list args) = 0;

        static uint32_t hash(const char *str);

    protected:
        typedef std::map<uint32_t, ResourcePtr>     Resources;
        typedef Resources::iterator                 ResourcesItr;
        typedef Resources::const_iterator           ResourcesConstItr;
        typedef Resources::value_type               ResPairValue;

        typedef std::map<String, Resources>         ResourcesMap;
        typedef ResourcesMap::iterator              ResourcesMapItr;
        typedef ResourcesMap::const_iterator        ResourcesMapConstItr;
        typedef ResourcesMap::value_type            ResMapPairValue;

        ResourcesMap    mResourceCache;     /// cache all resources
        uint32_t        mCloneID;           /// used to clone
    };
}


#endif  /*__T3D_RESOURCE_MANAGER_H__*/
