// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Label.hpp"

namespace Wg {

/**
   Customized static label control.
*/
class VACA_DLL CustomLabel : public Label {
    // SS_OWNERDRAW has the bit 1, so we can't use the
    // SS_CENTER/SS_RIGHT to known the text-alignment. We must to hold
    // on in some place.
    TextAlign m_textAlign;

public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    CustomLabel(const String &text, Widget *parent, const Style &style = Styles::Default);

    ~CustomLabel() override;

    [[nodiscard]] TextAlign getTextAlign() const override;

    void setTextAlign(TextAlign align) override;

protected:

    // Reflected notifications
    bool onReflectedDrawItem(Graphics &g, LPDRAWITEMSTRUCT lpDrawItem) override;

};

} // namespace Wg
