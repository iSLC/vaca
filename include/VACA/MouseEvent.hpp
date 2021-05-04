// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Enum.hpp"
#include "VACA/ConsumableEvent.hpp"
#include "VACA/Point.hpp"

namespace Wg {

/**
   Namespace for MouseButton enumeration.
*/
struct MouseButtonEnum {
    /**
       Enumeration to know which mouse's button triggers the MouseEvent.
    */
    enum enumeration {
        None,
        Left,
        Right,
        Middle,
    };

    /**
       Default value for a MouseButton instance.
    */
    static const enumeration default_value = None;
};

/**
   A button of the mouse.

   One of the following values:
   @li MouseButton::None
   @li MouseButton::Left
   @li MouseButton::Right
   @li MouseButton::Middle
*/
typedef Enum<MouseButtonEnum> MouseButton;

/**
   An event from the mouse.

   It has a trigger mouse button (the button of the mouse that was the
   trigger of the event), a point where the event was happend
   (relative to the widget's client area).

   This kind of event is generated when the mouse position is changed,
   the wheel is moved, or a button is pressed (click or double-click).

   #getButton returns MouseButton::None if the event was produced by
   mouse movement (no button was pressed to trigger the event).
*/
class VACA_DLL MouseEvent : public ConsumableEvent {
    Point m_point;
    int m_clicks;
    int m_flags;
    MouseButton m_trigger;
    int m_delta;

public:

    MouseEvent(Widget *source, const Point &point, int clicks, int flags, MouseButton trigger, int delta = 0);

    ~MouseEvent() override;

    [[nodiscard]] int getX() const;

    [[nodiscard]] int getY() const;

    [[nodiscard]] Point getPoint() const;

    [[nodiscard]] int getClicks() const;

    [[nodiscard]] MouseButton getButton() const;

    [[nodiscard]] bool isLeftButtonPressed() const;

    [[nodiscard]] bool isRightButtonPressed() const;

    [[nodiscard]] bool isMiddleButtonPressed() const;

    [[nodiscard]] bool isShiftKeyPressed() const;

    [[nodiscard]] bool isControlKeyPressed() const;

    [[nodiscard]] bool isAltKeyPressed() const;

    [[nodiscard]] int getDelta() const;

};

} // namespace Wg
