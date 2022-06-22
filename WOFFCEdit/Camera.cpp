#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera()
{
	//camera fs
	m_moveSpeed = 15.f;
	m_camRotRate = 200.f;
	m_mouseSensitivity = 2.f;
	
	//camera vs
	m_camPosition.x = 0.f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0.f;
	m_camOrientation.y = 0.f;
	m_camOrientation.z = 0.f;

	m_camLookAt.x = 0.f;
	m_camLookAt.y = 0.f;
	m_camLookAt.z = 0.f;

	m_camLookDirection.x = 0.f;
	m_camLookDirection.y = 0.f;
	m_camLookDirection.z = 0.f;

	m_camRight.x = 0.f;
	m_camRight.y = 0.f;
	m_camRight.z = 0.f;

	HandleInput(NULL, InputCommands());
}

Camera::Camera(float moveSpeed, float camRotationRate)
{
	//camera fs
	m_moveSpeed = moveSpeed;
	m_camRotRate = camRotationRate;
	m_mouseSensitivity = 2.f;

	//camera vs
	m_camPosition.x = 0.f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0.f;
	m_camOrientation.y = 0.f;
	m_camOrientation.z = 0.f;

	m_camLookAt.x = 0.f;
	m_camLookAt.y = 0.f;
	m_camLookAt.z = 0.f;

	m_camLookDirection.x = 0.f;
	m_camLookDirection.y = 0.f;
	m_camLookDirection.z = 0.f;

	m_camRight.x = 0.f;
	m_camRight.y = 0.f;
	m_camRight.z = 0.f;

	HandleInput(NULL, InputCommands());
}

Camera::Camera(Vector3 camPosition, Vector3 camOrientation)
{
	//camera fs
	m_moveSpeed = 15.f;
	m_camRotRate = 200.f;

	//camera vs
	m_camPosition = camPosition;
	m_camOrientation = camOrientation;
	m_mouseSensitivity = 2.f;

	m_camLookAt.x = 0.f;
	m_camLookAt.y = 0.f;
	m_camLookAt.z = 0.f;

	m_camLookDirection.x = 0.f;
	m_camLookDirection.y = 0.f;
	m_camLookDirection.z = 0.f;

	m_camRight.x = 0.f;
	m_camRight.y = 0.f;
	m_camRight.z = 0.f;

	HandleInput(NULL, InputCommands());
}

Camera::Camera(float moveSpeed, float camRotationRate, Vector3 camPosition, Vector3 camOrientation)
{
	//camera fs
	m_moveSpeed = moveSpeed;
	m_camRotRate = camRotationRate;
	m_mouseSensitivity = 2.f;

	//camera vs
	m_camPosition = camPosition;
	m_camOrientation = camOrientation;

	m_camLookAt.x = 0.f;
	m_camLookAt.y = 0.f;
	m_camLookAt.z = 0.f;

	m_camLookDirection.x = 0.f;
	m_camLookDirection.y = 0.f;
	m_camLookDirection.z = 0.f;

	m_camRight.x = 0.f;
	m_camRight.y = 0.f;
	m_camRight.z = 0.f;

	HandleInput(NULL, InputCommands());
}

void Camera::HandleInput(float deltaTime, InputCommands input)
{
	// rotation
	if (input.useMouseForCam) {
		int midX = (m_viewportDimensions.right + m_viewportDimensions.left) / 2,
			midY = (m_viewportDimensions.bottom + m_viewportDimensions.top) / 2;

		int midToMouseX = input.mouseX - midX,
			midToMouseY = input.mouseY - midY;

		float magnitude = sqrtf(pow(midToMouseX, 2) + pow(midToMouseY, 2));

		if (magnitude) {
			float normalisedX = midToMouseX / magnitude,
				normalisedY = midToMouseY / magnitude;

			if (midToMouseX) {
				m_camOrientation.x -= normalisedY * m_camRotRate * m_mouseSensitivity * deltaTime;

				if (m_camOrientation.x > 85.f)
					m_camOrientation.x = 85.f;
				else if (m_camOrientation.x < -85.f)
					m_camOrientation.x = -85.f;
			}

			if (midToMouseY) {
				m_camOrientation.y += normalisedX * m_camRotRate * m_mouseSensitivity * deltaTime;
				if (m_camOrientation.y > 180.f)
					m_camOrientation.y = -180;
				else if (m_camOrientation.y < -180.f)
					m_camOrientation.y = 180;
			}
		}

		SetCursorPos(midX, midY);
	}
	else {
		if (input.yawRight)
		{
			m_camOrientation.y += m_camRotRate * deltaTime;
			if (m_camOrientation.y > 180.f)
				m_camOrientation.y = -180;
		}
		else if (input.yawLeft)
		{
			m_camOrientation.y -= m_camRotRate * deltaTime;
			if (m_camOrientation.y < -180.f)
				m_camOrientation.y = 180;
		}

		if (input.pitchUp)
		{
			m_camOrientation.x += m_camRotRate * deltaTime;
			if (m_camOrientation.x > 85.f)
				m_camOrientation.x = 85.f;
		}
		else if (input.pitchDown)
		{
			m_camOrientation.x -= m_camRotRate * deltaTime;
			if (m_camOrientation.x < -85.f)
				m_camOrientation.x = -85.f;
		}
	}

	//create look direction from Euler angles in m_camOrientation
	//m_camLookDirection.x = sin((m_camOrientation.y) * 3.1415 / 180);
	//m_camLookDirection.z = cos((m_camOrientation.y) * 3.1415 / 180);
	m_camLookDirection.x = cos((m_camOrientation.y) * 3.1415 / 180) * cos((m_camOrientation.x) * 3.1415 / 180);
	m_camLookDirection.y = sin((m_camOrientation.x) * 3.1415 / 180);
	m_camLookDirection.z = sin((m_camOrientation.y) * 3.1415 / 180) * cos((m_camOrientation.x) * 3.1415 / 180);
	m_camLookDirection.Normalize();

	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	// flight control input
	if (input.forward && input.slowFlight)
		m_camPosition += m_camLookDirection * m_moveSpeed * .5f * deltaTime;
	else if (input.forward)
		m_camPosition += m_camLookDirection * m_moveSpeed * deltaTime;

	if (input.back && input.slowFlight)
		m_camPosition -= m_camLookDirection * m_moveSpeed * .5f * deltaTime;
	else if (input.back)
		m_camPosition -= m_camLookDirection * m_moveSpeed * deltaTime;

	if (input.right && input.slowFlight)
		m_camPosition += m_camRight * m_moveSpeed * .5f * deltaTime;
	else if (input.right)
		m_camPosition += m_camRight * m_moveSpeed * deltaTime;

	if (input.left && input.slowFlight)
		m_camPosition -= m_camRight * m_moveSpeed * .5f * deltaTime;
	else if (input.left)
		m_camPosition -= m_camRight * m_moveSpeed * deltaTime;

	if (input.flyUp && input.slowFlight)
		m_camPosition.y += m_moveSpeed * .5f * deltaTime;
	else if (input.flyUp)
		m_camPosition.y += m_moveSpeed * deltaTime;

	if (input.flyDown && input.slowFlight)
		m_camPosition.y -= m_moveSpeed * .5f * deltaTime;
	else if (input.flyDown)
		m_camPosition.y -= m_moveSpeed * deltaTime;

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;
}

Matrix Camera::GetLookAt()
{
	return Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);
}
