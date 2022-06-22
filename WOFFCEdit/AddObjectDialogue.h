#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include <string>

class ToolMain;
class MFCMain;

class AddObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(AddObjectDialogue)

public:
	AddObjectDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AddObjectDialogue();
	void SetObjectData(MFCMain* mfcMainPtr);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddObject };
#endif

protected:
	MFCMain* m_mfcMain;
	CString m_objPath;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTvnSelChangedFileTree(NMHDR* pNMHDR, LRESULT* pResult);

	std::wstring GetParentDirectory(std::wstring str);
	void FindFile(CString strFileRootPath, HTREEITEM hRootItem);

	static DWORD WINAPI EnumFileThread(LPARAM lParam);

	CTreeCtrl m_fileTree;

	HTREEITEM m_rootItem = nullptr;
	HTREEITEM m_treeItem = nullptr;
};
