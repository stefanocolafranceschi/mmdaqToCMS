/*
 *  apv_raw.C
 *  mmdaqToCMS
 *
 *  Created by S. Colafranceschi on 22/9/21.
 *  Creative Commons (CC BY), aka do whatever you please
 *
 */

#define apv_raw_cxx
#include "ReadConf.h"
#include "Mapping.h"
#include "Color.h"
#include <unistd.h>

/**
 The class definition in apv_raw.h has been generated automatically
 by the ROOT utility TTree::MakeSelector(). This class is derived
 from the ROOT class TSelector.
 The following methods are defined in this file:
    Begin():        called every time a loop on the tree starts,
                    a convenient place to create your histograms.
    SlaveBegin():   called after Begin(), when on PROOF called only on the
                    slave servers.
    Process():      called for each event, in this function you decide what
                    to read and fill your histograms.
    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
                    called only on the slave servers.
    Terminate():    called at the end of the loop on the tree,
                    a convenient place to draw/fit your histograms.

 To use this file, try the following session on your Tree T:

 root> T->Process("apv_raw.C")
 root> T->Process("apv_raw.C","some options")
 root> T->Process("apv_raw.C+")

*/

#include "apv_raw.h"
#include <TH2.h>
#include <TStyle.h>

void apv_raw::Begin(TTree * /*tree*/) {
 
   TString option = GetOption();


   std:string ConfigFile = "ConfigFile.cfg";
   myconfiguration.InputFile = ConfigFile;
   myconfiguration.LoadConfiguration();     
   myconfiguration.PrintConfiguration(myconfiguration);

   recofile = new TFile(myconfiguration.OutputFile.c_str(), "RECREATE");

   TCluster = new TTree("TCluster","Cluster Branch");
   TCluster->Branch("evtID",&evtID,"evtID/I");
   TCluster->Branch("nclust",&nclust,"nclust/I");
   TCluster->Branch("clustPos",clustPos,"clustPos[nclust]/F");
   TCluster->Branch("clustSize",clustSize,"clustSize[nclust]/I");
   TCluster->Branch("clustADCs",clustADCs,"clustADCs[nclust]/F");
   TCluster->Branch("clustTimebin",clustTimebin,"clustTimebin[nclust]/I");
   TCluster->Branch("detID",detID,"detID[nclust]/I");
   TCluster->Branch("planeID",planeID,"planeID[nclust]/F");

   THit = new TTree("THit","Hit Branch");
   THit->Branch("evtID",&evtID,"evtID/I");
   THit->Branch("nch",&nch,"nch/I");
   THit->Branch("hitDistance",&hitDistance,"hitDistance/I");
   THit->Branch("strip",strip,"strip[nch]/I");
   THit->Branch("hitTimebin",hitTimebin,"hitTimebin[nch]/I");
   THit->Branch("detID",detID,"detID[nch]/I");
   THit->Branch("planeID",planeID,"planeID[nch]/F");
   THit->Branch("adc0",adc0,"adc0[nch]/S");
   THit->Branch("adc1",adc1,"adc1[nch]/S");
   THit->Branch("adc2",adc2,"adc2[nch]/S");
   THit->Branch("adc3",adc3,"adc3[nch]/S");
   THit->Branch("adc4",adc4,"adc4[nch]/S");
   THit->Branch("adc5",adc5,"adc5[nch]/S");
   THit->Branch("adc6",adc6,"adc6[nch]/S");
   THit->Branch("adc7",adc7,"adc7[nch]/S");
   THit->Branch("adc8",adc8,"adc8[nch]/S");
   THit->Branch("adc9",adc9,"adc9[nch]/S");
   THit->Branch("adc10",adc10,"adc10[nch]/S");
   THit->Branch("adc11",adc11,"adc11[nch]/S");
   THit->Branch("adc12",adc12,"adc12[nch]/S");
   THit->Branch("adc13",adc13,"adc13[nch]/S");
   THit->Branch("adc14",adc14,"adc14[nch]/S");
   THit->Branch("adc15",adc15,"adc15[nch]/S");
   THit->Branch("adc16",adc16,"adc16[nch]/S");
   THit->Branch("adc17",adc17,"adc17[nch]/S");
   THit->Branch("adc18",adc18,"adc18[nch]/S");
   THit->Branch("adc19",adc19,"adc19[nch]/S");
   THit->Branch("adc20",adc20,"adc20[nch]/S");
   THit->Branch("adc21",adc21,"adc21[nch]/S");
   THit->Branch("adc22",adc22,"adc22[nch]/S");
   THit->Branch("adc23",adc23,"adc23[nch]/S");
   THit->Branch("adc24",adc24,"adc24[nch]/S");
   THit->Branch("adc25",adc25,"adc25[nch]/S");
   THit->Branch("adc26",adc26,"adc26[nch]/S");
   THit->Branch("adc27",adc27,"adc27[nch]/S");
    
   if (myconfiguration.Verbose) cout << "Initialization done" << endl;
}

