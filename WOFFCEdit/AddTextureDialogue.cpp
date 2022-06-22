// AddTextureDialogue.cpp : implementation file
//

#include "AddTextureDialogue.h"
#include "MFCMain.h"

// AddObjectDialogue dialog
IMPLEMENT_DYNAMIC(AddTextureDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(AddTextureDialogue, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_SELCHANGED, IDC_FILETREE, &AddTextureDialogue::OnTvnSelChangedFileTree)
	ON_BN_CLICKED(IDOK, &AddTextureDialogue::OnBnClickedOk)				//ok button
	ON_BN_CLICKED(IDCANCEL, &AddTextureDialogue::OnBnClickedCancel)		//cancel button
END_MESSAGE_MAP()


AddTextureDialogue::AddTextureDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AddObject, pParent)
{
	m_mfcMain = nullptr;
}

AddTextureDialogue::~AddTextureDialogue()
{
}

void AddTextureDialogue::SetObjectData(MFCMain* mfcMainPtr, const CString inObjPath)
{
	m_mfcMain = mfcMainPtr;
	m_objPath = inObjPath;
}

void AddTextureDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILETREE, m_fileTree);
}

BOOL AddTextureDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here
	// Tree root ctrl operation
	m_rootItem = m_fileTree.InsertItem(L"Root");

	// Prevent fake interface death
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EnumFileThread, this, 0, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void AddTextureDialogue::OnBnClickedOk()
{
	if (m_texPath.Find(L".dds") == m_texPath.GetLength() - 4) {
		m_texPath.Insert(0, L"database/");
		m_mfcMain->AddObjectSelectTextureSetTransform(m_objPath, m_texPath);

		CDialogEx::OnOK();
		DestroyWindow();
	}
	else {
		MessageBox(L"Make sure to select a .dds file to act as the texture.", L"Error", MB_OK);
	}
}

void AddTextureDialogue::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();
}

void AddTextureDialogue::OnTvnSelChangedFileTree(NMHDR* pNMHDR, LRESULT* pResult)
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
	m_texPath = filepath;
	*pResult = 0;
}

std::wstring AddTextureDialogue::GetParentDirectory(std::wstring str)
{
	return str.substr(0, str.find_last_of(L"\\/") + 1);
}

void AddTextureDialogue::FindFile(CString strFileRootPath, HTREEITEM hRootItem)
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
				if (strFileName.Find(L".dds") == strFileName.GetLength() - 4)
					m_fileTree.InsertItem(strFileName, hRootItem);
			}
		}
	}
}


DWORD WINAPI AddTextureDialogue::EnumFileThread(LPARAM lParam) {
	AddTextureDialogue* dlg = (AddTextureDialogue*)lParam;

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