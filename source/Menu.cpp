// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/Menu.hpp"
#include "Wg/MenuItemEvent.hpp"
#include "Wg/Debug.hpp"
#include "Wg/System.hpp"
#include "Wg/Mdi.hpp"
#include "Wg/ResourceException.hpp"
#include "Wg/ScopedLock.hpp"
#include "Wg/Command.hpp"
#include "Wg/Point.hpp"

#include <stack>

#ifndef MIM_MENUDATA
#  define MIM_MENUDATA 8
#endif

#ifndef MIM_STYLE
#  define MIM_STYLE 16
#endif

using namespace Wg;

// ======================================================================
// MenuItem

MenuItem::MenuItem()
{
  m_parent = nullptr;
  m_id = 0;
  m_enabled = true;
  m_checked = false;
}

/**
   Creates a new menu item.

   @param text You can use the \& character to underline a letter (e.g.: "&New")
   @param defaultShortcut The default keyboard shortcut for this menu item.
   @param id You can use an specified CommandId for the menu.
*/
MenuItem::MenuItem(const String& text, CommandId id, Keys::Type defaultShortcut)
{
  m_parent = nullptr;
  m_text = text;
  m_id = id;
  m_enabled = true;
  m_checked = false;

  if (defaultShortcut != Keys::None)
    addShortcut(defaultShortcut);
}

MenuItem::~MenuItem()
{
  Menu* parent = getParent();
  if (parent != nullptr)
    parent->remove(this);
}

Menu* MenuItem::getParent()
{
  return m_parent;
}

Menu* MenuItem::getRoot()
{
  Menu* root = m_parent;
  if (root) {
    while (root->m_parent != nullptr)
      root = root->m_parent;
  }
  return root;
}

CommandId MenuItem::getId() const
{
  return m_id;
}

const String& MenuItem::getText() const
{
  return m_text;
}

void MenuItem::setText(const String& text)
{
  m_text = text;

  if (m_parent != nullptr) {
    MENUITEMINFO mii;

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STRING;
    mii.dwTypeData = const_cast<LPTSTR>(text.c_str());

    SetMenuItemInfo(m_parent->getHandle(),
                    static_cast<UINT>(m_parent->getMenuItemIndex(this)),
		    TRUE, &mii);
  }
}

void MenuItem::setId(CommandId id)
{
  m_id = id;

  if (m_parent != nullptr) {
    MENUITEMINFO mii;

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_ID;
    mii.wID = m_id;

    SetMenuItemInfo(m_parent->getHandle(),
                    static_cast<UINT>(m_parent->getMenuItemIndex(this)),
		    TRUE, &mii);
  }
}

bool MenuItem::isEnabled()
{
  if (m_parent) {
    MENUITEMINFO mii;
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STATE;
    if (GetMenuItemInfo(m_parent->getHandle(),
                        static_cast<UINT>(m_parent->getMenuItemIndex(this)),
  			TRUE, &mii)) {
      m_enabled = (mii.fState & (MFS_DISABLED | MFS_GRAYED)) == 0;
    }
  }
  return m_enabled;
}

void MenuItem::setEnabled(bool state)
{
  m_enabled = state;

  if (m_parent) {
    ::EnableMenuItem(m_parent->getHandle(),
                     static_cast<UINT>(m_parent->getMenuItemIndex(this)),
                     static_cast<UINT>(MF_BYPOSITION | (m_enabled ? MF_ENABLED : MF_GRAYED)));
  }
}

bool MenuItem::isChecked()
{
  if (m_parent) {
    MENUITEMINFO mii;
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STATE;
    if (GetMenuItemInfo(m_parent->getHandle(),
                        static_cast<UINT>(m_parent->getMenuItemIndex(this)),
			TRUE, &mii)) {
      m_checked = (mii.fState & MFS_CHECKED) != 0;
    }
  }
  return m_checked;
}

void MenuItem::setChecked(bool state)
{
  m_checked = state;

  if (m_parent) {
    ::CheckMenuItem(m_parent->getHandle(),
                    static_cast<UINT>(m_parent->getMenuItemIndex(this)),
                    static_cast<UINT>(MF_BYPOSITION |
                                      (state ? MF_CHECKED : MF_UNCHECKED)));
  }
}

