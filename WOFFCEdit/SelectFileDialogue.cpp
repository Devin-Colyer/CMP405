// SelectFileDialogue.cpp : implementation file
//

#include "SelectFileDialogue.h"


// SelectFileDialogue dialog

IMPLEMENT_DYNAMIC(SelectFileDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(SelectFileDialogue, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_SELCHANGED, IDC_FILETREE, &SelectFileDialogue::OnTvnSelChangedFileTree)
	ON_BN_CLICKED(IDOK, &SelectFileDialogue::OnBnClickedOk)				//ok button
	ON_BN_CLICKED(IDCANCEL, &SelectFileDialogue::OnBnClickedCancel)		//cancel button
END_MESSAGE_MAP()

SelectFileDialogue::SelectFileDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SelectFile, pParent)
{

}

SelectFileDialogue::~SelectFileDialogue()
{
}

void SelectFileDialogue::SetObjectData(CEdit* cEditPtr, CString fileExtension)
{
	m_cEdit = cEditPtr;
	m_fileExtension = fileExtension;
	if (m_fileExtension[0] != '.')
		m_fileExtension.Insert(0, '.');
}
void SelectFileDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILETREE, m_fileTree);
}

BOOL SelectFileDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here
	// Tree root ctrl operation
	m_rootItem = m_fileTree.InsertItem(L"Root");

	// Prevent fake interface death
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EnumFileThread, this, 0, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void SelectFileDialogue::OnBnClickedOk()
{
	if (m_filePath.Find(m_fileExtension) == m_filePath.GetLength() - 4 && IsWindow(m_cEdit->GetSafeHwnd())) {
		m_filePath.Insert(0, L"database/");
		m_cEdit->SetWindowText(m_filePath);

		CDialogEx::OnOK();
		DestroyWindow();
	}
	else if (IsWindow(m_cEdit->GetSafeHwnd())) {
		MessageBox(L"Make sure to select a " + m_fileExtension + L" file.", L"Error", MB_OK);
	}
	else {
		DestroyWindow();
	}
}

void SelectFileDialogue::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();
}

void SelectFileDialogue::OnTvnSelChangedFileTree(NMHDR* pNMHDR, LRESULT* pResult)
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
	m_filePath = filepath;
	*pResult = 0;
}

std::wstring SelectFileDialogue::GetParentDirectory(std::wstring str)
{
	return str.substr(0, str.find_last_of(L"\\/") + 1);
}

void SelectFileDialogue::FindFile(CString strFileRootPath, HTREEITEM hRootItem)
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
				if (strFileName.Find(m_fileExtension) == strFileName.GetLength() - m_fileExtension.GetLength())
					m_fileTree.InsertItem(strFileName, hRootItem);
			}
		}
	}
}


DWORD WINAPI SelectFileDialogue::EnumFileThread(LPARAM lParam) {
	SelectFileDialogue* dlg = (SelectFileDialogue*)lParam;

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


// SelectFileDialogue message handlers
