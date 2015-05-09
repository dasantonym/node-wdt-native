//
// Created by Anton on 24/04/15.
//

#include "cvMultiplyS.h"

#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

v8::Persistent<FunctionTemplate> cvMultiplyS::constructor;

void
cvMultiplyS::Init(Handle<Object> target) {
    NanScope();

    //Class
    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(cvMultiplyS::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("cvMultiplyS"));

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(ctor, "process", Process);

    target->Set(NanNew("cvMultiplyS"), ctor->GetFunction());
}

NAN_METHOD(cvMultiplyS::New) {
        NanScope();
        NanReturnValue(args.Holder());
}


cvMultiplyS::cvMultiplyS(): ObjectWrap() {

}

NAN_METHOD(cvMultiplyS::Process) {
        NanScope();

        Matrix *src = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        src->mat.convertTo(src->mat, CV_8UC3, (float)args[1]->NumberValue(), (float)args[2]->NumberValue());

        NanReturnNull();
}

