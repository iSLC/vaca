// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Widget.hpp"

#include <vector>

namespace Wg {

/**
   Combo box control. Widget to select an option from a drop down
   list.
*/
class VACA_DLL ComboBox : public Widget {
    Size m_maxItemSize;

public:

    struct VACA_DLL Styles {
        static const Style Default;
        static const Style Editable;
    };

    ComboBox(Widget *parent, const Style &style = Styles::Default);

    ~ComboBox() override;

    int addItem(const String &text);

    void insertItem(int itemIndex, const String &text);

    void removeItem(int itemIndex);

    void removeAllItems();

    int getItemCount();

    String getItemText(int itemIndex);

    void setItemText(int itemIndex, const String &text);

    int getSelectedItem();

    void setSelectedItem(int itemIndex);

    void setSelectedItem(const String &firstText);

    void setDropDownVisibile(bool visible);

    bool isDropDownVisible();

    Rect getDropDownBounds();

    // Signals
    Signal1<void, Event &> SelChange;  ///< @see onSelChange
    Signal1<void, Event &> EditChange; ///< @see onEditChange

protected:
    // Events
    void onPreferredSize(PreferredSizeEvent &ev) override;

    void onLayout(LayoutEvent &ev) override;

    // New events
    virtual void onSelChange(Event &ev);

    virtual void onEditChange(Event &ev);

    // Reflected notifications
    bool onReflectedCommand(int id, int code, LRESULT &lResult) override;

private:
    int getHeightForAllItems();

    void updateMaxItemSize(const String &text);

};

} // namespace Wg
