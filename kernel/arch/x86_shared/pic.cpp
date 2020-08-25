#include "pic.hpp"
#include "idt.hpp"
#include "interrupts.hpp"
#include "io.hpp"
#include "vga.hpp"

/// Command sent to begin PIC initialization.
constexpr uint8_t CMD_INIT = 0x11;

/// Command sent to acknowledge an interrupt.
constexpr uint8_t CMD_END_OF_INTERRUPT = 0x20;

// The mode in which we want to run our PICs.
constexpr uint8_t MODE_8086 = 0x01;

PicChip PIC::s_chips[2] = {
    {PIC_1_COMMAND, PIC_1_DATA, PIC_1_OFFSET, 4},
    {PIC_2_COMMAND, PIC_2_DATA, PIC_2_OFFSET, 2},
};

void PicChip::EndOfInterrupt() const {
  outb(commandPort, CMD_END_OF_INTERRUPT);
}

void PIC::Initialize() {
  // Write garbage data to the wait port
  auto wait = []() { outb(WAIT_PORT, 0); };

  for (const auto &pic : s_chips) {
    uint8_t savedMask = inb(pic.dataPort);

    // Tell PIC that we're initializing
    outb(pic.commandPort, CMD_INIT);
    wait();

    // Setup the PIC offsets
    outb(pic.dataPort, pic.offset);
    wait();

    // Configure chaining
    outb(pic.dataPort, pic.offset);
    wait();

    // Setup mode
    outb(pic.dataPort, MODE_8086);
    wait();

    outb(pic.dataPort, savedMask);
  }

  enable_interrupts();
}