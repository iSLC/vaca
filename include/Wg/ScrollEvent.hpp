// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Enum.hpp"
#include "Wg/Event.hpp"
#include "Wg/Point.hpp"

namespace Wg {

/**
   It's like a namespace for ScrollRequest.

   @see ScrollRequest
*/
struct ScrollRequestEnum {
    enum enumeration {
        FullBackward,
        FullForward,
        PageBackward,
        PageForward,
        LineBackward,
        LineForward,
        BoxPosition,
        BoxTracking,
        EndScroll,
    };

    static const enumeration default_value = FullBackward;
};

/**
   Defines how does the user want to scroll.

   One of the following values:
   @li ScrollRequest::FullBackward: go to the first page or beginning of line (Home).
   @li ScrollRequest::FullForward: go to the last page or end of line (End).
   @li ScrollRequest::LineBackward: go to to the previous line or character (Left/Up).
   @li ScrollRequest::LineForward: go to to the next line or character (Right/Down).
   @li ScrollRequest::PageBackward: go to the previous page (PageUp).
   @li ScrollRequest::PageForward: go to to the next page (PageDown).
   @li ScrollRequest::BoxPosition: the user moved the scroll box (also called "thumb").
   @li ScrollRequest::BoxTracking: the is moving the scroll box.
   @li ScrollRequest::EndScroll: the scroll request finished.
*/
typedef Enum<ScrollRequestEnum> ScrollRequest;

class VACA_DLL ScrollEvent : public Event {
    Orientation m_orientation;
    ScrollRequest m_request;
    int m_position;

public:

    ScrollEvent(Widget *source, Orientation orien, ScrollRequest req, int pos);

    ~ScrollEvent() override;

    [[nodiscard]] Orientation getOrientation() const;

    [[nodiscard]] ScrollRequest getRequest() const;

    [[nodiscard]] int getPosition() const;

};

} // namespace Wg
