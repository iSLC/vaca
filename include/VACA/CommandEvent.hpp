// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/ConsumableEvent.hpp"

namespace vaca {

class VACA_DLL CommandEvent : public ConsumableEvent {
    CommandId m_commandId;

public:

    CommandEvent(Component *source, CommandId commandId);

    ~CommandEvent() override;

    [[nodiscard]] CommandId getCommandId() const;

};

} // namespace vaca
