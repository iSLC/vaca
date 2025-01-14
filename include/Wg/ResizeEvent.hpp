// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Event.hpp"
#include "Wg/Size.hpp"

namespace Wg {

class VACA_DLL ResizeEvent : public Event {
    Size m_size;

public:

    ResizeEvent(Component *source, const Size &size);

    ~ResizeEvent() override;

    [[nodiscard]] Size getSize() const;

};

} // namespace Wg
