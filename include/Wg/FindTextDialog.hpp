// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Dialog.hpp"
#include "Wg/Signal.hpp"

namespace Wg {

/**
   A common dialog box to find (or replace) text.
*/
class VACA_DLL FindTextDialog : public Dialog {
    static UINT m_findMsgId;

    FINDREPLACE m_findReplace{};
    bool m_replace;

public:

    FindTextDialog(bool replace, Widget *parent);

    ~FindTextDialog() override;

    String getFindWhat();

    String getReplaceWith();

    void setFindWhat(const String &s);

    void setReplaceWith(const String &s);

    bool isWholeWord();

    bool isMatchCase();

    bool isBackward();

    bool isForward();

    // Signals
    Signal1<void, Event &> FindNext;
    Signal1<void, Event &> Replace;
    Signal1<void, Event &> ReplaceAll;

protected:

    // New events
    virtual void onFindNext(Event &ev);

    virtual void onReplace(Event &ev);

    virtual void onReplaceAll(Event &ev);

private:

    void getTextFromDlgItem(int id, String &s);

    void setTextFromDlgItem(int id, const String &s);

    HWND createHandle(LPCTSTR className, Widget *parent, Style style) override;

    static UINT_PTR CALLBACK hookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);

};

} // namespace Wg
