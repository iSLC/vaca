// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include <windows.h>

void Wg::details::MainArgs::setArgs(int, char**)
{
  // Convert the command-line to a vector of arguments using Win32 API
  std::vector<String> args;
  LPWSTR* arglist;
  int argc;

  arglist = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
  if (arglist != NULL) {
    args.reserve(static_cast<unsigned long long int>(argc));
    for (int i=0; i<argc; ++i)
      args.push_back(arglist[i]);

    ::LocalFree(arglist);
  }

  Application::setArgs(args);
}
