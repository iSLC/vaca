// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/Signal2.hpp"

namespace Wg::detail
{

/// Helper functor to locate specific slots.
template < class Slot > struct FullMatch {
    const uintptr_t mExec; ///< The executer to match with.
    const uintptr_t mThis; ///< The `this` instance to match with.
    /// Construct with a specific executer and environment.
    FullMatch(const uintptr_t e, const uintptr_t t) noexcept
        : mExec(e), mThis(t) { }
    /// Compare against another instance for matching member values.
    inline bool operator () (const Slot & s) const noexcept { return (mExec == s.mExec) && (mThis == s.mThis); }
};

/// Helper functor to locate slots with specific `this` instance.
template < class Slot > struct ThisMatch {
    const uintptr_t mThis; ///< The `this` instance to match with.
    /// Construct with a specific environment.
    explicit ThisMatch(const uintptr_t t) noexcept
        : mThis(t) { }
    /// Compare against another instance for matching member values.
    inline bool operator () (const Slot & s) const noexcept { return (mThis == s.mThis); }
};

/// Helper functor to locate slots with specific executors.
template < class Slot > struct ExecMatch {
    const uintptr_t mExec; ///< The executer to match with.
    /// Construct with a specific executer.
    explicit ExecMatch(const uintptr_t e) noexcept
        : mExec(e) { }
    /// Compare against another instance for matching member values.
    inline bool operator () (const Slot & s) const noexcept { return (mExec == s.mExec); }
};


/// See if a certain slot exists within a specific range using the provided functor.
template < typename F, class Slot > static bool ExistsIf(F func, Slot * itr, Slot * end) noexcept {
    while (itr != end) {
    	if (func(*(itr++)))
    		return true;
    }
    return false;
}


/// Count the number of slots that the provided functor deems worthy.
template < typename F, class Slot > static uint32_t CountIf(F func, Slot * itr, Slot * end) noexcept {
    uint32_t count = 0;
    while (itr != end) {
        if (func(*(itr++)))
        	++count;
    }
    return count;
}


/// Remove all slot from the specified range if the provided functor demands it.
template < typename F, class Slot, class Scope >
static uint32_t RemoveIf(F func, Slot * itr, Slot * end, Scope * scope) noexcept {
    uint32_t count = 0;
    for (Slot * dest = itr; itr != end; ++itr) {
        if (func(*itr)) {
            itr->mExec = 0u;
            itr->mThis = 0u;
            ++count;
            if (scope != 0u)
                scope->Descend(itr);
        } else {
            dest->mExec = itr->mExec;
            dest->mThis = itr->mThis;
            ++dest;
        }
    }
    return count;
}


/// Move to the front all slots that the provided functor demands.
template < typename F, class Slot, class Scope >
static void LeadIf(F func, Slot * front, Slot * end, bool one, bool append, Scope * scope) noexcept {
    Slot tmp;
    Slot * itr = front;
    while (itr != end) {
        if (!func(*itr)) {
            ++itr;
            continue;
        } else if ((itr - front) == 1) {
            tmp.mExec = itr->mExec;
            tmp.mThis = itr->mThis;
            itr->mExec = front->mExec;
            itr->mThis = front->mThis;
            front->mExec = tmp.mExec;
            front->mThis = tmp.mThis;
            if (scope != nullptr)
            {
                scope->Lead(itr);
            }
        } else if (itr != front) {
            tmp.mExec = itr->mExec;
            tmp.mThis = itr->mThis;
            for (Slot * dest = itr, * src = (itr - 1); dest != front; dest = src, --src) {
                dest->mExec = src->mExec;
                dest->mThis = src->mThis;
            }
            front->mExec = tmp.mExec;
            front->mThis = tmp.mThis;
            if (scope != nullptr) {
                scope->Lead(itr);
            }
        }
        ++itr;
        if (append) {
            ++front;
        }
        if (one) {
            break;
        }
    }
}


/// Move to the back all slots that the provided functor demands.
template < typename F, class Slot, class Scope >
static void TailIf(F func, Slot * front, Slot * back, bool one, bool append, Scope * scope) noexcept {
    Slot tmp;
    Slot * itr = back;
    while (itr >= front) {
        if (!func(*itr)) {
            --itr;
            continue;
        } else if ((back - itr) == 1) {
            tmp.mExec = itr->mExec;
            tmp.mThis = itr->mThis;
            itr->mExec = back->mExec;
            itr->mThis = back->mThis;
            back->mExec = tmp.mExec;
            back->mThis = tmp.mThis;
            if (scope != nullptr) {
                scope->Tail(itr);
            }
        } else if (itr != back) {
            tmp.mExec = itr->mExec;
            tmp.mThis = itr->mThis;
            for (Slot * dest = itr, * src = (itr + 1); dest != back; dest = src, ++src) {
                dest->mExec = src->mExec;
                dest->mThis = src->mThis;
            }
            back->mExec = tmp.mExec;
            back->mThis = tmp.mThis;
            if (scope != nullptr) {
                scope->Tail(itr);
            }
        }
        --itr;
        if (append) {
            --back;
        }
        if (one) {
            break;
        }
    }
}

SignalBase::SignalBase() noexcept
	: m_Connected{0u}, m_Capacity{1u}, m_Slots{&m_Local}, m_Scope{nullptr}, m_Local{} { }

SignalBase::SignalBase(SignalBase && o) noexcept = default;

SignalBase::~SignalBase() { if (m_Slots != &m_Local) delete[] m_Slots; }

bool SignalBase::Adjust(uint32_t capacity) noexcept {
    return capacity <= m_Capacity || Expand(capacity - m_Capacity);
}

bool SignalBase::Expand(uint32_t ammount) noexcept {
    // Specify the minimum required capacity
    ammount += m_Capacity;
    // Use current capacity as the base
    uint32_t size = m_Capacity;
    // Compute the next optimal capacity which satisfies the requirements
    while (size < ammount) size += (size + 1) >> 1;
    // Attempt to allocate a memory block of the resulted capacity 
    Slot * slots = new Slot[size];
    // Should we transfer anything from the current memory buffer?
    if (m_Connected) memcpy(slots, m_Slots, m_Connected * sizeof(Slot));
    // Release the current memory buffer if not local 
    if (m_Slots != &m_Local) delete[] m_Slots;
    //  Update the iterators from a chain of scopes to point to the same positions but from a new slot memory buffer.
    for (Scope * scope = m_Scope; scope != nullptr; scope = scope->mChild) {
        scope->mItr = slots + reinterpret_cast< intptr_t >(scope->mItr - m_Slots);
        scope->mEnd = slots + reinterpret_cast< intptr_t >(scope->mEnd - m_Slots);
    }
    // Take ownership of the new memory buffer and capacity
    m_Capacity = size;
    m_Slots = slots;
    // The memory buffer was successfully adjusted
    return true;
}

} // Namespace:: Wg::detail
