#include <nds.h>
#include <stdio.h>
#include <nds/arm9/input.h>
#include <nds/arm9/console.h>
#include <nds/arm9/boxtest.h>

#include "grass_side.h"
#include "texture_classic.h"
#include "center_cross.h"

#include "BoxAnim_fbx_bin.h"
#include "BoxAnimTexture.h"

//#include "Monkey_fbx_bin.h"
//#include "Monkey.h"

//#include "Fox_fbx_bin.h"
//#include "FoxTexture.h"

#include "Log.h"
#include "Renderer.h"
#include "Texture.h"
#include "Sprite.h"
#include "Camera.h"

Camera* mainCamera = nullptr;

// culling - CCW are front
int grass_texture;

bool cullFrustum = true;

void drawTriangle()
{
	uint16_t color = RGB15(0, 31, 0);

	uint32_t* fifoData = (uint32_t*)malloc(18 * sizeof(uint32_t));
	fifoData[0] = 17; // length of command list
/* 	fifoData[1] = FIFO_COMMAND_PACK(MTX_MODE, MTX_PUSH, MTX_TRANS, MTX_SCALE); // MTX_MODE, MTX_PUSH
	fifoData[2] = GL_MODELVIEW;
	// translate
	fifoData[3] = 0;
	fifoData[4] = inttof32(-1);
	fifoData[5] = inttof32(-1);
	// scale
	fifoData[6] = inttof32(2);
	fifoData[7] = inttof32(2);
	fifoData[8] = inttof32(2); */

	// vertices
	fifoData[1] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_NOP, FIFO_NOP, FIFO_NOP);
	fifoData[2] = GL_TRIANGLES;
	fifoData[3] = FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_TEX_COORD, FIFO_VERTEX16, FIFO_NOP);
	fifoData[4] = NORMAL_PACK(0, 0, floattov10(.97)); // point to positive z
	fifoData[5] = TEXTURE_PACK(inttot16(0), inttot16(0)); //color;
	fifoData[6] = VERTEX_PACK(inttov16(1), inttov16(0)); // x and y coordinate
	fifoData[7] = 0; // z coordinate
	fifoData[8] = FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_TEX_COORD, FIFO_VERTEX16, FIFO_NOP);
	fifoData[9] = NORMAL_PACK(0, 0, floattov10(.97)); // point to positive z
	fifoData[10] = TEXTURE_PACK(inttot16(16), inttot16(0)); //color;
	fifoData[11] = VERTEX_PACK(0, inttov16(2));
	fifoData[12] = 0;
	fifoData[13] = FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_TEX_COORD, FIFO_VERTEX16, FIFO_NOP);
	fifoData[14] = NORMAL_PACK(0, 0, floattov10(.97)); // point to positive z
	fifoData[15] = TEXTURE_PACK(inttot16(8), inttot16(16)); //color;
	fifoData[16] = VERTEX_PACK(inttov16(-1), 0);
	fifoData[17] = 0;

	glCallList(fifoData);

	free(fifoData);
}

int main(void) {

	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);

	consoleDemoInit();
	
	vramSetBankA(VRAM_A_TEXTURE_SLOT0);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT0);
	//Sprite::init(SpriteMapping_1D_32, false);

	Renderer::init();
	Renderer::setClearColor(3, 3, 3, 15);
	Renderer::setViewport(0, 0, 255, 191);
	Renderer::setMaterialProperty(GL_DIFFUSE, RGB15(31, 31, 31));
	Renderer::setMaterialProperty(GL_EMISSION, RGB15(31, 31, 31));
	Renderer::setMaterialProperty(GL_AMBIENT, RGB15(31, 31, 31));

/* 	Texture classicTexture = Texture(GL_RGB16, TEXTURE_SIZE_512, TEXTURE_SIZE_512, TEXGEN_OFF, (void*)texture_classicBitmap);
	classicTexture.SetParameter(GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | GL_TEXTURE_COLOR0_TRANSPARENT);
	classicTexture.SetPalette(texture_classicPalLen / 2, (uint16*)texture_classicPal); */

	Texture BoxAnimTexture = Texture(GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, TEXGEN_OFF, (void*)BoxAnimTextureBitmap);
	BoxAnimTexture.SetParameter(GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T);
	BoxAnimTexture.SetPalette(BoxAnimTexturePalLen / 2, (uint16*)BoxAnimTexturePal);
	
	/* Texture MonkeyTexture = Texture(GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, TEXGEN_OFF, (void*)MonkeyBitmap);
	MonkeyTexture.SetParameter(GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T);
	MonkeyTexture.SetPalette(MonkeyPalLen / 2, (uint16*)MonkeyPal); */

/* 	Texture FoxTexture = Texture(GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, TEXGEN_OFF, (void*)FoxTextureBitmap);
	FoxTexture.SetParameter(GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T);
	FoxTexture.SetPalette(FoxTexturePalLen / 2, (uint16*)FoxTexturePal); */

	mainCamera = new PerspectiveCamera({inttof32(0), inttof32(2), inttof32(5)}, 60, floattof32(256.0f / 192.0f), floattof32(0.1f), inttof32(20));

	//SpriteAttributes centerCrossAttr = {"centerCross", 256 / 2, 192 / 2, (void*)center_crossTiles, center_crossTilesLen, (void*)center_crossPal, center_crossPalLen, SpriteSize_16x16};
	//Sprite* center_cross = Sprite::create(centerCrossAttr);

	while(1) {

		// clear the depth buffer
		Renderer::clearDepthBuffer();

		// key input
		scanKeys();
		uint32 keysH = keysHeld();
		//uint32 keysD = keysDown();

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

		Renderer::drawArrow({0, 0, 0}, {inttof32(1), 0, 0}, RGB15(31, 0, 0));
		Renderer::drawArrow({0, 0, 0}, {0, inttof32(1), 0}, RGB15(0, 31, 0));
		Renderer::drawArrow({0, 0, 0}, {0, 0, inttof32(1)}, RGB15(0, 0, 31));

		//Texture::Bind(TestTexture.GetID());
		// Remember to unbind after rendered the related models
		BoxAnimTexture.Bind();

		//glMatrixMode(GL_MODELVIEW);
		//glPushMatrix();
		//glTranslate3f32(inttof32(0), inttof32(2), inttof32(-3));
		Renderer::drawModel(BoxAnim_fbx_bin);
		//glPopMatrix(1);

		Texture::Bind(0);


		//Renderer::drawTexturedQuad({0, 0, 0}, {inttof32(1), inttof32(1), 0}, {0, 0}, {256, 256}, {0, 0, 1});

		// draw bottom plane
		Renderer::drawQuad({inttof32(-5), inttof32(-2), inttof32(-5)}, {inttof32(10), 0, inttof32(10)}, {0, 1, 0}, RGB15(28, 3, 3));

		Renderer::endScene();

		//Sprite::updateAll();

		swiWaitForVBlank();
	}

	return 0;
}
