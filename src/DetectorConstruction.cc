#include "DetectorConstruction.hh"


#include <G4LogicalVolume.hh> 
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4Box.hh>
#include <G4Orb.hh>
#include <G4SDManager.hh>
#include <G4UserLimits.hh>
#include <G4MultiFunctionalDetector.hh>
#include <G4VPrimitiveScorer.hh>
#include <G4PSEnergyDeposit.hh>

#include <sstream>

using namespace std;

DetectorConstruction::DetectorConstruction(LabInfo *info_)
: G4VUserDetectorConstruction(){
    info = info_; 
}


DetectorConstruction::~DetectorConstruction(){ 
}


G4VPhysicalVolume* DetectorConstruction::Construct(){

    // Boolean variable to switch on/off the check of volume overlaps
    checkOverlaps = true;
   
    // Call functions to construct geometry
    defineMaterials();
    constructMaterialPropertiesTable();
    constructWorld();
    constructScintillator();
    constructPVC();
    //constructOpticalSurfaces();
    return physWorld;
  
}


void DetectorConstruction::defineMaterials() {

    G4NistManager* nist = G4NistManager::Instance();
    
    // check the following website for all the elements and materials available in the G4 database:
    // http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html)
    
    // NIST Elements (required for PVT)
    C  = nist->FindOrBuildElement("C");
    H  = nist->FindOrBuildElement("H"); 
    O  = nist->FindOrBuildElement("O");
    
    // NIST Materials
    Air      = nist->FindOrBuildMaterial("G4_AIR");
  
    // EJ200
    EJ200 = new G4Material("EJ200", 1.023*g/cm3, 2); //eljen technology 1.023
    G4int natoms;
    EJ200->AddElement(H, natoms = 524); // su G4 nist database
    EJ200->AddElement(C, natoms = 475); 
    
    //PVC
    PVC = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE"); // density 1.3, Eion = 108 eV

}


void DetectorConstruction::constructMaterialPropertiesTable() {
  
    G4double photon_energies[] = {info->enRange[0]*eV, info->enRange[1]*eV};
    const G4int num_ph_en = sizeof(photon_energies)/sizeof(G4double);

    // Air
    G4double air_rindex[]  = {1.0, 1.0};
    G4MaterialPropertiesTable* air_MPT = new G4MaterialPropertiesTable();
    air_MPT->AddProperty("RINDEX", photon_energies, air_rindex, num_ph_en);
    Air->SetMaterialPropertiesTable(air_MPT);
    
    // EJ200
    G4double ej200_rindex [] = {1.58, 1.58}; 
    G4double ej200_absLength[] = {380.*cm, 380.*cm};
    G4MaterialPropertiesTable* ej200_MPT = new G4MaterialPropertiesTable();
    ej200_MPT->AddProperty("RINDEX", photon_energies, ej200_rindex, num_ph_en);       
    ej200_MPT->AddProperty("ABSLENGTH", photon_energies, ej200_absLength, num_ph_en);
    EJ200->SetMaterialPropertiesTable(ej200_MPT);
  
    G4double enPlastic[] = {2.40*eV,3.10*eV};
    G4double emSpectrumEJ200[] = {0.5, 0.5};
    const G4int nrEJ200 = sizeof(enPlastic)/sizeof(G4double);
    assert(sizeof(enPlastic) == sizeof(emSpectrumEJ200));
    ej200_MPT->AddProperty("SCINTILLATIONCOMPONENT1", enPlastic, emSpectrumEJ200, nrEJ200);    
    ej200_MPT->AddConstProperty("SCINTILLATIONYIELD", 10./keV); 
    ej200_MPT->AddConstProperty("RESOLUTIONSCALE",    1);
    ej200_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1",   2.1*ns);
    ej200_MPT->AddConstProperty("SCINTILLATIONRISETIME1", 0.9*ns);   
    //EJ200->GetIonisation()->SetBirksConstant(3.00e-3*cm/MeV); // <------------- UPDATE

    // print tables of material properties
    G4cout << "----- Material properties table printed by DetectorConstruction: -----" << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    G4cout << "----- End on material properties table -----" << G4endl;
    G4cout << G4endl;
    
}


