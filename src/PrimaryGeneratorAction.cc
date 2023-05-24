#include "PrimaryGeneratorAction.hh"

#include <G4ParticleTable.hh>
#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
#include <Randomize.hh>
#include <G4GeneralParticleSource.hh>

using namespace std;

PrimaryGeneratorAction::PrimaryGeneratorAction(LabInfo *info_)  
: G4VUserPrimaryGeneratorAction()
{
    info = info_;

    fGPS = new G4GeneralParticleSource();
    G4ParticleDefinition* electron = G4ParticleTable::GetParticleTable()->FindParticle("e-");

    fGPS->SetParticleDefinition(electron);
    fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(10. * MeV);
    fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0., 0., +1.));
    fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., - 10. * mm));
    fGPS->GetCurrentSource()->GetPosDist()->SetPosDisType("Beam");
    fGPS->GetCurrentSource()->GetPosDist()->SetBeamSigmaInR(0);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fGPS;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  /*if (fGPS->GetParticleDefinition()->GetParticleName() == "opticalphoton"){
      G4ThreeVector normal (1., 0., 0.);
      G4ThreeVector kphoton = fGPS->GetParticleMomentumDirection();
      G4ThreeVector product = normal.cross(kphoton);
      G4double modul2       = product*product;
      
      G4ThreeVector e_perpend (0., 0., 1.);
      if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
      G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
      
      G4double angle = G4UniformRand() * 360.0*deg;
      G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
      fGPS->SetParticlePolarization(polar);
   }*/

    fGPS->GeneratePrimaryVertex(anEvent);
    info->event_id++;
    if(info->numOfEvents >= 100){
        G4int percent = info->event_id/(info->numOfEvents/100);
        if(info->event_id%(info->numOfEvents/100) == 0){ // %
            G4cout << percent << "%" << G4endl;
        }
    }

    info->particleName = fGPS->GetParticleDefinition()->GetParticleName();
    info->beamEnergy   = fGPS->GetParticleEnergy()/keV;
    info->beamPos[0]   = fGPS->GetParticlePosition().x()/mm;
    info->beamPos[1]   = fGPS->GetParticlePosition().y()/mm;
    info->beamPos[2]   = fGPS->GetParticlePosition().z()/mm;
    info->beamDirection[0]   = fGPS->GetParticleMomentumDirection().x();
    info->beamDirection[1]   = fGPS->GetParticleMomentumDirection().y();
    info->beamDirection[2]   = fGPS->GetParticleMomentumDirection().z();
}
