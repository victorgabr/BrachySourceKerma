
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
//    *    BrachySourceKerma.cc                *
//    *                             *
//    *******************************

#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "Randomize.hh"
#include "globals.hh"
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "ScoreQuantityMessenger.hh"
#include <ActionInitialization.hh>
#include <G4MTRunManager.hh>
#include <G4ScoringManager.hh>
#include <ScoreWriter.hh>

int main(int argc, char **argv) {

#ifdef G4MULTITHREADED
    auto *pRunManager = new G4MTRunManager;
    pRunManager->SetNumberOfThreads(4); // Is equal to 2 by default
#else
    auto *pRunManager = new G4RunManager;
#endif

    G4cout << "***********************" << G4endl;
    G4cout << "*** " << G4Random::getTheSeed() << " ***" << G4endl;
    G4cout << "***********************" << G4endl;
    // Access to the Scoring Manager pointer
    // Construct the default Scoring Manager singleton object pointer
    auto *pScoringManager = G4ScoringManager::GetScoringManager();
    auto *pSQM = new ScoreQuantityMessenger(pScoringManager);
    pScoringManager->SetScoreWriter(new ScoreWriter());

    G4String fileName = argv[1];
    auto *detector = new DetectorConstruction(fileName);

    pRunManager->SetUserInitialization(detector);
    pRunManager->SetUserInitialization(new PhysicsList);

    auto *actions = new ActionInitialization();
    pRunManager->SetUserInitialization(actions);

    pRunManager->Initialize();

#ifdef G4VIS_USE
    // Initialize visualization
    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();
#endif

    // Get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

// interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive *ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init.mac");
#else
    UImanager->ApplyCommand("/control/execute vis.mac");
#endif
    ui->SessionStart();
    delete ui;
#endif

// Job termination
// Free the store: user actions, physics_list and detector_description are
// owned and deleted by the run manager, so they should not be deleted
// in the main() program !

#ifdef G4VIS_USE
    delete visManager;
#endif
    delete pRunManager;
    return 0;
}
