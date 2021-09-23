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

   recofile = new TFile("output.root", "RECREATE");

   TCluster = new TTree("TCluster","Cluster Branch");
   TCluster->Branch("evtID",&evtID,"evtID/I");
   TCluster->Branch("nclust",&nclust,"nclust/I");
   TCluster->Branch("clustPos",clustPos,"clustPos[nclust]/F");
   TCluster->Branch("clustSize",clustSize,"clustSize[nclust]/I");
   TCluster->Branch("clustADCs",clustADCs,"clustADCs[nclust]/F");
   TCluster->Branch("clustTimebin",clustTimebin,"clustTimebin[nclust]/I");
   TCluster->Branch("detID",detID,"detID[nclust]/I");
   TCluster->Branch("planeID",planeID,"planeID[nclust]/I");

   THit = new TTree("THit","Hit Branch");
   THit->Branch("evtID",&evtID,"evtID/I");
   THit->Branch("nCh",&nCh,"nCh/I");
   THit->Branch("strip",strip,"strip[nCh]/I");
   THit->Branch("hitTimebin",hitTimebin,"hitTimebin[nCh]/I");
   THit->Branch("detID",detID,"detID[nCh]/I");
   THit->Branch("planeID",planeID,"planeID[nCh]/F");
   THit->Branch("adc0",adc0,"adc0[nCh]/S");
   THit->Branch("adc1",adc1,"adc1[nCh]/S");
   THit->Branch("adc2",adc2,"adc2[nCh]/S");
   THit->Branch("adc3",adc3,"adc3[nCh]/S");
   THit->Branch("adc4",adc4,"adc4[nCh]/S");
   THit->Branch("adc5",adc5,"adc5[nCh]/S");
   THit->Branch("adc6",adc6,"adc6[nCh]/S");
   THit->Branch("adc7",adc7,"adc7[nCh]/S");
   THit->Branch("adc8",adc8,"adc8[nCh]/S");
   THit->Branch("adc9",adc9,"adc9[nCh]/S");
   THit->Branch("adc10",adc10,"adc10[nCh]/S");
   THit->Branch("adc11",adc11,"adc11[nCh]/S");
   THit->Branch("adc12",adc12,"adc12[nCh]/S");
   THit->Branch("adc13",adc13,"adc13[nCh]/S");
   THit->Branch("adc14",adc14,"adc14[nCh]/S");
   THit->Branch("adc15",adc15,"adc15[nCh]/S");
   THit->Branch("adc16",adc16,"adc16[nCh]/S");
   THit->Branch("adc17",adc17,"adc17[nCh]/S");
   THit->Branch("adc18",adc18,"adc18[nCh]/S");
   THit->Branch("adc19",adc19,"adc19[nCh]/S");
   THit->Branch("adc20",adc20,"adc20[nCh]/S");
   THit->Branch("adc21",adc21,"adc21[nCh]/S");
   THit->Branch("adc22",adc22,"adc22[nCh]/S");
   THit->Branch("adc23",adc23,"adc23[nCh]/S");
   THit->Branch("adc24",adc24,"adc24[nCh]/S");
   THit->Branch("adc25",adc25,"adc25[nCh]/S");
   THit->Branch("adc26",adc26,"adc26[nCh]/S");
   THit->Branch("adc27",adc27,"adc27[nCh]/S");
    
   std:string ConfigFile = "ConfigFile.cfg";
   myconfiguration.InputFile = ConfigFile;
   myconfiguration.LoadConfiguration();     
   myconfiguration.PrintConfiguration(myconfiguration);

   if (myconfiguration.Verbose) cout << "Initialization done" << endl;
}

void apv_raw::SlaveBegin(TTree * /*tree*/) {
   TString option = GetOption();
}

