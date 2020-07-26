#include "../interface/NTMET.h"
using namespace TopTree;


NTMET::NTMET ():
    p4(0.0, 0.0, 0.0, 0.0),
    correction(0.),
    turnOnWeight(-999.)
{
}

NTMET::~NTMET ()
{
}

void
NTMET::Reset ()
{
  p4.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  correction = 0.;
  turnOnWeight = -999.;
}

void NTMET::Dump(std::ostream & os){
        os << "Object: MET \n";
        PrintP4(p4, os);
        os << " correction = " << correction << " ";
        os << " turnOnWeight =  "<< turnOnWeight <<" ";
        os << endl;
}

