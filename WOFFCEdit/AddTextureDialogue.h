#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include <string>

class MFCMain;

// AddTextureDialogue dialog

class AddTextureDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(AddTextureDialogue)

public:
	AddTextureDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AddTextureDialogue();
	void SetObjectData(MFCMain* mfcMainPtr, const CString inObjPath);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddTexture };
#endif

protected:
	MFCMain* m_mfcMain;
	CString m_objPath;
	CString m_texPath;

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