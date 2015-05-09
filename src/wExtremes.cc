//
// Created by anton on 21.03.15.
//

#include "wExtremes.h"

#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

v8::Persistent<FunctionTemplate> wExtremes::constructor;

void
wExtremes::Init(Handle<Object> target) {
    NanScope();

    //Class
    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(wExtremes::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("wExtremes"));

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(ctor, "process", Process);

    target->Set(NanNew("wExtremes"), ctor->GetFunction());
}

NAN_METHOD(wExtremes::New) {
        NanScope();

        NanReturnValue(args.Holder());
}


wExtremes::wExtremes(): ObjectWrap() {

}

NAN_METHOD(wExtremes::Process) {
        NanScope();

        Matrix *src = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat gray;
        cv::cvtColor(src->mat, gray, CV_BGR2GRAY);

        unsigned char *data = gray.data;
        int w = gray.cols;
        int h = gray.rows;

        char finish=0;
        unsigned char threshold = 128;

        float left, right, top, bottom;

        for( int x=0; x<w && !finish; x++ ) {
            unsigned char *d = &data[ x ];
            for( int y=0; y<h-1 && !finish; y++ ) {
                d+=w;
                if( *d > threshold ) {
                    finish=1;
                    left = (float)x/w;
                }
            }
        }

        finish=0;
        for( int x=w-1; x>=0 && !finish; x-- ) {
            unsigned char *d = &data[ ((h-1)*w)+x ];
            for( int y=0; y<h-1 && !finish; y++ ) {
                d-=w;
                if( *d > threshold ) {
                    finish=1;
                    right = (float)x/w;
                }
            }
        }

        finish=0;
        for( int y=0; y<h-1 && !finish; y++ ) {
            unsigned char *d = &data[ (y*w) ];
            for( int x=0; x<w && !finish; x++ ) {
                d++;
                if( *d > threshold ) {
                    finish=1;
                    top = (float)y/h;
                }
            }
        }

        finish=0;
        for( int y=h-2; y>=0 && !finish; y-- ) {
            unsigned char *d = &data[ (y*w) ];
            for( int x=0; x<w && !finish; x++ ) {
                d++;
                if( *d > threshold ) {
                    finish=1;
                    bottom = (float)y/h;
                }
            }
        }

        v8::Local<v8::Array> arr = NanNew<Array>(4);

        arr->Set(0, NanNew<Number>(left));
        arr->Set(1, NanNew<Number>(right));
        arr->Set(2, NanNew<Number>(top));
        arr->Set(3, NanNew<Number>(bottom));

        NanReturnValue(arr);
}