//
// Created by Anton on 24/04/15.
//

#ifndef OPENCV_CVMULTIPLYS_H
#define OPENCV_CVMULTIPLYS_H


#include "OpenCV.h"

class cvMultiplyS : public node::ObjectWrap {
public:

    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static NAN_METHOD(New);
    cvMultiplyS();

    JSFUNC(Process);

};


#endif //OPENCV_CVMULTIPLYS_H
