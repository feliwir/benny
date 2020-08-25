#pragma once

class Vga;

class Arch final {
public:
  void Initialize();
  [[nodiscard]] inline const char *GetArchitecture() const { return m_archStr; }

private:
  const char *m_archStr = _ARCH_STR;
};
