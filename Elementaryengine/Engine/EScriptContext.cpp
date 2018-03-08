#include "EScriptContext.h"
#include <string>
#include <iostream>
#include <comdef.h>

using namespace std;

EScriptContext::EScriptContext()
{
	// Create a runtime. 
	JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);

	// Create an execution context. 
	JsCreateContext(runtime, &context);

	// Now set the current execution context.
	JsSetCurrentContext(context);

	// Convert your script result to String in JavaScript; redundant if your script returns a String
	JsValueRef resultJSString;
	JsConvertValueToString(result, &resultJSString);

	// Project script result back to C++.
	const wchar_t *resultWC;
	size_t stringLength;
	JsStringToPointer(resultJSString, &resultWC, &stringLength);

	wstring resultW(resultWC);
	cout << string(resultW.begin(), resultW.end()) << endl;

	AddBindings();

}


EScriptContext::~EScriptContext()
{
	// Dispose runtime
	JsSetCurrentContext(JS_INVALID_REFERENCE);
	JsDisposeRuntime(runtime);
}


void EScriptContext::loadScript(wstring script)
{
	this->script = script;

	// Run the script.
	JsRunScript(script.c_str(), currentSourceContext++, L"", &result);
}

void EScriptContext::RunFunction(const char * name)
{
	JsValueRef func, funcPropId, global, undefined, result;
	JsGetGlobalObject(&global);
	JsGetUndefinedValue(&undefined);
	JsValueRef args[] = { undefined };
	JsCreatePropertyId(name, strlen(name), &funcPropId);
	JsGetProperty(global, funcPropId, &func);
	// note that args[0] is thisArg of the call; actual args start at index 1
	JsCallFunction(func, args, 1, &result);
}

void EScriptContext::ReadScript(wstring filename)
{
	FILE *file;
	if (_wfopen_s(&file, filename.c_str(), L"rb"))
	{
		fwprintf(stderr, L"chakrahost: unable to open file: %s.\n", filename.c_str());
		return;
	}

	unsigned int current = ftell(file);
	fseek(file, 0, SEEK_END);
	unsigned int end = ftell(file);
	fseek(file, current, SEEK_SET);
	unsigned int lengthBytes = end - current;
	char *rawBytes = (char *)calloc(lengthBytes + 1, sizeof(char));

	if (rawBytes == nullptr)
	{
		fwprintf(stderr, L"chakrahost: fatal error.\n");
		return;
	}

	fread((void *)rawBytes, sizeof(char), lengthBytes, file);

	wchar_t *contents = (wchar_t *)calloc(lengthBytes + 1, sizeof(wchar_t));
	if (contents == nullptr)
	{
		free(rawBytes);
		fwprintf(stderr, L"chakrahost: fatal error.\n");
		return;
	}

	if (MultiByteToWideChar(CP_UTF8, 0, rawBytes, lengthBytes + 1, contents, lengthBytes + 1) == 0)
	{
		free(rawBytes);
		free(contents);
		fwprintf(stderr, L"chakrahost: fatal error.\n");
		return;
	}

	wstring sc = contents;
	free(rawBytes);
	free(contents);
	script = sc;

	// Run the script.
	JsRunScript(script.c_str(), currentSourceContext++, L"", &result);

	JsValueRef vref;
	JsGetAndClearException(&vref);
	
	JsValueType jsType;
	JsGetValueType(vref, &jsType);
	return;
}

void EScriptContext::projectNativeClass(const wchar_t *className, JsNativeFunction constructor, JsValueRef &prototype, vector<const wchar_t *> memberNames, vector<JsNativeFunction> memberFuncs) {
	// project constructor to global scope 
	JsValueRef globalObject;
	JsGetGlobalObject(&globalObject);
	JsValueRef jsConstructor;
	JsCreateFunction(constructor, nullptr, &jsConstructor);
	setProperty(globalObject, className, jsConstructor);
	// create class's prototype and project its member functions
	JsCreateObject(&prototype);
	assert(memberNames.size() == memberNames.size());
	for (int i = 0; i < memberNames.size(); ++i) {
		setCallback(prototype, memberNames[i], memberFuncs[i], nullptr);
	}
	setProperty(jsConstructor, L"prototype", prototype);
}

