// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Layout.hpp"

namespace Wg {

/**
   An AnchorLayout tries to maintain the same aspect of the widgets
   using references rectangles. It uses Anchor contraints to known
   which side of a child must be "anchored" (to maintain the same
   distance) to which side of the parent.

   @image html AnchorLayout.png

   @see AnchorLayout#AnchorLayout, Anchor
*/
class VACA_DLL AnchorLayout : public Layout {
    /**
       The size of the imaginary rectangle @c Rect(Point(0,0),refSize).

       @see AnchorLayout#AnchorLayout
     */
    Size m_refSize;

public:

    AnchorLayout(const Size &refSize);

    void layout(Widget *parent, WidgetList &widgets, const Rect &rc) override;

};

} // namespace Wg
