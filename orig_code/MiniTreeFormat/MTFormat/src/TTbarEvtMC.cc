#include "../interface/TTbarEvtMC.h"

using namespace TopTree;

TTbarEvtMC::TTbarEvtMC ():
QLep_gen (-999.), QALep_gen (-999.), mc_LepID (0), mc_ALepID (0), Tmeme (0), ProdProc (0), TTbarDecayType ("")
{
}

TTbarEvtMC::~TTbarEvtMC ()
{
}

void
TTbarEvtMC::Reset ()
{
  p4Parton1_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4Parton2_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Top_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_ATop_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Lep_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_ALep_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Neu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_ANeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Wminus_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Wplus_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_B_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Bbar_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_BRad_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_BbarRad_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_SumTauNeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_SumTauANeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);


  QLep_gen = -999;
  QALep_gen = -999;
  mc_LepID = 0;
  mc_ALepID = 0;
  Tmeme = 0;
  ProdProc = 0;
  TTbarDecayType = "";

}

void
TTbarEvtMC::GetTTbarTruth (edm::Handle < reco::GenParticleCollection > genParticles, bool verbose)
{
  this->Reset ();

  double Qlep1 = 0;
  double Qlep2 = 0;
  const Candidate *lep1 = 0;
  const Candidate *lep2 = 0;
  const Candidate *W1 = 0;
  const Candidate *W2 = 0;
  const Candidate *Nu1 = 0;
  const Candidate *Nu2 = 0;
  const Candidate *Top1 = 0;
  const Candidate *Top2 = 0;
  const Candidate *B1 = 0;
  const Candidate *B2 = 0;
  const Candidate *B1Rad = 0;
  const Candidate *B2Rad = 0;
  const Candidate *TauNu1 = 0;
  const Candidate *TauNu2 = 0;
  const Candidate *TauANu1 = 0;
  const Candidate *TauANu2 = 0;

  math::XYZTLorentzVector paV;


  //---------------------------------------------------------------------------
  //                 decay channel: (DecChan)
  //---------------------------------------------------------------------------


// Define the type of tt events final states
//
//   TMEME variable means :
//
//  T = # of taus from W decays
//  M = # of muons from W->tau->muon
//  E = # of electrons from W->tau->electron
//  M = # of muons from W->muon
//  E = # of electrons from W->electron
//

  int tmeme = 0;

  for (size_t i = 0; i < genParticles->size (); ++i) {
    const GenParticle & paIt = (*genParticles)[i];
    const Candidate *W = &(*genParticles)[i];


//    // finding top and antitop 
//    //------------------------
    if (abs (paIt.pdgId ()) == 6 && paIt.status () == 3) {
      if (paIt.pdgId () == 6) {
        Top1 = &(*genParticles)[i];
      }
      if (paIt.pdgId () == -6) {
        Top2 = &(*genParticles)[i];
      }
    }

//       // finding B1 & B2:
//       //------------------------          
    if (abs (paIt.pdgId ()) == 6 && paIt.status () == 3) {
      int FoundWB = 0;
      int FoundB1 = -1;
      int FoundB2 = -1;
      for (unsigned int j = 0; j < paIt.numberOfDaughters (); j++) {
        if (abs (paIt.daughter (j)->pdgId ()) == 24 || abs (paIt.daughter (j)->pdgId ()) == 5) {
          FoundWB++;
          if (paIt.daughter (j)->pdgId () == 5)
            FoundB1 = j;
          if (paIt.daughter (j)->pdgId () == -5)
            FoundB2 = j;
        }
      }
      if (FoundWB >= 2) {
        if (FoundB1 >= 0)
          B1 = paIt.daughter (FoundB1);
        if (FoundB2 >= 0)
          B2 = paIt.daughter (FoundB2);
      }
    }

    if (B1) {
      for (unsigned int j = 0; j < B1->numberOfDaughters (); j++) {
        if (abs (B1->daughter (j)->pdgId ()) == 5 && B1->daughter (j)->status () == 2)
          B1Rad = B1->daughter (j);
      }
    }

    if (B2) {
      for (unsigned int j = 0; j < B2->numberOfDaughters (); j++) {
        if (abs (B2->daughter (j)->pdgId ()) == 5 && B2->daughter (j)->status () == 2)
          B2Rad = B2->daughter (j);
      }
    }

    if (abs (paIt.pdgId ()) != 24)
      continue;
    if (paIt.status () != 3)
      continue;

    for (unsigned int j = 0; j < paIt.numberOfDaughters (); j++) {
      if (abs (paIt.daughter (j)->pdgId ()) == 12 || abs (paIt.daughter (j)->pdgId ()) == 14 || abs (paIt.daughter (j)->pdgId ()) == 16) {
//     // finding nue/antinue ( t->W->nue )
//     //--------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 12) {
          if (paIt.daughter (j)->pdgId () == -12) {
            Nu1 = paIt.daughter (j);
          }
          if (paIt.daughter (j)->pdgId () == 12) {
            Nu2 = paIt.daughter (j);
          }
        }
//     // finding numu/antinumu ( t->W->numu )
//     //------------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 14) {
          if (paIt.daughter (j)->pdgId () == -14) {
            Nu1 = paIt.daughter (j);
          }
          if (paIt.daughter (j)->pdgId () == 14) {
            Nu2 = paIt.daughter (j);
          }
        }
//     // finding nutau/antinutau ( t->W->nutau )
//     //---------------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 16) {
          if (paIt.daughter (j)->pdgId () == -16) {
            Nu1 = paIt.daughter (j);
          }
          if (paIt.daughter (j)->pdgId () == 16) {
            Nu2 = paIt.daughter (j);
          }
        }

      }

      if (abs (paIt.daughter (j)->pdgId ()) == 11 || abs (paIt.daughter (j)->pdgId ()) == 13 || abs (paIt.daughter (j)->pdgId ()) == 15) {

//     // finding electron/positron ( t->W->e )
//     //--------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 11) {
          tmeme += 1;
          if (paIt.daughter (j)->pdgId () == 11) {
            W1 = &(*genParticles)[i];
            lep1 = paIt.daughter (j);
            Qlep1 = (paIt.daughter (j)->pdgId () == 11) ? -1 : 1;
          }
          if (paIt.daughter (j)->pdgId () == -11) {
            W2 = &(*genParticles)[i];
            lep2 = paIt.daughter (j);
            Qlep2 = (paIt.daughter (j)->pdgId () == 11) ? -1 : 1;
          }
        }