/**
   Radio MenuItem groups are automatically delimited by MenuSeparator
   or by the beginning and the end of the menu.
*/
void MenuItem::setRadio(bool state)
{
  if (state) {
    assert(m_parent != NULL);

    int index = m_parent->getMenuItemIndex(this);
    int count = m_parent->getItemCount();
    int first = index;
    int last = index;

    do {
      first--;
    } while (first >= 0 && !m_parent->getMenuItemByIndex(first)->isSeparator());
    first++;

    do {
      last++;
    } while (last < count && !m_parent->getMenuItemByIndex(last)->isSeparator());
    last--;

    ::CheckMenuRadioItem(m_parent->getHandle(), static_cast<UINT>(first), static_cast<UINT>(last),
                         static_cast<UINT>(index), MF_BYPOSITION);
  }
  else {
    setChecked(false);
  }
}

void MenuItem::addShortcut(Keys::Type shortcut)
{
  assert(shortcut != Keys::None);

  m_shortcuts.push_back(shortcut);
}

MenuItem* MenuItem::checkShortcuts(Keys::Type pressedKey)
{
  for (int & m_shortcut : m_shortcuts) {
    if (pressedKey == m_shortcut)
      return this;
  }
  return nullptr;
}

bool MenuItem::isMenu() const { return false; }
bool MenuItem::isMenuBar() const { return false; }
bool MenuItem::isSeparator() const { return false; }
bool MenuItem::isMdiList() const { return false; }

/**
   It is called when the menu item is selected. Also it's called when
   some keyboard shortcut of this MenuItem is pressed
   (MenuItem::m_shortcuts). Remember that onClick() is called only
   after an onUpdate() and only if it leaves the MenuItem enabled (see
   #setEnabled member function),
*/
void MenuItem::onClick(MenuItemEvent& ev)
{
  // do nothing
}

/**
   It's called when a menu is shown for first time. Also when the user
   press a keyboard shortcut (MenuItem::m_shortcuts) it's called to
   known if the item is available after execute onClick().

   Internally, when the WM_INITMENU message is received, a Frame calls
   this event.

   The default implementation search for a Command with ID equal to
   MenuItem#getId, so the Command#isEnabled routine will be called to
   change the state of this MenuItem.
*/
void MenuItem::onUpdate(MenuItemEvent& ev)
{
  // does this menu item have an associated command-id?
  if (m_id != 0) {
    // search the command in the frame where is the menu-bar
    Menu* rootMenu = getRoot();
    if (rootMenu && rootMenu->isMenuBar()) {
      auto* menuBar = dynamic_cast<MenuBar*>(rootMenu);
      if (Frame* frame = menuBar->getFrame()) {
	if (Command* cmd = frame->getCommandById(m_id))
	  updateFromCommand(cmd);
      }
    }

    // check if the application is a CommandsClient
    if (auto* cc = dynamic_cast<CommandsClient*>(Application::getInstance())) {
      if (Command* cmd = cc->getCommandById(m_id))
	updateFromCommand(cmd);
    }
  }
}

void MenuItem::updateFromCommand(Command* cmd)
{
  assert(cmd);
  setEnabled(cmd->isEnabled());
  setChecked(cmd->isChecked());
}

// ======================================================================
// MenuSeparator

MenuSeparator::MenuSeparator()
= default;

MenuSeparator::~MenuSeparator()
= default;

bool MenuSeparator::isSeparator() const
{
  return true;
}

// ======================================================================
// Menu

Menu::Menu()
{
  m_handle = ::CreateMenu();
  VACA_TRACE("%p = CreateMenu()\n", m_handle)

  subClass();
}

Menu::Menu(const String& text)
  : MenuItem(text, 0)
{
  m_handle = ::CreatePopupMenu();
  VACA_TRACE("%p = CreatePopupMenu()\n", m_handle)

  subClass();
}

Menu::Menu(CommandId menuId)
{
  m_handle = ::LoadMenu(Application::getHandle(),
		       MAKEINTRESOURCE(menuId));

  if (m_handle == nullptr)
    throw ResourceException(format_string(L"Can't load the menu resource %d", menuId));

  subClass();
}

Menu::Menu(HMENU hmenu)
{
  assert(hmenu != NULL);

  m_handle = hmenu;
  subClass();
}

Menu::~Menu()
{
  assert(m_handle != NULL);

  while (!m_container.empty()) {
    MenuItem* menuItem = m_container.front();
    delete menuItem;
  }

  DestroyMenu(m_handle);
}

