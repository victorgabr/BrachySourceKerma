#ifndef SCOREQUANTITYMESSENGER_HH
#define SCOREQUANTITYMESSENGER_HH

#include "G4ScoreQuantityMessenger.hh"
#include "G4ScoringManager.hh"
class G4ScoringManager;

class ScoreQuantityMessenger : public G4ScoreQuantityMessenger {
  public:
    ScoreQuantityMessenger(G4ScoringManager *SManager);
    ~ScoreQuantityMessenger();

    void SetNewValue(G4UIcommand *command, G4String newVal);

  private:
    G4UIcommand *qKermaTrackLengthCmd;
    void setKermaTrackLengthCmd();
};

#endif // SCOREQUANTITYMESSENGER_HH
