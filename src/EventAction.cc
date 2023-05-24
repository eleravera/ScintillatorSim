#include "EventAction.hh"

#include <G4SDManager.hh>
#include <G4THitsMap.hh>
#include <G4SystemOfUnits.hh>
#include <G4Event.hh>
#include <G4RunManager.hh>

#include "Analysis.hh"
// #include "EnergyTimeHit.hh" 

using namespace std;

EventAction::EventAction(LabInfo *info_)
    : G4UserEventAction()
{
    info = info_;
}

EventAction::~EventAction(){;}


void EventAction::EndOfEventAction(const G4Event* event)
{
    G4SDManager* sdm = G4SDManager::GetSDMpointer();
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();

    //Get the hit collections: If there is no hit collection, there is nothing to be done
    G4HCofThisEvent* hcofEvent = event->GetHCofThisEvent();
    G4cout << "GetHCofThisEvent: " << hcofEvent->GetNumberOfCollections() << G4endl; 

    if(!hcofEvent) return;


    // The variable fPVC_Id is initialized to -1 (see EventAction.hh) so this block 
    // of code is executed only at the end of the first event. After the first execution 
    // fPVC_Id gets a non-negative value and this block is skipped for all subsequent 
    // events.
    if (fPVC_Id < 0)
    {
      //fPVC_Id = sdm->GetSDMpointer()->GetCollectionID("PVCLayerScorerDetector/energy"); 
      fPVC_Id = sdm->GetCollectionID("PVCLayerScorerDetector/energy"); 
      G4cout << "EventAction: PVCLayer energy scorer ID: " << fPVC_Id << G4endl;
    }

    G4int histogramId = 1;     // Note: We know this but in principle, we should ask

    if (fPVC_Id >= 0)
    {
      /// Get and cast hit collection with energy in absorber
      G4THitsMap<G4double>* hitMap = dynamic_cast<G4THitsMap<G4double>*>(hcofEvent->GetHC(fPVC_Id));
      
      G4cout<< "EventID: "<< event->GetEventID()  << G4endl; 


      if (hitMap){
        G4cout << "sizeof hitMap->GetMap(): " << hitMap->entries() << G4endl;
        G4int myInt  {0};
        for (auto pair : *(hitMap->GetMap()))
            {  
            G4cout << "Entra nel loop " <<myInt<< G4endl;
            G4double energy = *(pair.second);
	        //The positions of the center of the i-th PVC layer are:
            G4double x = info->PVCPos[2]/mm + info->PVCSize[2]*0.5/mm + (pair.first * info->PVCSize[2]/mm);   

            G4cout << "HITMAP: first-> " <<(pair.first)<<" and second->" << *(pair.second) << G4endl; 

            analysis->FillH1(histogramId, x, energy / keV);
            myInt = myInt+1;
            }
      }
    }

    // Hit collections IDs to be looped over ("Don't Repeat Yourself" principle)
    /*vector<G4int> hitCollectionIds = {
        fScintillatorETId, fAbsorberETId
    };
    for (G4int collectionId : hitCollectionIds)
    {
        if (collectionId == -1) continue;
        // Task 4d.2: Get and cast hit collection with EnergyTimeHits
        // EnergyTimeHitsCollection* hitCollection = ...;

        // Task 4d.3: Uncomment the following block
         if (hitCollection)
        {
            for (auto hit: *hitCollection->GetVector())
            {
                // Task 4d.3: Fill ntuple columns (one prepared for you already)
		// Do not forget units of measurements, if you want your numbers 
		// to be stored consistenty in the ntuple
                analysis->FillNtupleDColumn(0, hit->GetDeltaEnergy() / MeV);

                // Task 4d.3: Add the ntuple row
            }
        }
    }*/
}
