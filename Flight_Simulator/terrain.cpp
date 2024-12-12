#include "terrain.h"

Terrain::Terrain(pcStr heightFile, pcStr surfaceTexFile, GLuint width, GLint height) {
	_map_width = width;
	_map_height = height;
	_height = nullptr;
	_texId_ground = 0;
	_texId_water = 0;

	// ���� �ؽ�ó ����
	int imgWidth, imgHeight, nrChannels;
	unsigned char *data = stbi_load(surfaceTexFile, &imgWidth, &imgHeight, &nrChannels, 0);
	if (!data) {
		printf("Failed to load texture: %s\n", surfaceTexFile);
		throw std::runtime_error("Failed to load terrain texture.");
	}
	if (imgWidth != 257 || imgHeight != 257) {
		printf("Warning: Terrain texture size is not 257 x 257.\n");
	}

	glGenTextures(1, &_texId_ground);
	glBindTexture(GL_TEXTURE_2D, _texId_ground);

	GLenum format;
	if (nrChannels == 1) format = GL_RED;
	else if (nrChannels == 3) format = GL_RGB;
	else if (nrChannels == 4) format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

	// �� �ؽ�ó ����
	const char *waterTexFile = "space/water.bmp";
	data = stbi_load(waterTexFile, &imgWidth, &imgHeight, &nrChannels, 0);
	if (!data) {
		printf("Failed to load texture: %s\n", waterTexFile);
		throw std::runtime_error("Failed to load water texture.");
	}

	glGenTextures(1, &_texId_water);
	glBindTexture(GL_TEXTURE_2D, _texId_water);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

	// ���� ������ �б�
	_height = readRawData(heightFile, width, height);
	if (!_height) {
		throw std::runtime_error("Height data not loaded.");
	}
	if (width != _map_width || height != _map_height) {
		throw std::runtime_error("Heightmap dimensions mismatch.");
	}


	// ������ �ּ� ���� ���
	minHeight = 255; // �ִ밪���� �ʱ�ȭ
	for (int i = 0; i < _map_width * _map_height; i++) {
		if (_height[i] < minHeight) {
			minHeight = _height[i];
		}
	}

	// ������ ���� ������ ����
	for (int i = 0; i < _map_width * _map_height; i++) {
		_height[i] -= minHeight;
	}


	// �� ���� ����
	waterLevel = /*static_cast<GLfloat>(minHeight)*/ + 100.0f;


	// ���� Ÿ�ϸ� ���÷��� ����Ʈ ����
	baseTerrain = glGenLists(5);
	glNewList(baseTerrain, GL_COMPILE);
	TileTerrain(1);
	glEndList();
	glNewList(baseTerrain + 1, GL_COMPILE);
	TileTerrain(4);
	glEndList();
	glNewList(baseTerrain + 2, GL_COMPILE);
	TileTerrain(8);
	glEndList();
	glNewList(baseTerrain + 3, GL_COMPILE);
	TileTerrain(16);
	glEndList();
	glNewList(baseTerrain + 4, GL_COMPILE);
	RenderWater();
	glEndList();
}
Terrain::~Terrain() {
	if (_height) {
		delete[] _height;
	}
	if (_texId_ground) {
		glDeleteTextures(1, &_texId_ground);
	}
	if (_texId_water) {
		glDeleteTextures(1, &_texId_water);
	}
}
void Terrain::RenderTerrain(GLfloat x, GLfloat y) {
	int i, j;
	// Ÿ�ϸ��� ���� ���÷��� ����Ʈ�� �ҷ����� �Լ�
	// ���� 1 ������ ����, ī�޶� ��ġ�� ���� �� ��ϸ� �׸�
	glPushMatrix();
	glTranslatef(getTileX(x) * (_map_width - 1), 0.0, getTileY(y) * (_map_height - 1));
	glCallList(baseTerrain);
	glPopMatrix();
	// ���� 4 ������ ����
	// ���� 1 ����� �����ϰ� 3 x 3������ �׸�
	for (i = -1; i < 2; i++) {
		for (j = -1; j < 2; j++) {
			if (!(i == 0 && j == 0)) {
				glPushMatrix();
				glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0, getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
				glCallList(baseTerrain + 1);
				glPopMatrix();
			}
		}
	}
	// ���� 8 ������ ����
	// ���� 4 ����� �����ϰ� 5 x 5 ������ �׸�
	for (i = -2; i < 3; i++) {
		for (j = -2; j < 3; j++) {
			if ((i < -1 || i > 1) || (j < -1 || j > 1)) {
				glPushMatrix();
				glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0, getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
				glCallList(baseTerrain + 2);
				glPopMatrix();
			}
		}
	}
	// ���� 16 ������ ����
	// ���� 8 ����� �����ϰ� 7 x 7 ������ �׸�
	for (i = -3; i < 4; i++) {
		for (j = -3; j < 4; j++) {
			if ((i < -2 || i > 2) || (j < -2 || j > 2)) {
				glPushMatrix();
				glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0, getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
				glCallList(baseTerrain + 3);
				glPopMatrix();
			}
		}
	}
	// �� ���÷��� ����Ʈ 3 x 3������ �׸�
	for (i = -2; i < 3; i++) {
		for (j = -2; j < 3; j++) {
			glPushMatrix();
			glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0, getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
			glCallList(baseTerrain + 4);
			glPopMatrix();
		}
	}
}
void Terrain::TileTerrain(GLint levelOfDetail) {												// LoD �Ķ���͸� �޾� �� Ÿ���� �׸��� �Լ�
	glEnable(GL_TEXTURE_2D);																	// �ؽ�ó Ȱ��ȭ
	glMatrixMode(GL_MODELVIEW);																	// �� �� ���
	glPushMatrix();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);									// �ؽ�ó ȯ�� ����
	glBindTexture(GL_TEXTURE_2D, _texId_ground);												// ���� �ؽ�ó ID�� ����Ű�� �ؽ�ó�� �Ʒ� ���� ����
	float coordX = (float)1.0 / _map_width;														// ���� �̵� ����
	float coordZ = (float)1.0 / _map_height;													// ���� �̵� ����
	for (int z = 0; z < _map_height - levelOfDetail; z += levelOfDetail) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < _map_width - levelOfDetail; x += levelOfDetail) {
			glTexCoord2f(coordX * x, coordZ * z);
			glVertex3f(x, _height[z * _map_width + x], z);

			glTexCoord2f(coordX * x, coordZ * (z + levelOfDetail));
			glVertex3f(x, _height[(z + levelOfDetail) * _map_width + x], z + levelOfDetail);
		}
		glEnd();
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void Terrain::RenderWater() {																	// �� �׸���
	glEnable(GL_TEXTURE_2D);																	// �ؽ�ó Ȱ��ȭ
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);								// �ؽ�ó ȯ�� ����

	glEnable(GL_BLEND);																			// ���� Ȱ��ȭ
	glDisable(GL_CULL_FACE);																	// �ĸ� ���� ��Ȱ��ȭ
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);											// ���� ��� ����
	glColor4f(0.0, 0.4, 0.5, 0.7);

	glBindTexture(GL_TEXTURE_2D, _texId_water);													// �ؽ�ó ����
	glBegin(GL_QUADS);																			// �簢�� ������ �׸���
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, waterLevel, 0.0);									// ���� ������ �����ϰ� ���� ����
	glTexCoord2f(15.0, 0.0); glVertex3f(0.0, waterLevel, _map_height);
	glTexCoord2f(15.0, 15.0); glVertex3f(_map_height, waterLevel, _map_height);
	glTexCoord2f(0.0, 15.0); glVertex3f(_map_height, waterLevel, 0.0);
	glEnd();
	glEnable(GL_CULL_FACE);																		// �ĸ� ���� Ȱ��ȭ
	glDisable(GL_BLEND);																		// ���� ��Ȱ��ȭ
	glDisable(GL_TEXTURE_2D);
}
//.raw ������ �д� �Լ�. filename: ���� �� ������ ���. w: ���� ���� ���� ũ��. h: ���� ���� ���� ũ��
unsigned char* Terrain::readRawData(pcStr filename, uint width, uint height) {
    int imgWidth, imgHeight, nrChannels;
    unsigned char* data = stbi_load(filename, &imgWidth, &imgHeight, &nrChannels, 1);
    if (!data) {
        printf("Failed to load heightmap image: %s\n", filename);
        return nullptr;
    }

    if (imgWidth != width || imgHeight != height) {
        printf("Heightmap dimensions mismatch. Expected: %d x %d, Got: %d x %d\n", width, height, imgWidth, imgHeight);
        stbi_image_free(data);
        return nullptr;
    }

    return data;
}

GLfloat Terrain::getHeight(GLfloat x, GLfloat z) {
	// (x, z)�� �ش��ϴ� ���� ���� ��ȯ
	int col = static_cast<int>(x);
	int row = static_cast<int>(z);

	if (col < 0 || col >= _map_width || row < 0 || row >= _map_height) {
		return 0.0f; // ���� �ܺθ� �⺻��
	}

	int index = row * _map_width + col;
	return static_cast<GLfloat>(_height[index]); // ���� �����͸� ��ȯ
}

GLint Terrain::getTileX(GLfloat x) {															// ���� ī�޶� ��ġ�� �ؽ�ó Ÿ���� ��� ��ǥ�� ���� : x ��ǥ
	if (x > 0) return ((int)x) / (int)_map_width;
	else return ((int)x)/ (int)_map_width - 1;
}
GLint Terrain::getTileY(GLfloat y) {															// ���� ī�޶� ��ġ�� �ؽ�ó Ÿ���� ��� ��ǥ�� ���� : y ��ǥ
	if (y > 0) return ((int)y) / (int)_map_height;
	else return ((int)y) / (int)_map_height - 1;
}