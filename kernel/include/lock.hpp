#pragma once
#include <stdatomic.h>

class ILock {
  virtual void Lock() = 0;
  virtual bool TryLock() = 0;
  virtual void Unlock() = 0;
};

class TicketLock final : ILock {
public:
  void Lock() override;
  bool TryLock() override;
  void Unlock() override;

private:
  atomic_size_t m_nowServing;
  atomic_size_t m_nextTicket;
};

class SpinLock final : ILock {
public:
  void Lock() override;
  bool TryLock() override;
  void Unlock() override;

private:
  int m_mutex;
};