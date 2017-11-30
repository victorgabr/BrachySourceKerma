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
//    *    G4PSKermaTrackLength.hh                *
//    *                             *
//    *******************************

#ifndef G4PSKermaTrackLength_h
#define G4PSKermaTrackLength_h 1

#include "G4THitsMap.hh"
#include "G4VPrimitiveScorer.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring track length.
//
//
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
// Modified: 2007-02-02 Tsukasa ASO, Add MultiplyKineticEnergy()
//                                  and DivideByVelocity().
//
//
//
///////////////////////////////////////////////////////////////////////////////

class G4PSKermaTrackLength : public G4VPrimitiveScorer {
public:  // with description
    G4PSKermaTrackLength(G4String name, G4int depth = 0);
    virtual ~G4PSKermaTrackLength();

    inline void Weighted(G4bool flg = true) {
        weighted = flg;
    }
    // Multiply track weight

    inline void MultiplyKineticEnergy(G4bool flg = true) {
        multiplyKinE = flg;
    }
    // Multiply Kinetic Energy

    inline void DivideByVelocity(G4bool flg = true) {
        divideByVelocity = flg;
    }
    // Divide by velocity

protected:  // with description
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
    virtual void Initialize(G4HCofThisEvent*);
    virtual void EndOfEvent(G4HCofThisEvent*);
    virtual void clear();
    virtual void DrawAll();
    virtual void PrintAll();
    G4double getUen(G4double en);

private:
    G4int HCID;
    G4THitsMap<G4double>* EvtMap;
    G4bool weighted;
    G4bool multiplyKinE;
    G4bool divideByVelocity;
};
#endif
