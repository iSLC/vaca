// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/ConsumableEvent.hpp"
#include "Wg/Keys.hpp"

namespace Wg {

/**
   Data for an event that comes from the keyboard.
*/
class VACA_DLL KeyEvent : public ConsumableEvent {
    /**
       Virtual-key code.

       @see #getKeyCode, Keys
    */
    int m_keys;

    /**
       Character-key code.

       @see #getCharCode
    */
    Char m_charCode;

public:

    KeyEvent(Widget *source, Keys::Type keys, Char charCode);

    ~KeyEvent() override;

    [[nodiscard]] Keys::Type getKeyCode() const;

    [[nodiscard]] Keys::Type getModifiers() const;

    [[nodiscard]] Char getCharCode() const;

    [[nodiscard]] bool isShift() const;

    [[nodiscard]] bool isControl() const;

    [[nodiscard]] bool isAlt() const;

};

} // namespace Wg
