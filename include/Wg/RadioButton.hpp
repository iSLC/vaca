// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/ButtonBase.hpp"

namespace Wg {

typedef std::vector<RadioButton *> RadioButtonList;

/**
   Groups a set of @link Wg::RadioButton RadioButtons@endlink. It's used to known
   which RadioButton should be desactived when the user select a
   RadioButton of its same group.
*/
class VACA_DLL RadioGroup : public NonCopyable {
    friend class RadioButton;

    /**
       Unique ID of the group.

       @internal You never use (get or set) this value.
    */
    unsigned int m_groupId;

    /**
       Members in the group.
    */
    RadioButtonList m_members;

public:
    RadioGroup();

    virtual ~RadioGroup();

    [[nodiscard]] int getSelectedIndex() const;

    void setSelectedIndex(int index);

    bool operator==(const RadioGroup &other) const;

    // Signals
    Signal1<void, Event &> Change; ///< @see onChange

protected:
    // New events
    virtual void onChange(Event &ev);

private:
    void addMember(RadioButton *newMember);

    void removeMember(RadioButton *currentMember);
};

/**
   A radio button.

   @image html RadioButton.png

   @warning
     Win32: It is not a BS_AUTORADIOBUTTON, it's only an
     BS_RADIOBUTTON (so you can't get @msdn{Q261192} bug).
*/
class VACA_DLL RadioButton : public ButtonBase {
    RadioGroup &m_radioGroup;

public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    RadioButton(const String &text, RadioGroup &group, Widget *parent,
                const Style &style = Styles::Default);

    ~RadioButton() override;

    [[nodiscard]] const RadioGroup &getRadioGroup() const;

protected:
    // Events
    void onClick(Event &ev) override;

    // Reflected notifications
    bool onReflectedCommand(int id, int code, LRESULT &lResult) override;
};

} // namespace Wg