void Menu::subClass()
{
  assert(m_handle != NULL);

  MENUINFO mi;
  mi.cbSize = sizeof(MENUINFO);
  mi.fMask = MIM_MENUDATA | MIM_STYLE;
  GetMenuInfo(m_handle, &mi);

  // Vaca doesn't use MNS_NOTIFYBYPOS
  assert((mi.dwStyle & MNS_NOTIFYBYPOS) == 0);

  // the menu can't have data
  assert(mi.dwMenuData == 0);

  // set the associated data with this Menu
  mi.fMask = MIM_MENUDATA;
  mi.dwMenuData = reinterpret_cast<ULONG_PTR>(this);
  SetMenuInfo(m_handle, &mi);

  // now we must sub-class all existent menu items
  int menuItemCount = GetMenuItemCount(m_handle);
  for (int itemIndex=0; itemIndex<menuItemCount; ++itemIndex) {
    Char buf[4096];		// TODO buffer overflow
    MENUITEMINFO mii;

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_FTYPE | MIIM_DATA | MIIM_ID | MIIM_STRING | MIIM_SUBMENU;
    mii.dwTypeData = buf;
    mii.cch = sizeof(buf) / sizeof(Char);

    BOOL res = GetMenuItemInfo(m_handle, static_cast<UINT>(itemIndex), TRUE, &mii);
    assert(res == TRUE);

    // the item can't have data
    assert(mii.dwItemData == 0);

    MenuItem* menuItem = nullptr;

    switch (mii.fType) {

      case MFT_STRING:
	if (mii.hSubMenu != nullptr) {
	  menuItem = new Menu(mii.hSubMenu);
	  menuItem->m_text = buf;
	}
	else {
	  menuItem = new MenuItem(buf, mii.wID, Keys::None);
	}
	break;

      case MFT_SEPARATOR:
	menuItem = new MenuSeparator();
	break;

	// TODO more MFT_ support
	// TODO MdiListMenu
      default:
	assert(false);		// TODO unsupported MENUITEM type
	break;
    }

    menuItem->m_parent = this;
    m_container.push_back(menuItem);
  }
}

/**
   Adds a MenuItem at the end of the Menu.

   @warning The @a menuItem is deleted automatically if you don't
	    remove it before to destroy the Menu.

   @see insert(int, MenuItem*), remove(MenuItem*)
*/
MenuItem* Menu::add(MenuItem* menuItem)
{
  insert(static_cast<int>(m_container.size()), menuItem);
  return menuItem;
}

/**
   Adds a new MenuItem with the specified @a text and @a defaultShortcut
   at the end of the Menu.

   @warning The returned MenuItem is deleted automatically when the
	    Menu is destroyed.
   @warning If you use the Menu::remove member function with the returned
	    MenuItem, you will be responsible to delete it.

   @see add(MenuItem*), remove(MenuItem*)
*/
MenuItem* Menu::add(const String& string, CommandId id, Keys::Type defaultShortcut)
{
  MenuItem* menuItem = add(new MenuItem(string, id, defaultShortcut));
  return menuItem;
}

/**
   Adds a new MenuSeparator at the end of the Menu.

   @see add(MenuItem*), remove(MenuItem*)
*/
void Menu::addSeparator()
{
  MenuItem* menuItem = add(new MenuSeparator());
}

/**
   Inserts a MenuItem at the @a index position of the Menu.

   @warning The inserted @a menuItem is deleted automatically if you don't
	    remove it before to destroy the Menu.

   @see remove(MenuItem*), insert(int, const String &)
*/
MenuItem* Menu::insert(int index, MenuItem* menuItem)
{
  assert(index >= 0 && index <= static_cast<int>(m_container.size()));

  MenuItemList::iterator it;
  int c = 0;

  for (it=m_container.begin(); it!=m_container.end(); ++it, ++c)
    if (c == index)
      break;

  String text(menuItem->getText());
  int len = static_cast<int>(text.size());
  Char* buf = new Char[len+1];
#ifdef UNICODE
  wcscpy(buf, text.c_str());
#else
  strcpy(buf, text.c_str());
#endif

  MENUITEMINFO mii;
  mii.cbSize = sizeof(MENUITEMINFO);
  mii.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID | MIIM_DATA;
  mii.dwItemData = reinterpret_cast<ULONG_PTR>(menuItem);
  mii.fState =
          static_cast<UINT>((menuItem->isEnabled() ? MFS_ENABLED : MFS_DISABLED) |
                            (menuItem->isChecked() ? MFS_CHECKED : 0));
  mii.wID = menuItem->getId();

  if (menuItem->isSeparator()) {
    mii.fType = MFT_SEPARATOR;
  }
  else if (menuItem->isMenu()) {
    mii.fMask |= MIIM_STRING | MIIM_SUBMENU;
    mii.fType = MFT_STRING;
    Menu* menu = dynamic_cast<Menu*>(menuItem);
    mii.hSubMenu = menu->getHandle();
    mii.dwTypeData = buf;
    mii.cch = static_cast<UINT>(len);
  }
  else {
    mii.fMask |= MIIM_STRING;
    mii.fType = MFT_STRING;
    mii.dwTypeData = buf;
    mii.cch = static_cast<UINT>(len);
    // HBITMAP hbmpChecked;
    // HBITMAP hbmpUnchecked;
    // HBITMAP hbmpItem;
  }

  InsertMenuItem(m_handle, static_cast<UINT>(index), TRUE, &mii);

  delete[] buf;

  m_container.insert(it, menuItem);
  menuItem->m_parent = this;
  return menuItem;
}

