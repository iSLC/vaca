// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Event.hpp"
#include "VACA/Rect.hpp"

namespace Wg {

class VACA_DLL LayoutEvent : public Event {
    Rect m_bounds;

public:

    LayoutEvent(Widget *source, const Rect &bounds);

    ~LayoutEvent() override;

    [[nodiscard]] Rect getBounds() const;

    void setBounds(const Rect &rc);

};

} // namespace Wg
