// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"

namespace vaca {

/**
   Auxiliary structure to get and set scroll information.

   Scroll information includes: the valid range where the scroll
   position can be, and the page size.

   @see Widget#getScrollInfo, Widget#getScrollPos
*/
class VACA_DLL ScrollInfo {
    int m_minPos;
    int m_maxPos;
    int m_pageSize;

public:
    ScrollInfo();

    ScrollInfo(int minPos, int maxPos, int pageSize);

    virtual ~ScrollInfo();

    [[nodiscard]] int getMinPos() const;

    [[nodiscard]] int getMaxPos() const;

    [[nodiscard]] int getPageSize() const;

    void setMinPos(int minPos);

    void setMaxPos(int maxPos);

    void setPageSize(int pageSize);

};

} // namespace vaca
