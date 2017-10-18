#pragma once
#include "vga.hpp"
#include <string.hpp>
//some util funcs we need for exceptions

void DumpRegisters(Vga& screen,InterruptFrame* frame)
{
  char buffer[32];
  buffer[0]='0';
  buffer[1]='x';

  screen << "REGISTERS:\n";
  itoa(frame->ip,buffer+2,16);
  screen << "IP:" << buffer << "\n";
  itoa(frame->cs,buffer+2,16);
  screen << "CS:" << buffer << "\n";
  itoa(frame->flags,buffer+2,16);
  screen << "FL:" << buffer << "\n";
  itoa(frame->sp,buffer+2,16);
  screen << "SP:" << buffer << "\n";
  itoa(frame->ss,buffer+2,16);
  screen << "SS:" << buffer << "\n";
}

void Panic(Vga& screen,const char* message)
{
  screen << message;
  asm volatile("hlt");
}