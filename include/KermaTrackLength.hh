// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// --------------------------------------------------------------
//                 GEANT 4 - BrachySourceKerma
// --------------------------------------------------------------
//
// Code developed by:  Victor Gabriel Leandro Alves
// Copyright 2008-2017
//    *******************************
//    *                             *
//    *    KermaTrackLength.hh                *
//    *                             *
//    *******************************

#ifndef KermaTrackLength_h
#define KermaTrackLength_h 1

#include "G4THitsMap.hh"
#include "G4VPrimitiveScorer.hh"

//#include <G4NistManager.hh>

class KermaTrackLength : public G4VPrimitiveScorer {
  public: // with description
    KermaTrackLength(G4String name, G4int depth = 0);
    KermaTrackLength(G4String name, const G4String &unit, G4int depth = 0);
    virtual ~KermaTrackLength();

  protected: // with description
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    virtual G4double ComputeVolume(G4Step *aStep, G4int idx);

  public:
    virtual void Initialize(G4HCofThisEvent *);
    virtual void EndOfEvent(G4HCofThisEvent *);
    virtual void clear();
    virtual void DrawAll();
    virtual void PrintAll();
    virtual void SetUnit(const G4String &unit);
    //    G4double calc
    G4double getUen(G4double en);

    G4double calcMassAttenuationTotal(G4double energy, G4Material *mat);

    // TODO Mass energy absorption coefficient prototypes
    G4double calcMassEnergyAbsorptionCoefficient(G4double Z, G4double uTotal);
    G4double calcUenZ19(G4double Z, G4double uTotal);
    G4double calcUenZ1028(G4double Z, G4double uTotal);
    G4double calcUenZ2938(G4double Z, G4double uTotal);
    G4double calcUenZ3970(G4double Z, G4double uTotal);
    G4double calcUenZ7192(G4double Z, G4double uTotal);

  private:
    G4int HCID;
    G4THitsMap<G4double> *EvtMap;
    // nist Material Manager
    //    G4NistManager *_matManager = G4NistManager::Instance();
};
#endif
