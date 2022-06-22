#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"
#include "EditorSettingsDialogue.h"
#include "InspectorDialogue.h"
#include "AddObjectDialogue.h"
#include "AddTextureDialogue.h"
#include "SetTransformDialogue.h"


class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();
	void AddObjectSelectTexture(const CString inObjPath);
	void AddObjectSelectTextureSetTransform(const CString inObjPath, const CString inTexPath);

private:

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_ToolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	EditorSettingsDialogue m_ToolEditorSettingsDialogue;
	SelectDialogue m_ToolSelectDialogue;			//for modeless dialogue, declare it here
	InspectorDialogue m_ToolInspectorDialogue;
	AddObjectDialogue m_ToolAddObjectDialogue;
	AddTextureDialogue m_ToolAddTextureDialogue;
	SetTransformDialogue m_ToolSetTransformDialogue;
	int m_width;		
	int m_height;
	
	int m_selectedID;

	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileEditorSettings();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg void MenuEditInspector();
	afx_msg void MenuEditAddObject();
	afx_msg	void ToolBarButton1();


	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};
