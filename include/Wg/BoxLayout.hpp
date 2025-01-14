// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Layout.hpp"

namespace Wg {

// ======================================================================
// BoxLayout

class VACA_DLL BoxLayout : public Layout {
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

    [[nodiscard]] bool isHomogeneous() const;

    [[nodiscard]] int getBorder() const;

    void setBorder(int border);

    [[nodiscard]] int getChildSpacing() const;

    void setChildSpacing(int childSpacing);

    Size getPreferredSize(Widget *parent, WidgetList &widgets, const Size &fitIn) override;

protected:

    void layout(Widget *parent, WidgetList &widgets, const Rect &rc) override;

};

} // namespace Wg
