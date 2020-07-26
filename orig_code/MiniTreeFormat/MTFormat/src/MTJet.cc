#include "../interface/MTJet.h"

using namespace TopTree;

MTJet::MTJet ():
    scale(-999.),
    isLOOSE(-999),
    isTIGHT(-999),
    jetArea(-999.)
{
}

MTJet::~MTJet ()
{
}

void
MTJet::Reset ()
{
    NTJet::Reset();
    vertex.SetXYZ (-999.0, -999.0, -999.0);
    scale = -999.;
    isLOOSE = -999;
    isTIGHT = -999;
    TracksOfJet.clear();
    trackassociatedVec.clear();
    jetArea = -999.;
}


void
MTJet::setTracks (const reco::TrackRefVector & trk_)
{

  for (unsigned int itk = 0; itk < trk_.size (); itk++) {
    const reco::Track tk = *trk_[itk];
    TracksOfJet.push_back (tk);
  }

}

MTTrack *
MTJet::NewTrack ()
{
  MTTrack evtrack;
  evtrack.Reset ();
  trackassociatedVec.push_back (evtrack);
  return &(trackassociatedVec.back ());
}


void MTJet::DumpExtended(std::ostream & os){
        Dump(os);
        os << " scale = " << scale << " ";
        os << " isLOOSE = " << isLOOSE << " ";
        os << " isTIGHT = " << isTIGHT << " ";
        os << endl;
}

