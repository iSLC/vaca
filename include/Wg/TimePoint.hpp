// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"

namespace Wg {

/**
   Class to measure elapsed time, like a chronometer.
*/
class VACA_DLL TimePoint {
    LARGE_INTEGER m_point{};
    LARGE_INTEGER m_freq{};

public:
    TimePoint();

    ~TimePoint();

    void reset();

    [[nodiscard]] double elapsed() const;
};

} // namespace Wg