void EScriptContext::setCallback(JsValueRef object, const wchar_t *propertyName, JsNativeFunction callback, void *callbackState)
{
	JsPropertyIdRef propertyId;
	JsGetPropertyIdFromName(propertyName, &propertyId);
	JsValueRef function;
	JsCreateFunction(callback, callbackState, &function);
	JsSetProperty(object, propertyId, function, true);
}

void EScriptContext::setProperty(JsValueRef object, const wchar_t *propertyName, JsValueRef property)
{
	JsPropertyIdRef propertyId;
	JsGetPropertyIdFromName(propertyName, &propertyId);
	JsSetProperty(object, propertyId, property, true);
}


void EScriptContext::AddBindings()
{
	JsValueRef globalObject;
	JsGetGlobalObject(&globalObject);

	vector<const wchar_t *> memberNamesVec3;
	vector<JsNativeFunction> memberFuncsVec3;
	projectNativeClass(L"Vec3", EJSFunction::JSConstructorVec3, EJSFunction::JSVec3Prototype, memberNamesVec3, memberFuncsVec3);


	vector<const wchar_t *> memberNamesTexture;
	vector<JsNativeFunction> memberFuncsTexture;
	projectNativeClass(L"Texture", EJSFunction::JSConstructorTexture, EJSFunction::JSTexturePrototype , memberNamesTexture, memberFuncsTexture);

	vector<const wchar_t *> memberNamesMaterial;
	vector<JsNativeFunction> memberFuncsMaterial;
	//memberNamesMaterial.push_back(L"setAlbedo");
	//memberFuncsMaterial.push_back(EJSFunction::JSMaterialSetAlbedo);
	projectNativeClass(L"Material", EJSFunction::JSConstructorMaterial, EJSFunction::JSMaterialPrototype, memberNamesMaterial, memberFuncsMaterial);

	vector<const wchar_t *> memberNamesMesh;
	vector<JsNativeFunction> memberFuncsMesh;
	memberNamesMesh.push_back(L"attachto");
	memberFuncsMesh.push_back(EJSFunction::JSMeshAttachTo);
	projectNativeClass(L"Mesh", EJSFunction::JSConstructorMesh, EJSFunction::JSMeshPrototype, memberNamesMesh, memberFuncsMesh);

	vector<const wchar_t *> memberNamesAsset;
	vector<JsNativeFunction> memberFuncsAsset;
	projectNativeClass(L"Asset", EJSFunction::JSConstructorAsset, EJSFunction::JSAssetPrototype, memberNamesAsset, memberFuncsAsset);

	vector<const wchar_t *> memberNamesUI;
	vector<JsNativeFunction> memberFuncsUI;
	projectNativeClass(L"UIElement", EJSFunction::JSConstructorUI, EJSFunction::JSUIPrototype, memberNamesUI, memberFuncsUI);

	//vector<const wchar_t *> memberNamesConsole;
	//vector<JsNativeFunction> memberFuncsConsole;
	//memberNamesConsole.push_back(L"log");
	//memberFuncsConsole.push_back(EJSFunction::LogCB);
	//projectNativeClass(L"console", EJSFunction::JSConstructorMesh, EJSFunction::JSMeshPrototype, memberNamesConsole, memberFuncsConsole);

	JsValueRef console, logFunc, global;
	JsPropertyIdRef consolePropId, logPropId;
	const char* logString = "log";
	const char* consoleString = "console";
	// create console object, log function, and set log function as property of console
	JsCreateObject(&console);
	JsCreateFunction(EJSFunction::LogCB, nullptr, &logFunc);
	JsCreatePropertyId(logString, strlen(logString), &logPropId);
	JsSetProperty(console, logPropId, logFunc, true);
	// set console as property of global object
	JsGetGlobalObject(&global);
	JsCreatePropertyId(consoleString, strlen(consoleString), &consolePropId);
	JsSetProperty(global, consolePropId, console, true);




}