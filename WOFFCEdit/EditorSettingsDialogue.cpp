// EditorSettingsDialog.cpp : implementation file
//

#include "EditorSettingsDialogue.h"


// EditorSettingsDialog dialog

IMPLEMENT_DYNAMIC(EditorSettingsDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(EditorSettingsDialogue, CDialogEx)
	ON_BN_CLICKED(IDOK, &EditorSettingsDialogue::OnBnClickedOk)				//ok button
	ON_BN_CLICKED(IDCANCEL, &EditorSettingsDialogue::OnBnClickedCancel)		//cancel button
END_MESSAGE_MAP()

EditorSettingsDialogue::EditorSettingsDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Settings, pParent)
{

}

EditorSettingsDialogue::~EditorSettingsDialogue()
{
}

void EditorSettingsDialogue::SetObjectData(ToolMain* appPtr)
{
	m_app = appPtr;
	CString movSpd, rotSpd;

	movSpd.Format(_T("%f"), m_app->GetCameraMoveSpeed());
	RemoveTrailingZeros(movSpd);

	rotSpd.Format(_T("%f"), m_app->GetCameraRotationSpeed());
	RemoveTrailingZeros(rotSpd);

	m_moveSpeedControl.SetWindowTextW(movSpd);
	m_rotationSpeedControl.SetWindowTextW(rotSpd);
}

void EditorSettingsDialogue::RemoveTrailingZeros(CString& str)
{
	bool flag = false;
	while (!flag) {
		if (str[CString::StringLength(str) - 1] == '0') {
			str.Delete((CString::StringLength(str) - 1), 1);
		}
		else if (str[CString::StringLength(str) - 1] == '.') {
			str.Delete((CString::StringLength(str) - 1), 1);
			break;
		}
		else {
			break;
		}
	}
}

void EditorSettingsDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MoveSpeed, m_moveSpeedControl);
	DDX_Control(pDX, IDC_RotationSpeed, m_rotationSpeedControl);
}

BOOL EditorSettingsDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

void EditorSettingsDialogue::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
}

void EditorSettingsDialogue::OnBnClickedOk()
{
	CString MoveVal, RotationVal;

	m_moveSpeedControl.GetWindowText(MoveVal);
	m_rotationSpeedControl.GetWindowText(RotationVal);
	m_app->UpdateSettings(_ttof(MoveVal), _ttof(RotationVal));

	CDialogEx::OnOK();
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

void EditorSettingsDialogue::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}


// EditorSettingsDialogue message handlers