//     // finding muon/antimuon ( t->W->muon )
//     //-------------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 13) {
          tmeme += 10;
          if (paIt.daughter (j)->pdgId () == 13) {
            W1 = &(*genParticles)[i];
            lep1 = paIt.daughter (j);
            Qlep1 = (paIt.daughter (j)->pdgId () == 13) ? -1 : 1;
          }
          if (paIt.daughter (j)->pdgId () == -13) {
            W2 = &(*genParticles)[i];
            lep2 = paIt.daughter (j);
            Qlep2 = (paIt.daughter (j)->pdgId () == 13) ? -1 : 1;
          }
        }
//     // finding tau/antitau ( t->W->tau )
//     //-------------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 15) {
          tmeme += 10000;
          if (paIt.daughter (j)->pdgId () == 15) {
            W1 = &(*genParticles)[i];
            lep1 = paIt.daughter (j);
            Qlep1 = (paIt.daughter (j)->pdgId () == 15) ? -1 : 1;
          }
          if (paIt.daughter (j)->pdgId () == -15) {
            W2 = &(*genParticles)[i];
            lep2 = paIt.daughter (j);
            Qlep2 = (paIt.daughter (j)->pdgId () == 15) ? -1 : 1;
          }
        }


        int indxtau = -1;
        for (unsigned int k = 0; k < W->numberOfDaughters (); k++) {
          if (abs (W->daughter (k)->pdgId ()) == 15)
            indxtau = k;
        }
        while (indxtau >= 0) {
          if (!W)
            cout << "NULL " << endl;
          bool FoundTau = false;
          for (unsigned int k = 0; k < W->numberOfDaughters (); k++) {
            if (abs (W->daughter (k)->pdgId ()) == 24)
              continue;
            if (abs (W->daughter (k)->pdgId ()) == 11 || abs (W->daughter (k)->pdgId ()) == 13) {
              if (abs (W->daughter (k)->pdgId ()) == 11)
                tmeme += 100;
              if (abs (W->daughter (k)->pdgId ()) == 13)
                tmeme += 1000;
              indxtau = -1;
            }
            if (abs (W->daughter (k)->pdgId ()) == 15) {
              indxtau = k;
              FoundTau = true;
            }

            if (abs (W->pdgId ()) == 15) {
              if (W->daughter (k)->pdgId () == 16) {
                TauNu1 = W->daughter (k);
              }
              else if (W->daughter (k)->pdgId () == -16) {
                TauANu1 = W->daughter (k);
              }
              else if (W->daughter (k)->pdgId () == 12 || W->daughter (k)->pdgId () == 14) {
                TauNu2 = W->daughter (k);
              }
              else if (W->daughter (k)->pdgId () == -12 || W->daughter (k)->pdgId () == -14) {
                TauANu2 = W->daughter (k);
              }
            }
          }
          if (FoundTau) {
            W = W->daughter (indxtau);
          }
          else {
            indxtau = -1;
          }
        }
      }
    }
  }

  string DcT = "";
  if (tmeme == 20 || tmeme == 11010 || tmeme == 22000)
    DcT = "Dimuon";
  if (tmeme == 2 || tmeme == 10101 || tmeme == 20200)
    DcT = "Dielectron";
  if (tmeme == 11 || tmeme == 21100 || tmeme == 11001 || tmeme == 10110)
    DcT = "Dielecmuon";
  if (tmeme == 10 || tmeme == 11000 || tmeme == 21000)
    DcT = "Singlemuon";
  if (tmeme == 1 || tmeme == 10100 || tmeme == 20100)
    DcT = "Singleelectron";
  if (tmeme == 10000 || tmeme == 10001 || tmeme == 10010)
    DcT = "Singletauhad";
  if (tmeme == 20000)
    DcT = "Ditauhad";
  if (tmeme == 0)
    DcT = "Had";

  this->Tmeme = tmeme;
  this->TTbarDecayType = DcT;

  // lep, antilepton
  //-----------------------------
  if (lep1) {
    paV = lep1->p4 ();
    this->p4_Lep_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
    this->QLep_gen = Qlep1;
    this->mc_LepID = lep1->pdgId ();
  }

  if (lep2) {
    paV = lep2->p4 ();
    this->p4_ALep_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
    this->QALep_gen = Qlep2;
    this->mc_ALepID = lep2->pdgId ();
  }

  if (Nu1) {
    paV = Nu1->p4 ();
    this->p4_ANeu_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (Nu2) {
    paV = Nu2->p4 ();
    this->p4_Neu_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (W1) {
    paV = W1->p4 ();
    this->p4_Wminus_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (W2) {
    paV = W2->p4 ();
    this->p4_Wplus_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (Top1) {
    paV = Top1->p4 ();
    this->p4_Top_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (Top2) {
    paV = Top2->p4 ();
    this->p4_ATop_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (B1) {
    paV = B1->p4 ();
    this->p4_B_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (B2) {
    paV = B2->p4 ();
    this->p4_Bbar_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (B1Rad) {
    paV = B1Rad->p4 ();
    this->p4_BRad_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (B2Rad) {
    paV = B2Rad->p4 ();
    this->p4_BbarRad_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (TauNu1) {
    if (!TauANu2)
      this->p4_SumTauNeu_gen.SetPxPyPzE (TauNu1->p4 ().Px (), TauNu1->p4 ().Py (), TauNu1->p4 ().Pz (), TauNu1->p4 ().E ());
    else
      this->p4_SumTauNeu_gen.SetPxPyPzE (TauNu1->p4 ().Px () + TauANu2->p4 ().Px (), TauNu1->p4 ().Py () + TauANu2->p4 ().Py (), TauNu1->p4 ().Pz () + TauANu2->p4 ().Pz (),
                                             TauNu1->p4 ().E () + TauANu2->p4 ().E ());
  }
  if (TauANu1) {
    if (!TauNu2)
      this->p4_SumTauANeu_gen.SetPxPyPzE (TauANu1->p4 ().Px (), TauANu1->p4 ().Py (), TauANu1->p4 ().Pz (), TauANu1->p4 ().E ());
    else
      this->p4_SumTauANeu_gen.SetPxPyPzE (TauANu1->p4 ().Px () + TauNu2->p4 ().Px (), TauANu1->p4 ().Py () + TauNu2->p4 ().Py (), TauANu1->p4 ().Pz () + TauNu2->p4 ().Pz (),
                                              TauANu1->p4 ().E () + TauNu2->p4 ().E ());
  }



  if (verbose) {
    cout << "GetTTbarTruth" << endl;
    cout << "TT: " << endl;
    cout << "DecChan=" << this->Tmeme << endl;
    cout << "DcT=" << DcT << endl;

    cout << "\nlep  : " << setw (15) << this->p4_Lep_gen.Px ()
      << setw (15) << this->p4_Lep_gen.Py ()
      << setw (15) << this->p4_Lep_gen.Pz ()
      << setw (15) << this->p4_Lep_gen.E () << "\n" << "antilep  : " << setw (15) << this->p4_ALep_gen.Px ()
      << setw (15) << this->p4_ALep_gen.Py ()
      << setw (15) << this->p4_ALep_gen.Pz ()
      << setw (15) << this->p4_ALep_gen.E () << "\n" << "W-: " << setw (15) << this->p4_Wminus_gen.Px ()
      << setw (15) << this->p4_Wminus_gen.Py ()
      << setw (15) << this->p4_Wminus_gen.Pz ()
      << setw (15) << this->p4_Wminus_gen.E () << "\n" << "W+: " << setw (15) << this->p4_Wplus_gen.Px ()
      << setw (15) << this->p4_Wplus_gen.Py ()
      << setw (15) << this->p4_Wplus_gen.Pz ()
      << setw (15) << this->p4_Wplus_gen.E () << "\n" << "B: " << setw (15) << this->p4_B_gen.Px ()
      << setw (15) << this->p4_B_gen.Py ()
      << setw (15) << this->p4_B_gen.Pz ()
      << setw (15) << this->p4_B_gen.E () << "\n" << "Bbar: " << setw (15) << this->p4_Bbar_gen.Px ()
      << setw (15) << this->p4_Bbar_gen.Py ()
      << setw (15) << this->p4_Bbar_gen.Pz ()
      << setw (15) << this->p4_Bbar_gen.E () << "\n" << "BRad: " << setw (15) << this->p4_BRad_gen.Px ()
      << setw (15) << this->p4_BRad_gen.Py ()
      << setw (15) << this->p4_BRad_gen.Pz ()
      << setw (15) << this->p4_BRad_gen.E () << "\n" << "BbarRad: " << setw (15) << this->p4_BbarRad_gen.Px ()
      << setw (15) << this->p4_BbarRad_gen.Py ()
      << setw (15) << this->p4_BbarRad_gen.Pz ()
      << setw (15) << this->p4_BbarRad_gen.E () << "\n" << "T: " << setw (15) << this->p4_Top_gen.Px ()
      << setw (15) << this->p4_Top_gen.Py ()
      << setw (15) << this->p4_Top_gen.Pz ()
      << setw (15) << this->p4_Top_gen.E () << "\n\n" << "Tbar: " << setw (15) << this->p4_ATop_gen.Px ()
      << setw (15) << this->p4_ATop_gen.Py ()
      << setw (15) << this->p4_ATop_gen.Pz ()
      << setw (15) << this->p4_ATop_gen.E () << "\n\n" << "Nu1: " << setw (15) << this->p4_ANeu_gen.Px ()
      << setw (15) << this->p4_ANeu_gen.Py ()
      << setw (15) << this->p4_ANeu_gen.Pz ()
      << setw (15) << this->p4_ANeu_gen.E () << "\n\n" << "Nu2: " << setw (15) << this->p4_Neu_gen.Px ()
      << setw (15) << this->p4_Neu_gen.Py ()
      << setw (15) << this->p4_Neu_gen.Pz ()
      << setw (15) << this->p4_Neu_gen.E () << "\n\n" << "SumTauNu1: " << setw (15) << this->p4_SumTauANeu_gen.Px ()
      << setw (15) << this->p4_SumTauANeu_gen.Py ()
      << setw (15) << this->p4_SumTauANeu_gen.Pz ()
      << setw (15) << this->p4_SumTauANeu_gen.E () << "\n\n" << "SumTauNu2: " << setw (15) << this->p4_SumTauNeu_gen.Px ()
      << setw (15) << this->p4_SumTauNeu_gen.Py ()
      << setw (15) << this->p4_SumTauNeu_gen.Pz ()
      << setw (15) << this->p4_SumTauNeu_gen.E () << "\n\n";


    int z = 0;
    for (size_t i = 0; i < genParticles->size (); ++i) {
      const GenParticle & paIt = (*genParticles)[i];
      z++;
      paV = paIt.p4 ();
      cout << "pdgId status energy " << paIt.pdgId () << " " << paIt.status () << " " << paV.E () << endl;
      cout << "DAUGHTERS: " << endl;
      for (unsigned int j = 0; j < paIt.numberOfDaughters (); j++) {
        paV = paIt.daughter (j)->p4 ();
        cout << "daughter: " << j << " pdgid,status energy " << paIt.daughter (j)->pdgId () << " " << paIt.daughter (j)->status () << " " << paV.E () << endl;
      }
      if (z == 30)
        break;
    }
  }

  //delete pointers
  lep1 = 0;
  lep2 = 0;
  W1 = 0;
  W2 = 0;
  Nu1 = 0;
  Nu2 = 0;
  Top1 = 0;
  Top2 = 0;
  B1 = 0;
  B2 = 0;
  B1Rad = 0;
  B2Rad = 0;
  TauNu1 = 0;
  TauNu2 = 0;
  TauANu1 = 0;
  TauANu2 = 0;

  delete lep1;
  delete lep2;
  delete W1;
  delete W2;
  delete Nu1;
  delete Nu2;
  delete Top1;
  delete Top2;
  delete B1;
  delete B2;
  delete B1Rad;
  delete B2Rad;
  delete TauNu1;
  delete TauNu2;
  delete TauANu1;
  delete TauANu2;

}
