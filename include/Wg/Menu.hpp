// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Component.hpp"
#include "Wg/Event.hpp"
#include "Wg/Keys.hpp"

#include <vector>

namespace Wg {

typedef std::vector<MenuItem *> MenuItemList;

/**
   A menu item.
*/
class VACA_DLL MenuItem : public Component {
    friend class Frame;

    friend class Menu;

    Menu *m_parent;
    String m_text;
    CommandId m_id;
    std::vector<Keys::Type> m_shortcuts;
    bool m_enabled: 1;
    bool m_checked: 1;

public:

    MenuItem();

    MenuItem(const String &text, CommandId id, Keys::Type defaultShortcut = Keys::None);

    ~MenuItem() override;

    Menu *getParent();

    Menu *getRoot();

    [[nodiscard]] CommandId getId() const;

    [[nodiscard]] const String &getText() const;

    void setText(const String &text);

    void setId(CommandId id);

    bool isEnabled();

    void setEnabled(bool state);

    bool isChecked();

    void setChecked(bool state);

    void setRadio(bool state);

    void addShortcut(Keys::Type shortcut);
//   void addShortcut(Shortcut shortcut);

    virtual MenuItem *checkShortcuts(Keys::Type pressedKey);

    [[nodiscard]] virtual bool isMenu() const;

    [[nodiscard]] virtual bool isMenuBar() const;

    [[nodiscard]] virtual bool isSeparator() const;

    [[nodiscard]] virtual bool isMdiList() const;

protected:
    // Events
    virtual void onClick(MenuItemEvent &ev);

    virtual void onUpdate(MenuItemEvent &ev);

private:
    void updateFromCommand(Command *cmd);
};

/**
   A separator for menu items.
*/
class VACA_DLL MenuSeparator : public MenuItem {
public:
    MenuSeparator();

    ~MenuSeparator() override;

    [[nodiscard]] bool isSeparator() const override;
};

//   class CheckBoxMenuItem : public MenuItem {
//   public:
//     CheckBoxMenuItem(const String& text, bool state = false);
//     virtual ~CheckBoxMenuItem();

//     bool getState();
//     void setState(bool state);
//   };

//   class RadioButtonMenuItem : public MenuItem {
//   public:
//     RadioButtonMenuItem(const String& text, bool selected = false);
//     virtual ~RadioButtonMenuItem();
//   };

/**
   A container of menu items.
*/
class VACA_DLL Menu : public MenuItem {
    HMENU m_handle;
    MenuItemList m_container;

public:
    Menu();

    explicit Menu(const String &text);

    explicit Menu(CommandId menuId);

    explicit Menu(HMENU hmenu);

    ~Menu() override;

    MenuItem *add(MenuItem *menuItem);

    MenuItem *add(const String &text, CommandId id, Keys::Type defaultShortcut = Keys::None);

    void addSeparator();

    MenuItem *insert(int index, MenuItem *menuItem);

    MenuItem *insert(int index, const String &text, CommandId id, Keys::Type defaultShortcut = Keys::None);

    void insertSeparator(int index);

    MenuItem *remove(MenuItem *menuItem);

    MenuItem *remove(int index);

    MenuItem *getMenuItemByIndex(int index);

    MenuItem *getMenuItemById(CommandId id);

    int getMenuItemIndex(MenuItem *menuItem);

    // int getFirstMenuItemIndexByRadio(MenuItem* menuItem);
    // int getLastMenuItemIndexByRadio(MenuItem* menuItem);
    int getItemCount();

    MenuItemList getMenuItems();

    MenuItem *checkShortcuts(Keys::Type pressedKey) override;

    // PopupMenu* getPopupMenu();

    [[nodiscard]] bool isMenu() const override;

//   Menu* getMenuByHMENU(HMENU hmenu);

    HMENU getHandle();

private:
    void subClass();
};

/**
   A menu bar, has a collection of Menus.
*/
class VACA_DLL MenuBar : public Menu {
    friend class Frame;

    Frame *m_frame;

public:
    MenuBar();

    explicit MenuBar(CommandId menuId);

    ~MenuBar() override;

    Frame *getFrame();

    MdiListMenu *getMdiListMenu();

    [[nodiscard]] bool isMenuBar() const override;

private:
    void setFrame(Frame *frame);
};

class VACA_DLL PopupMenu : public Menu {
public:

    PopupMenu();

    ~PopupMenu() override;

    CommandId doModal(Widget *widget,
                      const Point &pt,
                      TextAlign horzAlign = TextAlign::Left,
                      VerticalAlign vertAlign = VerticalAlign::Top);

};

/**
   A menu that has the list of all MdiChild.
*/
class VACA_DLL MdiListMenu : public Menu {
public:
    MdiListMenu(const String &text);

    ~MdiListMenu() override;

    [[nodiscard]] bool isMdiList() const override;
};

} // namespace Wg
