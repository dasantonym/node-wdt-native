//
// Created by anton on 21.03.15.
//

#ifndef _OPENCV_CVMOTION_H_
#define _OPENCV_CVMOTION_H_

#include "OpenCV.h"

class cvMotion : public node::ObjectWrap {
public:

    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static NAN_METHOD(New);
    cvMotion();

    JSFUNC(Reset);
    JSFUNC(Process);

};


#endif //_OPENCV_CVMOTION_H_
