// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"

namespace vaca {

/**
   Class which can't be copied.

   If you derive from this class, then you will not be able to make
   copies of your own class. This means that the class will be
   copied neither using the copy constructor nor the assigment operator.

   Example:
   @code
   class MyClass : private NonCopyable
   {
   public:
     MyClass() {
     }
   };
   int main()
   {
     MyClass a, b;
     MyClass c(a);   // <-- compiler error
     b = a;          // <-- compiler error
   }
   @endcode
*/
class VACA_DLL NonCopyable
{
public:
  NonCopyable() { }
  ~NonCopyable() { }
private:
  NonCopyable(const NonCopyable&);
  NonCopyable& operator=(const NonCopyable&);
};

} // namespace vaca
