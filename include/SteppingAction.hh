#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include <G4UserSteppingAction.hh>

#include "LabInfo.hh" 

class RunAction;
 
class SteppingAction : public G4UserSteppingAction
{ 
public:
    SteppingAction(LabInfo* info, RunAction* fRunAction);
    virtual ~SteppingAction();

    void UserSteppingAction(const G4Step* aStep) override;

private:
    RunAction* fRunAction;
    LabInfo *info; 
};
#endif
