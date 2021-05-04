// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Widget.hpp"

namespace vaca {

class VACA_DLL Slider : public Widget {
public:

    static const int MinLimit;
    static const int MaxLimit;

    struct VACA_DLL Styles {
        static const Style Default;
    };

    Slider(Widget *parent, const Style &style = Styles::Default);

    Slider(int minValue, int maxValue, int value, Widget *parent, const Style &style = Styles::Default);

    ~Slider() override;

    Orientation getOrientation();

    void setOrientation(Orientation orientation);

    Sides getSides();

    void setSides(Sides sides);

    void setTickVisible(bool state);

    void setTickFreq(int freq);

    int getMinValue();

    int getMaxValue();

    void getRange(int &minValue, int &maxValue);

    void setRange(int minValue, int maxValue);

    int getValue();

    void setValue(int posValue);

    int getLineSize();

    void setLineSize(int lineSize);

    int getPageSize();

    void setPageSize(int pageSize);

    // Signals
    Signal1<void, Event &> Change; ///< @see onChange

protected:
    // Events
    void onPreferredSize(PreferredSizeEvent &ev) override;

    void onScroll(ScrollEvent &ev) override;

    // New events
    virtual void onChange(Event &ev);

};

} // namespace vaca
