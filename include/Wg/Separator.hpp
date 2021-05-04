// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Widget.hpp"

namespace Wg {

class VACA_DLL Separator : public Widget {
public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    Separator(Widget *parent, const Style &style = Styles::Default);

    ~Separator() override;

protected:

    // Events
    void onPreferredSize(PreferredSizeEvent &ev) override;

};

} // namespace Wg
