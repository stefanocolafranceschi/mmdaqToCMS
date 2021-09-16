//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep 15 22:33:34 2021 by ROOT version 6.20/04
// from TTree apv_raw/APVRawData
// found on file: run12.root
//////////////////////////////////////////////////////////

#ifndef apv_raw_h
#define apv_raw_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <vector>

#include <string>



class apv_raw : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<ULong64_t> evt = {fReader, "evt"};
   TTreeReaderValue<UInt_t> error = {fReader, "error"};
   TTreeReaderValue<Int_t> daqTimeSec = {fReader, "daqTimeSec"};
   TTreeReaderValue<Int_t> daqTimeMicroSec = {fReader, "daqTimeMicroSec"};
   TTreeReaderValue<Int_t> srsTimeStamp = {fReader, "srsTimeStamp"};
   TTreeReaderValue<UInt_t> srsTrigger = {fReader, "srsTrigger"};
   TTreeReaderArray<unsigned int> srsFec = {fReader, "srsFec"};
   TTreeReaderArray<unsigned int> srsChip = {fReader, "srsChip"};
   TTreeReaderArray<unsigned int> srsChan = {fReader, "srsChan"};
   TTreeReaderArray<string> mmChamber = {fReader, "mmChamber"};
   TTreeReaderArray<int> mmLayer = {fReader, "mmLayer"};
   TTreeReaderArray<char> mmReadout = {fReader, "mmReadout"};
   TTreeReaderArray<int> mmStrip = {fReader, "mmStrip"};
   TTreeReaderArray<vector<short>> raw_q = {fReader, "raw_q"};
   TTreeReaderArray<short> max_q = {fReader, "max_q"};
   TTreeReaderArray<int> t_max_q = {fReader, "t_max_q"};


   apv_raw(TTree * /*tree*/ =0) { }
   virtual ~apv_raw() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(apv_raw,0);

};

#endif

#ifdef apv_raw_cxx
void apv_raw::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t apv_raw::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef apv_raw_cxx
