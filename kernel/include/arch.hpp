#pragma once

class Arch {
public:
  void Initialize();
  inline const char *GetArchitecture() { return m_archStr; }

private:
  const char *m_archStr = _ARCH_STR;
};
