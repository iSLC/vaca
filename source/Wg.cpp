// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/MainArgs.hpp"
#include "Wg/Application.hpp"

using namespace Wg;

#if defined(VACA_WINDOWS)
  #include "win32/MainArgs.hpp"
#else
  #include "std/MainArgs.h"
#endif
