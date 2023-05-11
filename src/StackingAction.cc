#include "StackingAction.hh"
#include "RunAction.hh"

#include <G4SystemOfUnits.hh>

StackingAction::StackingAction(LabInfo *info_, RunAction* aRunAction) :
  G4UserStackingAction(), fRunAction(aRunAction) {
  info = info_;
  }

StackingAction::~StackingAction(){}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack (const G4Track*
 aTrack)
{    
  // Register only secondaries, i.e. tracks having ParentID > 0
  if (aTrack->GetParentID())
  {
    fRunAction->AddSecondary(aTrack->GetParticleDefinition(),
			     aTrack->GetKineticEnergy());
  }
  // Do not affect track classification. Just return what would have
  // been returned by the base class
  return G4UserStackingAction::ClassifyNewTrack(aTrack);
}
