#include "../interface/SelectionTable.h"

using namespace std;

  
SelectionTable::SelectionTable(){
        Lumi_ = 1;
}

SelectionTable::SelectionTable(vector<string> listOfCuts, vector<Dataset> listOfDatasets, string channel){
        Lumi_ = 1;
        channelName_ = channel;
        listOfCuts_ = listOfCuts;
        listOfDatasets_ = listOfDatasets;
        for(unsigned int i=0;i<listOfCuts.size();i++){
                vector<float> a;
                for(unsigned int j=0;j<listOfCuts.size();j++){
                        float tmp = 0;
                        a.push_back(tmp);
                }
                nofEventsRaw_.push_back(a);
                nofEventsRawError_.push_back(a);
                nofEvents_.push_back(a);
                nofEventsError_.push_back(a);
                cutEfficiency_.push_back(a);
                cutEfficiencyError_.push_back(a);
                totalCutEfficiency_.push_back(a);
                totalCutEfficiencyError_.push_back(a);
        }
}

SelectionTable::SelectionTable(const SelectionTable& table){
  Lumi_ = table.Lumi_;
  channelName_ = table.channelName_;
  listOfCuts_ = table.listOfCuts_;
  listOfDatasets_ = table.listOfDatasets_;
  nofEventsRaw_ = table.nofEventsRaw_;
  nofEventsRawError_ = table.nofEventsRawError_;
  nofEvents_ = table.nofEvents_;
  nofEventsError_ = table.nofEventsError_;
  cutEfficiency_ = table.cutEfficiency_;
  cutEfficiencyError_ = table.cutEfficiencyError_;
  totalCutEfficiency_ = table.totalCutEfficiency_;
  totalCutEfficiencyError_ = table.totalCutEfficiencyError_;
}

SelectionTable::~SelectionTable(){
}

void SelectionTable::MergeDatasets(vector<string> DatasetNames_ToMerge, string MergedName){
        if(DatasetNames_ToMerge.size()<=1){
                cerr<<"SelectionTable::MergeDataset:: only zero|one dataset name given as argument"<<endl;
                return;
        }
        bool isOk = true;
        vector<int> iDatasets;
        for(unsigned int i=0; i<DatasetNames_ToMerge.size();i++){
                bool found = false;
                for(unsigned int j=0;j<listOfDatasets_.size();j++){
                        if(listOfDatasets_[j].Name()==DatasetNames_ToMerge[i]){
                                found = true;
                                iDatasets.push_back(j);
                                break;
                        }
                }
                if(!found){
                        isOk = false;
                        break;
                }
        }
        if(!isOk){
                cerr<<"SelectionTable::MergeDataset::  dataset name not found"<<endl;
                return;
        }
        //sort the vector
        std::sort(iDatasets.begin(), iDatasets.end());
        //change Datasets' vector
        for(unsigned int i=iDatasets.size()-1;i>0;i--){
                listOfDatasets_.erase(listOfDatasets_.begin()+iDatasets[i]);
        }
        listOfDatasets_[iDatasets[0]].SetName(MergedName);
        //Sum the nofEvents
        for(unsigned int id=1;id<iDatasets.size();id++){
                for(unsigned int i=0;i<listOfCuts_.size();i++){
                        nofEventsRaw_[i][iDatasets[0]]+= nofEventsRaw_[i][iDatasets[id]];
                        nofEventsRawError_[i][iDatasets[0]]+= nofEventsRawError_[i][iDatasets[id]];
                        nofEvents_[i][iDatasets[0]]+= nofEvents_[i][iDatasets[id]];     
                }
        }
        for(unsigned int i=iDatasets.size()-1;i>0;i--){
                //listOfDatasets_.erase(listOfDatasets_.begin()+iDatasets[i]);
                for(unsigned int l=0;l<listOfCuts_.size();l++){
                        nofEventsRaw_[l].erase(nofEventsRaw_[l].begin()+iDatasets[i]);
                        nofEventsRawError_[l].erase(nofEventsRawError_[l].begin()+iDatasets[i]);
                        nofEvents_[l].erase(nofEvents_[l].begin()+iDatasets[i]);
                        nofEventsError_[l].erase(nofEventsError_[l].begin()+iDatasets[i]);
                        cutEfficiency_[l].erase(cutEfficiency_[l].begin()+iDatasets[i]);
                        cutEfficiencyError_[l].erase(cutEfficiencyError_[l].begin()+iDatasets[i]);
                        totalCutEfficiency_[l].erase(totalCutEfficiency_[l].begin()+iDatasets[i]);
                        totalCutEfficiencyError_[l].erase(totalCutEfficiencyError_[l].begin()+iDatasets[i]);
                }
        }

}
            
