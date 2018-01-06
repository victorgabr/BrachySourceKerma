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
//                 GEANT 4
// --------------------------------------------------------------
//
// Code developed by:  Victor Gabriel Leandro Alves
// Copyright 2008-2017
//    *******************************
//    *                             *
//    *    DetectorConstruction.cc                *
//    *                             *
//    *******************************

#include "DetectorConstruction.hh"
// Scoring
#include <G4PSCylinderSurfaceCurrent.hh>
#include <G4PSEnergyDeposit.hh>
#include <G4PSKermaTrackLength.hh>
#include <G4SDManager.hh>
#include <G4SDParticleFilter.hh> // Geant4 includes

DetectorConstruction::DetectorConstruction(const G4String File)
    : _gdmlFile(File) {
    this->readGDML();
}

DetectorConstruction::~DetectorConstruction() {
    //    if(detectorMessenger) delete detectorMessenger;
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
    // set scoring using aux information fom GDML
    setScoring();
    return fWorldPhysVol;
}

void DetectorConstruction::readGDML() {
    // **** LOOK HERE*** FOR READING GDML FILES
    G4cout << "Reading GDML file: " << _gdmlFile << G4endl;
    parser.Read(_gdmlFile);
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    // Giving World Physical Volume from GDML Parser
    //
    fWorldPhysVol = parser.GetWorldVolume();
}

void DetectorConstruction::setScoring() {
    //--------------------------------------------- ------------------------------
    // Scoring implementation
    //---------------------------------------------------------------------------
    // Detector pointer
    G4MultiFunctionalDetector *detector =
        new G4MultiFunctionalDetector("MyDetector");

    //  SDManager pointer
    G4SDManager *manager = G4SDManager::GetSDMpointer();

    // registering detector
    manager->AddNewDetector(detector);

    ///////////////////////////////////////////////////////////////////////
    // Example how to retrieve Auxiliary Information for sensitive detector
    //
    const G4GDMLAuxMapType *auxmap = parser.GetAuxMap();
    G4cout << "Found " << auxmap->size()
           << " volume(s) with auxiliary information." << G4endl << G4endl;
    for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
            iter != auxmap->end(); iter++) {
        G4cout << "Volume " << ((*iter).first)->GetName()
               << " has the following list of auxiliary information: " << G4endl
               << G4endl;
        for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
                vit != (*iter).second.end(); vit++) {
            G4cout << "--> Type: " << (*vit).type << " Value: " << (*vit).value
                   << G4endl;
        }
    }
    G4cout << G4endl;

    for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
            iter != auxmap->end(); iter++) {
        G4cout << "Volume " << ((*iter).first)->GetName()
               << " has the following list of auxiliary information: " << G4endl
               << G4endl;
        for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
                vit != (*iter).second.end(); vit++) {
            if ((*vit).type == "SensDet") {
                G4cout << "Attaching sensitive detector " << (*vit).value
                       << " to volume " << ((*iter).first)->GetName() << G4endl
                       << G4endl;

                G4VSensitiveDetector *mydet =
                    manager->FindSensitiveDetector("MyDetector");

                if (mydet) {
                    G4LogicalVolume *myvol = (*iter).first;

                    myvol->SetSensitiveDetector(mydet);
                    G4cout << "Sensisite detector was set " << G4endl;

                } else {
                    G4cout << (*vit).value << " detector not found" << G4endl;
                }
            }
        }
    }

    //using implemented track lenght estimator
    G4PSKermaTrackLength *scorer = new G4PSKermaTrackLength("myScorer");

    // Registering scorer
    detector->RegisterPrimitive(scorer);

    G4cout << "Created G4MultiFunctionalDetector named " << detector->GetName()
           << ", and a G4SDParticleFilter "
           << "named " << scorer->GetName() << G4endl;
}
