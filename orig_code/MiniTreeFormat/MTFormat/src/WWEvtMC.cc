#include "../interface/WWEvtMC.h"

using namespace TopTree;


WWEvtMC::WWEvtMC ()
{
  p4_Lep_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_ALep_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);

  Q_Lep_gen = -999.0;
  Q_ALep_gen = -999.0;

  p4_Neu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_ANeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);

  p4_Vminus_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Vplus_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);

  p4_SumTauNeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_SumTauANeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);

}


WWEvtMC::~WWEvtMC ()
{
}

void
WWEvtMC::Reset ()
{
  p4_Lep_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_ALep_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);

  Q_Lep_gen = -999.0;
  Q_ALep_gen = -999.0;

  p4_Neu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_ANeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);

  p4_Vminus_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Vplus_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);

  p4_SumTauNeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_SumTauANeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);

}



void
WWEvtMC::GetWWTruth (edm::Handle < reco::GenParticleCollection > genParticles, bool verbose)
{

  this->Reset ();

  double Qlep1 = 0;
  double Qlep2 = 0;
  const Candidate *lep1 = 0;
  const Candidate *lep2 = 0;
  const Candidate *V1 = 0;
  const Candidate *V2 = 0;
  const Candidate *Nu1 = 0;
  const Candidate *Nu2 = 0;
  const Candidate *TauNu1 = 0;
  const Candidate *TauNu2 = 0;
  const Candidate *TauANu1 = 0;
  const Candidate *TauANu2 = 0;

  math::XYZTLorentzVector paV;


  //---------------------------------------------------------------------------
  //                 decay channel: (DecChan)
  //---------------------------------------------------------------------------


// Define the type of WW events final states
//
//   TMEME variable means :
//
//  T = # of taus from V decays
//  M = # of muons from V->tau->muon
//  E = # of electrons from V->tau->electron
//  M = # of muons from V->muon
//  E = # of electrons from V->electron
//

  int tmeme = 0;

  for (size_t i = 0; i < genParticles->size (); ++i) {
    const GenParticle & paIt = (*genParticles)[i];
    const Candidate *V = &(*genParticles)[i];

    // not necessary since not extended to WZ && ZZ
    if (abs (paIt.pdgId ()) != 24 && abs (paIt.pdgId ()) != 24)
      continue;

    if (paIt.status () != 3)
      continue;

    for (unsigned int j = 0; j < paIt.numberOfDaughters (); j++) {
      if (abs (paIt.daughter (j)->pdgId ()) == 12 || abs (paIt.daughter (j)->pdgId ()) == 14 || abs (paIt.daughter (j)->pdgId ()) == 16) {
//     // finding nue/antinue ( t->V->nue )
//     //--------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 12) {
          if (paIt.daughter (j)->pdgId () == -12) {
            Nu1 = paIt.daughter (j);
          }
          if (paIt.daughter (j)->pdgId () == 12) {
            Nu2 = paIt.daughter (j);
          }
        }
//     // finding numu/antinumu ( t->V->numu )
//     //------------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 14) {
          if (paIt.daughter (j)->pdgId () == -14) {
            Nu1 = paIt.daughter (j);
          }
          if (paIt.daughter (j)->pdgId () == 14) {
            Nu2 = paIt.daughter (j);
          }
        }
//     // finding nutau/antinutau ( t->V->nutau )
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

//     // finding electron/positron ( t->V->e )
//     //--------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 11) {
          tmeme += 1;
          if (paIt.daughter (j)->pdgId () == 11) {
            V1 = &(*genParticles)[i];
            lep1 = paIt.daughter (j);
            Qlep1 = (paIt.daughter (j)->pdgId () == 11) ? -1 : 1;
          }
          if (paIt.daughter (j)->pdgId () == -11) {
            V2 = &(*genParticles)[i];
            lep2 = paIt.daughter (j);
            Qlep2 = (paIt.daughter (j)->pdgId () == 11) ? -1 : 1;
          }
        }
//     // finding muon/antimuon ( t->V->muon )
//     //-------------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 13) {
          tmeme += 10;
          if (paIt.daughter (j)->pdgId () == 13) {
            V1 = &(*genParticles)[i];
            lep1 = paIt.daughter (j);
            Qlep1 = (paIt.daughter (j)->pdgId () == 13) ? -1 : 1;
          }
          if (paIt.daughter (j)->pdgId () == -13) {
            V2 = &(*genParticles)[i];
            lep2 = paIt.daughter (j);
            Qlep2 = (paIt.daughter (j)->pdgId () == 13) ? -1 : 1;
          }
        }
