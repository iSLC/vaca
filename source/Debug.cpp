// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/Debug.hpp"
#include "Wg/Mutex.hpp"
#include "Wg/ScopedLock.hpp"
#include "Wg/System.hpp"
#include "Wg/Thread.hpp"

#include <cstdio>

using namespace std;
using namespace Wg;

#ifndef NDEBUG
struct Debug;

static bool closed;
static Debug* dbg;

struct Debug {
  Mutex mutex;
  FILE* file;
  Debug() {
    file = fopen("Wg.log", "w");
    fprintf(file, "Log file created\n");
  }
  virtual ~Debug() {
    fprintf(file, "Log file closed\n");
    fclose(file);
    file = NULL;
    closed = true;
  }
};
#endif

void Wg::details::trace(const char* filename, size_t line, const char* fmt, ...)
{
#ifndef NDEBUG
  if (closed) { return; }
  if (!dbg) { dbg = new Debug; }

  ScopedLock hold(dbg->mutex);
  char buf[1024];		// TODO: overflow
  va_list ap;

  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);

  fprintf(dbg->file, "%s:%d: [%d] %s", filename, line,
	  static_cast<unsigned>(::GetCurrentThreadId()), buf);
  fflush(dbg->file);
#endif
}

void Wg::details::closeLogFile()
{
#ifndef NDEBUG
  delete dbg;
  dbg = NULL;
#endif
}
