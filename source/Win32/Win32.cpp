// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/Win32.hpp"
#include "Wg/Point.hpp"
#include "Wg/Size.hpp"
#include "Wg/Rect.hpp"
#include "Wg/Color.hpp"

using namespace Wg;

template<> POINT Wg::convert_to(const Point& pt)
{
  POINT res = { pt.x, pt.y };
  return res;
}

template<> Point Wg::convert_to(const POINT& pt)
{
  return Point(pt.x, pt.y);
}

template<> Point Wg::convert_to(const POINTS& pt)
{
  return Point(pt.x, pt.y);
}

template<> SIZE Wg::convert_to(const Size& sz)
{
  SIZE res = { sz.w, sz.h };
  return res;
}

template<> Size Wg::convert_to(const SIZE& sz)
{
  return Size(sz.cx, sz.cy);
}

/**
   Converts a Vaca's rectangle (Rect class) to a Win32's rectangle
   (RECT structure).

   @internal
*/
template<> RECT Wg::convert_to(const Rect& rc)
{
  RECT res = { rc.x, rc.y, rc.x+rc.w, rc.y+rc.h };
  return res;
}

/**
   Converts a Win32's rectangle (RECT structure) to a Vaca's rectangle
   (Rect class).

   @internal
*/
template<> Rect Wg::convert_to(const RECT& rc)
{
  return Rect(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
}

template<> COLORREF Wg::convert_to(const Color& color)
{
  return RGB(color.getR(),
	     color.getG(),
	     color.getB());
}

template<> Color Wg::convert_to(const COLORREF& colorref)
{
  return Color(GetRValue(colorref),
	       GetGValue(colorref),
	       GetBValue(colorref));
}
