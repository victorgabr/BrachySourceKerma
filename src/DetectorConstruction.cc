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
//    *    DetectorConstruction.cc                *
//    *                             *
//    *******************************

#include "DetectorConstruction.hh"

// Geant4 includes
//
#include "G4GeometryManager.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

// Materials
//
#include "G4Material.hh"

// Geometry includes

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"

// GDML parser include
//
#include "G4GDMLParser.hh"
#include "G4GDMLReadStructure.hh"

// Scoring

#include "G4MultiFunctionalDetector.hh"
#include "G4PSCylinderSurfaceFlux.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSKermaTrackLength.hh"
#include "G4SDManager.hh"
#include "G4TransportationManager.hh"
#include "G4VSensitiveDetector.hh"

#include <G4Version.hh>

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "globals.hh"

DetectorConstruction::DetectorConstruction() {
    fReadFile = "Dose_agua_MicroSelectron_V2_gdml";
}

DetectorConstruction::DetectorConstruction(const G4String& File) {
    fReadFile = File;
}

// ----------------------------------------------------------------------------
//
// Destructor
//
DetectorConstruction::~DetectorConstruction() {
    //    if(detectorMessenger) delete detectorMessenger;
}

// ----------------------------------------------------------------------------
//
// Constructs geometries and materials
//
G4VPhysicalVolume* DetectorConstruction::Construct() {
    // Writing or Reading of Geometry using G4GDML

    G4VPhysicalVolume* fWorldPhysVol;

    // **** LOOK HERE*** FOR READING GDML FILES
    //
    parser.Read(fReadFile);

    //    G4cout << *(G4Material::GetMaterialTable()) << G4endl;

    // Giving World Physical Volume from GDML Parser
    //
    fWorldPhysVol = parser.GetWorldVolume();

    fWorldPhysVol->GetLogicalVolume()->SetVisAttributes(
        G4VisAttributes::Invisible);

    //---------------------------------------------------------------------------

    // Implementação das Classes de Scoring

    //---------------------------------------------------------------------------

    // Criando os scorers
    G4MultiFunctionalDetector* detector =
        new G4MultiFunctionalDetector("MyDetector");

    // Ajustando o ponteiro da classe SDManager
    G4SDManager* manager = G4SDManager::GetSDMpointer();

    // Registrando o detector sensitivo no "manager"

    manager->AddNewDetector(detector);

    ///////////////////////////////////////////////////////////////////////
    //
    // Example how to retrieve Auxiliary Information for sensitive detector
    //
    const G4GDMLAuxMapType* auxmap = parser.GetAuxMap();
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

                G4VSensitiveDetector* mydet =
                    manager->FindSensitiveDetector("MyDetector");

                if (mydet) {
                    G4LogicalVolume* myvol = (*iter).first;
                    myvol->SetSensitiveDetector(mydet);
                    G4cout << "Sensisite detector was set " << G4endl;
                } else {
                    G4cout << (*vit).value << " detector not found" << G4endl;
                }
            }
        }
    }

    //    G4PSDoseDeposit* scorer = new G4PSDoseDeposit("myScorer");
    // G4PSEnergyDeposit* scorer = new G4PSEnergyDeposit("myScorer");

    // Primitive scorer para o Kerma track length
    G4PSKermaTrackLength* scorer = new G4PSKermaTrackLength("myScorer");

    // Registrando o Scorer com o Detector...

    detector->RegisterPrimitive(scorer);

    G4cout << "Created G4MultiFunctionalDetector named " << detector->GetName()
           << ", and a G4PSKermaTrackLength "
           << "named " << scorer->GetName() << G4endl;

    return fWorldPhysVol;
}

void DetectorConstruction::SetReadFile(const G4String& File) {
    fReadFile = File;
}
