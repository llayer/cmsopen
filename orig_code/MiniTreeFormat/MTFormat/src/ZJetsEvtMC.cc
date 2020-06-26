#include "../interface/ZJetsEvtMC.h"

using namespace TopTree;


ZJetsEvtMC::ZJetsEvtMC ()
{

  p4_Z_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Lep1_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Lep2_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_SumTauNeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_SumTauANeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  Q_Lep1_gen = -999.0;
  Q_Lep2_gen = -999.0;
  Lep1_pdgID = 0;
  Lep2_pdgID = 0;
  Tmeme = -1;
  Zgtobb = -1;
}


ZJetsEvtMC::~ZJetsEvtMC ()
{
}

void
ZJetsEvtMC::Reset ()
{
  p4_Z_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Lep1_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_Lep2_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_SumTauNeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4_SumTauANeu_gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  Q_Lep1_gen = -999.0;
  Q_Lep2_gen = -999.0;
  Lep1_pdgID = 0;
  Lep2_pdgID = 0;
  Tmeme = -1;
  Zgtobb = -1;
  p4_Jets.clear ();
}



void
ZJetsEvtMC::GetZJetPartonTruth (edm::Handle < reco::GenParticleCollection > genParticles, bool verbose)
{

  this->Reset ();

  const reco::Candidate * ZDaughter = 0;
  const reco::Candidate * QQDaughter = 0;;
  const reco::Candidate * Tau = 0;
  const reco::Candidate * TauDaughter = 0;
  const reco::Candidate * TauNu1 = 0;
  const reco::Candidate * TauNu2 = 0;
  const reco::Candidate * TauANu1 = 0;
  const reco::Candidate * TauANu2 = 0;

  int tmeme = 0;

  TLorentzVector v;

  bool Ztobb = false;
  int Nb_B = 0;
  int Nb_Bmax = 0;
  for (reco::GenParticleCollection::const_iterator p = genParticles->begin (); p != genParticles->end (); ++p) {
    Nb_B = 0;
    if (abs (p->pdgId ()) == 21) {
      for (unsigned int d = 0; d < p->numberOfDaughters (); d++) {
        if (abs (p->daughter (d)->pdgId ()) == 5 && p->daughter (d)->status () == 2)
          Nb_B++;
      }
    }
    if (Nb_B >= Nb_Bmax)
      Nb_Bmax = Nb_B;

    if (abs (p->pdgId ()) == 23)
      this->p4_Z_gen.SetPxPyPzE (p->p4 ().Px (), p->p4 ().Py (), p->p4 ().Pz (), p->p4 ().E ());

    if (abs (p->pdgId ()) == 23) {
      for (unsigned int d = 0; d < p->numberOfDaughters (); d++) {
        //ZDaughter = const_cast<reco::Candidate*> (p->daughter(d));
        ZDaughter = p->daughter (d);
        if (abs (ZDaughter->pdgId ()) == 5)
          Ztobb = true;
        if (ZDaughter->status () == 3) {
          if (ZDaughter->pdgId () == 11 || ZDaughter->pdgId () == 13 || ZDaughter->pdgId () == 15) {
            this->p4_Lep1_gen.SetPxPyPzE (ZDaughter->p4 ().Px (), ZDaughter->p4 ().Py (), ZDaughter->p4 ().Pz (), ZDaughter->p4 ().E ());
            this->Q_Lep1_gen = -1;
            this->Lep1_pdgID = ZDaughter->pdgId ();
          }
          if (ZDaughter->pdgId () == -11 || ZDaughter->pdgId () == -13 || ZDaughter->pdgId () == -15) {
            this->p4_Lep2_gen.SetPxPyPzE (ZDaughter->p4 ().Px (), ZDaughter->p4 ().Py (), ZDaughter->p4 ().Pz (), ZDaughter->p4 ().E ());
            this->Q_Lep2_gen = 1;
            this->Lep2_pdgID = ZDaughter->pdgId ();
          }

          if (abs (ZDaughter->pdgId ()) == 11)
            tmeme = tmeme + 1;
          if (abs (ZDaughter->pdgId ()) == 13)
            tmeme = tmeme + 10;

          if (abs (ZDaughter->pdgId ()) == 15) {
            Tau = ZDaughter;
            TauDaughter = ZDaughter->daughter (0);
            while (abs (TauDaughter->pdgId ()) == 15) {
              Tau = TauDaughter;
              //TauDaughter = const_cast<reco::Candidate*> (TauDaughter->daughter(0));
              TauDaughter = TauDaughter->daughter (0);
            }

            if (Tau) {
              bool lepDecay = false;
              for (unsigned int k = 0; k < Tau->numberOfDaughters (); k++) {
                //if      ( Tau->daughter(k)->pdgId() == 16)  { TauNu1  = const_cast<reco::Candidate*> (Tau->daughter(k));
                if (Tau->daughter (k)->pdgId () == 16) {
                  TauNu1 = Tau->daughter (k);
                }
                //else if ( Tau->daughter(k)->pdgId() == -16) { TauANu1 = const_cast<reco::Candidate*> (Tau->daughter(k));
                else if (Tau->daughter (k)->pdgId () == -16) {
                  TauANu1 = Tau->daughter (k);
                }
                else if (Tau->daughter (k)->pdgId () == 12 ||
                         //Tau->daughter(k)->pdgId() == 14)   { TauNu2 = const_cast<reco::Candidate*> (Tau->daughter(k));
                         Tau->daughter (k)->pdgId () == 14) {
                  TauNu2 = Tau->daughter (k);
                }
                else if (Tau->daughter (k)->pdgId () == -12 ||
                         //Tau->daughter(k)->pdgId() == -14)  { TauANu2 = const_cast<reco::Candidate*> (Tau->daughter(k));
                         Tau->daughter (k)->pdgId () == -14) {
                  TauANu2 = Tau->daughter (k);
                }
                else if (abs (Tau->daughter (k)->pdgId ()) == 11) {
                  tmeme = tmeme + 10100;
                  lepDecay = true;
                }
                else if (abs (Tau->daughter (k)->pdgId ()) == 13) {
                  tmeme = tmeme + 11000;
                  lepDecay = true;
                }
              }
              if (lepDecay == false) {
                tmeme = tmeme + 10000;
              }
            }
          }
        }
      }
    }




    if (abs (p->pdgId ()) == 23) {
      for (unsigned int qq = 0; qq < p->mother (0)->numberOfDaughters (); ++qq) {

        //QQDaughter = const_cast<reco::Candidate*> (p->mother(0)->daughter(qq));
        QQDaughter = p->mother (0)->daughter (qq);

        if (abs (QQDaughter->pdgId ()) != 23) {
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

  this->Tmeme = tmeme;

  int Zgtobbdecay = 0;
  if (!Ztobb && Nb_Bmax >= 2) {
    Zgtobbdecay = 1;
  }
  else {
    Zgtobbdecay = 0;
  }
  this->Zgtobb = Zgtobbdecay;

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

  //delete pointers
  ZDaughter = 0;
  delete ZDaughter;
  QQDaughter = 0;
  delete QQDaughter;
  Tau = 0;
  delete Tau;
  TauDaughter = 0;
  delete TauDaughter;
  TauNu1 = 0;
  delete TauNu1;
  TauNu2 = 0;
  delete TauNu2;
  TauANu1 = 0;
  delete TauANu1;
  TauANu2 = 0;
  delete TauANu2;
}
