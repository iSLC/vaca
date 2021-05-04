// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "VACA/Color.hpp"
#include "VACA/GdiObject.hpp"
#include "VACA/Win32.hpp"

class Wg::Brush::BrushImpl : public GdiObject<HBRUSH>
{
public:

  BrushImpl()
    : GdiObject<HBRUSH>(CreateSolidBrush(RGB(0, 0, 0))) {
  }

  BrushImpl(const Color& color)
    : GdiObject<HBRUSH>(CreateSolidBrush(convert_to<COLORREF>(color))) {
  }

  ~BrushImpl() {
  }

  Color getColor() const {
    LOGBRUSH lb;
    assert(getHandle());
    ::GetObject(getHandle(), sizeof(LOGBRUSH), &lb);
    return convert_to<Color>(lb.lbColor);
  }

};

template<> HBRUSH Wg::convert_to(const Brush& brush)
{
  // 1) getHandle is defined in GdiObject<HBRUSH> class
  // 2) convert_to<> is friend of Brush class
  return brush.m_impl->getHandle();
}
