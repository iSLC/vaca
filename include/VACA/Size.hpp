// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"

namespace vaca {

/**
   A 2D size.
*/
class VACA_DLL Size
{
public:

  int w, h;

  Size();
  Size(int w, int h);
  Size(const Size& size);
  explicit Size(const Point& point);

  [[nodiscard]] Size createUnion(const Size& sz) const;
  [[nodiscard]] Size createIntersect(const Size& sz) const;

  Size& operator=(const Size& sz);
  const Size& operator+=(const Size& sz);
  const Size& operator-=(const Size& sz);
  const Size& operator+=(int value);
  const Size& operator-=(int value);
  const Size& operator*=(int value);
  const Size& operator/=(int value);
  Size operator+(const Size& sz) const;
  Size operator-(const Size& sz) const;
  Size operator+(int value) const;
  Size operator-(int value) const;
  Size operator*(int value) const;
  Size operator/(int value) const;
  Size operator-() const;

  bool operator==(const Size& sz) const;
  bool operator!=(const Size& sz) const;

};

} // namespace vaca
