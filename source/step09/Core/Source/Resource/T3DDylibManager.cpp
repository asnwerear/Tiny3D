﻿/*******************************************************************************
 * This file is part of Tiny3D (Tiny 3D Graphic Rendering Agent)
 * Copyright (C) 2015-2019  Answer Wong
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


#include "Resource/T3DDylibManager.h"
#include "Resource/T3DDylib.h"


namespace Tiny3D
{
    T3D_INIT_SINGLETON(DylibManager);

    DylibManagerPtr DylibManager::create()
    {
        DylibManagerPtr mgr = new DylibManager();
        mgr->release();
        return mgr;
    }

    DylibManager::DylibManager()
    {

    }

    DylibManager::~DylibManager()
    {

    }

    DylibPtr DylibManager::loadDylib(const String &name)
    {
        return smart_pointer_cast<Dylib>(ResourceManager::load(name, 0));
    }

    ResourcePtr DylibManager::create(const String &name, int32_t argc, va_list args)
    {
        return Dylib::create(name);
    }

    void DylibManager::unloadDylib(DylibPtr &dylib)
    {
        unload((ResourcePtr &)dylib);
    }
}
