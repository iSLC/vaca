// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Exception.hpp"

namespace vaca {

/**
   A resource (from exe or from an external file) can't be loaded.
*/
class ResourceException : public Exception {
public:

    ResourceException() : Exception() {}

    ResourceException(const String &message) : Exception(message) {}

    ~ResourceException() noexcept override = default;

};

} // namespace vaca
