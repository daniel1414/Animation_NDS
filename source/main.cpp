#include <nds.h>
#include <stdio.h>
#include <nds/arm9/sassert.h>
#include <nds/arm9/input.h>

#include "Camera.h"
#include "Renderer.h"

int main(void) {

	//set mode 0, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);

	consoleDemoInit();

	// initialize Renderer
	Renderer::init();
	Renderer::setClearColor(0, 0, 0, 31);
	Renderer::setViewport(0, 0, 255, 255);
	Renderer::setMaterialProperty(GL_EMISSION, RGB15(31, 31, 31));

	Camera* mainCamera =  new PerspectiveCamera({0, 0, inttof32(5)}, 60, floattof32(256.0f / 192.0f), floattof32(0.1f), inttof32(20));

	while(1) {

		Renderer::clearDepthBuffer();

		scanKeys();
		uint32 keys = keysHeld();
		touchPosition touchPos;
		touchRead(&touchPos);
		mainCamera->processTouchInput(touchPos);

		Renderer::beginScene();

		//Renderer::drawQuad({0, 0, 0}, {inttof32(1), inttof32(1), inttof32(1)}, {0, 0}, {0, 0}, {0, intto, 0}, RGB15(31, 0, 0));
		Renderer::drawArrow({0, 0, 0}, {0, inttof32(1), 0}, RGB15(0, 31, 0));

		Renderer::endScene();
		
		swiWaitForVBlank();

		if(keys & KEY_START) break;
	}

	return 0;
}
