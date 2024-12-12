#pragma once
// camera.h : 점과 벡터, 카메라 클래스 구성
#include <iostream>
#include <Windows.h> 
#include <strstream>
#include <math.h>
#include <GL/freeglut.h>

class Point3 {																// 정점 클래스
public:
	float x, y, z;															// 좌표
	void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; }		//	정점 좌표 변경 메서드
	void set(Point3 &p) { x = p.x; y = p.y; z = p.z; }						//	타입일 경우 정점 좌표 변경 메서드
	Point3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }		// 생성자 : 정점 초기화
	Point3() { x = y = z = 0; }												// 생성자 : 초기화 없음 시 원점
};
class Vector3 {
public:
	float x, y, z; // 벡터 성분

	// 벡터 생성 메서드
	void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; }
	void set(const Vector3 &v) { x = v.x; y = v.y; z = v.z; }

	// 반대 방향 벡터
	void flip() { x = -x; y = -y; z = -z; }

	// 두 벡터의 차
	void setDiff(const Point3 &a, const Point3 &b) {
		x = a.x - b.x;
		y = a.y - b.y;
		z = a.z - b.z;
	}

	void normalize(); // 단위 벡터로

	// 생성자들
	Vector3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }
	Vector3(const Vector3 &v) { x = v.x; y = v.y; z = v.z; } // 복사 생성자
	Vector3() { x = y = z = 0.0; }

	// 외적
	Vector3 cross(const Vector3 &b);

	// 내적
	float dot(const Vector3 &b);
};

class Camera {
public:
	Point3 eye;																// 시점 좌표계 원점
	Vector3 u, v, n;														// 시점 좌표계를 구성하는 정규 직교 기기(Basis Vector)
	float aspect, nearDist, farDist, viewAngle;								// gluPerspective의 파라미터들 시점 변환 후 이용
	void setModelViewMatrix();												// 시점 변환에 따라 모델 뷰 행렬 조정
	Camera(void);															// 생성자

	// 비행 시뮬레이터의 회전 변환 함수들
	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);
	// 이동 변환
	void slide(double du, double dv, double dn);

	// 카메라 위치, 시점, 카메라 업 벡터 지정
	void set(Point3 Eye, Point3 look, Vector3 up);																			// 벡터로
	void set(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ);	// 점으로
	void setShape(float viewAngle, float aspect, float Near, float Far);													// 화면 정의
	void setAspect(float asp);																								// 종횡비 정의
	void getShape(float &viewAngle, float &aspect, float &Near, float &Far);												// 화면 구성값 보기
	void rotAxes(Vector3 &a, Vector3 &b, float angle);																		// 시점 좌표계 축 회전
	void setDefaultCamera();																								// 카메라 초기 설정 함수
};