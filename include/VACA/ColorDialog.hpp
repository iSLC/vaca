// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/CommonDialog.hpp"
#include "VACA/Color.hpp"

namespace Wg {

/**
   A common dialog box to select colors.
*/
class VACA_DLL ColorDialog : public CommonDialog {
    Color m_color;
    COLORREF m_customColors[16]{};

public:

    ColorDialog(const Color &color, Widget *parent);

    ~ColorDialog() override;

    bool doModal() override;

    [[nodiscard]] Color getColor() const;

};

} // namespace Wg
