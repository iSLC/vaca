// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Constraint.hpp"

namespace vaca {

/**
   Defines a constraint for the BoxLayout. With BoxLayout a Widget can
   be expansive or not. An expansive widget tries to ocuppy more space
   what Widget::getPreferredSize() returns (generally widgets like
   ListBox or Edit should be expansive).

   If you don't setup an BoxConstraint for a widget controlled by a
   BoxLayout, the widget'll be known as non-expansive.
*/
class VACA_DLL BoxConstraint : public Constraint {
    bool m_expansive;

public:

    BoxConstraint(bool expansive);

    ~BoxConstraint() override;

    [[nodiscard]] bool isExpansive() const;

    void setExpansive(bool expansive);

};

} // namespace vaca
