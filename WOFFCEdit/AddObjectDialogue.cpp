// AddObjectDialogue.cpp : implementation file
//

#include "AddObjectDialogue.h"
#include "MFCMain.h"

// AddObjectDialogue dialog
IMPLEMENT_DYNAMIC(AddObjectDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(AddObjectDialogue, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_SELCHANGED, IDC_FILETREE, &AddObjectDialogue::OnTvnSelChangedFileTree)
	ON_BN_CLICKED(IDOK, &AddObjectDialogue::OnBnClickedOk)				//ok button
	ON_BN_CLICKED(IDCANCEL, &AddObjectDialogue::OnBnClickedCancel)		//cancel button
END_MESSAGE_MAP()


AddObjectDialogue::AddObjectDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AddObject, pParent)
{
	m_mfcMain = nullptr;
}

AddObjectDialogue::~AddObjectDialogue()
{
}

void AddObjectDialogue::SetObjectData(MFCMain* mfcMainPtr)
{
	m_mfcMain = mfcMainPtr;
}

void AddObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILETREE, m_fileTree);
}

BOOL AddObjectDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here
	// Tree root ctrl operation
	m_rootItem = m_fileTree.InsertItem(L"Root");

	// Prevent fake interface death
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EnumFileThread, this, 0, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void AddObjectDialogue::OnBnClickedOk()
{
	if (m_objPath.Find(L".cmo") == m_objPath.GetLength() - 4) {
		m_objPath.Insert(0, L"database/");		
		m_mfcMain->AddObjectSelectTexture(m_objPath);

		CDialogEx::OnOK();
		DestroyWindow();
	}
	else {
		MessageBox(L"Make sure to select a .cmo file to act as the model.", L"Error", MB_OK);
	}
}

void AddObjectDialogue::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();
}

void AddObjectDialogue::OnTvnSelChangedFileTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString filepath;
	HTREEITEM selectedItem = m_fileTree.GetSelectedItem();
	filepath.Insert(0, m_fileTree.GetItemText(selectedItem));
	
	while (true) {
		selectedItem = m_fileTree.GetParentItem(selectedItem);
		CString itemText = m_fileTree.GetItemText(selectedItem);
		if (itemText == "" || filepath == "Root") {
			filepath.Empty();
			break;
		}
		else if (itemText == "Root") {
			break;
		}
		else {
			filepath.Insert(0, itemText + '/');
		}
	}
	m_objPath = filepath;
	*pResult = 0;
}

std::wstring AddObjectDialogue::GetParentDirectory(std::wstring str)
{
	return str.substr(0, str.find_last_of(L"\\/") + 1);
}

void AddObjectDialogue::FindFile(CString strFileRootPath, HTREEITEM hRootItem)
{
	CFileFind finder;
	CString strFilePath = strFileRootPath + L"*.*";	// *.* = all files

	// Current node
	HTREEITEM hCurrentNode;

	BOOL bRes = finder.FindFile(strFilePath);	// Searches a directory for a specified file name.
	while (bRes) {
		bRes = finder.FindNextFileW();	// Continues a file search from a previous call to FindFile
		if (!finder.IsDots()) {	// Determines if the name of the found file has the name "." or "..", indicating that is actually a directory
			CString strFileName;
			if (finder.IsDirectory()) {	// Determines if the found file is a directory.
				strFileName = finder.GetFileName();	// Gets the name, including the extension, of the found file
				CString strFilePath = finder.GetFilePath() + L"\\*.*";	// Gets the whole path of the found file.
				hCurrentNode = m_fileTree.InsertItem(strFileName, hRootItem);
				FindFile(strFilePath, hCurrentNode);
			}
			else {
				strFileName = finder.GetFileName();
				if (strFileName.Find(L".cmo") == strFileName.GetLength() - 4)
					m_fileTree.InsertItem(strFileName, hRootItem);
			}
		}
	}
}


DWORD WINAPI AddObjectDialogue::EnumFileThread(LPARAM lParam) {
	AddObjectDialogue* dlg = (AddObjectDialogue*)lParam;

	TCHAR szBuffer[MAX_PATH] = { 0 };

	// Specific Directory
	DWORD dwLength = GetModuleFileName(NULL, szBuffer, MAX_PATH);
	std::wstring wStr = dlg->GetParentDirectory(szBuffer);

	size_t pos = wStr.rfind(L"Debug");
	if (pos != std::wstring::npos)
		wStr.replace(pos, wStr.length(), L"database\\");

	HTREEITEM hRootItem = dlg->m_fileTree.GetRootItem();
	dlg->FindFile(wStr.c_str(), hRootItem);

	// All Drives
	//DWORD dwLength = GetLogicalDriveStrings(sizeof szBuffer, szBuffer);
	//std::wstring wStr = szBuffer;

	//for (size_t i = 0; i < dwLength / 4; i++)
	//{
	//	CString strPANFU = wStr.c_str() + 4 * i;
	//	// Add to tree
	//	HTREEITEM hRootItem = dlg->m_FileTree.InsertItem(strPANFU, dlg->m_hRootItem);
	//	dlg->FindFile(strPANFU, hRootItem);
	//}

	return 0;
}