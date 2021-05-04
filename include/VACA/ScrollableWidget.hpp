// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Widget.hpp"
#include "VACA/Size.hpp"
#include "VACA/Point.hpp"

namespace Wg {

/**
   A wrapper ready-to-use to create scrollable widgets.
*/
class VACA_DLL ScrollableWidget : public Widget {
    /**
       The full size area that can be viewed (using scrolling).
    */
    Size m_fullSize;

    /**
       This point is used to save the old mouse's position (on
       successive onMouseMove events).
    */
    Point m_oldPoint;

public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    ScrollableWidget(Widget *parent, const Style &style = Styles::Default);

    [[nodiscard]] Size getFullSize() const;

    void setFullSize(const Size &sz);

    // Events
protected:
    void onResize(ResizeEvent &ev) override;

    void onMouseEnter(MouseEvent &ev) override;

    void onMouseDown(MouseEvent &ev) override;

    void onMouseMove(MouseEvent &ev) override;

    void onMouseUp(MouseEvent &ev) override;

    void onMouseWheel(MouseEvent &ev) override;

    void onSetCursor(SetCursorEvent &ev) override;

    void onScroll(ScrollEvent &ev) override;

};

} // namespace Wg
