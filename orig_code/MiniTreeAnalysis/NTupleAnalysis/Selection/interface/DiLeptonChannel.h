#ifndef DiLeptonChannel_h
#define DiLeptonChannel_h

#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"


using namespace std;
using namespace TopTree;



class DiLeptonChannel{
  public:

        enum MCChannels{ee,emu,mumu,ltau,tautau,ljets,other};

        DiLeptonChannel();
        ~DiLeptonChannel();

        void LoadEvent(const NTEvent* event);
        bool isDiLeptonEvent() const;
        bool isEEEvent() const;
        bool isEMuEvent() const;
        bool isMuMuEvent() const;
        bool isSignal(string AnaChannel) const;
        int Channel()const;
        string ChannelName()const;

        
  private:
        int TMEME;
        MCChannels MCChannel;


};

#endif
