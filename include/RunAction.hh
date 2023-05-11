#ifndef RUNACTION_HH
#define RUNACTION_HH

#include <G4UserRunAction.hh>
#include <G4Run.hh>
#include <G4ParticleDefinition.hh>
#include <G4Accumulable.hh>

#include "LabInfo.hh" 

class G4Timer;
class G4Run;

class RunAction : public G4UserRunAction 
{
public:
  //! constructor 
  RunAction(LabInfo* info);

  //! destructor
  ~RunAction();

  //! Main interface
  void BeginOfRunAction(const G4Run* aRun);
  void EndOfRunAction(const G4Run* aRun);

  void AddSecondary(const G4ParticleDefinition*, G4double energy);
  void AddTrackLength(G4double length);

private: 
  G4Timer* fTimer;
  LabInfo *info;

  G4Accumulable<G4int>    fNGammas;
  G4Accumulable<G4int>    fNElectrons;
  G4Accumulable<G4double> fAverageGammaEnergy;
  G4Accumulable<G4double> fAverageElectronEnergy;
  G4Accumulable<G4double> fTotalTrackLength;

};

#endif
