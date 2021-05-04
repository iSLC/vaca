// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Widget.hpp"
#include "Wg/WidgetClass.hpp"
#include "Wg/Register.hpp"
#include "Wg/TextEdit.hpp"
#include "Wg/SpinButton.hpp"

namespace Wg {

/**
   Represents the Win32 class used by Spinner.
*/
class SpinnerClass : public WidgetClass {
public:
    static WidgetClassName getClassName() { return WidgetClassName(L"Vaca.Spinner"); }
};

/**
   A Spinner is a couple of Widgets: an Edit and a SpinButton at the
   right side.

   The default range is from 0 to 100. The default position is 0.
*/
class VACA_DLL Spinner : public Register<SpinnerClass>, public Widget {
    TextEdit m_edit;
    SpinButton m_spin;

public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    Spinner(Widget *parent,
            const Style &spinStyle = SpinButton::Styles::Default,
            const Style &style = Spinner::Styles::Default);

    Spinner(int minValue, int maxValue, int value,
            Widget *parent,
            const Style &spinStyle = SpinButton::Styles::Default,
            const Style &style = Spinner::Styles::Default);

    ~Spinner() override;

    TextEdit &getTextEdit();

    SpinButton &getSpinButton();

    int getMinimum();

    int getMaximum();

    void getRange(int &minValue, int &maxValue);

    void setRange(int minValue, int maxValue);

    int getValue();

    void setValue(int value);

    int getBase();

    void setBase(int base);

protected:

    // Events
    void onPreferredSize(PreferredSizeEvent &ev) override;

    void onLayout(LayoutEvent &ev) override;

};

} // namespace Wg
