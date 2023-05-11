#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include <G4UserEventAction.hh>
#include <globals.hh>

#include "LabInfo.hh" 

class EventAction : public G4UserEventAction
{
public:
    EventAction(LabInfo* info);
    virtual ~EventAction();

    void EndOfEventAction(const G4Event* event) override;

private:
    // Numerical IDs for hit collections (-1 means unknown yet)
    //G4int fScintillatorId { -1 };
    //G4int fAbsorberETId { -1 };
    //G4int fScintillatorETId { -1 }; 

    G4int fPVC_Id { -1 };

    LabInfo *info; 

};

#endif