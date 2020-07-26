#include "../interface/MTTrack.h"

using namespace TopTree;

MTTrack::MTTrack ():
    nHitTracker(-999.), 
    nHitPixel(-999.),   
    withFirstPixel(-999.),      
    nHitPXB(-999.),             
    nHitPXF(-999.),             
    nHitTIB(-999.),             
    nHitTOB(-999.),             
    nHitTID(-999.),             
    nHitTEC(-999.),             
    IP3Dsig(-999.),     
    IP3D(-999.),                
    IP3Derr(-999.),     
    IP2Dsig(-999.),     
    IP2D(-999.),
    IP2Derr(-999.),
    Prob3D(-999.),      
    Prob2D(-999.),      
    DecayLength(-999.)  
{
}

MTTrack::~MTTrack ()
{
}

void
MTTrack::Reset ()
{
    NTTrack::Reset();
    nHitTracker = -999.;        
    nHitPixel = -999.;  
    withFirstPixel = -999.;     
    nHitPXB = -999.;            
    nHitPXF = -999.;            
    nHitTIB = -999.;            
    nHitTOB = -999.;            
    nHitTID = -999.;            
    nHitTEC = -999.;            
    IP3Dsig = -999.;    
    IP3D = -999.;               
    IP3Derr = -999.;    
    IP2Dsig = -999.;    
    IP2D = -999.;
    IP2Derr = -999.;
    Prob3D = -999.;     
    Prob2D = -999.;     
    DecayLength = -999.;        

}

void MTTrack::DumpExtended(std::ostream & os){
        Dump(os);
        os << " nHitTracker = " << nHitTracker << " ";
        os << " nHitPixel = " << nHitPixel << " ";
        os << " withFirstPixel = " << withFirstPixel << " ";
        os << " nHitPXB = " << nHitPXB << " ";
        os << " nHitPXF = " << nHitPXF << " ";
        os << " nHitTIB = " << nHitTIB << " ";
        os << " nHitTOB = " << nHitTOB << " ";
        os << " nHitTID = " << nHitTID << " ";
        os << " nHitTEC = " << nHitTEC << " ";
        os << " IP3Dsig = " << IP3Dsig << " ";
        os << " IP3D = " << IP3D << " ";
        os << " IP3Derr = " << IP3Derr << " ";
        os << " IP2Dsig = " << IP2Dsig << " ";
        os << " IP2D = " << IP2D << " ";
        os << " IP2Derr = " << IP2Derr << " ";
        os << " Prob3D = " << Prob3D << " ";
        os << " Prob2D = " << Prob2D << " ";
        os << " DecayLength = " << DecayLength << " ";
        os << endl;
}
