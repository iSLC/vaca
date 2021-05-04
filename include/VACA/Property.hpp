// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Referenceable.hpp"

namespace vaca {

class VACA_DLL Property : public Referenceable {
    String m_name;

public:
    Property(const String &name);

    ~Property() override;

    [[nodiscard]] String getName() const;
};

} // namespace vaca
