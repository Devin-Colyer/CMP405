#pragma once
#include "afxdialogex.h"
#include "resource.h"

class ToolMain;
class SelectFileDialogue;

// InspectorScrollableDialogue dialog

class InspectorScrollableDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(InspectorScrollableDialogue)

public:
	InspectorScrollableDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~InspectorScrollableDialogue();
	void SetObjectData(ToolMain* appPtr);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_InspectorScrollable };
#endif

protected:
	CWnd* m_parent;
	ToolMain* m_app;
	// dialog size as in resource view
	CRect m_rcOriginalRect;

	// dragging
	BOOL m_bDragging;
	CPoint m_dragPoint;

	// scrolling
	int m_scrollPos;
	int m_currentHeight;
	
	SelectFileDialogue* m_selectFileDialogue;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	template<typename T>
	CString NumberValueToCString(CString format, T val);
	void MakeNewSelectorWindow();
	
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	void EndDrag();
	void ResetInspector();
	void Confirm();

	CEdit m_nameEdit, m_chunkIDEdit,
		m_posxEdit, m_posyEdit, m_poszEdit,
		m_rotxEdit, m_rotyEdit, m_rotzEdit,
		m_scaxEdit, m_scayEdit, m_scazEdit,
		m_parentIDEdit,
		m_meshPathEdit, m_texPathEdit, m_collisionMeshPathEdit,
		m_healthAmountEdit,
		m_lightColourREdit, m_lightColourGEdit, m_lightColourBEdit,
		m_specularColourREdit, m_specularColourGEdit, m_specularColourBEdit,
		m_spotlightAngleEdit,
		m_attConstantEdit, m_attLinearEdit, m_attQuadraticEdit,
		m_pivotXEdit, m_pivotYEdit, m_pivotZEdit,
		m_audioFilePathEdit,
		m_audioVolEdit, m_audioPitchEdit, m_audioPanEdit,
		m_minDistEdit, m_maxDistEdit;

	CButton m_enableCollisionCheck,
		m_collectableCheck, m_destructableCheck,
		m_renderCheck, m_cameraCheck,
		m_editorRenderCheck, m_editorWireframeCheck,
		m_editorTextureVisCheck, m_editorNormalsVisCheck,
		m_editorCollisionVisCheck, m_editorPivotVisCheck,
		m_snapToGroundCheck,
		m_aiNodeCheck, m_pathNodeCheck,
		m_pathNodeStartCheck, m_pathNodeEndCheck,
		m_oneShotCheck, m_playOnInitCheck, m_playInEditorCheck;

	CComboBox m_lightTypeCombo;

	afx_msg void OnClickedBnSelectMesh();
	afx_msg void OnClickedBnSelectTexture();
	afx_msg void OnClickedBnSelectCollisionMesh();
	afx_msg void OnClickedBnSelectAudioFile();

};

template<typename T>
inline CString InspectorScrollableDialogue::NumberValueToCString(CString format, T val)
{
	CString retVal;
	retVal.Format(format, val);
	return retVal;
}
