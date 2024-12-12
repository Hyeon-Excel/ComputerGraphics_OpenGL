#include "skybox.h"
#include <iostream>
#include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TEX_SIZE 3
#define GL_CLAMP_TO_EDGE 0x812F
const char *textFiles[6] = {
	"space/front.bmp",
	"space/back.bmp",
	"space/left.bmp",
	"space/right.bmp",
	"space/up.bmp",
	"space/down.bmp",
};

Skybox::Skybox(void) {
	for (int i = 0; i < 6; i++)
		textures[i] = _loadTexture(textFiles[i]);
}
Skybox::~Skybox(void) {}
void Skybox::draw() {
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// ��ī�� �ڽ��� ���� ������ ������ �ʵ��� depth_test�� ��Ȱ��ȭ �Ѵ�.
	glDisable(GL_DEPTH_TEST);
	// ��ī�� �ڽ��� �ٸ� ������ ������ ������ �� �ǹǷ� DECAL
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glColor3f(0, 0, 0);

	// ��
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glEnd();

	// ��
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glEnd();

	// ��
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glEnd();

	// ��
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glEnd();

	// ��
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glEnd();

	// �Ʒ�
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glEnd();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST); // Depth test ����
	glDisable(GL_TEXTURE_2D);
}
Skybox::uint Skybox::_loadTexture(pcStr filename) {
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else {
		printf("Failed to load texture: %s\n", filename);
	}

	return texId;
}