// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Component.hpp"

#include <vector>

namespace Wg {

/**
   An item in a ListView.
*/
class VACA_DLL ListItem : public Component {
    friend class ListView;

    int m_index;
    std::vector<String> m_text;
    int m_image;
    ListView *m_owner;

public:

    ListItem(const String &text = L"", int imageIndex = -1);

    ~ListItem() override;

    ListView *getListView();

    [[nodiscard]] int getIndex() const;

    [[nodiscard]] Rect getBounds() const;

    virtual String getText(size_t columnIndex);

    virtual int getImage();

    void setText(const String &text, size_t columnIndex = 0);

    void setImage(int image);

    void update();

    [[nodiscard]] bool isSelected() const;

    void setSelected(bool state);

    void ensureVisible();

private:
    void addToListView(ListView *listView);

    void removeFromListView();

};

} // namespace Wg
