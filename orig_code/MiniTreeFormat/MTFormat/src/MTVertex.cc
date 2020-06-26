#include "../interface/MTVertex.h"

using namespace TopTree;

MTVertex::MTVertex ()
{
}

MTVertex::~MTVertex ()
{
}

void
MTVertex::Reset ()
{
  p3Err.SetXYZ (0.0, 0.0, 0.0);

}

void MTVertex::DumpExtended(std::ostream & os){
        Dump(os);
}
