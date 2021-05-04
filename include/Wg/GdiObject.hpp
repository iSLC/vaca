// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Referenceable.hpp"

#include <cassert>

namespace Wg {

/**
   Helper for GdiObject class to destroy Win32's HGDIOBJ handles.

   @internal
*/
struct Win32DestroyGdiObject {
    /**
       Destroys the GDI handle calling the Win32's DeleteObject function.
    */
    static void destroy(HGDIOBJ handle) {
        ::DeleteObject(handle);
    }
};

/**
   This class is a wrapper for Win32's GDI objects.

   @internal

   @tparam T
     Type of handle (e.g. HCURSOR, HBRUSH, etc.).

   @tparam Destroyer
     A class with a static member function @c destroy to be used
     in the destructor of GdiObject (to free the @a T handle).
*/
template<typename T, class Destroyer = Win32DestroyGdiObject>
class GdiObject : public Referenceable {
    T m_handle;

public:

    /**
       Creates a new GdiObject with the handle pointing to NULL.
    */
    GdiObject() : m_handle(NULL) {}

    /**
       Creates a wrapper for the specified @a handle.

       @param handle
         Handle to be wrapped. It will be delete in the
         GdiObject destructor.
    */
    GdiObject(T handle) : m_handle(handle) {}

    /**
       Destroys the handle using the @a Destroyer template-parameter.
    */
    ~GdiObject() override {
        if (isValid())
            Destroyer::destroy(m_handle);
    }

    /**
       Returns true if the handle is valid (not NULL).
    */
    [[nodiscard]] bool isValid() const {
        return m_handle != NULL;
    }

    /**
       Returns the handle.
    */
    T getHandle() const {
        return m_handle;
    }

    /**
       Sets the handle.

       @warning
         The current handle (#getHandle) must be NULL. This means
         that you can use this member function just one time.

       @param handle
         The handle to be wrapped. It will be delete in the
         GdiObject destructor.
    */
    void setHandle(T handle) {
        assert(!m_handle);
        m_handle = handle;
    }

};

} // namespace Wg
