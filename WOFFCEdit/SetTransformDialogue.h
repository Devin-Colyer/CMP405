#pragma once
#include "afxdialogex.h"
#include "resource.h"

class ToolMain;
class MFCMain;

// SetTransformDialogue dialog

class SetTransformDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(SetTransformDialogue)

public:
	SetTransformDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SetTransformDialogue();
	void SetObjectData(MFCMain* mfcMainPtr, ToolMain* appPtr, const CString inObjPath, const CString inTexPath);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SetTransform };
#endif

protected:
	MFCMain* m_mfcMain;
	ToolMain* m_app;
	CString m_objPath;
	CString m_texPath;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()

public:

	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	CEdit m_PositionXControl;
	CEdit m_PositionYControl;
	CEdit m_PositionZControl;
	CEdit m_PitchControl;
	CEdit m_YawControl;
	CEdit m_RollControl;
	CEdit m_ScaleXControl;
	CEdit m_ScaleYControl;
	CEdit m_ScaleZControl;
};
