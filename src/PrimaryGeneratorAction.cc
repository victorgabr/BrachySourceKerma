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
//    *    PrimaryGeneratorAction.cc                *
//    *                             *
//    *******************************

#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"

#include "Randomize.hh"
#include "globals.hh"
//**************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <iostream.h>
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <math.h>
//**************************************************************************

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    G4int n_particulas = 1;
    particleGun = new G4ParticleGun(n_particulas);
    // Definindo a tabela de particulas
    G4ParticleTable *tabela = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particula = tabela->FindParticle("gamma");
    particleGun->SetParticleDefinition(particula);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *umEvento) {
    // Fundamentals of the Monte Carlo method for neutral and charged particle
    // transport Alex F Bielajew

    G4double R, radius, theta, x, y, z;

    // TODO replace hardcoded source geometry on particle generation
    R = 0.325 * mm; // souce radius

    // Sorteio

    radius = R * sqrt(G4UniformRand()) * mm;
    theta = 2 * pi * G4UniformRand();
    z = (G4UniformRand() - 0.5) * 3.6 * mm;
    x = radius * cos(theta) * mm;
    y = radius * sin(theta) * mm;

    G4ThreeVector position(x, y, z);
    particleGun->SetParticlePosition(position);

    // Sorteio da Direção qualquer em um cilindro com coordenadas r, theta, z

    G4double a, b, c, phi, cosTheta, sinTheta;

    phi = 2. * 3.14 * G4UniformRand();
    cosTheta = 1. - 2. * G4UniformRand();
    sinTheta = sqrt(1. - cosTheta * cosTheta);

    a = sinTheta * cos(phi);
    b = sinTheta * sin(phi);
    c = cosTheta;

    G4ThreeVector direction(a, b, c);
    particleGun->SetParticleMomentumDirection(direction);

    G4double primaryParticleEnergy;
    G4int i = 0;

    // Iridium 192 spectra
    G4double E1[] = {0 * keV,   8 * keV,   9 * keV,   10 * keV,  11 * keV,
                     12 * keV,  13 * keV,  14 * keV,  62 * keV,  63 * keV,
                     64 * keV,  65 * keV,  66 * keV,  67 * keV,  72 * keV,
                     73 * keV,  74 * keV,  75 * keV,  76 * keV,  77 * keV,
                     78 * keV,  79 * keV,  137 * keV, 202 * keV, 206 * keV,
                     284 * keV, 296 * keV, 309 * keV, 317 * keV, 375 * keV,
                     417 * keV, 469 * keV, 485 * keV, 490 * keV, 589 * keV,
                     605 * keV, 613 * keV, 885 * keV
                    };

    G4double soma[] = {0.0,      0.003506, 0.007012, 0.010518, 0.014024, 0.017530,
                       0.021036, 0.024542, 0.032102, 0.039662, 0.047222, 0.054781,
                       0.062341, 0.069901, 0.071431, 0.072960, 0.074490, 0.076020,
                       0.077549, 0.079079, 0.080608, 0.082138, 0.082904, 0.084956,
                       0.099046, 0.100172, 0.222245, 0.349395, 0.699747, 0.702798,
                       0.705607, 0.907863, 0.921234, 0.923041, 0.941997, 0.976525,
                       0.998781, 1.000000
                      };

    G4double r = G4UniformRand();

    while (r >= soma[i]) {
        i++;
    }

    G4double e = E1[i];
    primaryParticleEnergy = e;
    particleGun->SetParticleEnergy(primaryParticleEnergy);
    particleGun->GeneratePrimaryVertex(umEvento);
}
