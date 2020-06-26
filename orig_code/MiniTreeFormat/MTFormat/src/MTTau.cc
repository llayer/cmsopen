#include "../interface/MTTau.h"

using namespace TopTree;

MTTau::MTTau ()
{
}

MTTau::~MTTau ()
{
}

void
MTTau::Reset ()
{
  NTTau::Reset ();
  addInfo.Reset();
}

void MTTau::DumpExtended(std::ostream & os){
        Dump(os);
        addInfo.PrintAdditionalInfo(os);
}
