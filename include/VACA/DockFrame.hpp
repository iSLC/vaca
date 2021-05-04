// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Frame.hpp"
#include "VACA/Event.hpp"

namespace vaca {

/**
   Represents the Win32 class used by DockFrame.
*/
class DockFrameClass : public WidgetClass {
public:
    static WidgetClassName getClassName() { return WidgetClassName(L"Vaca.DockFrame"); }
};

/**
   A Frame for a DockBar, commondly called "Palette Window".

   A DockFrame is the container for a floating DockBar, it's a
   window with a small title-bar.

   @warning This class is intended to be only
	    used internally by DockBar.

   @win32
     This kind of windows has the @msdn{WS_EX_TOOLWINDOW} style.
   @endwin32

   @internal
*/
class VACA_DLL DockFrame : public Register<DockFrameClass>, public Frame {
    /**
       The DockBar that is inside the client area.
    */
    DockBar *m_dockBar;

public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    DockFrame(DockBar *dockBar, Widget *parent = nullptr, Style style = Styles::Default);

    ~DockFrame() override;

protected:

    bool keepSynchronized() override;

    // Events
    void onResizing(CardinalDirection dir, Rect &rc) override;

    bool wndProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT &lResult) override;
};

} // namespace vaca
