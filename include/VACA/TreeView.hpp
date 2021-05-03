// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Widget.hpp"
#include "VACA/TreeNode.hpp"
#include "VACA/ImageList.hpp"
#include "VACA/Point.hpp"

#include <iterator>

namespace vaca {

/**
   @internal You should use @link vaca::TreeView::iterator TreeView::iterator @endlink.
*/
class VACA_DLL TreeViewIterator
  : public std::iterator<std::bidirectional_iterator_tag,
			 TreeNode*>
{
  TreeNode* m_currentNode;

public:
  TreeViewIterator();
  TreeViewIterator(const TreeViewIterator& other);
  explicit TreeViewIterator(TreeNode* node);

  TreeViewIterator& operator=(const TreeViewIterator& other);

  TreeNode*& operator*() { return m_currentNode; }
  TreeNode** operator->() { return &m_currentNode; }
  TreeViewIterator& operator++() {
    increment();
    return *this;
  }
  TreeViewIterator operator++(int) {
    TreeViewIterator i = *this;
    increment();
    return i;
  }
  TreeViewIterator& operator--() {
    decrement();
    return *this;
  }
  TreeViewIterator operator--(int) {
    TreeViewIterator i = *this;
    decrement();
    return i;
  }
  bool operator==(const TreeViewIterator& i) const { return equal(i); }
  bool operator!=(const TreeViewIterator& i) const { return !equal(i); }

private:
  void increment();
  void decrement();
  [[nodiscard]] bool equal(TreeViewIterator const& other) const;
};

/**
   Handles a TreeView control.  A TreeView (internally) has a root
   TreeNode that is the parent of the first-level's nodes.
*/
class VACA_DLL TreeView : public Widget
{
  friend class TreeNode;

  TreeNode m_root;
  bool m_deleted;
  TreeNode* m_dragSource;
  ImageList m_dragImage;
  ImageList m_normalImageList;
  ImageList m_stateImageList;

  /**
     To use LPSTR_TEXTCALLBACK we need some space
     to allocate text temporally.
  */
  String m_tmpBuffer;

public:

  /**
     Iterator to go through all the nodes of the tree (it does a deep
     scan).
  */
  typedef TreeViewIterator iterator;

  struct VACA_DLL Styles {
    static const Style EditLabel;
    static const Style ShowSelectionAlways;
    static const Style WithButtons;
    static const Style WithLines;
    static const Style RootWithLines;
    static const Style FullRowSelect;
    static const Style NoDragAndDrop;
    static const Style Default;
  };

  TreeView(Widget* parent, const Style& style = Styles::Default);
  explicit TreeView(HWND handle);
  ~TreeView() override;

  iterator begin();
  iterator end();

  [[nodiscard]] bool isDragAndDrop() const;
  void setDragAndDrop(bool state);

  void setImageList(const ImageList& imageList);
  void setStateImageList(const ImageList& imageList);

  [[nodiscard]] int getRowHeight() const;
  void setRowHeight(int height);

  void addNode(TreeNode* node);
  void removeNode(TreeNode* node);

  [[nodiscard]] int getNodeCount() const;
  [[nodiscard]] int getVisibleCount() const;

  [[nodiscard]] TreeNode* getSelectedNode() const;
  void setSelectedNode(TreeNode* node);

  [[nodiscard]] TreeNode* getDropTarget() const;
  void setDropTarget(TreeNode* node);

  TreeNode* getNodeInPoint(const Point& pt);

  void setBgColor(const Color& color) override;

  // Signals
  Signal1<void, TreeViewEvent&> BeforeExpand;
  Signal1<void, TreeViewEvent&> BeforeCollapse;
  Signal1<void, TreeViewEvent&> BeforeSelect;
  Signal1<void, TreeViewEvent&> BeforeLabelEdit;
  Signal1<void, TreeViewEvent&> AfterExpand;
  Signal1<void, TreeViewEvent&> AfterCollapse;
  Signal1<void, TreeViewEvent&> AfterSelect;
  Signal1<void, TreeViewEvent&> AfterLabelEdit;
//   Signal1<void, TreeViewEvent&> BeginDrag;
//   Signal1<void, TreeViewEvent&> EndDrag;

protected:
  // Events
  void onMouseMove(MouseEvent& ev) override;
  void onMouseUp(MouseEvent& ev) override;
  void onSetCursor(SetCursorEvent& ev) override;

  // New events
  virtual void onBeforeExpand(TreeViewEvent& ev);
  virtual void onBeforeCollapse(TreeViewEvent& ev);
  virtual void onBeforeSelect(TreeViewEvent& ev);
  virtual void onBeforeLabelEdit(TreeViewEvent& ev);
  virtual void onAfterExpand(TreeViewEvent& ev);
  virtual void onAfterCollapse(TreeViewEvent& ev);
  virtual void onAfterSelect(TreeViewEvent& ev);
  virtual void onAfterLabelEdit(TreeViewEvent& ev);
//   virtual void onBeginDrag(TreeViewEvent& ev);
//   virtual void onEndDrag(TreeViewEvent& ev);

  // Reflected notifications
  bool onReflectedNotify(LPNMHDR lpnmhdr, LRESULT& lResult) override;

};

} // namespace vaca
