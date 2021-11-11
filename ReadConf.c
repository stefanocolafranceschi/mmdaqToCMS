/*
 *  ReadConf.c
 *  mmdaqToCMS
 *
 *  Created by S. Colafranceschi on 22/9/21.
 *  Creative Commons (CC BY), aka do whatever you please
 *
 */

#include "ReadConf.h"


/**
 Load a Configuration file that contains detetor geometry and Chip orientations
*/
int ReadConf::LoadConfiguration() {

    INIReader reader(InputFile);

    if (reader.ParseError() != 0) {
        std::cout << "Can't load " << InputFile <<" \n";
        return 1;
    }
    Verbose = reader.GetBoolean("General", "Verbose", false);
    MonitorEvents = reader.GetInteger("General", "MonitorEvents", 100);
    PedestalRemoval = reader.GetBoolean("General", "PedestalRemoval", true); 
    PedestalCut = reader.GetInteger("General", "PedestalCut", 3);
    ReadoutType = reader.Get("General", "ReadoutType", "PLAIN");
    NumberOfChips = reader.GetInteger("General", "NumberOfChips", 0);
    Reco = reader.Get("General", "Reco", "All");
    AnalysisType = reader.Get("General", "AnalysisType", "Integral");
    MaxEvents = reader.GetInteger("General", "MaxEvents", 100000000);
    for (int i = 1; i <= 4; i++) {
        Position[i] = reader.GetInteger("Sector"+std::to_string(i), "Position", 0);
        Size[i] = reader.GetInteger("Sector"+std::to_string(i), "Size", 0);
        Chips[i] = reader.GetInteger("Sector"+std::to_string(i), "Chips", 0);
        Pitch[i] = static_cast<double>(Size[i]) / (128*Chips[i]);
    }

    for (int i = 1; i <= 12; i++) {
        FecID[i] = reader.GetInteger("apv"+std::to_string(i), "FecID", 0);
        adcCh[i] = reader.GetInteger("apv"+std::to_string(i), "adcCh", 0);
        DetPlane[i] = reader.GetInteger("apv"+std::to_string(i), "DetPlane", 0);
        Flip[i] = reader.GetBoolean("apv"+std::to_string(i), "Flip", false);
        apvIndex[i] = reader.GetInteger("apv"+std::to_string(i), "apvIndex", 0);
    }
}


/*
 * Print the Configuration file that was previously loaded
*/
void ReadConf::PrintConfiguration(ReadConf myconfiguration) {
    std::cout << "Config loaded from " << InputFile << std::endl;
    std::cout << "\n\nGENERAL ---------------------------------------------"<< std::endl;

    for (int i = 1; i <= 4; i++) {
        std::cout << "Sector"<< i << " = " << myconfiguration.Position[i]
        << "\tSize = " << myconfiguration.Size[i]
        << "\tNumber of Chips = " << myconfiguration.Chips[i]
        << "\tCalculated pitch = " << myconfiguration.Pitch[i] << std::endl;

    }
    std::cout << "\nAPVs-----------------------------------------------" << std::endl;
    for (int i = 1; i <= 12; i++) {
        std::cout << "FecID"<< i << " = " << myconfiguration.FecID[i]
        << " adcCh = " << myconfiguration.adcCh[i]
        << " DetPlane = " << myconfiguration.DetPlane[i]
        << " Flip = " << myconfiguration.Flip[i]
        << " apvIndex = " << myconfiguration.apvIndex[i] << std::endl;
    }
    std::cout << "--------------------------------------------------------\n\n" << std::endl;

}
