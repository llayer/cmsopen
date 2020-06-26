#include "../interface/WJetsEvtMC.h"

using namespace TopTree;


WJetsEvtMC::WJetsEvtMC ()
{
  p4_Lep_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  Q_Lep_gen = -999.0;
  p4_Neu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Wminus_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Wplus_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_SumTauNeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  Tmeme = -1;
  mcLepId = 0;
}


WJetsEvtMC::~WJetsEvtMC ()
{
}

void
WJetsEvtMC::Reset ()
{
  p4_Lep_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  Q_Lep_gen = -999.0;
  p4_Neu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Wminus_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Wplus_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_SumTauNeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  Tmeme = -1;
  mcLepId = 0;
  p4_Jets.clear ();
}


void
WJetsEvtMC::GetWJetPartonTruth (edm::Handle < reco::GenParticleCollection > genParticles, bool verbose)
{

  this->Reset ();

  TLorentzVector v;

#if DEBUG
  PrintList (genParticles, 3);
#endif

  const Candidate *WDaughter = 0;
  const Candidate *TauDaughter = 0;
  const Candidate *Tau = 0;
  const Candidate *QQDaughter = 0;
  const Candidate *TauNu1 = 0;
  const Candidate *TauNu2 = 0;
  int tmeme = 0;

  for (GenParticleCollection::const_iterator p = genParticles->begin (); p != genParticles->end (); ++p) {

    if (p->pdgId () == 24)
      this->p4_Wplus_gen.SetPxPyPzE (p->p4 ().Px (), p->p4 ().Py (), p->p4 ().Pz (), p->p4 ().E ());
    if (p->pdgId () == -24)
      this->p4_Wminus_gen.SetPxPyPzE (p->p4 ().Px (), p->p4 ().Py (), p->p4 ().Pz (), p->p4 ().E ());

    if (abs (p->pdgId ()) == 24) {
      for (unsigned int d = 0; d < p->numberOfDaughters (); d++) {
        WDaughter = p->daughter (d);
        if (WDaughter->status () == 3) {
          if (abs (WDaughter->pdgId ()) == 11 || abs (WDaughter->pdgId ()) == 13 || abs (WDaughter->pdgId ()) == 15) {
            this->p4_Lep_gen.SetPxPyPzE (WDaughter->p4 ().Px (), WDaughter->p4 ().Py (), WDaughter->p4 ().Pz (), WDaughter->p4 ().E ());
            this->Q_Lep_gen = (WDaughter->pdgId () > 0) ? -1 : 1;
            this->mcLepId = WDaughter->pdgId ();
            if (abs (WDaughter->pdgId ()) == 11)
              tmeme = tmeme + 1;
            if (abs (WDaughter->pdgId ()) == 13)
              tmeme = tmeme + 10;
          }

          if (abs (WDaughter->pdgId ()) == 12 || abs (WDaughter->pdgId ()) == 14 || abs (WDaughter->pdgId ()) == 16) {
            this->p4_Neu_gen.SetPxPyPzE (WDaughter->p4 ().Px (), WDaughter->p4 ().Py (), WDaughter->p4 ().Pz (), WDaughter->p4 ().E ());
          }

          if (abs (WDaughter->pdgId ()) == 15) {
            Tau = WDaughter->daughter (0);
            TauDaughter = WDaughter->daughter (0);
            while (abs (TauDaughter->pdgId ()) == 15) {
              Tau = TauDaughter;
              TauDaughter = TauDaughter->daughter (0);
            }

            if (Tau) {
              bool lepDecay = false;
              for (unsigned int k = 0; k < Tau->numberOfDaughters (); k++) {

                if (abs (Tau->daughter (k)->pdgId ()) == 11) {
                  if (verbose)
                    cout << "Electron from tau decay" << endl;
                  tmeme = tmeme + 10100;
                  lepDecay = true;
                }
                else if (abs (Tau->daughter (k)->pdgId ()) == 13) {
                  if (verbose)
                    cout << "Muon from tau decay" << endl;
                  tmeme = tmeme + 11000;
                  lepDecay = true;
                }
                else if (abs (Tau->daughter (k)->pdgId ()) == 16) {
                  TauNu1 = Tau->daughter (k);
                }

                else if (abs (Tau->daughter (k)->pdgId ()) == 12 || abs (Tau->daughter (k)->pdgId () == 14)) {
                  TauNu2 = Tau->daughter (k);
                }


              }
              if (lepDecay == false)
                tmeme = tmeme + 10000;

            }



          }
        }
      }
    }



    if (abs (p->pdgId ()) == 24) {
      for (unsigned int qq = 0; qq < p->mother (0)->numberOfDaughters (); ++qq) {
        //for (unsigned int qq = 0; qq < p->numberOfDaughters(); qq++){

        QQDaughter = p->mother (0)->daughter (qq);

        if (abs (QQDaughter->pdgId ()) != 24) {
          if (verbose) {
            cout << "Test partons " << "\t" << QQDaughter->pdgId ()
              << "\t" << QQDaughter->status ()
              << "\t" << QQDaughter->px ()
              << "\t" << QQDaughter->py ()
              << "\t" << QQDaughter->pz ()
              << "\t" << QQDaughter->mass ()
              << "\t" << QQDaughter->energy () << endl;

          }
          v.SetPxPyPzE (QQDaughter->p4 ().Px (), QQDaughter->p4 ().Py (), QQDaughter->p4 ().Pz (), QQDaughter->p4 ().E ());
          this->p4_Jets.push_back (v);
        }
      }

    }
  }



  if (TauNu1) {
    if (!TauNu2)
      this->p4_SumTauNeu_gen.SetPxPyPzE (TauNu1->p4 ().Px (), TauNu1->p4 ().Py (), TauNu1->p4 ().Pz (), TauNu1->p4 ().E ());
    else
      this->p4_SumTauNeu_gen.SetPxPyPzE (TauNu1->p4 ().Px () + TauNu2->p4 ().Px (), TauNu1->p4 ().Py () + TauNu2->p4 ().Py (), TauNu1->p4 ().Pz () + TauNu2->p4 ().Pz (),
                                             TauNu1->p4 ().E () + TauNu2->p4 ().E ());
  }

  this->Tmeme = tmeme;

  //delete pointers
  WDaughter = 0;
  delete WDaughter;
  TauDaughter = 0;
  delete TauDaughter;
  Tau = 0;
  delete Tau;
  QQDaughter = 0;
  delete QQDaughter;
  TauNu1 = 0;
  delete TauNu1;
  TauNu2 = 0;
  delete TauNu2;

}
