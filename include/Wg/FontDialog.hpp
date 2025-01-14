// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/CommonDialog.hpp"
#include "Wg/Font.hpp"

namespace Wg {

/**
   A common dialog box to select fonts.

   @see Font
*/
class VACA_DLL FontDialog : public CommonDialog {
    Font m_font;
    LOGFONT m_logFont{};

public:

    FontDialog(const Font &font, Widget *parent);

    ~FontDialog() override;

    bool doModal() override;

    [[nodiscard]] Font getFont() const;

    bool getLogFont(LPLOGFONT lplf) const;

};

} // namespace Wg