void apv_raw::SlaveBegin(TTree * /*tree*/) {
   TString option = GetOption();
}

Bool_t apv_raw::Process(Long64_t entry) {

   fReader.SetLocalEntry(entry);
   evtID = *evt;
   nclust = 0;
   nch = srsChan.GetSize();
   if ( nch > MAXHITS) return kTRUE;

   if ( (entry%myconfiguration.MonitorEvents==0) && (myconfiguration.MonitorEvents!=123) ) cout << "Event = " << entry << endl;

   if (myconfiguration.Verbose) cout << BLUE << "\n\nProcessing event " << evtID << " (entry=" << entry << "), found " << nch << " fired channels." << RESET << endl;
   

   // Pedestal is not considered (at the beginning) -------
   for (int i = 0; i < nch; i++){
       aboveTHR[i] = true;
   }

   int kk = 0;
   // Pedestal Removal ------------------------------------
   if (myconfiguration.PedestalRemoval) {

/*
       vector<double> *vsrsChip = NULL;
       vector<double> *vsrsChan = NULL;
       vector<double> *vped_std = NULL;
*/
       TBranch *branchSrsChip = tped->GetBranch("srsChip");
       TBranch *branchSrsChan = tped->GetBranch("srsChan");
       TBranch *branchPedSTD = tped->GetBranch("ped_stdev");
    
       tped->SetBranchStatus("*",0);
       tped->SetBranchStatus("srsChip",1);
       tped->SetBranchStatus("srsChan",1);
       tped->SetBranchStatus("ped_stdev",1);
       tped->SetBranchAddress("srsChip",&vsrsChip);
       tped->SetBranchAddress("srsChan",&vsrsChan);
       tped->SetBranchAddress("ped_stdev",&vped_std);
       branchSrsChip->GetEntry(0);
       branchSrsChan->GetEntry(0);
       branchPedSTD->GetEntry(0);
       // The following printout get 1536 elements as 128*12=1536
       //cout << vsrsChip->size() << " " << vped_std->size() << " " << vsrsChan->size() << endl;

       for (int i = 0; i < nch; i++){

           for(int ik = 0; ik < vped_std->size(); ik++) {
               if (  (vsrsChip->at(ik) == srsChip[i]) && (vsrsChan->at(ik) == srsChan[i])  )  {
                   //if (myconfiguration.Verbose) cout << ", Chip"<<srsChip[i] << " Pedestal_STD of srsChan=" << srsChan[i] << " is " << vped_std->at(srsChan[i]) << endl;                   
                   pedSTDvalue = vped_std->at(srsChan[i]);
               }
           }

           Double_t HighestPeak;

           // For debugging (very slow) --------------------------------------------------
           //TH1D* pedMean = new TH1D("pedMean", "pedMean", 1, 0, 10000);
           //TH1D* pedSTD = new TH1D("pedSTD", "pedSTD", 1, 0, 100);
          
           //TString mySelector = "srsChan==" + std::to_string(srsChan[i]) + "&& srsChip==" + std::to_string(srsChip[i]) + "&& srsFec==" + std::to_string(srsFec[i]);

           //tped->Draw("ped_stdev>>pedSTD", mySelector);
           //tped->Draw("ped_mean>>pedMean", mySelector);

           //if (myconfiguration.Verbose) cout << "Pedestal_Mean= " << pedMean->GetMean();
           //if (myconfiguration.Verbose) cout << ", Pedestal_STD= " << pedSTD->GetMean() << endl;
           // ---------------------------------------------------------------------------------

           adc0[i] = raw_q[i][0];
           HighestPeak = raw_q[i][0];             

           adc1[i] = raw_q[i][1];
	       if ( raw_q[i][1] > HighestPeak ) HighestPeak = raw_q[i][1]; 

           adc2[i] = raw_q[i][2];
           if ( raw_q[i][2] > HighestPeak )	HighestPeak = raw_q[i][2];

           adc3[i] = raw_q[i][3];
           if ( raw_q[i][3] > HighestPeak )	HighestPeak = raw_q[i][3];

           adc4[i] = raw_q[i][4];
           if ( raw_q[i][4] > HighestPeak )	HighestPeak = raw_q[i][4];

           adc5[i] = raw_q[i][5];
           if ( raw_q[i][5] > HighestPeak )	HighestPeak = raw_q[i][5];

           adc6[i] = raw_q[i][6];
           if ( raw_q[i][6] > HighestPeak )	HighestPeak = raw_q[i][6];

           adc7[i] = raw_q[i][7];
           if ( raw_q[i][7] > HighestPeak )	HighestPeak = raw_q[i][7];

           adc8[i] = raw_q[i][8];
           if ( raw_q[i][8] > HighestPeak )	HighestPeak = raw_q[i][8];

           adc9[i] = raw_q[i][9];
           if ( raw_q[i][9] > HighestPeak )	HighestPeak = raw_q[i][9];

           adc10[i] = raw_q[i][10];
           if ( raw_q[i][10] > HighestPeak )	HighestPeak = raw_q[i][10];

           adc11[i] = raw_q[i][11];
           if ( raw_q[i][11] > HighestPeak )	HighestPeak = raw_q[i][11];

           adc12[i] = raw_q[i][12];
           if ( raw_q[i][12] > HighestPeak )	HighestPeak = raw_q[i][12];

           adc13[i] = raw_q[i][13];
           if ( raw_q[i][13] > HighestPeak )	HighestPeak = raw_q[i][13];

           adc14[i] = raw_q[i][14];
           if ( raw_q[i][14] > HighestPeak )	HighestPeak = raw_q[i][14];

           adc15[i] = raw_q[i][15];
           if ( raw_q[i][15] > HighestPeak )	HighestPeak = raw_q[i][15];

           adc16[i] = raw_q[i][16];
           if ( raw_q[i][16] > HighestPeak )	HighestPeak = raw_q[i][16];

           adc17[i] = raw_q[i][17];
           if ( raw_q[i][17] > HighestPeak )	HighestPeak = raw_q[i][17];

           adc18[i] = raw_q[i][18];
           if ( raw_q[i][18] > HighestPeak )	HighestPeak = raw_q[i][18];

           adc19[i] = raw_q[i][19];
           if ( raw_q[i][19] > HighestPeak )	HighestPeak = raw_q[i][19];

           adc20[i] = raw_q[i][20];
           if ( raw_q[i][20] > HighestPeak )	HighestPeak = raw_q[i][20];

           adc21[i] = raw_q[i][21];
           if ( raw_q[i][21] > HighestPeak )	HighestPeak = raw_q[i][21];

           adc22[i] = raw_q[i][22];
           if ( raw_q[i][22] > HighestPeak )	HighestPeak = raw_q[i][22];

           adc23[i] = raw_q[i][23];
           if ( raw_q[i][23] > HighestPeak )	HighestPeak = raw_q[i][23];

           adc24[i] = raw_q[i][24];
           if ( raw_q[i][24] > HighestPeak )	HighestPeak = raw_q[i][24];

           adc25[i] = raw_q[i][25];
           if ( raw_q[i][25] > HighestPeak )	HighestPeak = raw_q[i][25];

           adc26[i] = raw_q[i][26];
           if ( raw_q[i][26] > HighestPeak )	HighestPeak = raw_q[i][26];

           adc27[i] = raw_q[i][27];
           if ( raw_q[i][27] > HighestPeak )	HighestPeak = raw_q[i][27];

           if ( myconfiguration.AnalysisType == "Integral" ) {
               Double_t ChargeIntegral = 0;
               for (int jj = 0; jj < 27; jj++) {
                   ChargeIntegral = ChargeIntegral + raw_q[i][jj];
               }
               if ( (ChargeIntegral/27) > pedSTDvalue ) {
                   srsChanTemp[kk] = srsChan[i];
       	       	   kk++;
               }
           }
           if ( myconfiguration.AnalysisType == "Peak" ) {
               if ( HighestPeak > pedSTDvalue ) {
                   srsChanTemp[kk] = srsChan[i];
                   kk++;
               }
           }
           //delete pedSTD;
           //delete pedMean;

       }
   }
   else {
       for (int i = 0; i < nch; i++){
           srsChanTemp[i] = srsChan[i];
           adc0[i] = raw_q[i][0];
           adc1[i] = raw_q[i][1];
           adc2[i] = raw_q[i][2];
           adc3[i] = raw_q[i][3];
           adc4[i] = raw_q[i][4];
           adc5[i] = raw_q[i][5];
           adc6[i] = raw_q[i][6];
           adc7[i] = raw_q[i][7];
           adc8[i] = raw_q[i][8];
           adc9[i] = raw_q[i][9];
           adc10[i] = raw_q[i][10];
           adc11[i] = raw_q[i][11];
           adc12[i] = raw_q[i][12];
           adc13[i] = raw_q[i][13];
           adc14[i] = raw_q[i][14];
           adc15[i] = raw_q[i][15];
           adc16[i] = raw_q[i][16];
           adc17[i] = raw_q[i][17];
           adc18[i] = raw_q[i][18];
           adc19[i] = raw_q[i][19];
           adc20[i] = raw_q[i][20];
           adc21[i] = raw_q[i][21];
           adc22[i] = raw_q[i][22];
           adc23[i] = raw_q[i][23];
           adc24[i] = raw_q[i][24];
           adc25[i] = raw_q[i][25];
           adc26[i] = raw_q[i][26];
           adc27[i] = raw_q[i][27];
       }
   }
   // -------------------------------------------------------

   if (myconfiguration.PedestalRemoval) nch = kk;

   // Translating APV channel into Physical Mapping --------------
   for (int i = 0; i < nch; i++){
       srsChanMapped[i] = CustomMapping(myconfiguration.ReadoutType, srsChanTemp[i]);
   }
   // ------------------------------------------------------------


   //
   // Flip the chip (if needed) and get the Sector of the chip
   //
   // Check every hit in the event, if an hit belongs to a chip that needs to be flipped
   // utilizes the FlipChannel routine. Use both srsFec and srsChip to determine the ETA Sector
   //
   for (int i = 0; i < nch; i++) {
       if (myconfiguration.Verbose) cout << GREEN << "SRSFEC[" << i << "] = " << srsFec[i] << " SRSCHIP["<<i<<"] = "<< srsChip[i] << RESET << std::endl;

       bool Flip = false;
       for (int k = 1; k <= myconfiguration.NumberOfChips; k++){
           if ( (srsFec[i] == myconfiguration.FecID[k] ) && ( srsChip[i] == myconfiguration.adcCh[k]) ) {
               Flip = myconfiguration.Flip[k];
               // Get the DetectorPlane and consider the Virtual thing
               Sector[i] = myconfiguration.DetPlane[k] + EtaAdd(myconfiguration.ReadoutType, srsChan[i]);
               Position[i] = myconfiguration.Position[ Sector[i] ];
               Offset[i] = myconfiguration.apvIndex[k] * 64;
               StripPitch[i] = static_cast<double>(myconfiguration.Size[ Sector[i] ]) / (128*myconfiguration.Chips[ Sector[i] ]);

               //if (myconfiguration.Chips[k]<=0) StripPitch[i] = 0;
               if (myconfiguration.Verbose) cout << BOLDGREEN << " nch[= " << i << "], Sector = "<< Sector[i];
               if (myconfiguration.Verbose) cout << ", Position = " << Position[i];
               if (myconfiguration.Verbose) cout << ", Offset = "<< Offset[i];
               if (myconfiguration.Verbose) cout << ", Size = " << myconfiguration.Size[ Sector[i] ];
               if (myconfiguration.Verbose) cout << ", Chips = " << myconfiguration.Chips[ Sector[i] ];
               if (myconfiguration.Verbose) cout << ", StripPitch = " << StripPitch[i];
               if (myconfiguration.Verbose) cout << ", above THR = " << aboveTHR[i] << RESET << std::endl;
           }
       }

       if (Flip) {
           srsChanTemp[i] = Offset[i] + FlipChannel(srsChanMapped[i]);
       }
       else {
	   srsChanTemp[i] = Offset[i] + srsChanMapped[i];
       }
   }   
   for (int i = 0; i < nch; i++) {        
       srsChanMapped[i]=srsChanTemp[i];
   }
   // ----------------------------------------------------------


   // X-talk calculation
   for ( int i = 0; i < nch; i++ ) {
       for ( int j = i + 1; j < nch; j++ ) {
           if ( Sector[i] == Sector[j] ) hitDistance[i] = srsChanMapped[i] - srsChanMapped[j];
       }
   }


   //
   // Clusterization algorithm
   //
   for (int i = 0; i < nch; i++) {

       if (i>=1) {
           // Contiguous hit not found
           if ( (Position[i]==Position[i-1]) && ( abs(srsChanMapped[i]-srsChanMapped[i-1]) > 1) ) {

               // Archive the cluster found
               clustPos[nclust-1]= (float)clustPos[nclust-1]/clustSize[nclust-1];
               clustTimebin[nclust-1] = (float)clustTimebin[nclust-1]/clustSize[nclust-1];
               nclust++;

               // Start a new cluster (position, ADC, timing)......
               clustPos[nclust-1] = StripPitch[i] * srsChanMapped[i] - 192*StripPitch[i];
               clustSize[nclust-1] = 1;
               clustPlaneID[nclust-1] = Position[i];
               clustdetID[nclust-1] = srsChip[i];
               clustADCs[nclust-1] = raw_q[i][0];
               clustTimebin[nclust-1] = t_max_q[i];
               
               for (int z = 1; z < 26; z++) {
                   clustADCs[nclust-1] = clustADCs[nclust-1] + raw_q[i][z];
               }
           }
           else {
               clustPos[nclust-1] = clustPos[nclust-1] + StripPitch[i] * srsChanMapped[i] - 192*StripPitch[i];;
               clustTimebin[nclust-1] = clustTimebin[nclust-1] + t_max_q[i];
               clustPlaneID[nclust-1] = Position[i];
               clustdetID[nclust-1] = srsChip[i];
               clustSize[nclust-1]++;


               for (int z = 0; z < 26; z++) {
                   clustADCs[nclust-1] = clustADCs[nclust-1] + raw_q[i][z];
       	       }
               if (i==(nch-1)) {
                    clustPos[nclust-1]=(float)clustPos[nclust-1]/clustSize[nclust-1];
                    clustTimebin[nclust-1] = (float)clustTimebin[nclust-1]/clustSize[nclust-1];
                }

           }
           if (myconfiguration.Verbose) cout <<	RED << std::setw(3) << "srsChanMapped["<< i << "] = " << srsChanMapped[i];
           if (myconfiguration.Verbose) cout << ", apvHit["<< i << "] = " << srsChan[i];
           if (myconfiguration.Verbose) cout << ", StripPitch["<< i << "] = " << StripPitch[i];
           if (myconfiguration.Verbose) cout << " Sector = " << Sector[i] << " ";
           if (myconfiguration.Verbose) cout << " ClusterPosition[" << (nclust-1) << "] = " << clustPos[nclust-1] << RESET << std::endl;
       }

       // Initial seed of the cluster
       if (i==0) {
           nclust = 1;
           clustPlaneID[0] = Position[i];
           clustdetID[0] = srsChip[i];
           clustSize[0] = 1;
           clustPos[0] = StripPitch[i] * srsChanMapped[i] - 192*StripPitch[i];
           clustADCs[0] = raw_q[i][0];
           for (int z = 1; z < 26; z++) {
               clustADCs[0] = clustADCs[0] + raw_q[i][z];
           }
           clustTimebin[0] = t_max_q[i];

           if (myconfiguration.Verbose) cout << RED << std::setw(3) << "srsChanMapped["<< i << "] = " << srsChanMapped[i];
           if (myconfiguration.Verbose) cout << ", apvHit["<< i << "] = " << srsChan[i];
           if (myconfiguration.Verbose) cout << ", StripPitch["<< i << "] = " << StripPitch[i];
           if (myconfiguration.Verbose) cout << " Sector = " << Sector[i] << " ";
           if (myconfiguration.Verbose) cout << " ClusterPosition[" << (nclust-1) << "] = " << clustPos[0] << RESET << std::endl;
       }

       // Assigning ADC Branches
       strip[i] = srsChanMapped[i];           //Straight from hits: srsChanMapped[i];
/*
       adc0[i] = raw_q[i][0];
       adc1[i] = raw_q[i][1];
       adc2[i] = raw_q[i][2];
       adc3[i] = raw_q[i][3];
       adc4[i] = raw_q[i][4];
       adc5[i] = raw_q[i][5];
       adc6[i] = raw_q[i][6];
       adc7[i] = raw_q[i][7];
       adc8[i] = raw_q[i][8];
       adc9[i] = raw_q[i][9];
       adc10[i] = raw_q[i][10];
       adc11[i] = raw_q[i][11];
       adc12[i] = raw_q[i][12];
       adc13[i] = raw_q[i][13];
       adc14[i] = raw_q[i][14];
       adc15[i] = raw_q[i][15];
       adc16[i] = raw_q[i][16];
       adc17[i] = raw_q[i][17];
       adc18[i] = raw_q[i][18];
       adc19[i] = raw_q[i][19];
       adc20[i] = raw_q[i][20];
       adc21[i] = raw_q[i][21];
       adc22[i] = raw_q[i][22];
       adc23[i] = raw_q[i][23];
       adc24[i] = raw_q[i][24];
       adc25[i] = raw_q[i][25];
       adc26[i] = raw_q[i][26];
       adc27[i] = raw_q[i][27];
*/
       hitTimebin[i] = t_max_q[i];
       detID[i] = srsChip[i];
       planeID[i] = Position[i];
   }
   if (myconfiguration.Verbose) cout << std::setw(3) << "Number of Clusters = " << nclust << endl;



   //
   // Tree filling
   //    
   THit->Fill();

   for (int i = 0; i < nclust; i++) {
       detID[i] = clustdetID[i];
       planeID[i] = clustPlaneID[i];
   }
   TCluster->Fill();

   return kTRUE;
}

void apv_raw::SlaveTerminate() {
}

void apv_raw::Terminate() {
   if (myconfiguration.Verbose) cout << "\n\nProcess terminated succesfully." << endl;

   //THit->Print();
   //TCluster->Print();
   if ( myconfiguration.Reco == "All" ) {
       THit->Write();
       TCluster->Write();
cout << "AALL";
   }
   if ( myconfiguration.Reco == "Hit") THit->Write();
   if ( myconfiguration.Reco == "Cluster") TCluster->Write();

   recofile->Close();
}
