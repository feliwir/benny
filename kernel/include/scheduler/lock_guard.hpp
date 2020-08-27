#pragma once
#include "lock.hpp"

template <class M> class LockGuard {
public:
  inline LockGuard(M &mutex) : m_mutex(mutex) { m_mutex.Lock(); }

  inline ~LockGuard() { m_mutex.Unlock(); }

private:
  M &m_mutex;
};

using TicketLockGuard = LockGuard<TicketLock>();
using SpinLockGuard = LockGuard<SpinLock>();