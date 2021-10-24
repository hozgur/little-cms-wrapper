#include <napi.h>
#include "src/lcms2.h"
#include <stdio.h>
using namespace Napi;

void log(const char* msg) {
  printf("%s\n", msg);
}
// Default Profiles
#define DEFAULT_PROFILES_PATH "./profiles"


struct S_global {

  ~S_global() {
    if(sRGBProfile) cmsCloseProfile(sRGBProfile);
    if(hLabProfile) cmsCloseProfile(hLabProfile);    
    if(sRGB2LabDouble) cmsDeleteTransform(sRGB2LabDouble);
    if(Lab2sRGBDouble) cmsDeleteTransform(Lab2sRGBDouble);
    if(sRGB2LabEncoded) cmsDeleteTransform(sRGB2LabEncoded);
    if(LabEncoded2sRGB) cmsDeleteTransform(LabEncoded2sRGB);
    log("S_global destructor");
  }
  const cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfile();
  const cmsHPROFILE hLabProfile = cmsCreateLab4Profile(NULL);

  const cmsHTRANSFORM sRGB2LabDouble = cmsCreateTransform(sRGBProfile, TYPE_RGBA_8, hLabProfile, TYPE_Lab_DBL, INTENT_RELATIVE_COLORIMETRIC, 0);
  const cmsHTRANSFORM sRGB2LabEncoded = cmsCreateTransform(sRGBProfile, TYPE_RGBA_8, hLabProfile, TYPE_Lab_16, INTENT_RELATIVE_COLORIMETRIC, 0);

  const cmsHTRANSFORM Lab2sRGBDouble = cmsCreateTransform(hLabProfile, TYPE_Lab_DBL, sRGBProfile, TYPE_RGBA_8, INTENT_RELATIVE_COLORIMETRIC, 0);
  const cmsHTRANSFORM LabEncoded2sRGB = cmsCreateTransform(hLabProfile, TYPE_Lab_16, sRGBProfile, TYPE_RGBA_8, INTENT_RELATIVE_COLORIMETRIC, 0);
};


S_global global;


Value sRGB_to_Lab(const CallbackInfo& info) {
  auto env = info.Env();
  Buffer<uint8_t> buffer = Buffer<uint8_t>::New(env,3);
  uint8_t* data = buffer.Data();
  data[0] = info[0].As<Number>().Int32Value();
  data[1] = info[1].As<Number>().Int32Value();
  data[2] = info[2].As<Number>().Int32Value();

  Buffer<double> outBuf = Buffer<double>::New(env, 3);
  double* out = outBuf.Data();
  cmsDoTransform(global.sRGB2LabDouble, data, out, 1);
  return outBuf;
}



static void reverse_data(const uint8_t *inBuffer,uint8_t *outBuffer, int len) {
    for (int i = 0; i < len; i++) {
        outBuffer[i] = inBuffer[len - i - 1];
    }
    
}

Value cmsGetVersion(const CallbackInfo& info) {
    printf("cmsGetVersion\n");
    return Number::New(info.Env(), cmsGetEncodedCMMversion());
}




Value Method(const CallbackInfo& info) {
    Env env = info.Env();
    if (info.Length() != 1) {
    Error::New(info.Env(), "Expected exactly one argument")
        .ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }
  if (!info[0].IsBuffer()) {
    Error::New(info.Env(), "Expected an Buffer")
        .ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

    Buffer<uint8_t> buf = info[0].As<Buffer<uint8_t>>();
    Buffer<uint8_t> outBuf = Buffer<uint8_t>::New(env, buf.ByteLength());
    uint8_t *outData = (uint8_t *)outBuf.Data();
    uint8_t *inData = (uint8_t *)buf.Data();
    reverse_data(inData, outData, buf.ByteLength());
    return outBuf;
  //return Napi::String::New(env, "world");
}

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "GetVersion"),
                Function::New(env, cmsGetVersion));
    exports.Set(String::New(env, "sRGB_to_Lab"),
                Function::New(env, sRGB_to_Lab));
  return exports;
}

NODE_API_MODULE(addon, Init)