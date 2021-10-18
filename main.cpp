#include <napi.h>
#include "src/lcms2.h"
#include <stdio.h>
using namespace Napi;

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
  return exports;
}

NODE_API_MODULE(addon, Init)