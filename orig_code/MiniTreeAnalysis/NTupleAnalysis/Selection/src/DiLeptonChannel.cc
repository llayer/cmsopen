
#include "../interface/DiLeptonChannel.h"

DiLeptonChannel::DiLeptonChannel(){
        TMEME = -1;
        MCChannel = other;
}

DiLeptonChannel::~DiLeptonChannel(){
}

void DiLeptonChannel::LoadEvent(const NTEvent* event){
        TMEME = event->TMEME;
        MCChannel = other;
        if(TMEME == 2  || TMEME == 10101 || TMEME == 20200 ) MCChannel = ee;
        if(TMEME == 20 || TMEME == 11010 || TMEME == 22000 ) MCChannel = mumu;
        if(TMEME == 11 || TMEME == 11001 || TMEME == 10110 || TMEME == 21100 ) MCChannel = emu;
        if(TMEME == 10010 || TMEME == 10001) MCChannel = ltau;
        if(TMEME == 20000 ) MCChannel = tautau;
        if(TMEME == 1 || TMEME == 10 || TMEME == 10000 || TMEME == 11000 || TMEME == 10100 ) MCChannel = ljets;
}
        
bool DiLeptonChannel::isDiLeptonEvent() const{
        if (MCChannel == ee || MCChannel == mumu || MCChannel == tautau) return true; 
        return false;
}
        
bool DiLeptonChannel::isEEEvent() const{
        if (MCChannel == ee) return true;
        return false;
}
        
bool DiLeptonChannel::isEMuEvent() const{
        if (MCChannel == emu) return true;
        return false;
}
        
bool DiLeptonChannel::isMuMuEvent() const{
        if (MCChannel == mumu) return true;
        return false;
}
        
bool DiLeptonChannel::isSignal(string AnaChannel) const{
        if(AnaChannel == string("ee") && MCChannel == ee) return true;
        if(AnaChannel == string("emu") && MCChannel == emu) return true;
        if(AnaChannel == string("mumu") && MCChannel == mumu) return true;
        // added for the case of "all channel"
        if(AnaChannel == string("*") && (MCChannel == ee || MCChannel == emu|| MCChannel == mumu)) return true;
        if(AnaChannel == string("allChannels") && (MCChannel == ee || MCChannel == emu|| MCChannel == mumu)) return true;
        return false;
}
        
int DiLeptonChannel::Channel()const{
        return MCChannel;
}

string DiLeptonChannel::ChannelName()const{
        if(MCChannel == ee ) return string("ee");
        if(MCChannel == emu ) return string("emu");
        if(MCChannel == mumu ) return string("mumu");
        if(MCChannel == ltau ) return string("ltau");
        if(MCChannel == tautau ) return string("tautau");
        if(MCChannel == ljets ) return string("ljets");
        return string("other");
}