/**
   Inserts a new MenuItem with the specified @a text at the @a index
   position of the Menu.

   @warning The returned MenuItem is deleted automatically when the
	    Menu is destroyed.
   @warning If you use the Menu::remove member function with the returned
	    MenuItem, you will be responsible to delete it.

   @see insert(int, MenuItem*), remove(MenuItem*)
*/
MenuItem* Menu::insert(int index, const String& text, CommandId id, Keys::Type defaultShortcut)
{
  MenuItem* menuItem = insert(index, new MenuItem(text, id, defaultShortcut));
  return menuItem;
}

/**
   Inserts a new MenuSeparator at the @a index position of the Menu.
*/
void Menu::insertSeparator(int index)
{
  MenuItem* menuItem = insert(index, new MenuSeparator());
}

/**
   Removes the @a menuItem from the menu.

   @warning You must to delete the specified @a menuItem after this
	    (because it'll not be under the control of the Menu any more).

   @return The same pointer to the specified parameter @a menuItem
*/
MenuItem* Menu::remove(MenuItem* menuItem)
{
  assert(m_handle != NULL);

  // TODO check if this works
  ::RemoveMenu(m_handle, static_cast<UINT>(getMenuItemIndex(menuItem)), MF_BYPOSITION);

  // see the "RemoveMenu Function" in the MSDN, you "must call the
  // DrawMenuBar function whenever a menu changes"
  // http://msdn.microsoft.com/en-us/library/ms647994(VS.85).aspx
  Menu* rootMenu = getRoot();
  if (rootMenu && rootMenu->isMenuBar()) {
    auto* menuBar = dynamic_cast<MenuBar*>(rootMenu);
    Frame* frame = menuBar->getFrame();
    ::DrawMenuBar(frame ? frame->getHandle(): nullptr);
  }

  menuItem->m_parent = nullptr;
  remove_from_container(m_container, menuItem);

  return menuItem;
}

/**
   Removes the menu item that is the specified position by the @a
   index.

   @warning You should delete the returned pointer or save it
	    to destroy it in the future.

   @code
   Menu* menu = ...
   MenuItem* menuItem = menu->getMenuItemByIndex(2);
   menu->remove(menuItem);
   delete menuItem;
   @endcode

   It's the same as:

   @code
   delete menu->remove(2);
   @endcode

   @see remove(MenuItem*)
*/
MenuItem* Menu::remove(int index)
{
  MenuItem* menuItem = getMenuItemByIndex(index);
  remove(menuItem);
  return menuItem;
}

MenuItem* Menu::getMenuItemByIndex(int index)
{
  return m_container[index];
}

MenuItem* Menu::getMenuItemById(CommandId id)
{
  if (id == 0)
    return nullptr;

  MenuItem* menuItem = nullptr;

  std::stack<MenuItem*> stack;
  stack.push(this);

  while (!stack.empty()) {
    menuItem = stack.top();
    if (menuItem->getId() == id)
      return menuItem;

    stack.pop();

    if (menuItem->isMenu()) {
      MenuItemList& subMenus(dynamic_cast<Menu*>(menuItem)->m_container);

      for (auto & subMenu : subMenus)
	stack.push(subMenu);
    }
  }

  return nullptr;
}

int Menu::getMenuItemIndex(MenuItem* menuItem)
{
  MenuItemList::iterator it;
  int c = 0;

  for (it=m_container.begin(); it!=m_container.end(); ++it, ++c)
    if (*it == menuItem)
      return c;

  return -1;
}

