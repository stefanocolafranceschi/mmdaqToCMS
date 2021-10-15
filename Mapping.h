/*
 *  Mapping.h
 *  mmdaqToCMS
 *
 *  Created by S. Colafranceschi on 22/9/21.
 *  Creative Commons (CC BY), aka do whatever you please
 *
 */

/**
 This routing delivers the strip number of a specific readout

 @param  Readout A String that describes the type of Readout in use
 
 @param  chNo APV Channel number

 @return Physical strip number

*/
Int_t EtaAdd(std::string Readout, Int_t chNo) {
    int add = 0;
    if ( Readout == "CMSGE21" ) {
        if((chNo%2)==1){
            add = 1;
        }
	else {
            add = 0;
        }
    }
    return add;
}

Int_t CustomMapping(std::string Readout, Int_t chNo) {
    int strip=0;

    if ( Readout == "CMSGE21" ) {
        if((chNo%2)==1){
            strip = (chNo-1)/2;
        }
        else {
	    strip = chNo/2;
        }
    }

    if ( Readout == "PLAIN" ) {
        strip = chNo;
    }

    return strip ;
}

/**
 This routing flips a Chip (channel by channel)

 @param Input Channel

 @return Flipped Channel

*/
Int_t FlipChannel(Int_t chNo) {
    chNo = 127 - chNo ;
    return chNo;
}
