#include "G4SystemOfUnits.hh"
#include "QGSP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include <G4RunManagerFactory.hh>
#include "G4UnitsTable.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "LabInfo.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"
#include "Analysis.hh"

using namespace std;

/// Main function that enables to:
/// - run any number of macros (put them as command-line arguments)
/// - start interactive UI mode (no arguments or "-i")

int main(int argc, char** argv)
{
   /* get all information */
    LabInfo *info = NULL;
    if((info = new LabInfo()) == NULL ) {
        G4cerr << "Impossible to create LabInfo" << G4endl;
    }
    
    // ****************************************************************
    //
    //                Set simulation parameters here    
    // 
    // ****************************************************************
    
    info->seed                = (G4int)time(0); // use fixed seed for tests (e.g. 0), random number for simulation, e.g. (G4int)time(0);
    info->optPhysVerb         = 2;  // verbosity level of optical photons physics (0 -> 1)
    info->stepLimiter         = 100.0*um; // to be changed
    
    // World dimensions
    info->worldSize[0]        = 1000.0; // mm
    info->worldSize[1]        = 1000.0; // mm
    info->worldSize[2]        = 1000.0; // mm
    
    // Scintillator dimensions
    info->scintSize[0]        =  1.0; // mm 
    info->scintSize[1]        =  1.0; // mm scintillator thickness
    info->scintSize[2]        =  1.0; // mm bar length

    // Position of Scintillator in the world volume
    info->scintPos[0] 	      =  0.0; // mm
    info->scintPos[1]         =  0.0; // mm
    info->scintPos[2]         =  0.0; // mm  

   // PVC dimensions
    info->PVCSize[0]        =  100.0; // mm 
    info->PVCSize[1]        =  100.0; // mm 
    info->PVCSize[2]        =  1.0; // mm 
    info->PVCNumOfLayer     =  50;

    // Position of PVC in the world volume
    info->PVCPos[0] 	      =  0.0; // mm
    info->PVCPos[1]         =  0.0; // mm
    info->PVCPos[2]         =  0.0; // mm


    info->SiPMThickness     =  10;   //um
    
    
    // Energy range to simulate, optical properties of all materials should be defined in this range
    info->enRange[0]          = 1.90; // eV
    info->enRange[1]          = 3.31; // eV

    // Otical interfaces
    // Wrapping
    info->reflectivityLateral = 0.99; // to be tuned <---------------
    info->scintToLateralWrappingType   = "dielectric_metal";
    info->scintToLateralWrappingModel  = "unified";
    info->scintToLateralWrappingFinish = "polished";

    // Grease -> SiPM
    info->scintToSiPMType   = "dielectric_dielectric";
    info->scintToSiPMModel  = "unified";
    info->scintToSiPMFinish = "polished";    
    
    // Primary beam settings
    info->numOfEvents         = 100;                          // number of primary particles to generate
 
 

    // ****************************************************************
    //
    //                      End of settings    
    // 
    // ****************************************************************
    


  // construct the default run manager
  auto runManager = G4RunManagerFactory::CreateRunManager();
  // set mandatory initialization classes
    G4cout <<"merda"<<G4endl; 

  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new DetectorConstruction(info));
  runManager->SetUserInitialization(new ActionInitialization(info));

  // initialize G4 kernel
  runManager->Initialize();

  G4cout <<"merda"<<G4endl; 


  // get the pointer to the UI manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/run/verbose 1");
  UImanager->ApplyCommand("/event/verbose 1");
  UImanager->ApplyCommand("/tracking/verbose 1");

  //
  G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();


  if ( argc == 1 ) {
    // interactive mode : define UI session
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute macros/init_vis.mac");
    ui->SessionStart();
    delete ui;
  }
  else {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }


  // start a run
  //runManager->BeamOn(info->numOfEvents);

  // job termination
  delete runManager;

return 0;


  
}