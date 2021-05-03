// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Layout.hpp"

namespace vaca {

// ======================================================================
// BoxLayout

class VACA_DLL BoxLayout : public Layout
{
  Orientation m_orientation;
  bool m_homogeneous;
  int m_border;
  int m_childSpacing;

public:

  BoxLayout(Orientation orientation,
	    bool homogeneous,
	    int borderSize = 4,
	    int childSpacing = 4);

  bool isHorizontal();
  bool isVertical();

  bool isHomogeneous();

  int getBorder();
  void setBorder(int border);

  int getChildSpacing();
  void setChildSpacing(int childSpacing);

  virtual Size getPreferredSize(Widget* parent, WidgetList& widgets, const Size& fitIn);

protected:

  virtual void layout(Widget* parent, WidgetList& widgets, const Rect& rc);

};

} // namespace vaca
