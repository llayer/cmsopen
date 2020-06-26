#ifndef __TOP_TREE_MTTRACK_H__
#define __TOP_TREE_MTTRACK_H__


#include "MiniTreeFormat/NTFormat/interface/NTTrack.h"

/** MTTrack: class track for MiniTree - inherit from NTTrack \\
Contains nHits for subdectectors and IPs.\\
Add all relevant information that should be saved in MiniTree but not in the NTuple \\
*/

using namespace std;

namespace TopTree
{
  class MTTrack : public NTTrack
  {
  public:
    //methods
    MTTrack ();
    ~MTTrack ();
    void Reset ();
    void DumpExtended(std::ostream & os = std::cout);

    //data members

    //nHits
    int nHitTracker;    /** Number of Valid Hits */
    int nHitPixel;      /** Number of Valid Pixel Hits */
    int withFirstPixel; /** Has Valid Hit in first Pixel Barrel */
    int nHitPXB;                /** Number of Valid Hits in Pixel Barrel*/
    int nHitPXF;                /** Number of Valid Hits in Pixel EC */
    int nHitTIB;                /** Number of Valid Hits in Strip TIB */
    int nHitTOB;                /** Number of Valid Hits in Strip TOB */
    int nHitTID;                /** Number of Valid Hits in Strip TID */
    int nHitTEC;                /** Number of Valid Hits in Strip TEC */
    //IPs .. 
    double IP3Dsig;     /** track 3D signed impact parameter significance*/
    double IP3D;                /** track 3D signed impact parameter*/
    double IP3Derr;     /** track 3D impact parameter error*/
    double IP2Dsig;     /** 2D*/
    double IP2D;
    double IP2Derr;
    double Prob3D;      /** 2D track probability (IP)*/
    double Prob2D;      /** 3D track probability (IP)*/
    double DecayLength; /** track decay length (from PV)*/

  };
}
#endif

