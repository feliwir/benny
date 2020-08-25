#include "lock.hpp"

void TicketLock::Lock() {
  size_t ticket =
      atomic_fetch_add_explicit(&m_nextTicket, 1, memory_order_relaxed);
  while (atomic_load_explicit(&m_nowServing, memory_order_acquire) != ticket) {
    asm volatile("pause");
  }
}

bool TicketLock::TryLock() {
  size_t ticket = atomic_load_explicit(&m_nowServing, memory_order_relaxed);
  return atomic_compare_exchange_strong_explicit(
      &m_nowServing, &ticket, ticket + 1, memory_order_relaxed,
      memory_order_acquire);
}

void TicketLock::Unlock() {
  size_t successor =
      atomic_load_explicit(&m_nowServing, memory_order_relaxed) + 1;
  atomic_store_explicit(&m_nowServing, successor, memory_order_release);
}