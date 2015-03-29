#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "QGSP_BERT_HP.hh"
#include "PhysicsList.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
#include "Randomize.hh"

int main(int argc,char** argv)
{
	G4cout<<"\nWelcome to directional anti-neutrino detection!...\n\n";
	G4cout<<"Optional arguments are:\n";
	G4cout<<"===> -nrun\tto specify number of run,\n";
	G4cout<<"===> -output\tto specify the output ROOT file name\n";
	G4cout<<"===> -dz/-spacing\t to specify target thickness/ detector spacing\n";
	G4cout<<"===> To execute a macro, no option argument is required.\n";

//-------------------- Read in possible runtime arguments--------------------------
	G4String fileName = "NULL";
	G4String output_name = "test.root";
	G4int Nofrun = 0;
	G4double tgt_thickness(-1), tgt_spacing(-1);	// target thickness and spacing

        for(int i=1; i<argc;i++){
                if(strcmp(argv[i-1],"-nrun")==0) Nofrun = atoi(argv[i]);
                if(argv[i-1][0]!='-'&& argv[i][0]!='-') fileName = argv[i];
                if(strcmp(argv[i-1],"-output")==0) output_name = argv[i];
		if(strcmp(argv[i-1],"-dz")==0) tgt_thickness = atof(argv[i]);
 		if(strcmp(argv[i-1],"-spacing")==0) tgt_spacing = atof(argv[i]);
       }

//-------------------- Initialization----------------------------------------------
	CLHEP::HepRandom::setTheEngine( new CLHEP::RanecuEngine);
	G4RunManager * runManager = new G4RunManager;

//-------------------- Detector construction---------------------------------------
	DetectorConstruction* detector;
	detector = new DetectorConstruction;
	if(tgt_thickness > 0) DetectorConstruction::GetPointer()->SetThickness(tgt_thickness);
	if(tgt_spacing > 0) DetectorConstruction::GetPointer()->SetGap(tgt_spacing);
	runManager->SetUserInitialization( detector);

//-------------------- Physics list------------------------------------------------
	G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;//PhysicsList;
	physicsList->SetVerboseLevel(1);
	runManager->SetUserInitialization(physicsList);
    
//-------------------- Primary generator action------------------------------------
	PrimaryGeneratorAction* primary_gen = new PrimaryGeneratorAction();
	runManager->SetUserAction(primary_gen);

//-------------------- Initialize histogram manager--------------------------------
	HistoManager* histo = new HistoManager(output_name);

//-------------------- Set user action classes-------------------------------------
	RunAction* run_action = new RunAction( detector, histo);
	runManager->SetUserAction(run_action);

	EventAction* evt_action = new EventAction(histo);
	runManager->SetUserAction(evt_action);

	runManager->SetUserAction(new SteppingAction(run_action, evt_action));     

//-------------------- Initialize G4 Kernal----------------------------------------
	G4cout<<"Initializing run manager..."<<G4endl;
	runManager->Initialize();

//-------------------- Visualization, GUI------------------------------------------
  
#ifdef G4VIS_USE
	G4cout<<"Creating and initializing vis manager..."<<G4endl;
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif

	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	UImanager->ApplyCommand("/tracking/verbose 1");		
//	UImanager->ApplyCommand("/testpt/stepMax 15 um");

//---------------------Beginning run-----------------------------------------------
	if(Nofrun>0){			// if run number is specified, execute run and exit
		runManager->BeamOn(Nofrun);
	}
	else if (fileName!="NULL") {	// batch mode
                G4String command = "/control/execute ";
                UImanager->ApplyCommand(command+fileName);
        }
        else {				// interactive mode : define UI session
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
