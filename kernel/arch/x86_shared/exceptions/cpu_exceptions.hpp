#pragma once
#include "vga.hpp"
#include <exception.hpp>

__attribute__((interrupt)) void Exception_DivByZero(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Divison by zero!"
         << "\n";
  DumpRegisters(screen, frame);
  Panic(screen, "Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_Debug(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Debug interrupt!\n";
  DumpRegisters(screen, frame);
}

__attribute__((interrupt)) void Exception_Breakpoint(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Breakpoint triggered!\n";
  DumpRegisters(screen, frame);
}

__attribute__((interrupt)) void Exception_Overflow(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Trap: Overflow!\n";
  DumpRegisters(screen, frame);
}

__attribute__((interrupt)) void Exception_OutOfBounds(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Bound range exceeded!"
         << "\n";
  DumpRegisters(screen, frame);
  Panic(screen, "Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_InvalidOp(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Invalid opcode!"
         << "\n";
  DumpRegisters(screen, frame);
  Panic(screen, "Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_DeviceNA(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Device not available!"
         << "\n";
  DumpRegisters(screen, frame);
  Panic(screen, "Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_DoubleFault(InterruptFrame *frame,
                                                      uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Double Fault! Code: " << static_cast<unsigned int>(err) << "\n";
  DumpRegisters(screen, frame);
  Panic(screen, "Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_InvalidTSS(InterruptFrame *frame,
                                                     uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Invalid TSS! Code: " << static_cast<unsigned int>(err)
         << "\n";
  DumpRegisters(screen, frame);
  Panic(screen, "Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_SegNotPres(InterruptFrame *frame,
                                                     uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Present bit for segment not set! Code: "
         << static_cast<unsigned int>(err) << "\n";
  DumpRegisters(screen, frame);
  Panic(screen, "Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_StackSeg(InterruptFrame *frame,
                                                   uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Stack segment fault! Code: "
         << static_cast<unsigned int>(err) << "\n";
  DumpRegisters(screen, frame);
  Panic(screen, "Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_GenProtFault(InterruptFrame *frame,
                                                       uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Fault: General protection fault! Code: "
         << static_cast<unsigned int>(err) << "\n";
  DumpRegisters(screen, frame);
  Panic(screen, "Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_PageFault(InterruptFrame *frame,
                                                    uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Page fault! Code: " << static_cast<unsigned int>(err)
         << "\n";
  DumpRegisters(screen, frame);
  Panic(screen, "Cannot continue after fatal error!");
}