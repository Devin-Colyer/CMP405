//
// Camera.h
//

#pragma once

#include "pch.h"
#include "InputCommands.h"

class Camera
{
private:
	RECT m_viewportDimensions;

	//camera
	float m_moveSpeed;
	float m_camRotRate;
	float m_mouseSensitivity;

	DirectX::SimpleMath::Vector3 m_camPosition;
	DirectX::SimpleMath::Vector3 m_camOrientation;
	DirectX::SimpleMath::Vector3 m_camLookAt;
	DirectX::SimpleMath::Vector3 m_camLookDirection;
	DirectX::SimpleMath::Vector3 m_camRight;

public:
	Camera();
	Camera(float moveSpeed, float camRotationRate);
	Camera(DirectX::SimpleMath::Vector3 camPosition, DirectX::SimpleMath::Vector3 camOrientation);
	Camera(float moveSpeed, float camRotationRate, DirectX::SimpleMath::Vector3 camPosition, DirectX::SimpleMath::Vector3 camOrientation);

	void HandleInput(float deltaTime, InputCommands input);
	DirectX::SimpleMath::Matrix GetLookAt();

	inline void UpdateViewportRect(RECT in) { m_viewportDimensions = in; }
	// spd = units/s, default = 20
	inline void SetMoveSpeed(float spd) { m_moveSpeed = spd; }
	inline float GetMoveSpeed() { return m_moveSpeed; }
	// rate = deg/s, default = 200
	inline void SetRotationRate(float rate) { m_camRotRate = rate; }
	inline float GetRotationRate() { return m_camRotRate; }

	inline DirectX::SimpleMath::Vector3 GetCamPosition() { return m_camPosition; }
	inline DirectX::SimpleMath::Vector3 GetCamOrientation() { return m_camOrientation; }
};

