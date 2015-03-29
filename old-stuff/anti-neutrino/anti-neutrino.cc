#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BERT_HP.hh"
#include "PhysicsList.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
	// Choose the Random engine
	CLHEP::HepRandom::setTheEngine( new CLHEP::RanecuEngine);
  
	// Construct the default run manager
	G4RunManager * runManager = new G4RunManager;

//-------------------- Set mandatory initialization classes

	// Set field map name to load if there is any
	G4String fieldmap = "NULL";
	for(int i=1; i<argc;i++) if(strcmp(argv[i-1],"-fieldmap")==0) fieldmap = argv[i];
	EMfield::GetEMFieldPointer()->SetFieldMapName(fieldmap);

	// Detector construction
	DetectorConstruction* detector;
	detector = new DetectorConstruction;
	runManager->SetUserInitialization( detector);

	// Physics list
	G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
	physicsList->SetVerboseLevel(1);
	runManager->SetUserInitialization(physicsList);
    
	// Primary generator action
	PrimaryGeneratorAction* primary_gen = new PrimaryGeneratorAction();
	runManager->SetUserAction(primary_gen);

//-------------------- Initialize histogram manager

	G4String output_name = "ptolemy_phasespace.root";
	for(int i=1; i<argc;i++) if(strcmp(argv[i-1],"-output")==0) output_name = argv[i];
	HistoManager* histo = new HistoManager(output_name);

//-------------------- Set user action classes

	// Run action
	RunAction* run_action = new RunAction( detector, histo);
	runManager->SetUserAction(run_action);

	// Event action
	EventAction* evt_action = new EventAction(histo);
	runManager->SetUserAction(evt_action);

	// Stepping action
	runManager->SetUserAction(new SteppingAction(run_action, evt_action));     

//-------------------- Initialize G4 Kernal
	// Initialize G4 kernel
	G4cout<<"Initializing run manager..."<<G4endl;
	runManager->Initialize();

//-------------------- Visualization, GUI
  
#ifdef G4VIS_USE
	// Initialize visualization
	G4cout<<"Creating and initializing vis manager..."<<G4endl;
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif

	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
//	UImanager->ApplyCommand("/testpt/stepMax 15 um");
//	runManager->BeamOn(1000);
//	UImanager->ApplyCommand("/tracking/verbose 1");
//	runManager->BeamOn(1);

	G4int num_of_arg = (fieldmap=="NULL")?1:3;
	if (argc!=num_of_arg) {		// batch mode
		G4String command = "/control/execute ";
		G4String fileName;
		for(int i=1; i<argc;i++) if(strcmp(argv[i-1],"-fieldmap")!=0 && strcmp(argv[i],"-fieldmap")!=0 && strcmp(argv[i-1],"-output")!=0 && strcmp(argv[i],"-output")!=0) fileName = argv[i];
		UImanager->ApplyCommand(command+fileName);
	}
	else {	// interactive mode : define UI session
#ifdef G4UI_USE
		G4cout<<"Creating UI executive..."<<G4endl;
		G4UIExecutive* ui = new G4UIExecutive(argc, argv);
		G4String ui_cmd;
#ifdef G4VIS_USE
		ui_cmd = "/control/execute init_vis.mac";
		G4cout<<"Applying command: "<<ui_cmd<<G4endl;
		UImanager->ApplyCommand(ui_cmd); 
#else
		ui_cmd = "/control/execute init.mac";
		G4cout<<"Applying command: "<<ui_cmd<<G4endl;
		UImanager->ApplyCommand(ui_cmd); 	
#endif
		G4cout<<"Starting UI session..."<<G4endl;
		ui->SessionStart();
		G4cout<<"Ending UI session..."<<G4endl;
		delete ui;
#endif
	}
  
#ifdef G4VIS_USE
	G4cout<<"Deleting vis manager...\n";
	delete visManager;
#endif
	G4cout<<"Deleting histogram manager...\n";
	delete histo;

	G4cout<<"Deleting run manager...\n";
	delete runManager;

  return 0;
}
