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
Int_t CustomMapping(std::string Readout, Int_t chNo) {

    if ( Readout == "CMSGEM" ) {
        if((chNo%2)==1){
            chNo= 127-(chNo-1)/2;
        }
        else {
	    chNo = chNo/2; //63 - (chNo/2);
        }
    }

    if ( Readout == "PLAIN" ) {
        chNo = chNo;
    }

    return chNo ;
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
