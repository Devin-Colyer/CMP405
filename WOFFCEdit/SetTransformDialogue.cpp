// SetTransformDialogue.cpp : implementation file
//

#include "SetTransformDialogue.h"
#include "ToolMain.h"


// SetTransformDialogue dialog

IMPLEMENT_DYNAMIC(SetTransformDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(SetTransformDialogue, CDialogEx)
	ON_BN_CLICKED(IDOK, &SetTransformDialogue::OnBnClickedOk)				//ok button
	ON_BN_CLICKED(IDCANCEL, &SetTransformDialogue::OnBnClickedCancel)		//cancel button
END_MESSAGE_MAP()

SetTransformDialogue::SetTransformDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SetTransform, pParent)
{
	m_mfcMain = nullptr;
	m_app = nullptr;
}

SetTransformDialogue::~SetTransformDialogue()
{
}

void SetTransformDialogue::SetObjectData(MFCMain* mfcMainPtr, ToolMain* appPtr, const CString inObjPath, const CString inTexPath)
{
	m_mfcMain = mfcMainPtr;
	m_app = appPtr;
	m_objPath = inObjPath;
	m_texPath = inTexPath;
}

void SetTransformDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PositionX, m_PositionXControl);
	DDX_Control(pDX, IDC_PositionY, m_PositionYControl);
	DDX_Control(pDX, IDC_PositionZ, m_PositionZControl);
	DDX_Control(pDX, IDC_Pitch, m_PitchControl);
	DDX_Control(pDX, IDC_Yaw, m_YawControl);
	DDX_Control(pDX, IDC_Roll, m_RollControl);
	DDX_Control(pDX, IDC_ScaleX, m_ScaleXControl);
	DDX_Control(pDX, IDC_ScaleY, m_ScaleYControl);
	DDX_Control(pDX, IDC_ScaleZ, m_ScaleZControl);

	CString iniFloat;
	iniFloat.Format(_T("%f"), 0.f);

	m_PositionXControl.SetWindowTextW(iniFloat);
	m_PositionYControl.SetWindowTextW(iniFloat);
	m_PositionZControl.SetWindowTextW(iniFloat);
	m_PitchControl.SetWindowTextW(iniFloat);
	m_YawControl.SetWindowTextW(iniFloat);
	m_RollControl.SetWindowTextW(iniFloat);
	iniFloat.Format(_T("%f"), 1.f);
	m_ScaleXControl.SetWindowTextW(iniFloat);
	m_ScaleYControl.SetWindowTextW(iniFloat);
	m_ScaleZControl.SetWindowTextW(iniFloat);
}

BOOL SetTransformDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

void SetTransformDialogue::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
}

void SetTransformDialogue::OnBnClickedOk()
{
	CString posX, posY, posZ,
		rotX, rotY, rotZ,
		scaX, scaY, scaZ;

	m_PositionXControl.GetWindowText(posX);
	m_PositionYControl.GetWindowText(posY);
	m_PositionZControl.GetWindowText(posZ);
	m_PitchControl.GetWindowText(rotX);
	m_YawControl.GetWindowText(rotY);
	m_RollControl.GetWindowText(rotZ);
	m_ScaleXControl.GetWindowText(scaX);
	m_ScaleYControl.GetWindowText(scaY);
	m_ScaleZControl.GetWindowText(scaZ);

	m_app->AddObjectToScene(m_objPath,m_texPath,
		_ttof(posX), _ttof(posY), _ttof(posZ),
		_ttof(rotX), _ttof(rotY), _ttof(rotZ),
		_ttof(scaX), _ttof(scaY), _ttof(scaZ));

	CDialogEx::OnOK();
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

void SetTransformDialogue::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

// SetTransformDialogue message handlers
