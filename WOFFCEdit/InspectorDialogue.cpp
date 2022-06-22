// InspectorDialogue.cpp : implementation file
//

#include "InspectorDialogue.h"


// InspectorDialogue dialog

IMPLEMENT_DYNAMIC(InspectorDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(InspectorDialogue, CDialogEx)
	ON_BN_CLICKED(IDOK, &InspectorDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &InspectorDialogue::OnBnClickedCancel)
END_MESSAGE_MAP()

InspectorDialogue::InspectorDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_InspectorFrame, pParent)
{
	m_scrollableChild = nullptr;
}

InspectorDialogue::~InspectorDialogue()
{
	delete m_scrollableChild;
}

void InspectorDialogue::SetObjectData(ToolMain* appPtr)
{
	m_scrollableChild->SetObjectData(appPtr);
}

void InspectorDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL InspectorDialogue::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_scrollableChild = new InspectorScrollableDialogue(this);

	CRect rc;
	GetDlgItem(IDC_InspectorFrame)->GetWindowRect(rc);
	ScreenToClient(&rc);

	m_scrollableChild->MoveWindow(rc);

	return TRUE;
}

void InspectorDialogue::OnBnClickedOk()
{
	m_scrollableChild->Confirm();
}

void InspectorDialogue::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();
}

void InspectorDialogue::ResetInspector()
{
	m_scrollableChild->ResetInspector();
}