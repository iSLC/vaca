// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/MouseEvent.hpp"
#include "Wg/WidgetHit.hpp"

namespace Wg {

class VACA_DLL SetCursorEvent : public MouseEvent {
    WidgetHit m_hit;

public:

    SetCursorEvent(Widget *source, const Point &point, WidgetHit hit);

    ~SetCursorEvent() override;

    void setCursor(const Cursor &cursor);

    [[nodiscard]] WidgetHit getWidgetHit() const;

};

} // namespace Wg
