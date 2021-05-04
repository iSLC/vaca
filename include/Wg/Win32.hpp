// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"

#ifndef VACA_ON_WINDOWS
#error You cannot use this header file outside Windows platform.
#endif

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

namespace Wg {

template<>
VACA_DLL POINT convert_to(const Point &pt);

template<>
VACA_DLL Point convert_to(const POINT &pt);

template<>
VACA_DLL Point convert_to(const POINTS &pt);

template<>
VACA_DLL SIZE convert_to(const Size &sz);

template<>
VACA_DLL Size convert_to(const SIZE &sz);

template<>
VACA_DLL RECT convert_to(const Rect &rc);

template<>
VACA_DLL Rect convert_to(const RECT &rc);

template<>
VACA_DLL COLORREF convert_to(const Color &color);

template<>
VACA_DLL Color convert_to(const COLORREF &colorref);

template<>
VACA_DLL HPEN convert_to(const Pen &pen);

template<>
VACA_DLL HBRUSH convert_to(const Brush &brush);

}
