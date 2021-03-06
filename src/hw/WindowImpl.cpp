////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "WindowImpl.hpp"
#include "Event.hpp"
#include <algorithm>
#include <cmath>
#include "Common/Sleep.hpp"

#if defined(SFML_SYSTEM_WINDOWS)

    #include "Win32/WindowImplWin32.hpp"
    typedef hw::priv::WindowImplWin32 WindowImplType;

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

    #include <SFML/Window/Unix/WindowImplX11.hpp>
    typedef hw::priv::WindowImplX11 WindowImplType;

#elif defined(SFML_SYSTEM_MACOS)

    #include <SFML/Window/OSX/WindowImplCocoa.hpp>
    typedef hw::priv::WindowImplCocoa WindowImplType;

#elif defined(SFML_SYSTEM_IOS)

    #include <SFML/Window/iOS/WindowImplUIKit.hpp>
    typedef hw::priv::WindowImplUIKit WindowImplType;

#elif defined(SFML_SYSTEM_ANDROID)

    #include <SFML/Window/Android/WindowImplAndroid.hpp>
    typedef hw::priv::WindowImplAndroid WindowImplType;

#endif


namespace hw
{
	namespace priv
	{
		////////////////////////////////////////////////////////////
		WindowImpl* WindowImpl::create(VideoMode mode, const String& title, Uint32 style)
		{
			return new WindowImplType(mode, title, style);
		}


		////////////////////////////////////////////////////////////
		WindowImpl* WindowImpl::create(WindowHandle handle)
		{
			return new WindowImplType(handle);
		}


		////////////////////////////////////////////////////////////
		WindowImpl::WindowImpl()
		{

		}


		////////////////////////////////////////////////////////////
		WindowImpl::~WindowImpl()
		{
			// Nothing to do
		}

		////////////////////////////////////////////////////////////
		bool WindowImpl::popEvent(Event& event, bool block)
		{
			// If the event queue is empty, let's first check if new events are available from the OS
			if (m_events.empty())
			{
				// Get events from the system
				processEvents();

				// In blocking mode, we must process events until one is triggered
				if (block)
				{
					// Here we use a manual wait loop instead of the optimized
					// wait-event provided by the OS, so that we don't skip joystick
					// events (which require polling)
					while (m_events.empty())
					{
						sleep(milliseconds(10));
						processEvents();
					}
				}
			}

			// Pop the first event of the queue, if it is not empty
			if (!m_events.empty())
			{
				event = m_events.front();
				m_events.pop();

				return true;
			}

			return false;
		}


		////////////////////////////////////////////////////////////
		void WindowImpl::pushEvent(const Event& event)
		{
			m_events.push(event);
		}


	}
}