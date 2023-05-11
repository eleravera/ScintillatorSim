#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include "G4NistManager.hh"
#include "G4Material.hh" 
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Trd.hh" 
#include "G4LogicalVolume.hh"

#include "LabInfo.hh" 

class G4UserLimits;
class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction(LabInfo* info);
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct() override;


    // Task 1c.1: Uncomment the declaration of this method (also necessary for 4.c and 4.d)
    void ConstructSDandField() override;


private:
    void defineMaterials();
    void constructMaterialPropertiesTable();
    void constructWorld();

    void constructScintillator();
    void constructPVC();
    //void constructOpticalSurfaces();
    
    G4bool checkOverlaps;
    
    G4Element *C, *H, *O, *Si;

    G4Material *Air, *EJ200,*PVC;
     
    G4Box *solidWorld, *solidScint, *solidPVC, *solidSiPM; 
    
    
    G4LogicalVolume  *logicWorld, *logicScint, *logicPVC, *logicSiPM;

    G4VPhysicalVolume *physWorld, *physScint, *physPVC, *physSiPM;
    
    G4OpticalSurface *ScintToLateralWrapping, *SiPMToScint, *ScintToSiPM, *WrapToScint;
    
    G4UserLimits* fStepLimit;            // pointer to user step limits

    LabInfo *info;
};

#endif