//     // finding tau/antitau ( t->V->tau )
//     //-------------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 15) {
          tmeme += 10000;
          if (paIt.daughter (j)->pdgId () == 15) {
            V1 = &(*genParticles)[i];
            lep1 = paIt.daughter (j);
            Qlep1 = (paIt.daughter (j)->pdgId () == 15) ? -1 : 1;
          }
          if (paIt.daughter (j)->pdgId () == -15) {
            V2 = &(*genParticles)[i];
            lep2 = paIt.daughter (j);
            Qlep2 = (paIt.daughter (j)->pdgId () == 15) ? -1 : 1;
          }
        }


        int indxtau = -1;
        for (unsigned int k = 0; k < V->numberOfDaughters (); k++) {
          if (abs (V->daughter (k)->pdgId ()) == 15)
            indxtau = k;
        }
        while (indxtau >= 0) {
          if (!V)
            std::cout << "NULL " << std::endl;
          bool FoundTau = false;
          for (unsigned int k = 0; k < V->numberOfDaughters (); k++) {
            if (abs (V->daughter (k)->pdgId ()) == 24 || abs (V->daughter (k)->pdgId ()) == 24)
              continue;
            if (abs (V->daughter (k)->pdgId ()) == 11 || abs (V->daughter (k)->pdgId ()) == 13) {
              if (abs (V->daughter (k)->pdgId ()) == 11)
                tmeme += 100;
              if (abs (V->daughter (k)->pdgId ()) == 13)
                tmeme += 1000;
              indxtau = -1;
            }
            if (abs (V->daughter (k)->pdgId ()) == 15) {
              indxtau = k;
              FoundTau = true;
            }

            if (abs (V->pdgId ()) == 15) {
              if (V->daughter (k)->pdgId () == 16) {
                TauNu1 = V->daughter (k);
              }
              else if (V->daughter (k)->pdgId () == -16) {
                TauANu1 = V->daughter (k);
              }
              else if (V->daughter (k)->pdgId () == 12 || V->daughter (k)->pdgId () == 14) {
                TauNu2 = V->daughter (k);
              }
              else if (V->daughter (k)->pdgId () == -12 || V->daughter (k)->pdgId () == -14) {
                TauANu2 = V->daughter (k);
              }
            }
          }
          if (FoundTau) {
            V = V->daughter (indxtau);
          }
          else {
            indxtau = -1;
          }
        }
      }
    }
    delete V;

  }

  std::string DcT = "";
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
//  this->WWDecayType = DcT;


  // lep, antilepton
  //-----------------------------
  if (lep1) {
    paV = lep1->p4 ();
    this->p4_Lep_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
    // this->QLep_gen = Qlep1;
    // this->mc_LepID  = lep1->pdgId();
  }

  if (lep2) {
    paV = lep2->p4 ();
    this->p4_ALep_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
    // this->QALep_gen = Qlep2;
    // this->mc_ALepID  = lep2->pdgId();
  }

  if (Nu1) {
    paV = Nu1->p4 ();
    this->p4_ANeu_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (Nu2) {
    paV = Nu2->p4 ();
    this->p4_Neu_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (V1) {
    paV = V1->p4 ();
    this->p4_Vminus_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
  }

  if (V2) {
    paV = V2->p4 ();
    this->p4_Vplus_gen.SetPxPyPzE (paV.Px (), paV.Py (), paV.Pz (), paV.E ());
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
    std::cout << "\nlep  : " << std::setw (15) << this->p4_Lep_gen.Px ()
      << std::setw (15) << this->p4_Lep_gen.Py ()
      << std::setw (15) << this->p4_Lep_gen.Pz ()
      << std::setw (15) << this->p4_Lep_gen.E () << "\n" << "antilep  : " << std::setw (15) << this->p4_ALep_gen.Px ()
      << std::setw (15) << this->p4_ALep_gen.Py ()
      << std::setw (15) << this->p4_ALep_gen.Pz ()
      << std::setw (15) << this->p4_ALep_gen.E () << "\n" << "V-: " << std::setw (15) << this->p4_Vminus_gen.Px ()
      << std::setw (15) << this->p4_Vminus_gen.Py ()
      << std::setw (15) << this->p4_Vminus_gen.Pz ()
      << std::setw (15) << this->p4_Vminus_gen.E () << "\n" << "V+: " << std::setw (15) << this->p4_Vplus_gen.Px ()
      << std::setw (15) << this->p4_Vplus_gen.Py ()
      << std::setw (15) << this->p4_Vplus_gen.Pz ()
      << std::setw (15) << this->p4_Vplus_gen.E () << "\n" << "Nu1: " << std::setw (15) << this->p4_ANeu_gen.Px ()
      << std::setw (15) << this->p4_ANeu_gen.Py ()
      << std::setw (15) << this->p4_ANeu_gen.Pz ()
      << std::setw (15) << this->p4_ANeu_gen.E () << "\n" << "Nu2: " << std::setw (15) << this->p4_Neu_gen.Px ()
      << std::setw (15) << this->p4_Neu_gen.Py ()
      << std::setw (15) << this->p4_Neu_gen.Pz ()
      << std::setw (15) << this->p4_Neu_gen.E () << "\n" << "SumTauNu1: " << std::setw (15) << this->p4_SumTauANeu_gen.Px ()
      << std::setw (15) << this->p4_SumTauANeu_gen.Py ()
      << std::setw (15) << this->p4_SumTauANeu_gen.Pz ()
      << std::setw (15) << this->p4_SumTauANeu_gen.E () << "\n" << "SumTauNu2: " << std::setw (15) << this->p4_SumTauNeu_gen.Px ()
      << std::setw (15) << this->p4_SumTauNeu_gen.Py ()
      << std::setw (15) << this->p4_SumTauNeu_gen.Pz ()
      << std::setw (15) << this->p4_SumTauNeu_gen.E () << "\n";


    int z = 0;
    for (size_t i = 0; i < genParticles->size (); ++i) {
      const GenParticle & paIt = (*genParticles)[i];
      z++;
      paV = paIt.p4 ();
      std::cout << "pdgId status energy " << paIt.pdgId () << " " << paIt.status () << " " << paV.E () << std::endl;
      std::cout << "DAUGHTERS: " << std::endl;
      for (unsigned int j = 0; j < paIt.numberOfDaughters (); j++) {
        paV = paIt.daughter (j)->p4 ();
        std::cout << "daughter: " << j << " pdgid,status energy " << paIt.daughter (j)->pdgId () << " " << paIt.daughter (j)->status () << " " << paV.E () << std::endl;
      }
      if (z == 30)
        break;
    }
  }
  lep1 = 0;
  lep2 = 0;
  V1 = 0;
  V2 = 0;
  Nu1 = 0;
  Nu2 = 0;
  TauNu1 = 0;
  TauNu2 = 0;
  TauANu1 = 0;
  TauANu2 = 0;

  delete lep1;
  delete lep2;
  delete V1;
  delete V2;
  delete Nu1;
  delete Nu2;
  delete TauNu1;
  delete TauNu2;
  delete TauANu1;
  delete TauANu2;
}