void SelectionTable::DefineFirstDataset(string DatasetName){
        bool found = false;
        int ipos = 0;
        for(unsigned int i=0; i<listOfDatasets_.size();i++){
                if(listOfDatasets_[i].Name() == DatasetName){
                        found = true;
                        ipos = i;
                        break;
                }
        }
        listOfDatasets_.insert(listOfDatasets_.begin(),listOfDatasets_[ipos]);
        listOfDatasets_.erase(listOfDatasets_.begin()+ipos+1);
        for(unsigned int l=0;l<listOfCuts_.size();l++){
                //insert
                nofEventsRaw_[l].insert(nofEventsRaw_[l].begin(),nofEventsRaw_[l][ipos]);
                nofEventsRawError_[l].insert(nofEventsRawError_[l].begin(),nofEventsRawError_[l][ipos]);
                nofEvents_[l].insert(nofEvents_[l].begin(),nofEvents_[l][ipos]);
                nofEventsError_[l].insert(nofEventsError_[l].begin(),nofEventsError_[l][ipos]);
                cutEfficiency_[l].insert(cutEfficiency_[l].begin(),cutEfficiency_[l][ipos]);
                cutEfficiencyError_[l].insert(cutEfficiencyError_[l].begin(),cutEfficiencyError_[l][ipos]);
                totalCutEfficiency_[l].insert(totalCutEfficiency_[l].begin(),totalCutEfficiency_[l][ipos]);
                totalCutEfficiencyError_[l].insert(totalCutEfficiencyError_[l].begin(),totalCutEfficiencyError_[l][ipos]);
                //erase
                nofEventsRaw_[l].erase(nofEventsRaw_[l].begin()+ipos+1);
                nofEventsRawError_[l].erase(nofEventsRawError_[l].begin()+ipos+1);
                nofEvents_[l].erase(nofEvents_[l].begin()+ipos+1);
                nofEventsError_[l].erase(nofEventsError_[l].begin()+ipos+1);
                cutEfficiency_[l].erase(cutEfficiency_[l].begin()+ipos+1);
                cutEfficiencyError_[l].erase(cutEfficiencyError_[l].begin()+ipos+1);
                totalCutEfficiency_[l].erase(totalCutEfficiency_[l].begin()+ipos+1);
                totalCutEfficiencyError_[l].erase(totalCutEfficiencyError_[l].begin()+ipos+1);
        }
}

void SelectionTable::Scale(float Lumi){
        for(unsigned int i=0;i<listOfCuts_.size();i++){
                for(unsigned int j=0;j<listOfDatasets_.size();j++){
                        nofEventsRaw_[i][j] = nofEventsRaw_[i][j]*Lumi;
                        nofEventsRawError_[i][j] = nofEventsRawError_[i][j]*Lumi*Lumi;  // Lumi**2 because in TableCalculator n=sqrt(n)!
                        nofEvents_[i][j] = nofEvents_[i][j]*Lumi;
                        nofEventsError_[i][j] = nofEventsError_[i][j]*Lumi;
                        cutEfficiency_[i][j] = cutEfficiency_[i][j]*Lumi;
                        cutEfficiencyError_[i][j] = cutEfficiencyError_[i][j]*Lumi;
                        totalCutEfficiency_[i][j] = totalCutEfficiency_[i][j]*Lumi;
                        totalCutEfficiencyError_[i][j] = totalCutEfficiencyError_[i][j]*Lumi;
                }
        }
}

