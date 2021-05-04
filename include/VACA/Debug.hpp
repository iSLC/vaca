// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"

#include <cassert>

namespace Wg {

#ifdef NDEBUG
#  ifdef __GNUC__
#    define VACA_TRACE(msg...)
#  else
#    define VACA_TRACE(...)
#  endif
#else
#  ifdef __GNUC__
#    define VACA_TRACE(msg...) Wg::details::trace(__FILE__, __LINE__, msg)
#  else
#    define VACA_TRACE         Wg::details::make_trace(__FILE__, __LINE__)
#  endif
#endif

namespace details {

void VACA_DLL trace(const char *filename, size_t line, const char *msg, ...);

void VACA_DLL closeLogFile();

#ifndef __GNUC__
/**
   @internal
      Dirty trick for compilers that does not support
      macros with ellipsis (...).
*/
struct trace_t {
  const char* filename;
  size_t line;
  void operator()(const char* fmt, ...) {
    char buf[1024];		// TODO: overflow
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);
    Wg::details::trace(filename, line, buf);
  }
};
inline trace_t make_trace(const char* filename, size_t line) {
  trace_t tmp;
  tmp.filename = filename;
  tmp.line = line;
  return tmp;
}
#endif

} // namespace details

} // namespace Wg
