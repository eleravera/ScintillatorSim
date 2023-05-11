#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "StackingAction.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"


ActionInitialization::ActionInitialization(LabInfo *info_)
 : G4VUserActionInitialization(){ 
    info = info_;
}

ActionInitialization::~ActionInitialization(){}


void ActionInitialization::Build() const
{
    RunAction* theRunAction = new RunAction(info);
        
    SetUserAction(theRunAction);
    SetUserAction(new StackingAction(info, theRunAction));
    SetUserAction(new SteppingAction(info, theRunAction));
    SetUserAction(new PrimaryGeneratorAction(info));
    SetUserAction(new EventAction(info));
}

void ActionInitialization::BuildForMaster() const
{
  // By default, don't do anything. This applies only in MT mode:
  SetUserAction(new RunAction(info));
}
