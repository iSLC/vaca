// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Component.hpp"

namespace vaca {

/**
   A column in a ListView.
*/
class VACA_DLL ListColumn : public Component {
    friend class ListView;

    int m_index;
    String m_text;
    TextAlign m_textAlign;
    int m_width;
    ListView *m_owner;

public:

    ListColumn(const String &text, TextAlign textAlign = TextAlign::Left);

    ~ListColumn() override;

    ListView *getListView();

    [[nodiscard]] String getText() const;

    void setText(const String &text);

    [[nodiscard]] TextAlign getTextAlign() const;

    void setTextAlign(TextAlign textAlign);

    [[nodiscard]] int getWidth() const;

    void setWidth(int width);

    void setPreferredWidth(bool useHeader = true);

    [[nodiscard]] Rect getBounds() const;

    // int getIndex() const;
    // static ListColumn* fromIndex(HWND hwnd, int index);

private:
    void addToListView(ListView *listView);

    void removeFromListView();

};

} // namespace vaca
