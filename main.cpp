#include <napi.h>
#include "src/lcms2.h"
#include <stdio.h>
#include <iostream>
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
    if(sRGBA2LabDouble) cmsDeleteTransform(sRGB2LabDouble);
    if(LabDouble2sRGB) cmsDeleteTransform(LabDouble2sRGB);
    if(LabDouble2sRGBA) cmsDeleteTransform(LabDouble2sRGBA);
    if(sRGB2LabEncoded) cmsDeleteTransform(sRGB2LabEncoded);
    if(sRGBA2LabEncoded) cmsDeleteTransform(sRGB2LabEncoded);
    if(LabEncoded2sRGB) cmsDeleteTransform(LabEncoded2sRGB);
    if(LabEncoded2sRGBA) cmsDeleteTransform(LabEncoded2sRGBA);
    //log("S_global destructor"); yeah it's running.
  }
  const cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfile();
  const cmsHPROFILE hLabProfile = cmsCreateLab4Profile(NULL);

  const cmsHTRANSFORM sRGB2LabDouble = cmsCreateTransform(sRGBProfile, TYPE_RGB_8, hLabProfile, TYPE_Lab_DBL, INTENT_RELATIVE_COLORIMETRIC, 0);
  const cmsHTRANSFORM sRGB2LabEncoded = cmsCreateTransform(sRGBProfile, TYPE_RGB_8, hLabProfile, TYPE_Lab_16, INTENT_RELATIVE_COLORIMETRIC, 0);

  const cmsHTRANSFORM sRGBA2LabDouble = cmsCreateTransform(sRGBProfile, TYPE_RGBA_8, hLabProfile, TYPE_Lab_DBL, INTENT_RELATIVE_COLORIMETRIC, 0);
  const cmsHTRANSFORM sRGBA2LabEncoded = cmsCreateTransform(sRGBProfile, TYPE_RGBA_8, hLabProfile, TYPE_Lab_16, INTENT_RELATIVE_COLORIMETRIC, 0);

  const cmsHTRANSFORM LabDouble2sRGB = cmsCreateTransform(hLabProfile, TYPE_Lab_DBL, sRGBProfile, TYPE_RGB_8, INTENT_RELATIVE_COLORIMETRIC, 0);
  const cmsHTRANSFORM LabEncoded2sRGB = cmsCreateTransform(hLabProfile, TYPE_Lab_16, sRGBProfile, TYPE_RGB_8, INTENT_RELATIVE_COLORIMETRIC, cmsFLAGS_NOOPTIMIZE | cmsFLAGS_HIGHRESPRECALC);

  const cmsHTRANSFORM LabDouble2sRGBA = cmsCreateTransform(hLabProfile, TYPE_Lab_DBL, sRGBProfile, TYPE_RGBA_8, INTENT_RELATIVE_COLORIMETRIC, 0);
  const cmsHTRANSFORM LabEncoded2sRGBA = cmsCreateTransform(hLabProfile, TYPE_Lab_16, sRGBProfile, TYPE_RGBA_8, INTENT_RELATIVE_COLORIMETRIC, cmsFLAGS_NOOPTIMIZE | cmsFLAGS_HIGHRESPRECALC);
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

Value sRGB_to_Lab_encoded(const CallbackInfo& info) {
  auto env = info.Env();
  auto data = info[0].As<Buffer<uint8_t>>().Data();
  auto dataLength = info[0].As<Buffer<uint8_t>>().Length();
  if ((dataLength % 3) != 0) {
    return Number::New(env, -1);
  }
  auto length = dataLength / 3;
  Uint16Array result = Uint16Array::New(env,length * 3);
  auto resultData = result.Data();  
  cmsDoTransform(global.sRGB2LabEncoded, data, resultData, length);  
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

Value Lab_encoded_to_sRGB(const CallbackInfo& info) {
  auto env = info.Env();
  auto data = info[0].As<Buffer<uint16_t>>().Data();
  auto dataLength = info[0].As<Buffer<uint16_t>>().Length();
  if ((dataLength % 3) != 0) {
    return Number::New(env, -1);
  }
  auto length = dataLength / 3;
  Buffer<uint8_t> result = Buffer<uint8_t>::New(env,length * 3);
  auto resultData = result.Data();
  cmsDoTransform(global.LabEncoded2sRGB, data, resultData, length);
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

Value Lab_to_Lab_encoded(const CallbackInfo& info) {
  auto env = info.Env();
  auto dataBuffer = info[0].As<Buffer<double>>();
  int dataLength = dataBuffer.Length();
  double *data = dataBuffer.Data();
  if ((dataLength % 3) != 0) {
    return Number::New(env, -1);
  }
  auto length = dataLength / 3;
  std::cout << "length: " << length << std::endl;
  std::cout << data[0] << " " << data[1] << " " << data[2] << std::endl;
  Uint16Array result = Uint16Array::New(env,length * 3);
  auto resultData = result.Data();
  for(int i = 0;i<length;i++) {
    cmsUInt16Number Lab_encoded[3];
    cmsFloat2LabEncoded(Lab_encoded, (cmsCIELab*) &data[i*3]);
    resultData[i*3] = Lab_encoded[0];
    resultData[i*3+1] = Lab_encoded[1];
    resultData[i*3+2] = Lab_encoded[2];
  }
  return result; 
}

Value Lab_encoded_to_Lab(const CallbackInfo& info) {
  auto env = info.Env();
  auto dataBuffer = info[0].As<Buffer<uint16_t>>();
  int dataLength = dataBuffer.Length();
  std::cout << "dataLength: " << dataLength << std::endl;  
  uint16_t *data = dataBuffer.Data();
  if ((dataLength % 3) != 0) {
    return Number::New(env, -1);
  }
  auto length = dataLength / 3;

  Float64Array result = Float64Array::New(env,length * 3);
  auto resultData = result.Data();
  for(int i = 0;i<length;i++) {
    uint16_t Lab_encoded[3] = {data[i*3],data[i*3+1],data[i*3+2]};
    cmsLabEncoded2Float((cmsCIELab*)&resultData[i*3], Lab_encoded);    
  }
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
    exports.Set(String::New(env, "sRGB_to_Lab_encoded"),
                Function::New(env, sRGB_to_Lab_encoded));
    exports.Set(String::New(env, "Lab_encoded_to_sRGB"),
                Function::New(env, Lab_encoded_to_sRGB));
    exports.Set(String::New(env, "Lab_to_Lab_encoded"),
                Function::New(env, Lab_to_Lab_encoded));
    exports.Set(String::New(env, "Lab_encoded_to_Lab"),
                Function::New(env, Lab_encoded_to_Lab));
  return exports;
}

NODE_API_MODULE(addon, Init)