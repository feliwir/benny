#include "paging.hpp"
#include "pmm.hpp"

void Paging::Initialize() 
{
     // Place paging data after pmm bitmap
    tempMem = PMM::bitmap + PMM::bitmapSize;
}