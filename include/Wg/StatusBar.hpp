// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Widget.hpp"

namespace Wg {

class VACA_DLL StatusBar : public Widget {
public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    StatusBar(Widget *parent, const Style &style = Styles::Default);

    ~StatusBar() override;

    [[nodiscard]] bool isLayoutFree() const override;

protected:
    // Events
    void onPreferredSize(PreferredSizeEvent &ev) override;

    void onLayout(LayoutEvent &ev) override;
};

} // namespace Wg
