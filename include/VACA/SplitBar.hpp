// Vaca - Visual Application Components Abstraction
// Copyright (c) 2008, 2009, Jie Zhang, David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Point.hpp"
#include "VACA/Widget.hpp"
#include "VACA/Register.hpp"
#include "VACA/WidgetClass.hpp"

namespace vaca {

/**
   Represents the Win32 class used by SplitBar.
*/
class SplitBarClass : public WidgetClass {
public:
    static WidgetClassName getClassName() { return WidgetClassName(L"Vaca.SplitBar"); }
};

class VACA_DLL SplitBar : public Register<SplitBarClass>, public Widget {
    Orientation m_orientation;
    Widget *m_pane1;
    Widget *m_pane2;
    int m_barSize;
    double m_barPos;
    double m_oldBarPos{};
    Point m_oldPoint;
    bool m_fullDrag;
    bool m_trackerIsVisible;
    bool m_gripperVisible;

public:

    struct VACA_DLL Styles {
        static const Style Default;
        static const Style ByPixels;
    };

    SplitBar(Orientation orientation, Widget *parent, const Style &style = Styles::Default);

    ~SplitBar() override;

    void setPane1(Widget *widget);

    void setPane2(Widget *widget);

    [[nodiscard]] Widget *getPane1() const;

    [[nodiscard]] Widget *getPane2() const;

    [[nodiscard]] int getBarSize() const;

    void setBarSize(int size);

    [[nodiscard]] double getBarPosition() const;

    void setBarPosition(double pos);

    [[nodiscard]] Orientation getOrientation() const;

    void setOrientation(Orientation orientation);

    void setFullDrag(bool state);

    [[nodiscard]] bool isFullDrag() const;

    void setGripperVisible(bool state);

    [[nodiscard]] bool isGripperVisible() const;

protected:
    // Events
    void onLayout(LayoutEvent &ev) override;

    void onResize(ResizeEvent &ev) override;

    void onPaint(PaintEvent &ev) override;

    void onMouseMove(MouseEvent &ev) override;

    void onMouseDown(MouseEvent &ev) override;

    void onMouseUp(MouseEvent &ev) override;

    void onSetCursor(SetCursorEvent &ev) override;

private:
    void updateChildrenVisibility();

    [[nodiscard]] Rect getBarRect() const;

    void getRects(Rect &rcFirst, Rect &rcSecond) const;

    void drawTracker(Graphics &g);

    void cleanTracker(Graphics &g);
};

} // namespace vaca
