#include "OpenCV.h"
#include "Point.h"
#include "Matrix.h"
#include "VideoCaptureWrap.h"
#include "Constants.h"

#include "cvAverage.h"
#include "cvCamShift.h"
#include "cvDisplayFormat.h"
#include "cvMotion.h"
#include "cvMultiplyS.h"
#include "wAccumulate.h"
#include "wExtremes.h"
#include "wMotion.h"
#include "wTrigger.h"

extern "C" void
init(Handle<Object> target) {
    NanScope();
    OpenCV::Init(target);

    Point::Init(target);
    Matrix::Init(target);
    VideoCaptureWrap::Init(target);
    Constants::Init(target);

    cvAverage::Init(target);
    cvCamShift::Init(target);
    cvDisplayFormat::Init(target);
    cvMotion::Init(target);
    cvMultiplyS::Init(target);
    wAccumulate::Init(target);
    wExtremes::Init(target);
    wMotion::Init(target);
    wTrigger::Init(target);
};

NODE_MODULE(node_wdt_native, init)
