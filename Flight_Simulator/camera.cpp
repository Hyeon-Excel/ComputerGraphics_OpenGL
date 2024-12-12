#include "camera.h"

Camera::Camera(void) {
	setDefaultCamera();										// ������, �ʱ�ȭ �Լ��� ������ ȣ��
}
void Camera::setDefaultCamera() {
	printf("Initializing camera with default values.\n");
	setShape(45.0f, 640 / (float)480, 0.1f, 200.0f);
	Point3 eyePoint = Point3(10.0, 0.0, 0.0);
	Point3 lookPoint = Point3(0.0, 0.0, 0.0);
	Vector3 upVector = Vector3(0, 1, 0);
	set(eyePoint, lookPoint, upVector);
}
void Camera::set(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ) {
	Point3 Eye = Point3(eyeX, eyeY, eyeZ);					// ī�޶�, ����, �� ���� ����
	Point3 look = Point3(lookX, lookY, lookZ);
	Vector3 up = Vector3(upX, upY, upZ);
	eye.set(Eye);											// ī�޶� ��ġ ����
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);	// �������� ī�޶� ��ǥ�� �� n ���� ����
	u.set(up.cross(n));										// ī�޶� �� ���Ϳ��� �������� u ���� ����
	v.set(n.cross(u));										// u�� n�� �������� ���� ��ǥ���� v ���� ����
	u.normalize(); v.normalize(); n.normalize();			// ���� ��ǥ�� ���� ����ȭ
	setModelViewMatrix();									// ���ǵ� ���� ��ǥ�踦 �� �� ��Ŀ� ����
}
void Camera::set(Point3 Eye, Point3 look, Vector3 up) {		// �� set �Լ��� �������̵� ���
	eye.set(Eye);
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);
	u.set(up.cross(n));
	v.set(n.cross(u));
	u.normalize(); v.normalize(); n.normalize();
	setModelViewMatrix();
}
void Camera::setModelViewMatrix(void) {
	float m[16];											// �� �� ��Ŀ� ���� ��� ��, 4 x 4 = 16���� ���Ҹ� ����.
	Vector3 eVec(eye.x, eye.y, eye.z);						// ī�޶� ��ġ
	// ȸ�� ��ȯ�� �̵� ��ȯ ����� �� ��� �ۼ�
	m[0] = u.x;		m[4] = u.y;		m[8] = u.z;		m[12] = -eVec.dot(u);
	m[1] = v.x;		m[5] = v.y;		m[9] = v.z;		m[13] = -eVec.dot(v);
	m[2] = n.x;		m[6] = n.y;		m[10] = n.z;	m[14] = -eVec.dot(n);
	m[3] = 0;		m[7] = 0;		m[11] = 0;		m[15] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);										// ��� m�� �� �� ��Ŀ� �־���
}
void Camera::setShape(float vAngle, float asp, float nr, float fr) {	// ���� ����
	viewAngle = vAngle;													// �þ߰�
	aspect = asp;														// ��Ⱦ��
	nearDist = nr;														// ���� �����
	farDist = fr;														// �Ĺ� �����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();													// ���� ����� ���� �����ϱ� ���� �յ� ��� �ε�
	gluPerspective(viewAngle, aspect, nearDist, farDist);				// ���� ���� ����
	glMatrixMode(GL_MODELVIEW);
}
void Camera::setAspect(float asp) {
	aspect = asp;														// ��Ⱦ�� ���� ����
}
void Camera::getShape(float &vAngle, float &asp, float &nr, float &fr) {
	vAngle = viewAngle;
	asp = aspect;
	nr = nearDist;
	fr = farDist;
}
void Camera::slide(double du, double dv, double dn) {					// �̵� ��ȯ
	eye.x += du * u.x + dv * v.x + dn * n.x;							// ���� ��ǥ�� ����ȭ ��� ���Ϳ� �̵� ��ȯ ����� ����
	eye.y += du * u.y + dv * v.y + dn * n.y;							// ���� ��ǥ�谡 �̵��� ȿ��
	eye.z += du * u.z + dv * v.z + dn * n.z;
	setModelViewMatrix();
}
void Camera::rotAxes(Vector3 &a, Vector3 &b, float angle) {				// ���� ��ǥ�踦 �̷�� ��� ���͸� ��￩ ȸ�� ��ȯ
	float ang = 3.14159265 / 180 * angle;								// ���� ���� ������ ��ȯ
	float C = cos(ang), S = sin(ang);									// ��ȯ�Ϸ��� ���� cos, sin���� ����
	Vector3 t(C * a.x + S * b.x, C * a.y + S * b.y, C * a.z + S * b.z);
	b.set(-S * a.x + C * b.x, -S * a.y + C * b.y, -S * a.z + C * b.z);	// �̸� �̿��� �� ���� angle��ŭ ȸ��
	a.set(t.x, t.y, t.z);
}
void Camera::roll(float angle) {
	rotAxes(u, v, angle);												// n���� �������� ȸ��, ������ u, v���� ���� ��ŭ ȸ��
	setModelViewMatrix();												// �𵨺� ��Ŀ� ����
}
void Camera::pitch(float angle) {
	rotAxes(n, v, angle);												// u���� �������� ȸ��, ������ , v���� ������ŭ ȸ��
	setModelViewMatrix();												// �� �� ��Ŀ� ����
}
void Camera::yaw(float angle) {
	rotAxes(u, n, angle);												// v���� �������� ȸ��, ������ n, v���� ������ŭ ȸ��
	setModelViewMatrix();												// �� �� ��Ŀ� ����
}
Vector3 Vector3::cross(const Vector3 &b) {
	Vector3 c(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	return c;															// �޼��� ȣ�� ���Ϳ� �Ķ���� ������ ���� ���͸� �����ϴ� �Լ�
}
float Vector3::dot(const Vector3 &b) { return x * b.x + y * b.y + z * b.z; }	// �� ������ ����
void Vector3::normalize() {												// �ش� ���͸� ����ȭ �ϴ� �Լ�
	double sizeSq = x * x + y * y + z * z;								// ũ�⸦ ����
	if (sizeSq < 0.0000001) {
		std::cerr << "\nnormalize() sees vector(0, 0, 0)!";
		return;
	}
	float scaleFactor = 1.0 / (float)sqrt(sizeSq);						// ���͸� �� ���� ũ��� ������
	x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
}