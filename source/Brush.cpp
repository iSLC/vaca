// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/Brush.hpp"

#if defined(VACA_WINDOWS)
  #include "win32/BrushImpl.hpp"
#else
  #error Implement Brush class in your platform
#endif

using namespace Wg;

Brush::Brush()
  : m_impl(new BrushImpl())
{
}

Brush::Brush(const Brush& brush) = default;

Brush::Brush(const Color& color)
  : m_impl(new BrushImpl(color))
{
}

Brush::~Brush()
= default;

Brush& Brush::operator=(const Brush& brush)
= default;

Color Brush::getColor() const
{
  return m_impl->getColor();
}
