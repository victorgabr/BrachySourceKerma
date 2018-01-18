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
// Original code from geant4 / examples / extended / runAndEvent / RE03 by
// M.Asai *  and brachytherapy example.
#include "ScoreWriter.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>
#include <map>

#ifdef ANALYSIS_USE
#include "AnalysisManager.hh"
#endif

#include "G4MultiFunctionalDetector.hh"
#include "G4SDParticleFilter.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4VScoringMesh.hh"

// The default output is
// voxelX, voxelY, voxelZ, edep
// The ScoreWriter allows to change the format of the output file.
// in the specific case:
// xx (mm)  yy(mm) zz(mm) edep(MeV)

ScoreWriter::ScoreWriter() : G4VScoreWriter() {}

ScoreWriter::~ScoreWriter() { ; }

void ScoreWriter::DumpQuantityToFile(const G4String &psName,
                                     const G4String &fileName,
                                     const G4String &option) {

    // change the option string into lowercase to the case-insensitive.
    G4String opt = option;
    std::transform(opt.begin(), opt.end(), opt.begin(),
                   (int (*)(int))(tolower));

    // confirm the option
    if (opt.size() == 0)
        opt = "csv";
    if (opt.find("csv") == std::string::npos &&
        opt.find("sequence") == std::string::npos) {
        G4cerr << "ERROR : DumpToFile : Unknown option -> " << option << G4endl;
        return;
    }

    // open the file
    std::ofstream ofile(fileName);
    if (!ofile) {
        G4cerr << "ERROR : DumpToFile : File open error -> " << fileName
               << G4endl;
        return;
    }
    ofile << "# mesh name: " << fScoringMesh->GetWorldName() << G4endl;

    // retrieve the map
    auto fSMap = fScoringMesh->GetScoreMap();

    auto msMapItr = fSMap.find(psName);
    if (msMapItr == fSMap.end()) {
        G4cerr << "ERROR : DumpToFile : Unknown quantity, \"" << psName << "\"."
               << G4endl;
        return;
    }
    // getting primitive scorer name
    auto *score = msMapItr->second->GetMap();
    ofile << "# primitive scorer name: " << msMapItr->first << std::endl;

    // add mesh information
    ofile << "# size: " << fScoringMesh->GetSize() << G4endl;
    // get number of voxels
    ofile << "# Voxels: "
          << "(" << fNMeshSegments[0] << ", " << fNMeshSegments[1] << ", "
          << fNMeshSegments[2] << ")" << G4endl;

    G4double unitValue = fScoringMesh->GetPSUnitValue(psName);
    G4String unit = fScoringMesh->GetPSUnit(psName);
    G4String divisionAxisNames[3];
    fScoringMesh->GetDivisionAxisNames(divisionAxisNames);
    // index order
    ofile << "# i" << divisionAxisNames[0] << ", i" << divisionAxisNames[1]
          << ", i" << divisionAxisNames[2];
    // unit of scored value
    ofile << ", total(value) ";
    if (unit.size() > 0)
        ofile << "[" << unit << "]";
    ofile << ", total(val^2), entry" << G4endl;

    // "sequence" option: write header info
    if (opt.find("sequence") != std::string::npos) {
        ofile << fNMeshSegments[0] << " " << fNMeshSegments[1] << " "
              << fNMeshSegments[2] << G4endl;
    }

    // write quantity
    long count = 0;
    ofile << std::setprecision(16); // for double value with 8 bytes

    for (int x = 0; x < fNMeshSegments[0]; x++) {
        for (int y = 0; y < fNMeshSegments[1]; y++) {
            for (int z = 0; z < fNMeshSegments[2]; z++) {
                G4int idx = GetIndex(x, y, z);

                if (opt.find("csv") != std::string::npos)
                    ofile << x << "," << y << "," << z << ",";

                auto value = score->find(idx);
                if (value == score->end()) {
                    ofile << 0. << "," << 0. << "," << 0;
                } else {
                    ofile << (value->second->sum_wx()) / unitValue << ","
                          << (value->second->sum_wx2()) / unitValue / unitValue
                          << "," << value->second->n();
                }

                if (opt.find("csv") != std::string::npos) {
                    ofile << G4endl;
                } else if (opt.find("sequence") != std::string::npos) {
                    ofile << " ";
                    if (count++ % 5 == 4)
                        ofile << G4endl;
                }

#ifdef ANALYSIS_USE
                // score z position
                if (value != score->end()) {
                    G4int numberOfVoxel_x = fNMeshSegments[0];
                    G4int numberOfVoxel_y = fNMeshSegments[1];
                    G4int numberOfVoxel_z = fNMeshSegments[2];
                    // TODO implement getting voxel widthb
                    G4double voxelWidth = 2 * mm;
                    G4double xx =
                        (-numberOfVoxel_x + 1 + 2 * x) * voxelWidth / 2;
                    G4double yy =
                        (-numberOfVoxel_y + 1 + 2 * y) * voxelWidth / 2;
                    G4double zz =
                        (-numberOfVoxel_z + 1 + 2 * z) * voxelWidth / 2;

                    if (zz > -174 * mm && zz < -170 * mm) {

                        auto *analysis = AnalysisManager::GetInstance();

                        analysis->FillH2WithEnergyDeposition(
                            xx, yy, (value->second->sum_wx()));
                    }
                }
#endif

            } // z
        }     // y
    }         // x
    ofile << std::setprecision(6);

    // close the file
    ofile.close();
}
