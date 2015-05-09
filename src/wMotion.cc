//
// Created by anton on 21.03.15.
//

#include "wMotion.h"

#include "Matrix.h"
#include <nan.h>

v8::Persistent<FunctionTemplate> wMotion::constructor;

int buffersSize;
unsigned char *cur;
float *recent;
float *bg;

int motionThreshold, presenceThreshold;
float motionWeight, presenceWeight;

void
wMotion::Init(Handle<Object> target) {
    NanScope();

    //Class
    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(wMotion::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("wMotion"));

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(ctor, "process", Process);

    target->Set(NanNew("wMotion"), ctor->GetFunction());
}

NAN_METHOD(wMotion::New) {
        NanScope();

        NanReturnValue(args.Holder());
}


wMotion::wMotion(): ObjectWrap() {
    if( cur ) delete[] cur;
    if( bg ) delete[] bg;
    if( recent ) delete[] recent;

    buffersSize = 0;
    cur = NULL;
    bg = recent = NULL;

    motionThreshold = 8; // min 0 max 255
    presenceThreshold = 8; // min 0 max 255
    motionWeight = 0.1; // min 0 max 2
    presenceWeight = 0.0001; // min 0 max 2
}

void quarterScale( unsigned char *to, unsigned char *from, int w, int h ) {
    for( int y=0; y<h-1; y+=2 ) {
        int yw = (y*w);
        for( int x=0; x<w-1; x+=2 ) {
            to[ (yw/4) + (x/2) ] =
                    (
                            from[ yw + x ]
                                    +from[ yw + x + 1 ]
                                    +from[ yw + w + x ]
                                    +from[ yw + w + x + 1 ]
                    ) / 4;
        }
    }
}

NAN_METHOD(wMotion::Reset) {
        NanScope();
        buffersSize = -1;

        NanReturnNull();
}

NAN_METHOD(wMotion::Process) {
        NanScope();

        Matrix *src = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat yuv;
        cv::cvtColor(src->mat, yuv, CV_RGB2YCrCb);

        motionThreshold = args[1]->IntegerValue();
        presenceThreshold = args[2]->IntegerValue();
        motionWeight = (float)args[3]->NumberValue();
        presenceWeight = (float)args[4]->NumberValue();

        int sz = yuv.cols*yuv.rows;
        int sz4 = ( (yuv.cols/2)*(yuv.rows/2));

        unsigned char *Y = yuv.data;
        unsigned char *U = Y + sz;
        unsigned char *V = U + sz4;

        if( buffersSize != sz4 ) {
            if( cur ) delete[] cur;
            if( recent ) delete[] recent;
            if( bg ) delete[] bg;

            buffersSize = sz4;
            cur = new unsigned char[sz4];
            recent = new float[sz4];
            bg = new float[sz4];

            quarterScale( cur, Y, yuv.cols, yuv.rows );

            for( int i=0; i<sz4; i++ ) recent[i]=bg[i]=cur[i];

        } else {
            quarterScale( cur, Y, yuv.cols, yuv.rows );
        }

        unsigned char mthresh = motionThreshold;
        unsigned char pthresh = presenceThreshold;

        unsigned char *P = U;
        unsigned char *M = V;
        float pw = presenceWeight;
        float pwn = 1.-pw;
        float mw = motionWeight;
        float mwn = 1.-mw;
        for( int i=0; i<sz4; i++ ) {
            // 0-255, threshold
            //M[i] = abs( late[i]-cur[i] )>mthresh?255:0;
            //P[i] = abs( bg[i]-cur[i] )>pthresh?255:0;

            // good looking
            M[i] = abs( recent[i]-cur[i] )>mthresh?64:128;
            P[i] = abs( bg[i]-cur[i] )>pthresh?64:128;

            // "real"
            //M[i] = 128+(late[i]-cur[i]);
            //P[i] = 128+(bg[i]-cur[i]);

            bg[i] *= pwn;
            bg[i] += cur[i]*pw;
            recent[i] *= mwn;
            recent[i] += cur[i]*mw;
        }

        v8::Local<v8::Array> arr = NanNew<Array>(2);

        v8::Handle<v8::Object> currentArray = NanNew<v8::Object>();
        currentArray->SetIndexedPropertiesToExternalArrayData(&cur, v8::kExternalUnsignedByteArray, sz4);

        v8::Handle<v8::Object> recentArray = NanNew<v8::Object>();
        recentArray->SetIndexedPropertiesToExternalArrayData(&cur, v8::kExternalUnsignedByteArray, sz4);

        arr->Set(0, currentArray);
        arr->Set(1, recentArray);

        NanReturnValue(arr);
}
