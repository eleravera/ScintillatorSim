#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh> 

#include "LabInfo.hh"

class G4GeneralParticleSource; 

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction(LabInfo* info);
    virtual ~PrimaryGeneratorAction();
    void GeneratePrimaries(G4Event* anEvent) override;

private:
    //void SetOptPhotonPolar();
    //void SetOptPhotonPolar(G4double);

    G4GeneralParticleSource* fGPS;
    LabInfo *info; 
};

#endif