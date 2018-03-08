#include "EJSFunctions.h"
#include "Model.h"

JsValueRef EJSFunction::JSVec3Prototype;

JsValueRef EJSFunction::JSTexturePrototype;
JsValueRef EJSFunction::JSMaterialPrototype;
JsValueRef EJSFunction::JSMeshPrototype;
JsValueRef EJSFunction::JSAssetPrototype;
JsValueRef EJSFunction::JSUIPrototype;




vec3 EJSFunction::JSToNativeVec3(JsValueRef jsVec3)
{
	void* p;
	JsGetExternalData(jsVec3, &p);
	return *reinterpret_cast<glm::vec3*> (p);
}

Texture * EJSFunction::JSToNativeTexture(JsValueRef jsTexture)
{
	void* p;
	JsGetExternalData(jsTexture, &p);
	return reinterpret_cast<Texture*>(p);
}

Material * EJSFunction::JSToNativeMaterial(JsValueRef jsMaterial)
{
	void* p;
	JsGetExternalData(jsMaterial, &p);
	return reinterpret_cast<PBRMaterial*>(p);
}

Mesh * EJSFunction::JSToNativeMesh(JsValueRef jsMesh)
{
	void* p;
	JsGetExternalData(jsMesh, &p);
	return reinterpret_cast<Mesh*>(p);
}

Asset * EJSFunction::JSToNativeAsset(JsValueRef jsAsset)
{
	void* p;
	JsGetExternalData(jsAsset, &p);
	return reinterpret_cast<Asset*>(p);
}

UIElement * EJSFunction::JSToNativeUI(JsValueRef jsAsset)
{
	return nullptr;
}

// new Vec3(number x, number y, number z)
JsValueRef EJSFunction::JSConstructorVec3(JsValueRef callee, bool isConstructCall, JsValueRef * arguments, unsigned short argumentCount, void * callbackState)
{
	assert(isConstructCall && argumentCount == 4);
	JsValueRef output = JS_INVALID_REFERENCE;
	double x, y, z;

	bool toArray;
	char* buffer;


	JsNumberToDouble(arguments[1], &x);
	JsNumberToDouble(arguments[2], &y);
	JsNumberToDouble(arguments[3], &z);

	vec3* v = new vec3(x, y, z);

	JsCreateExternalObject(v, nullptr, &output);
	JsSetPrototype(output, JSTexturePrototype);
	return output;
}

// new Texture(string path)
JsValueRef EJSFunction::JSConstructorTexture(JsValueRef callee, bool isConstructCall, JsValueRef * arguments, unsigned short argumentCount, void * callbackState)
{
	assert(isConstructCall && argumentCount == 3);
	JsValueRef output = JS_INVALID_REFERENCE;
	const wchar_t path = L't';
	const wchar_t* p = &path;
	const wchar_t** pa = &p;
	size_t t;
	size_t length;

	bool toArray;
	char* buffer;

	JsStringToPointer(arguments[1], pa, &t);
	JsBooleanToBool(arguments[2], &toArray);

	wstring ws(p);
	string str(ws.begin(), ws.end());
	Texture* texture;
	if (toArray) {
		texture = new Texture(str.c_str(), toArray);
	}
	else {
		texture = new Texture("");
	}

	JsCreateExternalObject(texture, nullptr, &output);
	JsSetPrototype(output, JSTexturePrototype);
	return output;
}

// new Material(vec3 color)
JsValueRef EJSFunction::JSConstructorMaterial(JsValueRef callee, bool isConstructCall, JsValueRef * arguments, unsigned short argumentCount, void * callbackState)
{
	JsValueRef output = JS_INVALID_REFERENCE;
	
	void* v;
	JsGetExternalData(arguments[1], &v);
	vec3* color = static_cast<vec3*>(v);

	PBRMaterial* mat = new PBRMaterial();
	mat->albedo = *color;

	JsCreateExternalObject(mat, nullptr, &output);
	JsSetPrototype(output, JSTexturePrototype);
	return output;
}

// new Mesh(string path, Material mat)
JsValueRef EJSFunction::JSConstructorMesh(JsValueRef callee, bool isConstructCall, JsValueRef * arguments, unsigned short argumentCount, void * callbackState)
{
	JsValueRef output = JS_INVALID_REFERENCE;
	const wchar_t path = L't';
	const wchar_t* p = &path;
	const wchar_t** pa = &p;
	size_t t;
	size_t length;

	bool toArray;
	char* buffer;

	JsStringToPointer(arguments[1], pa, &t);

	wstring ws(p);
	string str(ws.begin(), ws.end());
	char *cpath = new char[str.length() + 1];
	strcpy(cpath, str.c_str());
	Model* mod = new Model(cpath);
	delete[] cpath;
	Mesh* mesh = mod->meshes[0];

	void* mat;
	JsGetExternalData(arguments[2], &mat);
	PBRMaterial* material = static_cast<PBRMaterial*>(mat);

	mesh->material = material;

	JsCreateExternalObject(mesh, nullptr, &output);
	JsSetPrototype(output, JSTexturePrototype);
	return output;
}

