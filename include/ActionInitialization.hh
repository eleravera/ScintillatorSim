#ifndef ACTION_INITIALIZATION_HH
#define ACTION_INITIALIZATION_HH

#include <G4VUserActionInitialization.hh>

#include "LabInfo.hh"

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization(LabInfo* info);  
    virtual ~ActionInitialization();

    void Build() const override;
    void BuildForMaster() const override;  

private:
    LabInfo *info;  
};

#endif