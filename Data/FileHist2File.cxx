//read two files and plot histogram of them on one canvas
#include<fstream>
using namespace std;

int FileHist2File(){
   //TString str1="data1.txt";
   //TString str2="data1b.txt";
   TString str1="/home/suerfu/auger_spec.txt";
   TString str2="/home/suerfu/auger_spec_before_fix.txt";
   ifstream f(str1);
   Double_t x, xmin, xmax;
   TString str1_save="/scratch/network/xiong/auger_spect_hist.root";
   TString str2_save="/scratch/network/xiong/auger_spect_hist_before.root";
   Int_t i, n_x, n_bin, n_in1bin=200;
   //TTree *Tr= new TTree("Tr", "Data tree");
   //Tr->Branch("data", &x, "data/D");
   if(f>>x){
       xmin=x;
       xmax=x;
       n_x=1;
       }
   else {
      .std::cout<<"no data in the file"<<endl;
       }
     
   while(f>>x){
      n_x++;
      if(x>xmax){
          xmax=x;
	  }
      else if(x<xmin){
          xmin=x;
	  }
   }
   f.close();
   f.open(str2);
   while(f>>x){
      if(x>xmax){
          xmax=x;
	  }
      else if(x<xmin){
          xmin=x;
	  }
   }
   f.close();
   n_bin=n_x/n_in1bin;
   TH1F *h1= new TH1F("h1", "auger Data file read", n_bin, xmin, xmax);
   TH1F *h2= new TH1F("h2", "auger Data file read", n_bin, xmin, xmax);
   TFile *fsave1 = new TFile(str1_save, "RECREATE");
   f.open(str1);
   //ifstream f2(str1);
   //h1->SetBins(n_bin,h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXMax());
   //h1->SetBins(n_bin, h1->GetMinimum(), h1->GetMaximum());
   while(f>>x){
      h1->Fill(x);
   }
   f.close();
   f.open(str2);
   h1->Write();
   fsave1->Close();
   TFile *fsave2 = new TFile(str2_save, "RECREATE");
   while(f>>x){
      h2->Fill(x);
   }
   f.close();
   h1->GetXaxis()->SetRangeUser(xmin, 1);
   h1->SetLineColor(kRed);
   h1->Draw();
   h2->Draw("same");
   fsave2->Close();
   leg = new TLegend(0.6, 0.7, 0.78, 0.88);
   leg->AddEntry(h1, "Auger Spec", "l");
   leg->AddEntry(h2, "Auger Spec Before Fix", "l");
   leg->Draw();
   //legend has to be written to save, outside this script, it is lost
}
 
        
