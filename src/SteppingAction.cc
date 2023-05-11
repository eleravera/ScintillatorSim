#include "SteppingAction.hh"
#include "RunAction.hh"

#include <G4Step.hh>
#include <G4Electron.hh>

SteppingAction::SteppingAction(LabInfo *info_, RunAction* runAction)
    : G4UserSteppingAction(), fRunAction(runAction)
{
    info = info_;
}

SteppingAction::~SteppingAction(){}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  G4VPhysicalVolume* volume = aStep->GetPreStepPoint()->GetTouchable()->GetVolume();

  //   Take care, because this volume might not be available: be sure that the pointer  
  //   "volume" is non-NULL, otherwise  any volume->Get... would cause a crash.
  if ((volume!=NULL) && (volume->GetName() == "PVC_Layer0") && (aStep->GetTrack()->GetParticleDefinition() == G4Electron::Electron()) ) 
    { 
      fRunAction->AddTrackLength(aStep->GetStepLength()); 
    }   

}
