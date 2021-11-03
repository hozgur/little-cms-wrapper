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
    if(LabDouble2sRGB) cmsDeleteTransform(LabDouble2sRGB);
    if(sRGB2LabEncoded) cmsDeleteTransform(sRGB2LabEncoded);
    if(LabEncoded2sRGB) cmsDeleteTransform(LabEncoded2sRGB);
    //log("S_global destructor"); yeah it's running.
  }
  const cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfile();
  const cmsHPROFILE hLabProfile = cmsCreateLab4Profile(NULL);

  const cmsHTRANSFORM sRGB2LabDouble = cmsCreateTransform(sRGBProfile, TYPE_RGBA_8, hLabProfile, TYPE_Lab_DBL, INTENT_RELATIVE_COLORIMETRIC, 0);
  const cmsHTRANSFORM sRGB2LabEncoded = cmsCreateTransform(sRGBProfile, TYPE_RGBA_8, hLabProfile, TYPE_Lab_16, INTENT_RELATIVE_COLORIMETRIC, 0);

  const cmsHTRANSFORM LabDouble2sRGB = cmsCreateTransform(hLabProfile, TYPE_Lab_DBL, sRGBProfile, TYPE_RGBA_8, INTENT_RELATIVE_COLORIMETRIC, 0);
  const cmsHTRANSFORM LabEncoded2sRGB = cmsCreateTransform(hLabProfile, TYPE_Lab_16, sRGBProfile, TYPE_RGBA_8, INTENT_RELATIVE_COLORIMETRIC, 0);
};


S_global global;

Value cmsGetVersion(const CallbackInfo& info) {
    printf("cmsGetVersion\n");
    return Number::New(info.Env(), cmsGetEncodedCMMversion()/1000.0);
}

Value sRGB_to_Lab(const CallbackInfo& info) {
  auto env = info.Env();
  Buffer<uint8_t> buffer = Buffer<uint8_t>::New(env,3);
  uint8_t* data = buffer.Data();
  data[0] = info[0].As<Number>().Int32Value();
  data[1] = info[1].As<Number>().Int32Value();
  data[2] = info[2].As<Number>().Int32Value();

  Float64Array result = Float64Array::New(env,3);  
  auto resultData = result.Data();  
  cmsDoTransform(global.sRGB2LabDouble, data, resultData, 1);  
  return result;  
}

Value Lab_to_sRGB(const CallbackInfo& info) {
  auto env = info.Env();
  Buffer<double> buffer = Buffer<double>::New(env,3);
  double* data = buffer.Data();
  data[0] = info[0].As<Number>().DoubleValue();
  data[1] = info[1].As<Number>().DoubleValue();
  data[2] = info[2].As<Number>().DoubleValue();

  Uint8Array result = Uint8Array::New(env,3);  
  auto resultData = result.Data();  
  cmsDoTransform(global.LabDouble2sRGB, data, resultData, 1);
  return result;  
}

Value Lab_to_Lch(const CallbackInfo& info) {
  auto env = info.Env();
  Buffer<double> buffer = Buffer<double>::New(env,3);
  double* data = buffer.Data();
  data[0] = info[0].As<Number>().DoubleValue();
  data[1] = info[1].As<Number>().DoubleValue();
  data[2] = info[2].As<Number>().DoubleValue();

  Float64Array result = Float64Array::New(env,3);  
  auto resultData = result.Data();
  cmsLab2LCh((cmsCIELCh*)resultData, (cmsCIELab*)data);  
  return result;  
}

Value Lch_to_Lab(const CallbackInfo& info) {
  auto env = info.Env();
  Buffer<double> buffer = Buffer<double>::New(env,3);
  double* data = buffer.Data();
  data[0] = info[0].As<Number>().DoubleValue();
  data[1] = info[1].As<Number>().DoubleValue();
  data[2] = info[2].As<Number>().DoubleValue();

  Float64Array result = Float64Array::New(env,3);  
  auto resultData = result.Data();
  cmsLCh2Lab((cmsCIELab*)resultData, (cmsCIELCh*)data);  
  return result;  
}


Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "GetVersion"),
                Function::New(env, cmsGetVersion));
    exports.Set(String::New(env, "sRGB_to_Lab"),
                Function::New(env, sRGB_to_Lab));
    exports.Set(String::New(env, "Lab_to_sRGB"),
                Function::New(env, Lab_to_sRGB));
    exports.Set(String::New(env, "Lab_to_Lch"),
                Function::New(env, Lab_to_Lch));
    exports.Set(String::New(env, "Lch_to_Lab"),
                Function::New(env, Lch_to_Lab));
  return exports;
}

NODE_API_MODULE(addon, Init)