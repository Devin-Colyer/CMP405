#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include <string>

// SelectFileDialogue dialog

class SelectFileDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(SelectFileDialogue)

public:
	SelectFileDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SelectFileDialogue();
	void SetObjectData(CEdit* cEditPtr, CString fileExtension);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SelectFile };
#endif

protected:
	CEdit* m_cEdit;
	CString m_filePath, m_fileExtension;

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
