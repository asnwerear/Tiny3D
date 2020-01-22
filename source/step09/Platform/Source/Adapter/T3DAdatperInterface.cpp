/*******************************************************************************
 * This file is part of Tiny3D (Tiny 3D Graphic Rendering Engine)
 * Copyright (C) 2015-2020  Answer Wong
 * For latest info, see https://github.com/answerear/Tiny3D
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

#include "Adapter/T3DApplicationInterface.h"
#include "Adapter/T3DConsoleInterface.h"
#include "Adapter/T3DDeviceInfoInterface.h"
#include "Adapter/T3DDirInterface.h"
#include "Adapter/T3DFactoryInterface.h"
#include "Adapter/T3DTimeInterface.h"
#include "Adapter/T3DTimerInterface.h"
#include "Adapter/T3DWindowInterface.h"

namespace Tiny3D
{
    T3D_IMPLEMENT_CLASS_NO_BASE(IApplication);
    T3D_IMPLEMENT_CLASS_NO_BASE(IConsole);
    T3D_IMPLEMENT_CLASS_NO_BASE(IDeviceInfo);
    T3D_IMPLEMENT_CLASS_NO_BASE(IDir);
    T3D_IMPLEMENT_CLASS_NO_BASE(IFactory);
    T3D_IMPLEMENT_CLASS_NO_BASE(ITime);
    T3D_IMPLEMENT_CLASS_NO_BASE(ITimerService);
    T3D_IMPLEMENT_CLASS_NO_BASE(IWindow);
}