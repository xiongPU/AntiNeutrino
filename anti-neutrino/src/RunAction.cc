#include "RunAction.hh"

RunAction::RunAction(DetectorConstruction* det, HistoManager* h):fDetector(det), histo(h)
{ 
//	analysisManager = G4AnalysisManager::Instance();
//	analysisManager->SetVerboseLevel(1); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction(){

//	G4cout<<"Deleting analysis manager...\n";
//	delete analysisManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run)
{

	G4cout << "---------- Run " << run->GetRunID() << " start ----------" << G4endl;

	histo->book();

/*	G4String ext = analysisManager->GetFileType();
	G4String filename = "PTOLEMY-TESHit."+ext;
	G4cout<<"Opening file: "<<filename<<G4endl;
	G4bool fileOpen = analysisManager->OpenFile(filename);

	if (!fileOpen){
		G4cout<< "\n---> HistoManager::book(): cannot open " << filename
        	   << G4endl;
		return;
	}

	analysisManager->CreateNtuple( "PTOLEMY", "Transition_Edge_Sensor");
	analysisManager->CreateNtupleIColumn("electron_hits");
	analysisManager->CreateNtupleDColumn("t");
	analysisManager->CreateNtupleDColumn("x");
	analysisManager->CreateNtupleDColumn("y");
	analysisManager->CreateNtupleDColumn("z");
	analysisManager->CreateNtupleDColumn("dr");
	analysisManager->CreateNtupleDColumn("px");
	analysisManager->CreateNtupleDColumn("py");
	analysisManager->CreateNtupleDColumn("pz");
	analysisManager->CreateNtupleDColumn("dp");
	analysisManager->CreateNtupleDColumn("E");
	analysisManager->CreateNtupleDColumn("dE");
*/	
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);
//	CLHEP::HepRandom::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
	histo->save();
	G4cout<<"Processed "<< aRun->GetNumberOfEvent()<<" events...\n";
//	analysisManager = G4AnalysisManager::Instance();
//	analysisManager->Write();
//	analysisManager->CloseFile();
 
//	CLHEP::HepRandom::showEngineStatus();
}
