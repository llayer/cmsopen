#ifndef MuonJetsSelection_h
#define MuonJetsSelection_h

#include "Selection.h"
#include "SelectionTable.h"
#include "../../Tools/interface/Dataset.h"

// system include files
#include <memory>
#include <vector>


using namespace std;
using namespace TopTree;

/**
        Steps of the selection: (integer returned by doFullSelection() or FillTable(...))
        - Step 1        Dilepton pair choice
        - Step 2        Z mass veto 
        - Step 3        Minimal jet multiplicity 
        - Step 4        MET cuts
        - Step 5        btagging cuts

*/

class MuonJetsSelection: public Selection{


   public:
      MuonJetsSelection();
      MuonJetsSelection(const MuonJetsSelection &);
      ~MuonJetsSelection();

      void SetParameters(float MinValue, pair<float,float> METCuts, pair<float,float> ZMassWindow, int btagAlgo, float btagDiscriCut, int NofBtagJets);
      vector<string> GetCutList(){return cuts_;};
      vector<string> GetChannelList(){return channels_;};
      int GetChannel(string& CandPairType);
      
      int doFullSelection(Dataset* dataset, string channelName=string(""));/** return a integer which correspond to the last step that the event passes in the selection: [0-4] - possibility to check if the candPair correspond to the correct channel*/
      int FillTable(SelectionTable& selTable, Dataset* dataset, int idataset, float weight); /** Fill the selectionTable according to the result of doFullSelection  for an event of weight "weight" of a given dataset idataset - Returns the integer of doFullSelection() */
      bool passTriggerSelection(string datasetname);
    
    
    private:
      float MinMassCut_;
      pair<float,float> ZMassWindow_;
      pair<float,float> METCuts_;
      vector<string> cuts_;
      vector<string> channels_;
      int btagAlgo_;
      float btagDiscriCut_;
      int NofBtagJets_;

};

#endif
