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
//    *    RunAction.c              *
//    *                             *
//    *******************************
#include "RunAction.hh"
#include <assert.h>
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "Run.hh"

RunAction::RunAction() {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    //    sumEAbs = 0.;
    //    Eabs_tally = new G4ConvergenceTester();
}

G4Run* RunAction::GenerateRun() {
    G4cout << "Creating user define run class Run" << G4endl;
    return new Run("MyDetector");
}

//void RunAction::fillPerEvent(G4double EAbs) {
//    // accumulate statistic
//    //    sumEAbs += EAbs;
//    //    sum2EAbs += EAbs * EAbs;

//    //    Eabs_tally->AddScore(EAbs);
//}

void RunAction::EndOfRunAction(const G4Run* aRun) {
    // Print interesting data
    G4cout << "Number of Events Processed:" << aRun->GetNumberOfEvent()
           << " events. " << G4endl;

    const Run* theRun = dynamic_cast<const Run*>(aRun);
    assert(0 != theRun);

    theRun->DumpData();

    G4int NbOfEvents = aRun->GetNumberOfEvent();
    if (NbOfEvents == 0) return;
}
