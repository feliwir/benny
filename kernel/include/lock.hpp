#pragma once
#include <stdatomic.h>

class TicketLock final {
public:
  void Lock();
  bool TryLock();
  void Unlock();

private:
  atomic_size_t m_nowServing;
  atomic_size_t m_nextTicket;
};