int Menu::getItemCount()
{
  // TODO
  return static_cast<int>(m_container.size());
}

MenuItemList Menu::getMenuItems()
{
  return m_container;
}

MenuItem* Menu::checkShortcuts(Keys::Type pressedKey)
{
  for (auto & it : m_container) {
    MenuItem* menuItem = it->checkShortcuts(pressedKey);
    if (menuItem != nullptr)
      return menuItem;
  }

  return nullptr;
}

// PopupMenu* Menu::getPopupMenu()
// {
//   // TODO
//   return NULL;
// }

bool Menu::isMenu() const
{
  return true;
}

// Menu* Menu::getMenuByHMENU(HMENU hmenu)
// {
//   Menu* lastMenu = NULL;

//   std::stack<Menu*> stack;
//   stack.push(this);

//   while (!stack.empty()) {
//     lastMenu = stack.top();
//     if (lastMenu->getHandle() == hmenu)
//       return lastMenu;

//     stack.pop();

//     MenuItemList& subMenus(lastMenu->m_container);
//     for (MenuItemList::iterator it=subMenus.begin();
// 	 it!=subMenus.end(); ++it) {
//       if ((*it)->isMenu())
// 	stack.push(static_cast<Menu*>(*it));
//     }
//   }

//   return NULL;
// }

HMENU Menu::getHandle()
{
  return m_handle;
}

// ======================================================================
// MenuBar

MenuBar::MenuBar()
  : Menu()
  , m_frame(nullptr)
{
}

MenuBar::MenuBar(CommandId menuId)
  : Menu(menuId)
  , m_frame(nullptr)
{
}

MenuBar::~MenuBar()
= default;

Frame* MenuBar::getFrame()
{
  return m_frame;
}

void MenuBar::setFrame(Frame* frame)
{
  m_frame = frame;
}

MdiListMenu* MenuBar::getMdiListMenu()
{
  std::stack<MenuItem*> stack;
  stack.push(this);

  while (!stack.empty()) {
    MenuItem* menuItem = stack.top();
    stack.pop();

    if (menuItem->isMdiList())
      return dynamic_cast<MdiListMenu*>(menuItem);
    else if (menuItem->isMenu()) {
      MenuItemList container = dynamic_cast<Menu*>(menuItem)->getMenuItems();
      for (auto & it : container)
	stack.push(it);
    }
  }

  return nullptr;
}

bool MenuBar::isMenuBar() const { return true; }

// ======================================================================
// PopupMenu

PopupMenu::PopupMenu()
  : Menu(String())		// creates a Menu with an empty label
{
}

PopupMenu::~PopupMenu()
= default;

/**
   Displays the popup-menu in the screen at the specified location.

   @param widget The widget to be locked by the menu, can be NULL.
   @param pt The location where to show the menu, it's relative to the
     client area of the specified @a widget, or if @a widget is NULL, it
     is absolute position in the screen.
   @param horzAlign Where to align the menu .
   @param vertAlign

   @return The ID of the selected MenuItem, or zero if no item was selected.
*/
CommandId PopupMenu::doModal(Widget* widget,
			     const Point& pt,
			     TextAlign horzAlign,
			     VerticalAlign vertAlign)
{
  assert(getHandle());

  Point absPt = pt;
  if (widget)
    absPt += widget->getAbsoluteClientBounds().getOrigin();

  UINT flags = TPM_RETURNCMD | TPM_NONOTIFY;

  switch (horzAlign) {
    case TextAlign::Left: flags |= TPM_LEFTALIGN; break;
    case TextAlign::Center: flags |= TPM_CENTERALIGN; break;
    case TextAlign::Right: flags |= TPM_RIGHTALIGN; break;
  }

  switch (vertAlign) {
    case VerticalAlign::Top: flags |= TPM_TOPALIGN; break;
    case VerticalAlign::Middle: flags |= TPM_VCENTERALIGN; break;
    case VerticalAlign::Bottom: flags |= TPM_BOTTOMALIGN; break;
  }

  auto id = static_cast<CommandId>(TrackPopupMenuEx(getHandle(),
                                                         flags,
                                                         absPt.x, absPt.y,
                                                         widget ? widget->getHandle() : nullptr,
                                                         nullptr));

  return id;
}

// ======================================================================
// MdiListMenu

MdiListMenu::MdiListMenu(const String& text)
 : Menu(text)
{
}

MdiListMenu::~MdiListMenu()
= default;

bool MdiListMenu::isMdiList() const
{
  return true;
}
