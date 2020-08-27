#pragma once

class MMap;

class Arch final {
public:
  void Initialize(MMap &mmap);
  [[nodiscard]] inline const char *GetArchitecture() const { return m_archStr; }

private:
  const char *m_archStr = _ARCH_STR;
};
