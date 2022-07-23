#include <nds.h>
#include <stdio.h>
#include <nds/arm9/input.h>
#include <nds/arm9/console.h>
#include <nds/arm9/boxtest.h>

#include "grass_side.h"
#include "texture_classic.h"
#include "center_cross.h"

#include "Log.h"
#include "Renderer.h"
#include "Texture.h"
#include "Sprite.h"
#include "Camera.h"

Camera* mainCamera = nullptr;

// culling - CCW are front
int grass_texture;

bool cullFrustum = true;

int main(void) {

	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);

	consoleDemoInit();
	
	vramSetBankA(VRAM_A_TEXTURE_SLOT0);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT0);
	//Sprite::init(SpriteMapping_1D_32, false);

	Renderer::init();
	Renderer::setClearColor(3, 3, 3, 31);
	Renderer::setViewport(0, 0, 255, 191);
	Renderer::setMaterialProperty(GL_EMISSION, RGB15(31, 31, 31));

	Texture classicTexture = Texture(GL_RGB16, TEXTURE_SIZE_512, TEXTURE_SIZE_512, TEXGEN_OFF, (void*)texture_classicBitmap);
	classicTexture.SetParameter(GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | GL_TEXTURE_COLOR0_TRANSPARENT);
	classicTexture.SetPalette(texture_classicPalLen / 2, (uint16*)texture_classicPal);

	mainCamera =  new PerspectiveCamera({inttof32(0), inttof32(2), inttof32(5)}, 60, floattof32(256.0f / 192.0f), floattof32(0.1f), inttof32(20));

	//SpriteAttributes centerCrossAttr = {"centerCross", 256 / 2, 192 / 2, (void*)center_crossTiles, center_crossTilesLen, (void*)center_crossPal, center_crossPalLen, SpriteSize_16x16};
	//Sprite* center_cross = Sprite::create(centerCrossAttr);

	while(1) {

		// clear the depth buffer
		Renderer::clearDepthBuffer();

		// key input
		scanKeys();
		uint32 keysH = keysHeld();
		uint32 keysD = keysDown();

		if(keysH)
		{
			mainCamera->processKeyInput(keysH, floattof32(0.1f));
		}
		//touch input
		touchPosition touchPos;
		touchRead(&touchPos);
		mainCamera->processTouchInput(touchPos);

		// rendering
		Renderer::beginScene();

		//Texture::Bind(classicTexture.GetID());

		//Renderer::drawTexturedQuad({0, 0, 0}, {inttof32(1), inttof32(1), 0}, {0, 0}, {16, 16}, {0, 0, 1});
		// draw bottom plane
		Renderer::drawQuad({inttof32(-10), 0, inttof32(-10)}, {inttof32(20), 0, inttof32(20)}, {0, 1, 0}, RGB15(31, 0, 0));

		Renderer::endScene();

		//Sprite::updateAll();

		swiWaitForVBlank();
	}

	return 0;
}
