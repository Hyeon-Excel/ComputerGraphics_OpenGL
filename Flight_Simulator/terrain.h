#pragma once
// terrain.h : ���� �ؽ�ó, �� �ؽ�ó, �Ȱ��� �������� ���� 4�ܰ� �ؽ�ó Ÿ�ϸ�
#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <math.h>
#include <GL/freeglut.h>
#include "camera.h"
#include "stb_image.h"

class Terrain {
private:
    typedef const char *pcStr;
    typedef unsigned int uint;
    typedef unsigned char ubyte;
    uint _texId_ground, _texId_water; // ���� �ؽ�ó�� �� �ؽ�ó�� ID
    ubyte *_height;                   // ���� ���� �迭�� �����ϴ� ������
    GLint _map_width, _map_height;   // ���� ������ ���� ���� ũ��
    GLuint baseTerrain;               // Ÿ���� �ε��� ��
    GLubyte minHeight;                // ���� �������� �ּ� ����
    static ubyte *readRawData(pcStr filename, uint width, uint height); // ���� ������ �о����

public:
    Terrain(pcStr heightmap, pcStr surfaceTex, GLuint width, GLint height); // ���� ������
    ~Terrain(void);                                                         // �Ҹ���
    void RenderTerrain(GLfloat, GLfloat);                                   // ���� �׸���
    void TileTerrain(GLint);                                                // LoD�� ���� Ÿ�ϸ�
    void RenderWater(void);                                                 // �� �׸���
    GLfloat getHeight(GLfloat, GLfloat);                                    // �ش� ��ġ�� ���� ���ϱ�
    GLint getTileX(GLfloat);                                                // �ش� ��ġ�� x Ÿ�� ���ϱ�
    GLint getTileY(GLfloat);                                                // �ش� ��ġ�� y Ÿ�� ���ϱ�
    GLfloat waterLevel;                                                     // ���� ����
};