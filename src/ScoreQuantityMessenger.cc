#include "ScoreQuantityMessenger.hh"
#include "KermaTrackLength3D.hh"

ScoreQuantityMessenger::ScoreQuantityMessenger(G4ScoringManager *SManager)
    : G4ScoreQuantityMessenger(SManager) {
    setKermaTrackLengthCmd();
}

ScoreQuantityMessenger::~ScoreQuantityMessenger() {
    delete qKermaTrackLengthCmd;
}

void ScoreQuantityMessenger::setKermaTrackLengthCmd() {

    G4UIparameter *param;
    qKermaTrackLengthCmd =
        new G4UIcommand("/score/quantity/kermaTrackLength", this);
    qKermaTrackLengthCmd->SetGuidance("Kerma Track length scorer.");
    qKermaTrackLengthCmd->SetGuidance(
        "[usage] /score/quantity/kermaTrackLength qname unit");
    qKermaTrackLengthCmd->SetGuidance("  qname  :(String) scorer name");
    qKermaTrackLengthCmd->SetGuidance("  unit   :(String) unit");
    param = new G4UIparameter("qname", 's', false);
    qKermaTrackLengthCmd->SetParameter(param);
    param = new G4UIparameter("unit", 's', true);
    param->SetDefaultValue("Gy");
    qKermaTrackLengthCmd->SetParameter(param);
    //
}

void ScoreQuantityMessenger::SetNewValue(G4UIcommand *command,
                                         G4String newVal) {
    // Get Current Mesh
    auto *fSMan = G4ScoringManager::GetScoringManagerIfExist();
    auto *mesh = fSMan->GetCurrentMesh();
    if (!mesh) {
        G4cerr << "ERROR : No mesh is currently open. Open/create a mesh "
                  "first. Command ignored."
               << G4endl;
        return;
    }
    auto mName = mesh->GetWorldName();

    // Tokens
    G4TokenVec token;
    FillTokenVec(newVal, token);
    if (command == qKermaTrackLengthCmd) {
        if (CheckMeshPS(mesh, token[0])) {
            auto *ps = new KermaTrackLength3D(token[0]);
            ps->SetUnit(token[1]);
            mesh->SetPrimitiveScorer(ps);
        }
    }
}
