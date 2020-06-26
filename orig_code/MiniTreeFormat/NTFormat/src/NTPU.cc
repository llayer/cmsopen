#include "../interface/NTPU.h"
using namespace TopTree;


NTPU::NTPU ():
    bunchCrossing(-999),
    zpositions(-999.),
    sumpT_lowpT(-999),
    sumpT_highpT(-999),
    ntrks_lowpT(-999),
    ntrks_highpT(-999)
{
}

NTPU::~NTPU ()
{
}

void NTPU::Reset ()
{
    bunchCrossing=-999;
    zpositions=-999; 
    sumpT_lowpT=-999; 
    sumpT_highpT=-999; 
    ntrks_lowpT=-999;   
    ntrks_highpT=-999;
}

  
void NTPU::Dump(std::ostream & os){
        os << "Object: Pileup \n";
        os << " bunchCrossing = " << bunchCrossing << " ";
        os << " zpositions = " << zpositions << " ";
        os << " sumpT_lowpT = " << sumpT_lowpT << endl;
        os << " sumpT_highpT = " << sumpT_highpT << endl;
        os << " ntrks_lowpT = " << ntrks_lowpT << endl;
        os << " ntrks_highpT = " << ntrks_highpT << endl;
}

