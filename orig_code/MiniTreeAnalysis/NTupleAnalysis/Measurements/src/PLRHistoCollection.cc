#include "../interface/PLRHistoCollection.h"


PLRHistoCollection::PLRHistoCollection(){
}
                

PLRHistoCollection::~PLRHistoCollection(){
}
        
void PLRHistoCollection::LoadSetup(vector<string> Channels, vector<ExtendedSyst> Systematics, vector<Observable> Observables){
        channels = Channels;
        systematics = Systematics;
        observables = Observables;
}


void PLRHistoCollection::CreateHistos(){
        
        for(unsigned int c=0; c<channels.size(); c++){
                vector<vector<vector<vector<TH1F> > > > vvvvh1;
                vector<vector<vector<vector<TH2D> > > > vvvvh2;
                for(unsigned int o=0; o<observables.size(); o++){
                        vector<vector<vector<TH1F> > > vvvh1;
                        vector<vector<vector<TH2D> > > vvvh2;
                        for(unsigned int s=0; s<systematics.size(); s++){
                                vector<vector<TH1F> > vvh1;
                                vector<vector<TH2D> > vvh2;
                                if(systematics[s].discretized){
                                        vector<TH1F> vh1;
                                        vector<TH2D> vh2;
                                        //fill twice: minus and plus
                                        vvh1.push_back(vh1);
                                        vvh1.push_back(vh1);
                                        vvh2.push_back(vh2);
                                        vvh2.push_back(vh2);
                                }
                                else{
                                        vector<TH1F> vh1;
                                        vector<TH2D> vh2;
                                        for(unsigned int x=0;x<systematics[s].values.size();x++){
                                                vvh1.push_back(vh1);
                                                vvh2.push_back(vh2);
                                        }
                                }
                                vvvh1.push_back(vvh1);
                                vvvh2.push_back(vvh2);
                        }
                        vvvvh1.push_back(vvvh1);
                        vvvvh2.push_back(vvvh2);
                }
                //Problem here
                //if(observables[o].dim == 1) {
                        histos1D.push_back(vvvvh1);
                //}     
                //if(observables[o].dim == 2) {
                        histos2D.push_back(vvvvh2);
                //}     
                //if(observables[o].dim != 1 || observables[o].dim != 2) {
                //      cerr<<"PLRHistoCollection::CreateHistos:: size of histo should be 1 or 2 !!"<<endl;
                //}
        }
}

vector<TH1F>& PLRHistoCollection::GetHisto1D(int iChannel, int iObservables, int iSystematic, int iSystValue){
                if(observables[iObservables].dim == 1) return histos1D[iChannel][iObservables][iSystematic][iSystValue];
                return null1;
}

vector<TH2D>& PLRHistoCollection::GetHisto2D(int iChannel, int iObservables, int iSystematic, int iSystValue){
                if(observables[iObservables].dim == 2) return histos2D[iChannel][iObservables][iSystematic][iSystValue];
                return null2;
}
                
void PLRHistoCollection::FillHistos(bool FlagB, bool IsSignal, float weight){
}
                        
/*
        private:
                vector<string> channels;
                vector<PLRHistoCollectionSystematic> systematics;
                vector<string, int> observables;  //name and dimension

                vector<vector<vector<TH1F> > > histos1D;
                vector<vector<vector<TH2D> > > histos2D;
};
*/

///////////////////////////////////////////////////////////





