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
   The more simplest Layout manager: positions all the widgets in the
   full client area. It's useful only if you have one child in the
   parent widget.
*/
class VACA_DLL ClientLayout : public Layout {
    int m_border;

public:

    ClientLayout();

    ClientLayout(int border);

    ~ClientLayout() override;

    Size getPreferredSize(Widget *parent, WidgetList &widgets, const Size &fitIn) override;

    void layout(Widget *parent, WidgetList &widgets, const Rect &rc) override;

};

} // namespace Wg
