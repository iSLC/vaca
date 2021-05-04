// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Event.hpp"

namespace Wg {

/**
   Event that can be canceled.

   For example, a CloseEvent can be canceled if you don't want to
   close the Frame (or the user cancel the event answering a MsgBox).
*/
class VACA_DLL CancelableEvent : public Event {
    /**
       The event was canceled.
    */
    bool m_canceled;

public:

    explicit CancelableEvent(Component *source);

    ~CancelableEvent() override;

    void cancel();

    [[nodiscard]] bool isCanceled() const;

};

} // namespace Wg
