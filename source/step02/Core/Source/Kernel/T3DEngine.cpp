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

#include "Kernel/T3DEngine.h"


namespace Tiny3D
{
    T3D_INIT_SINGLETON(Engine);

    Engine::Engine()
        : mWindow(nullptr)
        , mIsRunning(false)
//         , mWindowCreated(true)
    {
        mEventMgr = new EventManager(10);
    }

    Engine::~Engine()
    {
//         if (mWindowCreated)
        {
            T3D_SAFE_DELETE(mWindow);
        }

        T3D_SAFE_DELETE(mEventMgr);
    }

    bool Engine::startup(/*Window *window / * = nullptr * /*/)
    {
        bool ret = false;

        Application *theApp = Application::getInstancePtr();
        if (theApp != nullptr)
        {
            ret = theApp->init();
        }

        if (ret)
        {
//             if (window == nullptr)
            {
                mWindow = new Window();
                if (ret = mWindow->create("Demo_Hello", 100, 100, 800, 600, Window::WINDOW_SHOWN))
                {
//                     mWindowCreated = true;

//                     mWindow->setWindowEventListener(this);
                }
            }
//             else
//             {
//                 mWindow = window;
//                 mWindowCreated = false;
//                 mWindow->setWindowEventListener(this);
//             }
        }

        mIsRunning = ret;

        return ret;
    }

    bool Engine::run()
    {
        Application *theApp = Application::getInstancePtr();
        theApp->applicationDidFinishLaunching();

        while (mIsRunning)
        {
            // 轮询系统事件
            mIsRunning = theApp->pollEvents();
            // 系统平台层轮询
            T3D_SYSTEM.update();
            // 事件系统派发事件
            T3D_EVENT_MGR.dispatchEvent();
            renderOneFrame();
        }

        theApp->applicationWillTerminate();

        return true;
    }

    void Engine::renderOneFrame()
    {
        
    }

//     void Engine::windowResized(int32_t w, int32_t h)
//     {
// 
//     }
// 
//     void Engine::windowMoved(int32_t x, int32_t y)
//     {
// 
//     }
// 
//     void Engine::windowRender()
//     {
// 
//     }
// 
//     void Engine::windowClosed()
//     {
//         mIsRunning = false;
//     }
}
