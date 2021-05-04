// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Widget.hpp"
#include "Wg/CancelableEvent.hpp"

namespace Wg {

// TODO move this to "SpinButtonEvent.h"
class SpinButtonEvent : public CancelableEvent {
    int m_value;
    int m_delta;
    Side m_side;

public:

    SpinButtonEvent(Widget *widget, int value, int delta, Side side)
            : CancelableEvent(widget), m_value(value), m_delta(delta), m_side(side) {
    }

    [[nodiscard]] int getValue() const { return m_value; }

    [[nodiscard]] int getDelta() const { return m_delta; }

    Side getSide() { return m_side; }

};

/**
   A couple of buttons to up or down a value.

   @win32
     This is a wrapper for the @msdn{Up-Down} control.
   @endwin32
*/
class VACA_DLL SpinButton : public Widget {
public:

    struct VACA_DLL Styles {
        static const Style Default;
        static const Style Horizontal; // TODO change with set/getOrientation
        static const Style HotTrack;
    };

    SpinButton(Widget *parent, const Style &style = Styles::Default);

    SpinButton(int minValue, int maxValue, int value, Widget *parent, const Style &style = Styles::Default);

    ~SpinButton() override;

    bool isHorizontal();

    bool isVertical();

    int getMinimum();

    int getMaximum();

    void getRange(int &minValue, int &maxValue);

    void setRange(int minValue, int maxValue);

    int getValue();

    void setValue(int value);

    int getBase();

    void setBase(int base);

    Widget *getBuddy();

    void setBuddy(Widget *buddy);

    // Signals
    Signal1<void, SpinButtonEvent &> Change;
//   Signal1<void, SpinButtonEvent&> BeforeChange;
//   Signal1<void, SpinButtonEvent&> AfterChange;

protected:
    // Events
    void onPreferredSize(PreferredSizeEvent &ev) override;

    // New events
    virtual void onChange(SpinButtonEvent &ev);

//   virtual void onBeforeChange(SpinButtonEvent& ev);
//   virtual void onAfterChange(SpinButtonEvent& ev);
    // Events
//   virtual void onScroll(ScrollEvent& ev);
    // Reflected notifications
    bool onReflectedNotify(LPNMHDR lpnmhdr, LRESULT &lResult) override;

};

} // namespace Wg
