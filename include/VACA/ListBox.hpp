// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Widget.hpp"

#include <vector>

namespace Wg {

/**
   List box control. Widget to manage a list of strings.
*/
class VACA_DLL ListBox : public Widget {
public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    ListBox(Widget *parent, const Style &style = Styles::Default);

    ~ListBox() override;

    int addItem(const String &text);

    void insertItem(int itemIndex, const String &text);

    void removeItem(int itemIndex);

    int getItemCount();

    Rect getItemBounds(int itemIndex);

    String getItemText(int itemIndex);

    void setItemText(int itemIndex, const String &text);

    int getSelectedItem();

    void setSelectedItem(int itemIndex);

    std::vector<int> getSelectedItems();

    // Signals
    Signal1<void, Event &> ItemDoubleClick; ///< @see onItemDoubleClick
    Signal1<void, Event &> SelChange; ///< @see onSelChange

protected:
    // Events
    void onPreferredSize(PreferredSizeEvent &ev) override;

    // New events
    virtual void onItemDoubleClick(Event &ev);

    virtual void onSelChange(Event &ev);

    // Reflected notifications
    bool onReflectedCommand(int id, int code, LRESULT &lResult) override;

};

class VACA_DLL DragListBox : public ListBox {
public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    DragListBox(Widget *parent, const Style &style = Styles::Default);

};

} // namespace Wg
