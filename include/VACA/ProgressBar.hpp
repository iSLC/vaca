// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Widget.hpp"

namespace Wg {

/**
   Shows progress for a task that is running in background.
*/
class VACA_DLL ProgressBar : public Widget {
public:

    struct VACA_DLL Styles {
        static const Style Default;
        static const Style Smooth;
        static const Style Vertical; // TODO change with set/getOrientation
        static const Style Marquee;
    };

    ProgressBar(Widget *parent, const Style &style = Styles::Default);

    ProgressBar(int minValue, int maxValue, Widget *parent, const Style &style = Styles::Default);

    ~ProgressBar() override;

    void setBgColor(const Color &color) override;

    int getMinimum();

    int getMaximum();

    void getRange(int &minValue, int &maxValue);

    void setRange(int minValue, int maxValue);

    int getValue();

    void setValue(int value);

    void addValue(int delta);

    void setMarquee(int msecsInterval);

protected:
    // Events
    void onPreferredSize(PreferredSizeEvent &ev) override;

};

} // namespace Wg
