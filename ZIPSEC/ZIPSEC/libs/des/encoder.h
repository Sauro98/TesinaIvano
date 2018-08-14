#ifndef ENCODER_H
#define ENCODER_H

#include "permutations.h"
#include "key_manager.h"
#include "secure_boxes.h"

#define get_sixs_bits(orig, step) (((orig) >> (42 -6 * step)) & 0x3F)



class Encoder {
public:
    explicit Encoder(ulong key);

    Encoder();

    ulong encode(ulong to_encode,bool rev);
private:
    Key_manager k_manager;
};



typedef ulong (Encoder::*EncoderFunc)(ulong what);

#endif
