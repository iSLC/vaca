// Vaca - Visual Application Components Abstraction
// Copyright (c) 2008, 2009, Jie Zhang, David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/ImageList.hpp"
#include "VACA/Widget.hpp"

namespace vaca {

struct ReBarBandStyleEnumSet
{
  enum {
    Break		/*RBBS_BREAK*/		= 0x0001,
    FixedSize		/*RBBS_FIXEDSIZE*/	= 0x0002,
    ChildEdge		/*RBBS_CHILDEDGE*/	= 0x0004,
    Hidden		/*RBBS_HIDDEN*/		= 0x0008,
    NoVert		/*RBBS_NOVERT*/		= 0x0010,
    FixedBmp		/*RBBS_FIXEDBMP*/	= 0x0020,
    VariableHeight	/*RBBS_VARIABLEHEIGHT*/	= 0x0040,
    GripperAlways	/*RBBS_GRIPPERALWAYS*/	= 0x0080,
    NoGripper		/*RBBS_NOGRIPPER*/	= 0x0100,
    UseChevron		/*RBBS_USECHEVRON*/	= 0x0200,
    HideTitle		/*RBBS_HIDETITLE*/	= 0x0400,
    TopAlign		/*RBBS_TOPALIGN*/	= 0x0800,

    Simple = ChildEdge | GripperAlways
  };
};

typedef EnumSet<ReBarBandStyleEnumSet> ReBarBandStyle;

class ReBarException : public Exception
{
public:
  ReBarException() : Exception() { }
  ReBarException(const String& message) : Exception(message) { }
  ~ReBarException() noexcept override = default;
};

/**
   A band of a ReBar.
*/
class VACA_DLL ReBarBand
{
  ReBar* m_rebar;
  int m_index;

public:
  ReBarBand(ReBar* parent, int index);
  ReBarBand(const ReBarBand& band);
  virtual ~ReBarBand();

  [[nodiscard]] ReBarBandStyle getStyle() const;
  void setStyle(ReBarBandStyle style);

  void getColors(Color& fg, Color& bg) const;
  void setColors(const Color& fg, const Color& bg);

  [[nodiscard]] String getText() const;
  void setText(const String& text);

  [[nodiscard]] int getImageIndex() const;
  void setImageIndex(int index);

  [[nodiscard]] Widget* getChild() const;
  void setChild(Widget* widget);

  inline void operator=(const ReBarBand& rbb) {
    m_rebar = rbb.m_rebar;
    m_index = rbb.m_index;
  }

protected:
  void setBand(REBARBANDINFO* rbbi);
  void getBand(REBARBANDINFO* rbbi) const;
};

/**
   Wrapper for ReBar Win32
*/
class VACA_DLL ReBar : public Widget
{
  ImageList m_imageList;

public:

  struct VACA_DLL Styles {
    static const Style Default;
  };

  ReBar(Widget* parent, const Style& style = ReBar::Styles::Default);
  ~ReBar() override;

  [[nodiscard]] bool isLayoutFree() const override;

  void setImageList(const ImageList& imageList);

  int getBandCount();
  int getBarHeight();

  ReBarBand addBand(Widget* item, ReBarBandStyle style = ReBarBandStyle::Simple, int position=-1);
  ReBarBand getBand(int index);
  void removeBand(int index);

  Color getBgColor();
  Color getFgColor();

  void setBgColor(const Color& color) override;
  void setFgColor(const Color& color) override;

  Rect getBandRect(int index);
  int getRowCount();
  int getRowHeight(int nRow);

  void showBand(int index, bool show);

  void maximizeBand(int index, bool ideal = false);
  void minimizeBand(int index);

public:
  // Signals
  Signal1<void, Event&> AutoSize;

protected:
  // Events
  void onPreferredSize(PreferredSizeEvent& ev) override;
  void onLayout(LayoutEvent& ev) override;
  bool onReflectedNotify(LPNMHDR lpnmhdr, LRESULT& lResult) override;
  virtual void onAutoSize(Event& ev);

};

} // namespace vaca
