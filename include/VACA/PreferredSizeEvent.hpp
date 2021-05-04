// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Event.hpp"
#include "VACA/Size.hpp"

namespace Wg {

class VACA_DLL PreferredSizeEvent : public Event {
    Size m_fitIn;
    Size m_preferredSize;

public:

    PreferredSizeEvent(Widget *source, const Size &fitIn);

    ~PreferredSizeEvent() override;

    [[nodiscard]] Size fitInSize() const;

    [[nodiscard]] int fitInWidth() const;

    [[nodiscard]] int fitInHeight() const;

    [[nodiscard]] Size getPreferredSize() const;

    void setPreferredSize(const Size &preferredSize);

    void setPreferredSize(int w, int h);

};

} // namespace Wg