void SelectionTable::TableCalculator(){
 
  //scale to lumi
  //Scale(Lumi_);

  //loop on cuts
  for(unsigned int i=0;i<listOfCuts_.size();i++){
        //loop on datasets
        for(unsigned int j=0;j<listOfDatasets_.size();j++){
        //float factor = listOfDatasets_[j].NormFactor();
        float factor = 1.;
                nofEventsRawError_[i][j] = sqrt(nofEventsRawError_[i][j]); // here we take sqrt(sum w2)!
                nofEvents_[i][j] = nofEventsRaw_[i][j]*factor;
//              nofEventsError_[i][j] = ErrorCalculator(nofEventsRaw_[i][j],factor);
                nofEventsError_[i][j] = nofEventsRawError_[i][j]*factor;
                //push_back
                if(i>0){
                        cutEfficiency_[i][j] =  (float)(nofEventsRaw_[i][j]/nofEventsRaw_[i-1][j]);
                        cutEfficiencyError_[i][j] =  BinomialeError((float)(nofEventsRaw_[i][j]/nofEventsRaw_[i-1][j]),nofEventsRaw_[i-1][j]);
                        
                }
                else {
                        cutEfficiency_[i][j] = 1.;
                        cutEfficiencyError_[i][j] = 0.;
                }
                totalCutEfficiency_[i][j] = (float)(nofEventsRaw_[i][j]/nofEventsRaw_[0][j]);
                totalCutEfficiencyError_[i][j] =  BinomialeError((float)(nofEventsRaw_[i][j]/nofEventsRaw_[0][j]),nofEventsRaw_[0][j]);
        }
   }
}


void SelectionTable::Fill(unsigned int DatasetNumber, unsigned int CutNumber, float value){
        if(DatasetNumber>=listOfDatasets_.size()) cerr<<"DataNumber argument wrong !"<<endl;
        if(CutNumber>=listOfCuts_.size()) cerr<<"CutNumber argument wrong !"<<endl;
        nofEventsRaw_[CutNumber][DatasetNumber]+= value;
        nofEventsRawError_[CutNumber][DatasetNumber]+= value*value;
}

void SelectionTable::Fill( unsigned int DatasetNumber, vector<float> PassTheCuts){
        if(DatasetNumber>=listOfDatasets_.size()) cerr<<"DataNumber argument wrong !"<<endl;
        if(PassTheCuts.size()>listOfCuts_.size()) cerr<<"CutNumber argument wrong !"<<endl;
        for(unsigned int i=0;i<PassTheCuts.size();i++) {
              nofEventsRaw_[i][DatasetNumber]+= PassTheCuts[i];
              nofEventsRawError_[i][DatasetNumber]+= PassTheCuts[i]*PassTheCuts[i];
        }
}

void SelectionTable::Write(string filename, bool WithError, int maxColumns){
        ofstream fout(filename.c_str(), ios::out);
        SelectionTable::Write(fout, WithError, maxColumns);
}

void SelectionTable::WriteTable(ofstream& fout, vector<vector<float> > listTable_, unsigned int istart, unsigned int istop){
        fout<<"&";
        for(unsigned int i=istart;i<istop;i++) {
                fout<<listOfDatasets_[i].Name()<<"\t";
                if(i<istop-1) fout<<"&";
                else fout<<"\\\\"<<endl;
        }
        fout<<endl;
        fout<<"\\hline"<<endl;
        for(unsigned int i=istart;i<istop;i++){
                fout<<listOfCuts_[i]<<"&\t";
                for(unsigned int j=0;j<listOfDatasets_.size();j++){
                        fout<<listTable_[i][j]<<"\t";   
                        if(j<istop-1) fout<<"&";
                        else fout<<"\\\\"<<endl;
                }
                fout<<endl;
        }
}

void SelectionTable::WriteTable(ofstream& fout, vector<vector<float> > listTable_, vector<vector<float> > listTableError_, unsigned int istart, unsigned int istop){
        fout<<"&";
        for(unsigned int i=istart;i<istop;i++) {
                fout<<listOfDatasets_[i].Name()<<"\t";
                if(i<istop-1) fout<<"&";
                else fout<<"\\\\"<<endl;
        }
        fout<<endl;
        fout<<"\\hline"<<endl;
        for(unsigned int i=0;i<listOfCuts_.size();i++){
                fout<<listOfCuts_[i]<<"&\t";
                for(unsigned int j=istart;j<istop;j++){
                        fout<<listTable_[i][j]<<" $\\pm$ "<<listTableError_[i][j]<<"\t";        
                        if(j<istop-1) fout<<"&";
                        else fout<<"\\\\"<<endl;
                }
                fout<<endl;
        }
}

