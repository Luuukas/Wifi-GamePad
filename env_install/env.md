## Visual Studio中配置Node.js C++ Addon开发环境

$\color{red} 本教程整理自视频：$<https://www.youtube.com/watch?v=KvjHn59C-uQ&t=820s>

环境：

	* Visual Studio 2017
	
	* nasm 2.11.02
	
	* python 2.7.13

配置：

​	* Node.js 10.16.0



$\color{grey}环境本来就是python\ 2.x可跳过此步。\\\color{grey}环境中未配置python的可直接安装python\ 2.x后跳过此步。\\\color{grey}环境是python\ 3.x推荐安装anaconda，再在其中配置python\ 2.x环境。$

$\color{grey} anaconda3安装教程：$

<https://www.jianshu.com/p/026a2c43b081>​

$\color{grey}使用Anaconda切换Python3.x环境与Python2.7环境:$

<https://blog.csdn.net/levon2018/article/details/84316088>



### 1. 下载Node.js项目源码

Node.js github地址：<https://github.com/nodejs/node>

Clone or download 项目后，进入项目目录，其中有vcbuild.bat文件



### 2. 编译Node.js

命令行输入：vcbuild.bat nosign Debug x64

![1566709063922](.\1566709063922.png)

期间会出现一些warning，不要害怕。

![1566709351692](.\1566709351692.png)

漫长的等待后，编译完成

![1566710984589](.\1566710984589.png)



### 3. 配置环境变量
命令行cd进入项目目录后，输入：setx NODE_HOME "%CD%"

![1566711484055](.\1566711484055.png)

"NODE_ROOT"和"NODE_HOME"同时被添加进环境变量

![1566712238165](.\1566712238165.png)

![1566712271906](.\1566712271906.png)



### 4. vs创建一个C++空项目
在项目下新建"include"和"src"目录，"include"目录中新建"Main.h"，"src"目录中新建"Main.cpp"

![1566712835044](.\1566712835044.png)

![1566712937544](.\1566712937544.png)

![1566713149804](.\1566713149804.png)

![1566713186368](.\1566713186368.png)

![1566713254512](.\1566713254512.png)



### 5. 开始配置环境
（$\color{red}配置的是x64平台$）
按如下步骤，把相应的项改为如图的值

![1566713337758](.\1566713337758.png)

![1566713478457](.\1566713478457.png)

![1566713546471](.\1566713546471.png)

![1566715211113](.\1566715211113.png)

![1566715272124](.\1566715272124.png)

![1566713770691](.\1566713770691.png)

![1566713844547](.\1566713844547.png)

![1566715371602](.\1566715371602.png)

![1566713936160](.\1566713936160.png)



MyObject.h

```c++
#pragma once
// myobject.h
#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node.h>
#include <node_object_wrap.h>

namespace demo {

	class MyObject : public node::ObjectWrap {
	public:
		static void Init(v8::Local<v8::Object> exports);

	private:
		explicit MyObject(double value = 0);
		~MyObject();

		static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args);
		static v8::Persistent<v8::Function> constructor;
		double value_;
	};

}  // namespace demo

#endif
```

MyObject.cpp

```c++
// myobject.cc
#include "myobject.h"

namespace demo {

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

	Persistent<Function> MyObject::constructor;

	MyObject::MyObject(double value) : value_(value) {
	}

	MyObject::~MyObject() {
	}

	void MyObject::Init(Local<Object> exports) {
		Isolate* isolate = exports->GetIsolate();

		// Prepare constructor template
		Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
		tpl->SetClassName(String::NewFromUtf8(
			isolate, "MyObject", NewStringType::kNormal).ToLocalChecked());
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype
		NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);

		Local<Context> context = isolate->GetCurrentContext();
		constructor.Reset(isolate, tpl->GetFunction(context).ToLocalChecked());
		exports->Set(context, String::NewFromUtf8(
			isolate, "MyObject", NewStringType::kNormal).ToLocalChecked(),
			tpl->GetFunction(context).ToLocalChecked()).FromJust();
	}

	void MyObject::New(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		Local<Context> context = isolate->GetCurrentContext();

		if (args.IsConstructCall()) {
			// Invoked as constructor: `new MyObject(...)`
			double value = args[0]->IsUndefined() ?
				0 : args[0]->NumberValue(context).FromMaybe(0);
			MyObject* obj = new MyObject(value);
			obj->Wrap(args.This());
			args.GetReturnValue().Set(args.This());
		}
		else {
			// Invoked as plain function `MyObject(...)`, turn into construct call.
			const int argc = 1;
			Local<Value> argv[argc] = { args[0] };
			Local<Function> cons = Local<Function>::New(isolate, constructor);
			Local<Object> result =
				cons->NewInstance(context, argc, argv).ToLocalChecked();
			args.GetReturnValue().Set(result);
		}
	}

	void MyObject::PlusOne(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.Holder());
		obj->value_ += 1;

		args.GetReturnValue().Set(Number::New(isolate, obj->value_));
	}

}  // namespace demo
```

![1566714466279](.\1566714466279.png)

![1566714538422](.\1566714538422.png)

![1566714590033](.\1566714590033.png)

![1566715448164](.\1566715448164.png)

![1566715525662](.\1566715525662.png)

![1566716213112](.\1566716213112.png)



![1566716262140](.\1566716262140.png)

注意x64

![1566716525096](.\1566716525096.png)

![1566716678831](.\1566716678831.png)

![1566716743320](.\1566716743320.png)

![1566719917569](.\1566719917569.png)

![1566722009588](.\1566722009588.png)

![1566722037080](.\1566722037080.png)

%NODE_ROOT%\src\res

![1566722188805](.\1566722188805.png)

![1566722239012](.\1566722239012.png)

以后使用该模板时，只需要修改如下属性

![1566716678831](.\1566716678831.png)