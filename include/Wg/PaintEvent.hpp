// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Event.hpp"

namespace Wg {

/**
   Event to paint a widget.
*/
class VACA_DLL PaintEvent : public Event {
    Graphics &m_graphics;
    bool m_painted;

public:

    PaintEvent(Widget *source, Graphics &graphics);

    ~PaintEvent() override;

    Graphics &getGraphics();

    [[nodiscard]] bool isPainted() const;
};

} // namespace Wg
