// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"

namespace Wg {

/**
   @defgroup signal_group Signal Classes
   @{
 */

namespace detail {

/// Primitive structure responsible for storing information about a signal slot.
struct SignalSlotPOD {
    uintptr_t mExec{0u}; ///< The functor that will forward the signal.
    uintptr_t mThis{0u}; ///< The instance to which the slot belongs.
    // Discard the current slot, if any.
    void Drop() { mExec = mThis = 0u; }
};

/// Forward declaration of the signal forwarder for invalid slots.
template < class > class SignalForwarder;

/// Specialization of the signal forwarder type for slots that return a value.
template < class R, class... A > struct SignalForwarder< R (A...) >
{
    /// Signature of the object that forwards the call.
    using Type  = R (*) (uintptr_t, A...);
    /// Generate an executer for a function handler.
    template < R (*F) (A...) > static Type Free() noexcept
    {
        return [](uintptr_t /* null */, A... a) -> R {
                return (*F)(std::forward< A >(a)...);
            };
    }
    /// Generate an executer for a member handler.
    template < class T, R (T::*M) (A...) > static Type Member() noexcept
    {
        return [](uintptr_t _, A... a) -> R {
                return (reinterpret_cast< T * >(_)->*M)(std::forward< A >(a)...);
            };
    }
    /// Generate an executer for a constant member handler.
    template < class T, R (T::*M) (A...) const > static Type Member() noexcept
    {
        return [](uintptr_t _, A... a) -> R {
                return (reinterpret_cast< const T * >(_)->*M)(std::forward< A >(a)...);
            };
    }
    /// Generate an executer for a lambda handler.
    template < class L > static Type Lambda() noexcept
    {
        return [](uintptr_t _, A... a) -> R {
                return (reinterpret_cast< L * >(_)->operator()(std::forward< A >(a)...));
            };
    }
};

/// Specialization of the signal forwarder type for slots that don't return a value.
template < class... A > struct SignalForwarder< void (A...) >
{
    /// Signature of the object that forwards the call.
    using Type  = void (*) (uintptr_t, A...);
    /// Generate a forwarder for a function handler.
    template < void (*F) (A...) > static Type Free() noexcept
    {
        return [](uintptr_t /* _ */, A... a) -> void {
                (*F)(std::forward< A >(a)...);
            };
    }
    /// Generate a forwarder for a member handler.
    template < class T, void (T::*M) (A...) > static Type Member() noexcept
    {
        return [](uintptr_t _, A... a) -> void {
                (reinterpret_cast< T * >(_)->*M)(std::forward< A >(a)...);
            };
    }
    /// Generate a forwarder for a constant member handler.
    template < class T, void (T::*M) (A...) const > static Type Member() noexcept
    {
        return [](uintptr_t _, A... a) -> void {
                (reinterpret_cast< const T * >(_)->*M)(std::forward< A >(a)...);
            };
    }
    /// Generate a forwarder for a lambda handler.
    template < class L > static Type Lambda() noexcept
    {
        return [](uintptr_t _, A... a) -> void {
                (reinterpret_cast< L * >(_)->operator()(std::forward< A >(a)...));
            };
    }
};

// Structure which stores and forwards invocations to a given signal slot.
template < typename R, typename... A > struct SignalSlotBase : public SignalSlotPOD {
    using Return    = R ; ///< Type of value that the slot is expected to return.
    using Signature = R (*) (A...); ///< Type of signature the slot is expected to have.
    using Fwd = SignalForwarder< R (A...) >; ///< Utility used to generate the signal forwarders.
    /// Create a slot from the given function and stored it.
    template < R (*F) (A...) > void Use() noexcept { Set(Fwd::template Free< F >(), nullptr); }
    /// Create a slot from the given method and stored it.
    template < class T, R (T::*M) (A...) > void Use(T * p) noexcept { Set(Fwd:: template Member< T, M >(), p); }
    /// Create a slot from the given method and stored it.
    template < class T, R (T::*M) (A...) > void Use(T & r) noexcept { Set(Fwd:: template Member< T, M >(), std::addressof(r)); }
    /// Create a slot from the given constant method and stored it.
    template < class T, R (T::*M) (A...) const > void Use(const T * p) noexcept { Set(Fwd:: template Member< T, M >(), p); }
    /// Create a slot from the given constant method and stored it.
    template < class T, R (T::*M) (A...) const > void Use(const T & r) noexcept { Set(Fwd:: template Member< T, M >(), std::addressof(r)); }
    /// Create a slot from the given lambda and stored it.
    template < class L > void Use(L * p) noexcept { Set(Fwd:: template Lambda< L >(), p); }
    /// Create a slot from the given lambda and stored it.
    template < class L > void Use(L & r) noexcept { Set(Fwd:: template Lambda< L >(), std::addressof(r)); }
protected:
    using SignalSlotPOD::mExec; // Import the executer in protected scope.
    using SignalSlotPOD::mThis; // Import the environment in protected scope.
    // Default constructor.
    SignalSlotBase() noexcept
        : SignalSlotPOD{} { }
    /// Construct with the specified executer and environment object.
    SignalSlotBase(typename Fwd::Type e, const void * t) noexcept
        : SignalSlotPOD{reinterpret_cast< uintptr_t >(e), reinterpret_cast< uintptr_t >(t)} { }
    // Copy constructor (defaulted).
    SignalSlotBase(const SignalSlotBase & o) noexcept = default;
    // Copy assignment operator (defaulted).
    SignalSlotBase & operator = (const SignalSlotBase & o) noexcept = default;
    /// Activate the specified executer and `this` instance.
    inline void Set(typename Fwd::Type e, const void * t) noexcept {
        mExec = reinterpret_cast< uintptr_t >(e);
        mThis = reinterpret_cast< uintptr_t >(t);
    }
};

/// Forward declaration of the signal slot type for invalid slots.
template < typename > class SignalSlot;

/// Helper structure which stores and forwards signals to a slot with a return value.
template < typename R, typename... A > struct SignalSlot< R (A...) > : public SignalSlotBase< R, A... > {
    using Fwd = typename SignalSlotBase< R, A... >::Fwd; // Import signal forwarder generator.
    using SignalSlotBase< R, A... >::SignalSlotBase; // Immport constructors.
    /// Forward the call to the assigned slot, if any.
    template < typename... P > R operator() (P &&... p) noexcept {
        if (mExec) return (*reinterpret_cast< typename Fwd::Type >(mExec))(mThis, std::forward< P >(p)...);
        return R(); // Have to return something!
    }
protected:
    using SignalSlotBase< R, A... >::mExec; // Import the executer in protected scope.
    using SignalSlotBase< R, A... >::mThis; // Import the environment in protected scope.
};

/// Helper structure which stores and forwards signals to a slot without a return value.
template < typename... A > struct SignalSlot< void (A...) > : public SignalSlotBase< void, A... > {
    using Fwd = typename SignalSlotBase< void, A... >::Fwd; // Import signal forwarder generator.
    using SignalSlotBase< void, A... >::SignalSlotBase; // Immport constructors.
    /// Forward the call to the assigned slot, if any.
    template < typename... P > void operator() (P &&... p) noexcept {
        if (mExec) (*reinterpret_cast< typename Fwd::Type >(mExec))(mThis, std::forward< P >(p)...);
    }
protected:
    using SignalSlotBase< void, A... >::mExec; // Import the executer in protected scope.
    using SignalSlotBase< void, A... >::mThis; // Import the environment in protected scope.
};

/// Base signal class which implements code that can be shared by the signal specializations.
struct VACA_DLL SignalBase {
    using Slot = SignalSlotPOD; ///< Basic type used to store a signal slot.
    /// Connect the specified slot to the signal.
    bool Connect(const Slot & slot) noexcept;
    /// Connect the specified slot but not before disconnecting all other occurrences.
    bool ConnectOnce(const Slot & slot) noexcept {
        Eliminate(slot); // Disconnect first
        return Connect(slot); // Connect again
    }
    /// Disconnect all occurrences of the specified slot from the signal.
    uint32_t Disconnect(const Slot & slot) noexcept { return Eliminate(slot); }
    // See if the specified slot is connected to the signal.
    [[nodiscard]] bool Exists(const Slot & slot) const noexcept;
    /// See if the specified slot instance is connected to the signal.
    [[nodiscard]] bool ExistsThis(const Slot & slot) const noexcept;
    /// See if the specified slot executor is connected to the signal.
    [[nodiscard]] bool ExistsExec(const Slot & slot) const noexcept;
    /// Count all occurrences of the selected slot.
    [[nodiscard]] uint32_t Count(const Slot & slot) const noexcept;
    /// Count all occurrences of the selected slot instance.
    [[nodiscard]] uint32_t CountThis(const Slot & slot) const noexcept;
    /// Count all occurrences of the selected slot executor.
    [[nodiscard]] uint32_t CountExec(const Slot & slot) const noexcept;
    /// Move all occurrences of the selected slot to the front.
    void Lead(const Slot & slot, bool one = false, bool append = true) noexcept;
    /// Move all occurrences of the selected slot instance to the front.
    void LeadThis(const Slot & slot, bool one = false, bool append = true) noexcept;
    /// Move all occurrences of the selected slot executor to the front.
    void LeadExec(const Slot & slot, bool one = false, bool append = true) noexcept;
    /// Move all occurrences of the selected slot to the back.
    void Tail(const Slot & slot, bool one = false, bool append = true) noexcept;
    /// Move all occurrences of the selected slot instance to the back.
    void TailThis(const Slot & slot, bool one = false, bool append = true) noexcept;
    /// Move all occurrences of the selected slot executor to the back.
    void TailExec(const Slot & slot, bool one = false, bool append = true) noexcept;
    // Remove all occurrences of the selected slot.
    uint32_t Eliminate(const Slot & slot) noexcept;
    // Remove all occurrences of the selected slot instance.
    uint32_t EliminateThis(const Slot & slot) noexcept;
    // Remove all occurrences of the selected slot executor.
    uint32_t EliminateExec(const Slot & slot) noexcept;
    /// Disconnect all connected handlers.
    void Clear() noexcept;
    /// See if there are any handlers connected.
    bool Empty() const noexcept { return (m_Connected == 0u); }
    /// Execution scope used to adjust iterators when removing slots or modifying the storage during emission.
    struct Scope {
        Slot *          mItr{}; ///< Currently executed slot.
        Slot *          mEnd{}; ///< Where the execution ends.
        Scope *         mParent{}; ///< Previous execution scope.
        Scope *         mChild{}; ///< Next execution scope.
        /// Base constructor.
        Scope(Scope * p, Slot * b, Slot * e) noexcept
            : mItr(b), mEnd(e), mParent(p), mChild(nullptr) {
            // If we have a parent then tell it we're its child
            if (mParent != nullptr) mParent->mChild = this;
        }
        /// Copy constructor (disabled).
        Scope(const Scope &) =  delete;
        /// Move constructor (disabled).
        Scope(Scope &&) =  delete;
        /// Destructor.
        ~Scope() {
            // If we have a parent then tell it we're no longer its child
            if (mParent != nullptr) mParent->mChild = nullptr;
        }
        /// Copy assignment operator (disabled).
        Scope & operator = (const Scope &) =  delete;
        /// Move assignment operator (disabled).
        Scope & operator = (Scope &&) =  delete;
        /// Adjust the iterators to account for the fact that the specified slot was removed.
        void Descend(Slot * ptr) noexcept;
        /// Adjust the iterators to account for the fact that the specified slot is now leading.
        void Lead(Slot * ptr) noexcept;
        /// Adjust the iterators to account for the fact that the specified slot is now tailing.
        void Tail(Slot * ptr) noexcept;
        /// Adjust the iterators to finish the execution abruptly.
        void Finish() noexcept;
    };
    /// Used to create, activate and deactivate execution scopes in a RAII fashion.
    struct ScopeGuard {
        Scope       mScope; ///< The managed scope.
        Scope **    mPointer; ///< Where to activate the scope.
        /// Base constructor.
        template < class... A > explicit ScopeGuard(Scope ** p, A &&... a) noexcept
            : mScope(std::forward< A >(a)...), mPointer(p) {
            // Activate the new scope
            *mPointer = &mScope;
        }
        /// Copy constructor (disabled).
        ScopeGuard(const ScopeGuard &) =  delete;
        /// Move constructor (disabled).
        ScopeGuard(ScopeGuard &&) =  delete;
        /// Destructor.
        ~ScopeGuard() {
            // Restore the previous scope
            *mPointer = mScope.mParent;
        }
        /// Copy assignment operator (disabled).
        ScopeGuard & operator = (const ScopeGuard &) =  delete;
        /// Move assignment operator (disabled).
        ScopeGuard & operator = (ScopeGuard &&) =  delete;
    };
    /// Default constructor.
    SignalBase() noexcept;
    /// Copy constructor (disabled).
    SignalBase(const SignalBase & o) = delete;
    /// Move constructor.
    SignalBase(SignalBase && o) noexcept;
    /// Destructor.
    ~SignalBase();
    /// Copy assignment operator (disabled).
    SignalBase & operator = (const SignalBase & o) = delete;
    /// Move assignment operator (disabled).
    SignalBase & operator = (SignalBase && o) = delete;
    /// Scale the internal slot storage size to satisfy the requested capacity.
    bool Adjust(uint32_t capacity) noexcept;
    /// Increase the internal slot storage size by at least the specified amount. Amount must be greater than 0!
    bool Expand(uint32_t ammount) noexcept;
protected:
    uint32_t   m_Connected{0}; /// Number of slots that are currently active and can be invoked.
    uint32_t   m_Capacity{0}; /// Number of stored slots that can be stored in the allocated memory buffer.
    Slot *     m_Slots{&m_Local}; /// Pointer to the allocated memory where the slots currently are stored.
    Scope *    m_Scope{nullptr}; /// Pointer to the current execution state.
    Slot       m_Local{}; /// Local buffer optimization knowing most signals will have one slot connected.
};

} // Namespace:: detail

// ====================================================================================================================
///
template < typename T > using Callback = detail::SignalSlot< T >;

// ====================================================================================================================
/// Forward declaration of the signal type for invalid signals.
template < typename > class Signal;

// ====================================================================================================================
/// Specialization of the signal type for valid signals.
template < class R, class... A > struct Signal< R (A...) > : public detail::SignalBase {
    // ----------------------------------------------------------------------------------------------------------------
    using Return    = R ; ///< Type of value that the slot is expected to return.
    using Signature = R (*) (A...); ///< Type of signature the slot is expected to have.
    // ----------------------------------------------------------------------------------------------------------------
        using Fwd = detail::SignalForwarder< R (A...) >; ///< Utility used to generate the signal forwarders.
    // ----------------------------------------------------------------------------------------------------------------
    /// Emit the signal to the connected slots.
    template < class... P > void Emit(P &&... p) {
        // Are there any slots connected to this signal?
        if (m_Connected == 0) return; // No one to hear the news
        // Enter a new execution scope to avoid locking signal modifications
        ScopeGuard sg(&m_Scope, m_Scope, m_Slots, m_Slots + m_Connected);
        // Begin processing the slots from the current scope
        while (sg.mScope.mItr != sg.mScope.mEnd) {
            // Grab a reference to the current slot
            const Slot & slot = *(sg.mScope.mItr++);
            // Use the stored executer to forward the call to the designated slot
            (*reinterpret_cast< typename Fwd::Type >(slot.mExec))(slot.mThis, std::forward< P >(p)...);
        }
    }
protected:
    /// Base constructor.
    Signal() noexcept
        : SignalBase() { }
};

// --------------------------------------------------------------------------------------------------------------------
/// Helper class that extracts the return and argument types from the specified signature.
template < typename S, int I = 0 > struct UniqueSignal : public Signal< S > {
    // ----------------------------------------------------------------------------------------------------------------
    /// Default constructor.
    UniqueSignal() noexcept
        : Signal< S >()
    { }
protected:

};

/** @} */

} // namespace Wg
