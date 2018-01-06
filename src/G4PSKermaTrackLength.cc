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
// GEANT4 class G4PSTrackLength.hh was extended to calculate Kerma using Linear
// Kerma track-length
// estimator.
//      ref: https://www.ncbi.nlm.nih.gov/pubmed/3626996
//    *******************************
//    *                             *
//    *    G4PSKermaTrackLength.cc                *
//    *                             *
//    *******************************
#include "G4PSKermaTrackLength.hh"
#include <math.h>
#include <stdio.h>
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4UnitsTable.hh"
#include "G4VPVParameterisation.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"

G4PSKermaTrackLength::G4PSKermaTrackLength(G4String name, G4int depth)
    : G4VPrimitiveScorer(name, depth),
      HCID(-1),
      weighted(false),
      multiplyKinE(true),
      divideByVelocity(false) {
    ;
}

G4PSKermaTrackLength::~G4PSKermaTrackLength() {
    ;
}

G4bool G4PSKermaTrackLength::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    G4double trklength = aStep->GetStepLength();

    if (trklength == 0.) return FALSE;

    if (weighted) trklength *= aStep->GetPreStepPoint()->GetWeight();

    // Calculo do Kerma track length estimator

    // Obtendo os volumes dos detectores

    G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetPhysicalVolume();
    G4VPVParameterisation* physParam = physVol->GetParameterisation();
    G4VSolid* solid = 0;

    if (physParam) {  // for parameterized volume
        G4int idx = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(indexDepth);
        solid = physParam->ComputeSolid(idx, physVol);
        solid->ComputeDimensions(physParam, idx, physVol);
    } else {  // for ordinary volume
        solid = physVol->GetLogicalVolume()->GetSolid();
    }

    G4double energy = aStep->GetPreStepPoint()->GetKineticEnergy();
    G4double uen = getUen(energy);

    if (multiplyKinE)
        trklength *= aStep->GetPreStepPoint()->GetKineticEnergy() * uen /
                     (solid->GetCubicVolume());

    //----------------------------------------------------------------------------------------------//

    if (divideByVelocity) trklength /= aStep->GetPreStepPoint()->GetVelocity();

    const G4int index = GetIndex(aStep);
    EvtMap->add(index, trklength);
    return TRUE;
}

G4double G4PSKermaTrackLength::getUen(G4double en) {
    // Hardcoded  Uen data for water
    //  ref: https://physics.nist.gov/PhysRefData/Xcom/html/xcom1.html

    G4double log10e = log10(en);

    // Fitted Uen/r - NIST/Water
    G4double Log10uen =
        (-1.508473990248644 +
         log10e *
         (-3.990441810810287 +
          log10e *
          (-4.939193679834307 +
           log10e *
           (-3.660466557887870 +
            log10e * (-1.679146548266836 +
                      log10e * (-0.3992321053137942 +
                                log10e * -0.02725354161828657)))))) /
        (1.0 +
         log10e *
         (2.527911872893696 +
          log10e *
          (2.797856401815172 +
           log10e *
           (1.700542145298706 +
            log10e * (0.5878067109045983 +
                      log10e * (0.09967938698396550 +
                                log10e * 0.007659305032379002))))));

    G4double uen = pow(10, Log10uen);
    return uen;
}

void G4PSKermaTrackLength::Initialize(G4HCofThisEvent* HCE) {
    EvtMap = new G4THitsMap<G4double>(detector->GetName(), GetName());
    if (HCID < 0) {
        HCID = GetCollectionID(0);
    }
    HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void G4PSKermaTrackLength::EndOfEvent(G4HCofThisEvent*) {
    ;
}

void G4PSKermaTrackLength::clear() {
    EvtMap->clear();
}

void G4PSKermaTrackLength::DrawAll() {
    ;
}

void G4PSKermaTrackLength::PrintAll() {
    G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
    G4cout << " PrimitiveScorer " << GetName() << G4endl;
    G4cout << " Number of entries " << EvtMap->entries() << G4endl;
    std::map<G4int, G4double*>::iterator itr = EvtMap->GetMap()->begin();
    for (; itr != EvtMap->GetMap()->end(); itr++) {
        G4cout << "  copy no.: " << itr->first << "  track length: ";
        if (multiplyKinE && !divideByVelocity) {
            G4cout << *(itr->second) / (mm * MeV) << " mm*MeV";
        } else if (!multiplyKinE && divideByVelocity) {
            G4cout << *(itr->second) * second << " /second";
        } else if (multiplyKinE && divideByVelocity) {
            G4cout << *(itr->second) / MeV * second << " MeV/second";
        } else {
            G4cout << G4BestUnit(*(itr->second), "Length");
        }
        G4cout << G4endl;
    }
}