void SelectionTable::Write(ofstream& fout, bool WithError, int maxColumns){

        if(WithError){  
                        
                //splitting of cut flow tables
                //maxColumns, max # of datasets to be written per table
                //maxColumns=-1, all datasets written in one table
                unsigned int istart=0;
                unsigned int istop=0;
                
                if(maxColumns!=-1) istop+= maxColumns;
                else istop = listOfDatasets_.size();
                 
                while(istart<listOfDatasets_.size())
                { 
                  if (istop > listOfDatasets_.size()) istop = listOfDatasets_.size();
                  ////////////////////////////
                  fout<<"\\begin{table}"<<endl;
                  fout<<"\\centering"<<endl;
                  fout<<"\\begin{tabular}{|c|";
                  for(unsigned int i=istart;i<istop;i++) fout<<"c";
                  fout<<"|}"<<endl;
                  fout<<"\\hline"<<endl;
                  WriteTable(fout, nofEvents_, nofEventsError_, istart, istop);
                  fout<<"\\hline"<<endl;
                  fout<<"\\end{tabular}"<<endl;
                  fout<<"\\caption{Number of events - channel "<<channelName_<<"}"<<endl;
                  fout<<"\\label{tab:SelectionTable_"<<channelName_<<"}"<<endl;
                  fout<<"\\end{table}"<<endl;
                  ////////////////////////////
                  fout<<"\\begin{table}"<<endl;
                  fout<<"\\centering"<<endl;
                  fout<<"\\begin{tabular}{|c|";
                  for(unsigned int i=istart;i<istop;i++) fout<<"c";
                  fout<<"|}"<<endl;
                  fout<<"\\hline"<<endl;
                  WriteTable(fout, cutEfficiency_, cutEfficiencyError_, istart, istop);
                  fout<<"\\hline"<<endl;
                  fout<<"\\end{tabular}"<<endl;
                  fout<<"\\caption{Cut efficiencies - channel "<<channelName_<<"}"<<endl;
                  fout<<"\\label{tab:EfficienciesTable_"<<channelName_<<"}"<<endl;
                  fout<<"\\end{table}"<<endl;
                  //fout<<"Cut Effiency"<<endl;
                  ////////////////////////////
                  fout<<"\\begin{table}"<<endl;
                  fout<<"\\centering"<<endl;
                  fout<<"\\begin{tabular}{|c|";
                  for(unsigned int i=istart;i<istop;i++) fout<<"c";
                  fout<<"|}"<<endl;
                  fout<<"\\hline"<<endl;
                  WriteTable(fout, totalCutEfficiency_, totalCutEfficiencyError_, istart, istop);
                  fout<<"\\hline"<<endl;
                  fout<<"\\end{tabular}"<<endl;
                  fout<<"\\caption{Total cut efficiencies - channel "<<channelName_<<"}"<<endl;
                  fout<<"\\label{tab:TotalEfficienciesTable_"<<channelName_<<"}"<<endl;
                  fout<<"\\end{table}"<<endl;
                  fout<<"\\clearpage"<<endl;
                  ////////////////////////////
                  istart+=maxColumns;
                  istop+= maxColumns;
                }
        }
        else{   
                unsigned int istart=0;
                unsigned int istop=0;           
                if(maxColumns!=-1) istop+= maxColumns;
                else istop+= listOfDatasets_.size();
                
                while(istart<listOfDatasets_.size())
                {                
                 if (istop > listOfDatasets_.size()) istop = listOfDatasets_.size();    
                        
                 fout<<"nof Events - channel "<<channelName_<<endl;
                 WriteTable(fout, nofEvents_, istart, istop);
                 fout<<"Cut Effiency - channel "<<channelName_<<endl;
                 WriteTable(fout, cutEfficiency_, istart, istop);
                 fout<<"Total Cut Effiency - channel "<<channelName_<<endl;
                 WriteTable(fout, totalCutEfficiency_, istart, istop);
                 
                 istart+=maxColumns;
                 istop+= maxColumns;
                }
        }
}

    

