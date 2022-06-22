#include "MFCMain.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT, &MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SETTINGS, &MFCMain::MenuFileEditorSettings)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_EDIT_INSPECTOR, &MFCMain::MenuEditInspector)
	ON_COMMAND(ID_EDIT_ADDOBJECT, &MFCMain::MenuEditAddObject)
	ON_COMMAND(ID_BUTTON40001,	&MFCMain::ToolBarButton1)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	m_frame->Create(	NULL,
					_T("World Of Flim-Flam Craft Editor"),
					WS_OVERLAPPEDWINDOW,
					CRect(100, 100, 1024, 768),
					NULL,
					NULL,
					0,
					NULL
				);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();


	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);
	
	m_selectedID = -1;
	
	return TRUE;
}

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);
		}
		else
		{	
			if (m_selectedID != m_ToolSystem.getCurrentSelectionID())
				if (IsWindow(m_ToolInspectorDialogue.GetSafeHwnd()))
					m_ToolInspectorDialogue.ResetInspector();

			m_selectedID = m_ToolSystem.getCurrentSelectionID();
			std::wstring statusString = L"Selected Object: " + std::to_wstring(m_selectedID);
			m_ToolSystem.Tick(&msg);

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);	
		}
	}

	return (int)msg.wParam;
}

void MFCMain::AddObjectSelectTexture(const CString inObjPath)
{
	m_ToolAddTextureDialogue.Create(IDD_AddTexture);	//Start up modeless
	m_ToolAddTextureDialogue.ShowWindow(SW_SHOW);	//show modeless
	m_ToolAddTextureDialogue.SetObjectData(this, inObjPath);
}

void MFCMain::AddObjectSelectTextureSetTransform(const CString inObjPath, const CString inTexPath)
{
	m_ToolSetTransformDialogue.Create(IDD_SetTransform);	//Start up modeless
	m_ToolSetTransformDialogue.ShowWindow(SW_SHOW);	//show modeless
	m_ToolSetTransformDialogue.SetObjectData(this, &m_ToolSystem, inObjPath, inTexPath);
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileEditorSettings()
{
	m_ToolEditorSettingsDialogue.Create(IDD_Settings);	//Start up modeless
	m_ToolEditorSettingsDialogue.ShowWindow(SW_SHOW);	//show modeless
	m_ToolEditorSettingsDialogue.SetObjectData(&m_ToolSystem);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}

void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);		//create our dialoguebox //modal constructor
	//m_ToolSelectDialogue.DoModal();	// start it up modal

	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolSelectDialogue.Create(IDD_SelectObject);	//Start up modeless
	m_ToolSelectDialogue.ShowWindow(SW_SHOW);	//show modeless
	m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject);
}

void MFCMain::MenuEditInspector()
{
	if (m_ToolSystem.m_selectedObject != -1) {
		m_ToolInspectorDialogue.Create(IDD_InspectorFrame);	//Start up modeless
		m_ToolInspectorDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_ToolInspectorDialogue.SetObjectData(&m_ToolSystem);
	}
	else {
		MessageBox(m_toolHandle, L"Make sure to select an object before opening the inspector.", L"Error", MB_OK);
	}
}

void MFCMain::MenuEditAddObject()
{
	m_ToolAddObjectDialogue.Create(IDD_AddObject);	//Start up modeless
	m_ToolAddObjectDialogue.ShowWindow(SW_SHOW);	//show modeless
	m_ToolAddObjectDialogue.SetObjectData(this);
}

void MFCMain::ToolBarButton1()
{
	
	m_ToolSystem.onActionSave();
}


MFCMain::MFCMain()
{
}


MFCMain::~MFCMain()
{
}