Bool_t apv_raw::Process(Long64_t entry) {

   fReader.SetLocalEntry(entry);
   evtID = *evt;
   nCh = srsChan.GetSize();
   if (myconfiguration.Verbose) cout << "\n\nProcessing event " << entry << ", found " << nCh << " fired channels." << endl;
   
   nclust = 0;

   // Translating APV channel into Physical Mapping --------------
   for (int i = 0; i < nCh; i++){
       srsChanMapped[i] = CustomMapping(myconfiguration.ReadoutType, srsChan[i]);
   }
   // ------------------------------------------------------------


   //
   // Flip the chip (if needed) and get the Sector of the chip
   //
   // Check every hit in the event, if an hit belongs to a chip that needs to be flipped
   // utilizes the FlipChannel routine. Use both srsFec and srsChip to determine the ETA Sector
   //
   for (int i = 0; i < nCh; i++) {

       bool Flip = false;
       for (int k = 0; k < myconfiguration.NumberOfChips; k++){
           if ( (srsFec[i] == myconfiguration.FecID[k] ) && ( srsChip[i] == myconfiguration.adcCh[k]) ) {
               Flip = myconfiguration.Flip[k];
               Sector[i] = myconfiguration.DetPlane[k];
               Position[i] = myconfiguration.Position[ Sector[i] ];
               Offset[i] = myconfiguration.apvIndex[k] * 128;
               StripPitch[i] = static_cast<double>(myconfiguration.Size[k]) / (128*myconfiguration.Chips[k]);
           }
       }

       if (Flip) {
           srsChanTemp[i] = Offset[i] + FlipChannel(srsChanMapped[i]);
       }
       else {
	   srsChanTemp[i] = Offset[i] + srsChanMapped[i];
       }
   }   
   for (int i = 0; i < nCh; i++) {        
       srsChanMapped[i]=srsChanTemp[i];
   }
   // ----------------------------------------------------------


   //
   // Clusterization algorithm
   //
   for (int i = 0; i < nCh; i++) {

       if (i>=1) {
           if ( (srsChanMapped[i]-srsChan[i-1]) > 1) {
               clustPos[nclust-1]= (float)clustPos[nclust-1]/clustSize[nclust-1];
               nclust++;
               clustPos[nclust-1] = StripPitch[i] * srsChanMapped[i];
               clustSize[nclust-1] = 1;

               clustADCs[nclust-1] = raw_q[i][0];
               for (int z = 1; z < 26; z++) {
                   clustADCs[nclust-1] = clustADCs[nclust-1] + raw_q[i][z];
               }
               clustTimebin[nclust-1] = t_max_q[i];
           }
           else {
               clustPos[nclust-1] = clustPos[nclust-1] + StripPitch[i] * srsChanMapped[i];
               clustSize[nclust-1]++;

               for (int z = 0; z < 26; z++) {
                   clustADCs[nclust-1] = clustADCs[nclust-1] + raw_q[i][z];
       	       }
               clustTimebin[nclust-1] = clustTimebin[nclust-1] + t_max_q[i];
               if (i==(nCh-1)) clustPos[nclust-1]=(float)clustPos[nclust-1]/clustSize[nclust-1];
           }
       }
       else {
           nclust = 1;
           clustPos[nclust-1] = StripPitch[i] * srsChanMapped[i];
           clustSize[nclust-1] = 1;
           
           clustADCs[nclust-1] = raw_q[i][0];
           for (int z = 1; z < 26; z++) {
               clustADCs[nclust-1] = clustADCs[nclust-1] + raw_q[i][z];
           }
           clustTimebin[nclust-1] = t_max_q[i];
       }

       // Verbose Printout of Mapped channels and raw strips
       if (myconfiguration.Verbose) if (i==0) cout << "srsChanMapped = ";
       if (myconfiguration.Verbose) if (i>=0) cout << srsChanMapped[i] << " ";
       if (myconfiguration.Verbose) if (i>=0) cout << "(" << srsChan[i] << ") ";
       if (myconfiguration.Verbose) if (i>=0) cout << Sector[i] << " ";
       if (myconfiguration.Verbose) if (i>=0) cout << " Plane Pos.=" << Position[i] << ", ";

       // Assigning THit Branches
       strip[i] = srsChanTemp[i];           //Straight from hits: srsChan[i];
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
       hitTimebin[i] = t_max_q[i];
       detID[i] = srsChip[i];
       planeID[i] = Position[i];
   }
   if (myconfiguration.Verbose) cout << "Number of Clusters = " << nclust << endl;


   //
   // Tree filling
   //
   THit->Fill();
   TCluster->Fill();

   for (int i = 0; i < nclust; i++){
       detID[i] = srsChip[i];
       if (myconfiguration.Verbose) cout << "Position Cluster["<<i<<"]=" << clustPos[i] << " "; 
       if (myconfiguration.Verbose) cout << "Cluster Size["<<i<<"]=" << clustSize[i] << " ";
   }

   return kTRUE;
}

void apv_raw::SlaveTerminate() {
}

void apv_raw::Terminate() {
   if (myconfiguration.Verbose) cout << "\n\nProcess terminated succesfully." << endl;

   //THit->Print();
   //TCluster->Print();
   THit->Write();
   TCluster->Write();   
   recofile->Close();
}
