// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Widget.hpp"
#include "VACA/CancelableEvent.hpp"
#include "VACA/ImageList.hpp"

#include <vector>

namespace vaca {

// ======================================================================

/**
   @see ListViewType
*/
struct ListViewTypeEnum
{
  enum enumeration {
    Icon,
    Report,
    SmallIcon,
    List
  };
  static const enumeration default_value = List;
};

/**
   Kind of ListView.
 */
typedef Enum<ListViewTypeEnum> ListViewType;

// ======================================================================

typedef std::vector<ListItem*> ListItemList;
typedef std::vector<ListColumn*> ListColumnList;

/**
   A ListView control.
*/
class VACA_DLL ListView : public Widget
{
  ListItemList m_items;
  ListColumnList m_columns;
  ImageList m_imageList;
  ImageList m_smallImageList;
  ImageList m_stateImageList;

  /**
     To use LPSTR_TEXTCALLBACK we need some space to allocate text temporally.
  */
  String m_tmpBuffer;

public:

  struct VACA_DLL Styles {
    static const Style Default;
    static const Style SingleSelection;
    static const Style ShowSelectionAlways;
  };

  ListView(Widget* parent, const Style& style = Styles::Default);
  ~ListView() override;

  void setBgColor(const Color& color) override;

  [[nodiscard]] Color getTextColor() const;
  void setTextColor(const Color& color);

  [[nodiscard]] Color getTextBgColor() const;
  void setTextBgColor(const Color& color);

  [[nodiscard]] ListViewType getType() const;
  void setType(ListViewType type);

  void setImageList(const ImageList& imageList);
  void setSmallImageList(const ImageList& imageList);
  void setStateImageList(const ImageList& imageList);
  [[nodiscard]] ImageList getImageList() const;
  [[nodiscard]] ImageList getSmallImageList() const;
  [[nodiscard]] ImageList getStateImageList() const;

  int addColumn(ListColumn* column);
  // int insertColumn(int columnIndex, const String& header, TextAlign textAlign = TextAlign::Left);
  void removeColumn(ListColumn* column);
  void removeAllColumns();

  [[nodiscard]] int getColumnCount() const;
  [[nodiscard]] ListColumn* getColumn(size_t columnIndex) const;

  int addItem(ListItem* item);
  // int insertItem(int itemIndex, const String& text, int imageIndex = -1);
  void removeItem(ListItem* item);
  void removeAllItems();

  [[nodiscard]] int getItemCount() const;
  [[nodiscard]] ListItem* getItem(size_t index) const;
  // Rect getItemBounds(int itemIndex, int code = LVIR_BOUNDS);

  // void editItemText(int itemIndex);

  [[nodiscard]] int getSelectedItemCount() const;

//   void sortItems(std::less<ListItem> functor);
//   int getCurrentItem();

  // Signals
  Signal1<void, ListViewEvent&> BeforeSelect;
  Signal1<void, ListViewEvent&> AfterSelect;
  Signal1<void, ListViewEvent&> ColumnClick;

protected:
  // Events
  void onResize(ResizeEvent& ev) override;

  // New events
  virtual void onBeforeSelect(ListViewEvent& ev);
  virtual void onAfterSelect(ListViewEvent& ev);
  virtual void onColumnClick(ListViewEvent& ev);

  // Reflected notifications
  bool onReflectedNotify(LPNMHDR lpnmhdr, LRESULT& lResult) override;

private:

  void insertDummyColumn();
  void removeDummyColumn();

};

/**
   Event where interact a ListView.
*/
class ListViewEvent : public CancelableEvent
{
  ListItem* m_item;
  ListColumn* m_column;

public:

  ListViewEvent(ListView* listView, ListItem* item, ListColumn* column)
    : CancelableEvent(listView)
    , m_item(item)
    , m_column(column)
  {
  }

  [[nodiscard]] ListItem* getItem() const { return m_item; }
  [[nodiscard]] ListColumn* getColumn() const { return m_column; }

};

} // namespace vaca
