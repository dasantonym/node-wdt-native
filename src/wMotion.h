//
// Created by anton on 21.03.15.
//

#ifndef _OPENCV_WMOTION_H_
#define _OPENCV_WMOTION_H_

#include "OpenCV.h"

class wMotion : public node::ObjectWrap {
public:

    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static NAN_METHOD(New);
    wMotion();

    JSFUNC(Reset);
    JSFUNC(Process);

};


#endif //_OPENCV_WMOTION_H_
