#pragma once

class Vga;
struct multiboot_tag;

class Arch final {
public:
  void Initialize(multiboot_tag* bootInfo);
  [[nodiscard]] inline const char *GetArchitecture() const { return m_archStr; }

private:
  const char *m_archStr = _ARCH_STR;
};