void DetectorConstruction::constructWorld() {
 
    solidWorld = new G4Box("World", 0.5*info->worldSize[0]*mm, 0.5*info->worldSize[1]*mm, 0.5*info->worldSize[2]*mm);  
    logicWorld = new G4LogicalVolume(solidWorld, Air, "World");            
    physWorld  = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);        

}

void DetectorConstruction::constructScintillator() {
  
    solidScint = new G4Box("Scintillator", 0.5*info->scintSize[0]*mm, 0.5*info->scintSize[1]*mm, 0.5*info->scintSize[2]*mm);  
    logicScint = new G4LogicalVolume(solidScint, EJ200, "Scintillator");            
    physScint  = new G4PVPlacement(0, G4ThreeVector(info->scintPos[0]*mm, info->scintPos[1]*mm, info->scintPos[2]*mm), logicScint, "Scintillator", logicWorld, false, 0, checkOverlaps);  
    
    fStepLimit = new G4UserLimits(info->stepLimiter);
    logicScint->SetUserLimits(fStepLimit);

    G4VisAttributes* blue = new G4VisAttributes(G4Colour::Blue());
    blue->SetVisibility(true);
    blue->SetForceSolid(true);
    logicScint->SetVisAttributes(blue);
    
}


void DetectorConstruction::constructPVC() {
    solidPVC = new G4Box("PVCLayer", 0.5*info->PVCSize[0]*mm, 0.5*info->PVCSize[1]*mm, 0.5*info->PVCSize[2]*mm);  
    logicPVC = new G4LogicalVolume(solidPVC, PVC, "PVCLayer");            
    fStepLimit = new G4UserLimits(info->stepLimiter);
    logicPVC->SetUserLimits(fStepLimit);

    G4VisAttributes* grey = new G4VisAttributes( G4Colour::Gray());
    grey->SetVisibility(true);
    grey->SetForceSolid(true);
    logicPVC->SetVisAttributes(grey);
    //logicPVC->SetVisAttributes( G4VisAttributes::GetInvisible() );

    G4double minX =  info->PVCSize[2] / 2;


    vector<G4ThreeVector> PVCLayerPositions;
    for (int i = 0; i < info->PVCNumOfLayer; i++)
    {
        PVCLayerPositions.push_back({info->PVCPos[0]*mm, info->PVCPos[1]*mm, minX + i * info->PVCSize[2]*mm});
        //scintillatorPositions.push_back({minX + (i * 2 + 1) * info->PVCthickness, 0, 0});
        //physPVC  = new G4PVPlacement(0, G4ThreeVector(info->PVCPos[0]*mm, info->PVCPos[1]*mm, info->PVCPos[2]*mm), logicPVC, "PVC", logicWorld, false, 0, checkOverlaps); 
    }

    for (int i = 0; i < info->PVCNumOfLayer; i++)
    {
        ostringstream aName; aName << "PVCLayer" << i;
        //new G4PVPlacement(nullptr, PVCLayerPositions[i],
		//	  logicPVC, aName.str(), logicWorld, 0, i, checkOverlaps);
        physPVC  = new G4PVPlacement(nullptr, PVCLayerPositions[i], logicPVC, aName.str(), logicWorld, false, 0, checkOverlaps);
    }
}

// Task 4c.1: Include the proper header for the multi-functional detector

// Task 4c.1: Include the proper header for energy deposit primitive scorer

void DetectorConstruction::ConstructSDandField()
{
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    sdManager->SetVerboseLevel(2);

    G4MultiFunctionalDetector* PVCLayerDetector = new G4MultiFunctionalDetector("PVCLayer");
    G4VPrimitiveScorer* PVCScorer = new G4PSEnergyDeposit("energy"); 
    PVCLayerDetector->RegisterPrimitive(PVCScorer);

    //Assign multi-functional detectors to the logical volumes and register them to the SDmanager
    SetSensitiveDetector("PVCLayer", PVCLayerDetector);
    sdManager->AddNewDetector(PVCLayerDetector);

    // Task 4d.1: Comment out the attachment of previous sensitive detectors
    // Task 4d.1: Create and assign the custom sensitive detector. Do not forget to register them 
    //  to the SDmanager
    // EnergyTimeSD* absorberET = ...
}
