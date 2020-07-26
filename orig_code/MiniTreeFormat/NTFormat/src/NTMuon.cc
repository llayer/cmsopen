#include "../interface/NTMuon.h"
using namespace TopTree;


NTMuon::NTMuon ():
        MuonType(-999),
        GlobalMuonPromptTight(-999),
        NTrValidHits(-999),
        D0Inner(-999.),NOfMatches(-999.)
{
}


NTMuon::~NTMuon ()
{
}

void
NTMuon::Reset ()
{
  NTLepton::Reset ();
  MuonType = -999;
  GlobalMuonPromptTight = -999;
  NTrValidHits = -999;
  NValidHits = -999;
  D0Inner = -999.;
  NOfMatches= -999.;
}
   

void NTMuon::Dump(std::ostream & os){
        os << "Object: Muon \n";
        PrintInfo();
        os << " MuonType = " << MuonType << " ";
        os << " GlobalMuonPromptTight = " << GlobalMuonPromptTight << " ";
        os << " NTrValidHits = " << NTrValidHits << endl;
}
