#include "js_convertxml.h"
#include "utils/log.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"

extern const char _binary_js_convertxml_js_start[];
extern const char _binary_js_convertxml_js_end[];

static napi_value ConvertXmlConstructor(napi_env env, napi_callback_info info)
{
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, nullptr, nullptr, &thisVar, &data);
    auto objectInfo = new ConvertXml(env);
    napi_wrap (
        env, thisVar, objectInfo,
        [](napi_env env, void *data, void *hint) {
            auto objectInfo = (ConvertXml*)data;
            if (objectInfo != nullptr) {
                delete objectInfo;
            }
        },
        nullptr, nullptr);
    return thisVar;
}

static napi_value Convert(napi_env env, napi_callback_info info)
{
    napi_value thisVar = nullptr;
    size_t requireMaxArgc = 2;
    size_t requireMinArgc = 1;
    size_t argc = 2;
    napi_value args[2] = {nullptr};
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, &thisVar, nullptr));
    NAPI_ASSERT(env, argc <= requireMaxArgc, "Wrong number of arguments(Over)");
    NAPI_ASSERT(env, argc >= requireMinArgc, "Wrong number of arguments(Less)");
    std::string strXml;
    napi_valuetype valuetype;
    ConvertXml *object = nullptr;
    NAPI_CALL(env, napi_unwrap(env, thisVar, (void**)&object));
    if (args[0] == nullptr) {
        NAPI_CALL(env, napi_throw_error(env, "", "parameter is empty"));
    } else {
        NAPI_CALL(env, napi_typeof(env, args[0], &valuetype));
        NAPI_ASSERT(env, valuetype == napi_string, "Wrong argument typr. String expected.");
        object->DealNapiStrValue(args[0], strXml);
    }
    if (args[1] != nullptr) {
        object->DealOptions(args[1]);
    }
    napi_value result = object->convert(strXml);
    HILOG_INFO("LHC....Convert start011");
    return result;
}


static napi_value ConvertXmlInit(napi_env env, napi_value exports)
{
    HILOG_INFO("LHC....ConvertXmlInit start02");
    const char *ConvertXmlClassName = "ConvertXml";
    napi_value ConvertXmlClass = nullptr;
    static napi_property_descriptor ConvertXmlDesc[] = {
        DECLARE_NAPI_FUNCTION("convert", Convert)
    };
    NAPI_CALL(env, napi_define_class(env, ConvertXmlClassName, strlen(ConvertXmlClassName), ConvertXmlConstructor,
                                     nullptr, sizeof(ConvertXmlDesc) / sizeof(ConvertXmlDesc[0]), ConvertXmlDesc,
                                     &ConvertXmlClass));
    static napi_property_descriptor desc[] = {
        DECLARE_NAPI_PROPERTY("ConvertXml", ConvertXmlClass)
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    HILOG_INFO("LHC....ConvertXmlInit end02");
    return exports;
}

extern "C"
__attribute__((visibility("default"))) void NAPI_convertxml_GetJSCode(const char **buf, int *bufLen)
{
    if (buf != nullptr) {
        *buf = _binary_js_convertxml_js_start;
    }

    if (bufLen != nullptr) {
        *bufLen = _binary_js_convertxml_js_end - _binary_js_convertxml_js_start;
    }
}

static napi_module ConvertXmlModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = ConvertXmlInit,
    .nm_modname = "ConvertXML",
    .nm_priv = ((void*)0),
    .reserved = { 0 },
};

extern "C" __attribute__ ((constructor)) void RegisterModule() {
    napi_module_register(&ConvertXmlModule);
}

