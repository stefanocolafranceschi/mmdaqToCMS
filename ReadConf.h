/*
 *  ReadConf.h
 *  mmdaqToCMS
 *
 *  Created by S. Colafranceschi on 22/9/21.
 *  Creative Commons (CC BY), aka do whatever you please
 *
 */

#include <string.h>
#include <iostream>
#include "INIReader.h"

class ReadConf {

public:
    ReadConf() {
    }

    int LoadConfiguration();
    void PrintConfiguration(ReadConf myconfiguration);

    int Position[9], Size[9], Chips[9], apvIndex[25], NumberOfChips, FecID[25], adcCh[25], DetPlane[25];
    std::string InputFile, ReadoutType;
    double Pitch[9];
    bool Flip[25], Verbose;

private:
  int member;
};
