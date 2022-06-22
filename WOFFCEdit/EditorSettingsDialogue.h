#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "ToolMain.h"

// EditorSettingsDialog dialog

class EditorSettingsDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(EditorSettingsDialogue)

public:
	EditorSettingsDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~EditorSettingsDialogue();
	void SetObjectData(ToolMain* appPtr);	//passing in pointers to the data the class will operate on.

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Settings };
#endif
private:
	ToolMain* m_app;

	void RemoveTrailingZeros(CString& str);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_moveSpeedControl;
	CEdit m_rotationSpeedControl;

	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
