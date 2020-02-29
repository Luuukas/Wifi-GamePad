#include <node.h>
#include <string>
#include <Windows.h>

namespace demo
{
#define MAPVK_VK_TO_VSC 0
#define MAPVK_VSC_TO_VK 1
#define MAPVK_VK_TO_CHAR 2
#define MAPVK_VSC_TO_VK_EX 3
// for key pushing
BYTE scan_code(DWORD pKey)
{
	const DWORD result = MapVirtualKey(pKey, MAPVK_VK_TO_VSC);
	return static_cast<BYTE>(result);
}
void press_key(DWORD pKey)
{
	keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), 0, 0);
}
void release_key(DWORD pKey)
{
	keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), KEYEVENTF_KEYUP, 0);
}
// for testing
#define PRESS(x) press_key(x)
#define RELEASE(x) release_key(x)

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;

void do_press(const FunctionCallbackInfo<Value> &args)
{
	Isolate *isolate = args.GetIsolate();

	int key = args[0]->NumberValue();

	Local<Number> res = Number::New(isolate, key);

	PRESS(key);

	args.GetReturnValue().Set(res);
}

void do_release(const FunctionCallbackInfo<Value> &args)
{
	Isolate *isolate = args.GetIsolate();

	int key = args[0]->NumberValue();

	Local<Number> res = Number::New(isolate, key);

	RELEASE(key);

	args.GetReturnValue().Set(res);
}

void Initialize(Local<Object> exports)
{
	NODE_SET_METHOD(exports, "do_press", do_press);
	NODE_SET_METHOD(exports, "do_release", do_release);
}

NODE_MODULE(win_sendKey, Initialize)

} // namespace demo