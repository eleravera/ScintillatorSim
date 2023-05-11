#ifndef STACKINGACTION_HH
#define STACKINGACTION_HH

#include <G4UserStackingAction.hh>
#include <G4Track.hh>

#include "LabInfo.hh" 

class RunAction;

class StackingAction : public G4UserStackingAction
{
public:
  StackingAction(LabInfo* info, RunAction* const fRunAction);
  virtual ~StackingAction();

  //! Main interface
  G4ClassificationOfNewTrack   ClassifyNewTrack (const G4Track* aTrack);

private: 
  RunAction* fRunAction;
  LabInfo *info; 
};

#endif
