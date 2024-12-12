#include "terrain.h"

Terrain::Terrain(pcStr heightFile, pcStr surfaceTexFile, GLuint width, GLint height) {
	_map_width = width;
	_map_height = height;
	_height = nullptr;
	_texId_ground = 0;
	_texId_water = 0;

	// 지형 텍스처 설정
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

	// 물 텍스처 설정
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

	// 높이 데이터 읽기
	_height = readRawData(heightFile, width, height);
	if (!_height) {
		throw std::runtime_error("Height data not loaded.");
	}
	if (width != _map_width || height != _map_height) {
		throw std::runtime_error("Heightmap dimensions mismatch.");
	}


	// 지형의 최소 높이 계산
	minHeight = 255; // 최대값으로 초기화
	for (int i = 0; i < _map_width * _map_height; i++) {
		if (_height[i] < minHeight) {
			minHeight = _height[i];
		}
	}

	// 지형의 높이 데이터 조정
	for (int i = 0; i < _map_width * _map_height; i++) {
		_height[i] -= minHeight;
	}


	// 물 높이 설정
	waterLevel = /*static_cast<GLfloat>(minHeight)*/ + 100.0f;


	// 지형 타일링 디스플레이 리스트 생성
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
	// 타일링한 지형 디스플레이 리스트를 불러오는 함수
	// 레벨 1 디테일 지형, 카메라가 위치한 곳에 한 블록만 그림
	glPushMatrix();
	glTranslatef(getTileX(x) * (_map_width - 1), 0.0, getTileY(y) * (_map_height - 1));
	glCallList(baseTerrain);
	glPopMatrix();
	// 레벨 4 디테일 지형
	// 레벨 1 블록을 제외하고 3 x 3단위로 그림
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
	// 레벨 8 디테일 지형
	// 레벨 4 블록을 제외하고 5 x 5 단위로 그림
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
	// 레벨 16 디테일 지형
	// 레벨 8 블록을 제외하고 7 x 7 단위로 그림
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
	// 물 디스플레이 리스트 3 x 3면적에 그림
	for (i = -2; i < 3; i++) {
		for (j = -2; j < 3; j++) {
			glPushMatrix();
			glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0, getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
			glCallList(baseTerrain + 4);
			glPopMatrix();
		}
	}
}
void Terrain::TileTerrain(GLint levelOfDetail) {												// LoD 파라미터를 받아 각 타일을 그리는 함수
	glEnable(GL_TEXTURE_2D);																	// 텍스처 활성화
	glMatrixMode(GL_MODELVIEW);																	// 모델 뷰 행렬
	glPushMatrix();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);									// 텍스처 환경 설정
	glBindTexture(GL_TEXTURE_2D, _texId_ground);												// 지형 텍스처 ID가 가리키는 텍스처로 아래 정점 연결
	float coordX = (float)1.0 / _map_width;														// 가로 이동 단위
	float coordZ = (float)1.0 / _map_height;													// 세로 이동 단위
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
void Terrain::RenderWater() {																	// 물 그리기
	glEnable(GL_TEXTURE_2D);																	// 텍스처 활성화
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);								// 텍스처 환경 설정

	glEnable(GL_BLEND);																			// 블렌딩 활성화
	glDisable(GL_CULL_FACE);																	// 후면 제거 비활성화
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);											// 블렌딩 방식 지정
	glColor4f(0.0, 0.4, 0.5, 0.7);

	glBindTexture(GL_TEXTURE_2D, _texId_water);													// 텍스처 매핑
	glBegin(GL_QUADS);																			// 사각형 단위로 그리기
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, waterLevel, 0.0);									// 같은 물높이 유지하고 정점 연결
	glTexCoord2f(15.0, 0.0); glVertex3f(0.0, waterLevel, _map_height);
	glTexCoord2f(15.0, 15.0); glVertex3f(_map_height, waterLevel, _map_height);
	glTexCoord2f(0.0, 15.0); glVertex3f(_map_height, waterLevel, 0.0);
	glEnd();
	glEnable(GL_CULL_FACE);																		// 후면 제거 활성화
	glDisable(GL_BLEND);																		// 블렌드 비활성화
	glDisable(GL_TEXTURE_2D);
}
//.raw 파일을 읽는 함수. filename: 높이 맵 파일의 경로. w: 높이 맵의 가로 크기. h: 높이 맵의 세로 크기
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
	// (x, z)에 해당하는 높이 값을 반환
	int col = static_cast<int>(x);
	int row = static_cast<int>(z);

	if (col < 0 || col >= _map_width || row < 0 || row >= _map_height) {
		return 0.0f; // 범위 외부면 기본값
	}

	int index = row * _map_width + col;
	return static_cast<GLfloat>(_height[index]); // 높이 데이터를 반환
}

GLint Terrain::getTileX(GLfloat x) {															// 현재 카메라가 위치한 텍스처 타일의 블록 좌표를 리턴 : x 좌표
	if (x > 0) return ((int)x) / (int)_map_width;
	else return ((int)x)/ (int)_map_width - 1;
}
GLint Terrain::getTileY(GLfloat y) {															// 현재 카메라가 위치한 텍스처 타일의 블록 좌표를 리턴 : y 좌표
	if (y > 0) return ((int)y) / (int)_map_height;
	else return ((int)y) / (int)_map_height - 1;
}