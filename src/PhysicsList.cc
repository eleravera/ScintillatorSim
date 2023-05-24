#include "PhysicsList.hh"
#include <G4EmExtraPhysics.hh>
#include <G4EmStandardPhysics.hh>
#include <G4ProductionCutsTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4StepLimiterPhysics.hh>
#include <G4EmStandardPhysics_option4.hh>

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  // Standard EM physics 
  RegisterPhysics(new G4EmStandardPhysics());
  //RegisterPhysics(new G4EmExtraPhysics());

  //G4VModularPhysicsList* physicsList = new QGSP_BERT;
  //RegisterPhysics(new G4StepLimiterPhysics()); // add step limiter
  //ReplacePhysics(new G4EmStandardPhysics_option4()); // replace EM physics option (try option3 and option4)
  //physicsList->ReplacePhysics(new G4EmLivermorePhysics()); // replace EM physics with Livermore
  //physicsList->RegisterPhysics(new G4OpticalPhysics(info->optPhysVerb)); // add optical physics to list
  // options 3 and 4 are more accurate than 1 and 2 for low energy
  // they have the same models for gammas
  // for electrons, option4 is the most accurate because it includes single coulomb scattering, and the models for msc are different in opt3 and 4
  
}


void PhysicsList::SetCuts()
{
  // The method SetCuts() is mandatory in the interface. Here, one just use 
  // the default SetCuts() provided by the base class.
  G4VUserPhysicsList::SetCuts();
  

   PhysicsList::SetCutValue(0.05* mm, "e-");

  // Task 3c.1: Temporarily update the production cuts table energy range
  // G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV,100.*GeV);  
    
  // In addition, dump the full list of cuts for the materials used in 
  // the setup
  DumpCutValuesTable();
}
