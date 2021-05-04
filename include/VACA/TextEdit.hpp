// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Widget.hpp"

namespace vaca {

/**
   Widget to edit text.

   @see @ref page_tn_008
*/
class VACA_DLL TextEdit : public Widget {
public:

    struct VACA_DLL Styles {
        static const Style Default;
        static const Style Password;
        static const Style TextArea;
        static const Style RightAligned;
        static const Style ReadOnly;
        static const Style AutoHorizontalScroll;
        static const Style AutoVerticalScroll;
    };

    TextEdit(const String &text, Widget *parent, const Style &style = Styles::Default);

    ~TextEdit() override;

    // ============================================================
    // ANY TEXT EDIT
    // ============================================================

    [[nodiscard]] int getTextLength() const;

    [[nodiscard]] int getTextLimit() const;

    void setTextLimit(int textLimit);

    [[nodiscard]] bool isReadOnly() const;

    void setReadOnly(bool readOnly);

    [[nodiscard]] bool canUndo() const;

    void undo();

    void cut();

    void copy();

    void paste();

    void selectAll();

    void selectRange(int start, int end);

    void deselect();

    void getSelection(int &start, int &end);

    // ============================================================
    // PASSWORD
    // ============================================================

    Char getPasswordChar();

    void setPasswordChar(Char passChar);

    // ============================================================
    // TEXT AREA
    // ============================================================

    bool getWantReturnMode();

    void setWantReturnMode(bool wantReturn);

    int getLineCount();

    String getLine(int lineNo);

    int getLineLength(int lineNo);

    void scrollLines(int lines);

    // ============================================================
    // SIGNALS
    // ============================================================

    // Signals
    Signal1<void, Event &> Change; ///< @see onChange

protected:
    // Events
    void onPreferredSize(PreferredSizeEvent &ev) override;

    // New events
    virtual void onChange(Event &ev);

    // Reflected notifications
    bool onReflectedCommand(int id, int code, LRESULT &lResult) override;

};

} // namespace vaca
