#include <vga.hpp>



extern "C" /* Use C linkage for kernel_early. */
void kernel_early(void)
{

}

extern "C" /* Use C linkage for kernel_main. */
void kernel_main(void) 
{
    Vga terminal;
}
