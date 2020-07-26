#include "../interface/MTPU.h"

using namespace TopTree;

MTPU::MTPU ()
{
}

MTPU::~MTPU ()
{
}

void
MTPU::Reset ()
{
  NTPU::Reset ();
}

void MTPU::DumpExtended(std::ostream & os){
        Dump(os);
}

