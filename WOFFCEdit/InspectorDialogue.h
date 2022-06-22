#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "InspectorScrollableDialogue.h"

class ToolMain;

// InspectorDialogue dialog

class InspectorDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(InspectorDialogue)

public:
	InspectorDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~InspectorDialogue();
	void SetObjectData(ToolMain* appPtr);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_InspectorFrame };
#endif

protected:
	InspectorScrollableDialogue* m_scrollableChild;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void ResetInspector();
};
