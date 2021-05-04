// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Event.hpp"

namespace vaca {

/**
   Event that can be consumed.
*/
class VACA_DLL ConsumableEvent : public Event {
    /**
       The event was consumed.
    */
    bool m_consumed;

public:

    ConsumableEvent(Component *source);

    ~ConsumableEvent() override;

    void consume();

    [[nodiscard]] bool isConsumed() const;

};

} // namespace vaca
