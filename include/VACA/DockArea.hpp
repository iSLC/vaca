// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Widget.hpp"
#include "VACA/WidgetClass.hpp"
#include "VACA/Register.hpp"

namespace vaca {

/**
   Information needed for every DockBar that is docked in a specific
   DockArea. You have to specialized this class to use it in your own
   DockArea (e.g. BasicDockArea uses a BasicDockInfo for each DockBar
   docked on it).
*/
class DockInfo
{
public:
  virtual Size getSize() = 0;
  virtual Side getSide() = 0;
};

/**
   Represents the Win32 class used by DockArea.
*/
class DockAreaClass : public WidgetClass
{
public:
  static WidgetClassName getClassName()
  { return WidgetClassName(L"Vaca.DockArea"); }
};

/**
   The abstract class to create DockAreas. A DockArea is the place
   where you can dock toolbars (any DockBar really).
*/
class VACA_DLL DockArea : public Register<DockAreaClass>, public Widget
{
  friend class DockBar;

  Side m_side;

public:

  struct VACA_DLL Styles {
    static const Style Default;
  };

  DockArea(Side side, Widget* parent, const Style& style = Styles::Default);
  ~DockArea() override;

  bool isHorizontal();
  bool isVertical();

  Side getSide();

  [[nodiscard]] bool isLayoutFree() const override;

  /**
     Returns true if the point @a pt is inside a valid rectangle to dock
     in this DockArea.

     @param bar
	 The DockBar to dock.

     @param cursor
	 Absolute position.

     @param fromInside
	 True means that we are already inside the
     	 DockArea, so try to use the more biggest are to
	 return a true hit-test.
  */
  virtual bool hitTest(DockBar* bar, const Point& cursor, const Point& anchor, bool fromInside) = 0;

  /**
     Returns a default DockInfo for the floating DockBar that want to be
     docked by double-click by first-time.
  */
  virtual DockInfo* createDefaultDockInfo(DockBar* bar) = 0;

  /**
     @return A new created dockInfo (you must to delete this pointer).
  */
  virtual DockInfo* createDockInfo(DockBar* bar, const Point& cursor, const Point& anchor) = 0;

  /**
     Draws the tracker (generally a rectangle) in Xor mode that
     represents the specified @a dockInfo.
  */
  virtual void drawXorTracker(Graphics& g, DockInfo* dockInfo) = 0;

  /**
     Lays out all dock bars inside the dock area using the DockInfo of
     each DockBar.

     @see DockBar, DockInfo
  */
  void layout() override = 0;

private:

  void addDockBar(DockBar* dockBar);
  void removeDockBar(DockBar* dockBar);

protected:
  // Events
  void onPreferredSize(PreferredSizeEvent& ev) override = 0;

  // New events
  virtual void onAddDockBar(DockBar* dockBar);
  virtual void onRemoveDockBar(DockBar* dockBar);
  virtual void onRedock(DockBar* dockBar, DockInfo* newDockInfo);

};

} // namespace vaca
