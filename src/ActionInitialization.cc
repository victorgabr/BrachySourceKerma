//
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
// Author: S. Guatelli, susanna@uow.edu.au
// Extended by Victor Alves

#include "ActionInitialization.hh"
#include "G4RunManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "ScoreQuantityMessenger.hh"
#include <G4ScoringManager.hh>
#include <ScoreWriter.hh>

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
    // In MT mode, to be clearer, the RunAction class for the master thread
    // might be
    // different than the one used for the workers.
    // This RunAction will be called before and after starting the
    // workers.
    // Virtual method to be implemented by the user to instantiate user run
    // action
    // class object to be used by G4MTRunManager. This method is not invoked in
    // the sequential mode. The user should not use this method to instantiate
    // user action classes rather than user run action.
}

void ActionInitialization::Build() const {
    // Initialize the primary particles
    PrimaryGeneratorAction *primary = new PrimaryGeneratorAction();
    SetUserAction(primary);
    // Setting up messenger class at a tread-local class
    // refer to
    // https://twiki.cern.ch/twiki/bin/view/Geant4/QuickMigrationGuideForGeant4V10
    auto *pScoringManager = G4ScoringManager::GetScoringManagerIfExist();
    // using  own messenger
    // Overwrite the default output file with user-defined one
    auto *pSQM = new ScoreQuantityMessenger(pScoringManager);
    pScoringManager->SetScoreWriter(new ScoreWriter());
}
