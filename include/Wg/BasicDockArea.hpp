// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/DockArea.hpp"

namespace vaca {

/**
   The most basic dock area management.
*/
class VACA_DLL BasicDockArea : public DockArea {
public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    BasicDockArea(Side side, Widget *parent, Style style = Styles::Default);

    virtual ~BasicDockArea();

    virtual bool hitTest(DockBar *bar, const Point &cursor, const Point &anchor, bool fromInside);

    virtual DockInfo *createDefaultDockInfo(DockBar *bar);

    virtual DockInfo *createDockInfo(DockBar *bar, const Point &cursor, const Point &anchor);

    virtual void drawXorTracker(Graphics &g, DockInfo *dockInfo);

    virtual void layout();

protected:
    // Events
    virtual void onPreferredSize(PreferredSizeEvent &ev);

};

} // namespace vaca
