// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/Pen.hpp"

#if defined(VACA_WINDOWS)
  #include "win32/PenImpl.hpp"
#else
  #error Implement Pen class in your platform
#endif

using namespace Wg;

Pen::Pen()
  : m_impl(new PenImpl())
{
}

// Copy shared pointers
Pen::Pen(const Pen& pen) = default;

/**
   Creates a pen.

   @param color Color of the pen.
   @param width Width of the pen. If it's 1, the pen will be cosmetic,
		if width > 0 the pen will be geometric.
*/
Pen::Pen(const Color& color, int width)
  : m_impl(new PenImpl(color, width))
{
}

Pen::Pen(const Color& color, int width,
	 PenStyle style, PenEndCap endCap, PenJoin join)
  : m_impl(new PenImpl(color, width, style, endCap, join))
{
}

/**
   Destroys this pen reference.

   @warning
     The last reference to be destroyed will call Win32's DeleteObject to destroy
     the HPEN handle.
*/
Pen::~Pen()
= default;

Pen& Pen::operator=(const Pen& pen)
= default;

/**
   Gets pen's color.

   @return Current pen's color.
*/
Color Pen::getColor() const
{
  return m_impl->getColor();
}

/**
   Gets pen's width (in pixels).

   @return Current pen's width.
*/
int Pen::getWidth() const
{
  return m_impl->getWidth();
}

/**
   Gets pen's style.

   @return Current pen's style.
*/
PenStyle Pen::getStyle() const
{
  return m_impl->getStyle();
}

PenEndCap Pen::getEndCap() const
{
  return m_impl->getEndCap();
}

PenJoin Pen::getJoin() const
{
  return m_impl->getJoin();
}
