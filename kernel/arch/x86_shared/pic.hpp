#pragma once
#include <stdint.h>

constexpr uint8_t PIC_1_OFFSET = 32;
constexpr uint8_t PIC_1_COMMAND = 0x20;
constexpr uint8_t PIC_1_DATA = 0x21;

constexpr uint8_t PIC_2_OFFSET = PIC_1_OFFSET + 8;
constexpr uint8_t PIC_2_COMMAND = 0xA0;
constexpr uint8_t PIC_2_DATA = 0xA1;

enum class HardwareInterrupts {
  Timer = PIC_1_OFFSET,
};

struct PicChip {
  const uint8_t commandPort;
  const uint8_t dataPort;
  const uint8_t offset;
  const uint8_t chaining;

  inline bool HandlesInterupt(uint8_t id) const {
    return offset <= id && id < offset + 8;
  }

  void EndOfInterrupt() const;
};

class PIC {
public:
  static void Initialize();

  static inline bool HandlesInterrupt(uint8_t id) {
    for (const auto &pic : s_chips) {
      if (pic.HandlesInterupt(id))
        return true;
    }
    return false;
  }

  static inline void NotifyEndOfInterrupt(uint8_t id) {
    for (const auto &pic : s_chips) {
      if (pic.HandlesInterupt(id))
        pic.EndOfInterrupt();
    }
  }

private:
  static constexpr uint16_t WAIT_PORT = 0x80;
  static PicChip s_chips[2];
};