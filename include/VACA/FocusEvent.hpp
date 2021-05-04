// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Event.hpp"

namespace Wg {

class VACA_DLL FocusEvent : public Event {
    Widget *m_oldFocus;
    Widget *m_newFocus;

public:

    FocusEvent(Widget *source, Widget *oldFocus, Widget *newFocus);

    ~FocusEvent() override;

    [[nodiscard]] Widget *getOldFocus() const;

    [[nodiscard]] Widget *getNewFocus() const;

};

} // namespace Wg
