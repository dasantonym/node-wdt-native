//
// Created by anton on 21.03.15.
//

#ifndef _OPENCV_CVCAMSHIFT_H_
#define _OPENCV_CVCAMSHIFT_H_


#include "OpenCV.h"

class cvCamShift: public node::ObjectWrap {
public:

    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static NAN_METHOD(New);
    cvCamShift();

    JSFUNC(Process);

};


#endif //_OPENCV_CVCAMSHIFT_H_
