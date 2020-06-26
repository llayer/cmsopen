#include "..//interface/NTSampleInfo.h"

using namespace TopTree;
using namespace std;

NTSampleInfo::NTSampleInfo ()
{
  luminosity = 0.;
  cross_section = 0.;
  TotalEvents = 0;
  Name = "";

}

NTSampleInfo::~NTSampleInfo ()
{
}


void
NTSampleInfo::Print (std::ostream & os) const
{
  os << "Sample called " << Name << " - Cross-section = " << cross_section << " - #events = " << TotalEvents << " luminosity " << luminosity << endl;
}

