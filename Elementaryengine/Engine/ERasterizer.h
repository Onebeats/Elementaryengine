#pragma once
#include <ERender.h>
#include <Game.h>

class ERasterizer: public ERenderer
{
public:
	ERasterizer();
	~ERasterizer();

	void SetupFrame(bool meshChanged, EOpenGl* eOpenGl);
	void RenderFrame(EOpenGl* eOpenGl, EDisplaySettings* displaySettings, mat4 View, mat4 Projection);
	void RenderFX(EOpenGl* eOpenGl, EDisplaySettings* displaySettings);
	void Setup(EOpenGl* eOpenGl, EDisplaySettings* displaySettings);
	Texture* loadTexture(const char* path);

	static void AssetCreatedCallback(Asset* asset);
	static void AssetChangedCallback(Asset* asset);
	static void AssetDestroyedCallback(Asset* asset);
	
	static bool assetCreated;
	static bool assetChanged;

	const unsigned int TextureSize = 1024;
	const unsigned int TextureCount = 64;
private:

	///<summary>
	///builds the composed mesh needed for multiDrawIndirect and copies it to the GPU buffers
	///</summary> 
	///<param name="assetsChanged">
	///indicates if the assets changed last frame (NOT their atributes)
	///</param>
	///<param name="meshChanged">
	///indicates if the mesh changed last frame (INCLUDING their atributes)
	///</param>
	///<param name="eOpenGl">
	///the EOpenGl object that holds the buffers ids that should be worked on
	///</param>
	void BuildMeshes(bool assetsChanged, bool meshChanged, EOpenGl* eOpenGl);


	void BuildUI(EOpenGl* eOpenG);

	///<summary>
	///builds the list of draw atributes and copies it to the GPU buffers
	///</summary> 
	///<param name="eOpenGl">
	///the EOpenGl object that holds the buffers ids that should be worked on
	///</param>
	void BuildDrawAtrib(EOpenGl* eOpenGl);

	///<summary>
	///change the draw atributes for certain assets if only the asset atributes have changed, not the assets
	///</summary> 
	// TODO: implement ChangeAssetInfo using compute shader on ssbo
	void ChangeAssetInfo();

	///<summary>
	///renders the Shadowmaps to the cubemap array specified in eOpenGl
	///</summary> 
	///<param name="eOpenGl">
	///the EOpenGl object that holds the buffer ids that should be worked on and the cubemap array texture
	///</param>
	void RenderShadowMaps(EOpenGl* eOpenGl);

	///<summary>
	///renders the frame and PostFX in 3 passes: geometry, lighting, postFX
	///</summary> 
	///<param name="eOpenGl">
	///the EOpenGl object that holds the buffer and texture ids 
	///</param>
	///<param name="displaySettings">
	///the display settings to use
	///</param>
	///<param name="View">
	///the view matrix to use
	///</param>
	///<param name="Projection">
	///the projection matrix to use
	///</param>
	void RenderFrameMain(EOpenGl* eOpenGl, EDisplaySettings* displaySettings, mat4 View, mat4 Projection);

	///<summary>
	///set the uniforms and ssbos for the lamps
	///</summary> 
	///<param name="eOpenGl">
	///the EOpenGl object that holds the buffer and texture ids 
	///</param>
	///<param name="shader">
	///the shader that needs the uniforms set
	///</param>
	void SetupLamps(EOpenGl* eOpenGl, Shader * shader);

	///<summary>
	///render the UI elements
	///</summary> 
	///<param name="eOpenGl">
	///the EOpenGl object that holds the buffer and texture ids 
	///</param>
	///<param name="displaySettings">
	///the display settings to use
	///</param>
	void RenderUI(EOpenGl* eOpenGl, EDisplaySettings* displaySettings);
};