// new Asset(vec3 position, vec3 scale, int mass)
JsValueRef EJSFunction::JSConstructorAsset(JsValueRef callee, bool isConstructCall, JsValueRef * arguments, unsigned short argumentCount, void * callbackState)
{
	JsValueRef output = JS_INVALID_REFERENCE;

	void* posV;
	JsGetExternalData(arguments[1], &posV);
	vec3* pos = static_cast<vec3*>(posV);

	void* scaleV;
	JsGetExternalData(arguments[2], &scaleV);
	vec3* scale = static_cast<vec3*>(scaleV);
		
	int mass;
	JsNumberToInt(arguments[3], &mass);

	Asset* asset = new Asset(*pos, *scale, mass, assetShapes::cube);

	JsCreateExternalObject(asset, nullptr, &output);
	JsSetPrototype(output, JSTexturePrototype);
	return output;
}

JsValueRef EJSFunction::JSConstructorUI(JsValueRef callee, bool isConstructCall, JsValueRef * arguments, unsigned short argumentCount, void * callbackState)
{
	JsValueRef output = JS_INVALID_REFERENCE;

	void* posV;
	JsGetExternalData(arguments[1], &posV);
	vec3* pos = static_cast<vec3*>(posV);
	
	void* posPxV;
	JsGetExternalData(arguments[2], &posPxV);
	vec3* posPx = static_cast<vec3*>(posPxV);

	void* sizeV;
	JsGetExternalData(arguments[3], &sizeV);
	vec3* size = static_cast<vec3*>(sizeV);

	void* sizePxV;
	JsGetExternalData(arguments[4], &sizePxV);
	vec3* sizePx = static_cast<vec3*>(sizePxV);

	void* foregroundColorV;
	JsGetExternalData(arguments[5], &foregroundColorV);
	vec3* foregroundColor = static_cast<vec3*>(foregroundColorV);

	void* backgroundColorV;
	JsGetExternalData(arguments[6], &sizePxV);
	vec3* backgroundColor = static_cast<vec3*>(sizePxV);

	double opacity;
	JsNumberToDouble(arguments[7], &opacity);

	double bb;
	JsNumberToDouble(arguments[8], &bb);

	double zid;
	JsNumberToDouble(arguments[9], &zid);


	UIElement* ele2 = new UIElement();
	ele2->opacity = opacity;

	ele2->foregroundColor = *foregroundColor;
	ele2->backgroundColor = *backgroundColor;

	ele2->sizePixel = vec2(sizePx->x, sizePx->y );
	ele2->sizePercent = vec2(size->x, size->y);
	ele2->posisionPercent = vec2(pos->x, pos->y);
	ele2->positionPixel = vec2(posPx->x, posPx->y);
	ele2->backgoundBlurr = bb;
	ele2->texture = JSToNativeTexture(arguments[10]);
	ele2->alphamap = JSToNativeTexture(arguments[11]);
	ele2->zindex = zid;


	JsCreateExternalObject(ele2, nullptr, &output);
	JsSetPrototype(output, JSTexturePrototype);
	return output;
}

// material.setAlbedo(vec3 color)
JsValueRef EJSFunction::JSMaterialSetAlbedo(JsValueRef callee, bool isConstructCall, JsValueRef * arguments, unsigned short argumentCount, void * callbackState)
{
	JsValueRef output = JS_INVALID_REFERENCE;

	void* material;
	if (JsGetExternalData(arguments[0], &material) == JsNoError) {
		PBRMaterial* mat = static_cast<PBRMaterial*>(material);

		void* v;
		JsGetExternalData(arguments[1], &v);
		vec3* color = static_cast<vec3*>(v);

		mat->albedo = *color;
	};
	return output;
}

// mesh.attachTo();
JsValueRef EJSFunction::JSMeshAttachTo(JsValueRef callee, bool isConstructCall, JsValueRef * arguments, unsigned short argumentCount, void * callbackState)
{
	JsValueRef output = JS_INVALID_REFERENCE;

	void* mesh;
	if (JsGetExternalData(arguments[0], &mesh) == JsNoError) {
		Mesh* me = static_cast<Mesh*>(mesh);

		void* v;
		JsGetExternalData(arguments[1], &v);
		Asset* asset = static_cast<Asset*>(v);
		me->attachTo(asset);
	};
	return output;
}

JsValueRef EJSFunction::LogCB(JsValueRef callee, bool isConstructCall, JsValueRef * arguments, unsigned short argumentCount, void * callbackState)
{
	for (unsigned int index = 1; index < argumentCount; index++)
	{
		if (index > 1)
		{
			printf(" ");
		}
		const wchar_t path = L't';
		const wchar_t* p = &path;
		const wchar_t** pa = &p;

		size_t t;
		size_t length;

		bool toArray;
		char* buffer;

		JsStringToPointer(arguments[1], pa, &t);

		wstring ws(p);
		string str(ws.begin(), ws.end());
		printf("%s", str.c_str());

	}
	printf("\n");
	return JS_INVALID_REFERENCE;
}