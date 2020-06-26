#include "../interface/NTJet.h"
using namespace TopTree;


NTJet::NTJet ():
    emEnergyFraction(-999.),    
    hadEnergyFraction(-999.),   
    CHEF(-999.),                
    CEEF(-999.),                
    NHEF(-999.),                
    NEEF(-999.),                
    n90Hits(-999.),             
    fHPD(-999.),
    passJetID(false),           
    partonFlavour(-999),
    TCDiscri(-999.),  
    SVDiscri(-999.),  
    SMDiscri(-999.),  
    nTracks(-999), 
    sumPtTracks(-999.),    
    nTracksSVX(-999),           
    nSVX(-999),                 
    chi2SVX(-999.),             
    ndofSVX(-999.),             
    flightDistanceSVX(-999.),   
    flightDistanceSigSVX(-999.),        
    flightDistanceErrSVX(-999.),        
    MassSVX(-999.),
    turnOnWeight(-999.),
    corrJES(-999.)
    //resCorr(-999.)            
{
}

NTJet::~NTJet ()
{
}

void
NTJet::Reset ()
{
    p4.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
    p4Gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
    p4Parton.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
    p4HLT.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
    emEnergyFraction = -999.;   
    hadEnergyFraction = -999.;  
    CHEF = -999.;               
    CEEF = -999.;               
    NHEF = -999.;               
    NEEF = -999.;               
    n90Hits = -999.;            
    fHPD = -999.;
    passJetID = false,                  
    partonFlavour = -999;
    TCDiscri = -999.;  
    SVDiscri = -999.;  
    SMDiscri = -999.;  
    nTracks = -999;     
    sumPtTracks = -999.;
    nTracksSVX = -999;          
    nSVX = -999;                        
    chi2SVX = -999.;            
    ndofSVX = -999.;            
    flightDistanceSVX = -999.;  
    flightDistanceSigSVX = -999.;       
    flightDistanceErrSVX = -999.;       
    MassSVX = -999.;
    turnOnWeight = -999.;
    //resCorr = -999.;   
    corrJES = -999.;
        
}

void NTJet::Dump(std::ostream & os){
        os << "Object: Jet \n";
        PrintP4(p4, os);
        os << " emEnergyFraction  = " << emEnergyFraction << " ";
        os << " hadEnergyFraction = " << hadEnergyFraction << " ";
        os << " CHEF = " << CHEF << " ";
        os << " CEEF = " << CEEF << " ";
        os << " NHEF = " << NHEF << " ";
        os << " NEEF = " << NEEF << " ";
        os << " n90Hits = " << n90Hits << " ";
        os << " fHPD = " << fHPD << " ";
        os << " partonFlavour = " << partonFlavour << " ";
        os << " TCDiscri = " << TCDiscri << " ";
        os << " SVDiscri = " << SVDiscri << " ";
        os << " SMDiscri = " << SMDiscri << " ";
        os << " nTracks = " << nTracks << " ";
        os << " SumPtTracks = " << sumPtTracks << " ";
        os << " nTracksSVX = " << nTracksSVX << " ";
        os << " nSVX = " << nSVX << " ";
        os << " chi2SVX = " << chi2SVX << " ";
        os << " ndofSVX = " << ndofSVX << " ";
        os << " flightDistanceSVX = " << flightDistanceSVX << " ";
        os << " flightDistanceSigSVX = " << flightDistanceSigSVX << " ";
        os << " flightDistanceErrSVX = " << flightDistanceErrSVX << " ";
        os << " MassSVX = " << MassSVX << " ";
        os << " turnOnWeight = " << turnOnWeight << " ";
        //os << " resCorr = " << resCorr << " ";
        os << endl;
}
    
float NTJet::GetDiscri(string algo){
        for(unsigned int i=0;i<DiscriPair.size();i++){
        //std::cout << DiscriPair[i].first<< " " << DiscriPair[i].second<< std::endl;
                        if(DiscriPair[i].first == algo) return DiscriPair[i].second;
                        
        }
        return -99999.;
}

