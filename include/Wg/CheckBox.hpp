// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/ButtonBase.hpp"

namespace Wg {

/**
   Handles a check box button.

   @image html CheckBox.png
*/
class VACA_DLL CheckBox : public ButtonBase {
public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    CheckBox(const String &text, Widget *parent, const Style &style = Styles::Default);

    ~CheckBox() override;

};

} // namespace Wg
