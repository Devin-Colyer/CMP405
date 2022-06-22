// InspectorScrollableDialogue.cpp : implementation file
//

#include "InspectorScrollableDialogue.h"
#include "SceneObject.h"
#include "ToolMain.h"

#include "InspectorDialogue.h"
#include "SelectFileDialogue.h"

// InspectorScrollableDialogue dialog

IMPLEMENT_DYNAMIC(InspectorScrollableDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(InspectorScrollableDialogue, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_BnSelectMesh, &InspectorScrollableDialogue::OnClickedBnSelectMesh)
	ON_BN_CLICKED(IDC_BnSelectTexture, &InspectorScrollableDialogue::OnClickedBnSelectTexture)
	ON_BN_CLICKED(IDC_BnSelectCollisionMesh, &InspectorScrollableDialogue::OnClickedBnSelectCollisionMesh)
	ON_BN_CLICKED(IDC_BnSelectAudioFile, &InspectorScrollableDialogue::OnClickedBnSelectAudioFile)
END_MESSAGE_MAP()

InspectorScrollableDialogue::InspectorScrollableDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_InspectorScrollable, pParent)
{
	m_app = nullptr;
	m_selectFileDialogue = nullptr;
	m_parent = pParent;
	m_scrollPos = 0;
	m_currentHeight = 0;
	m_bDragging = FALSE;
	Create(IDD_InspectorScrollable, pParent);
}

InspectorScrollableDialogue::~InspectorScrollableDialogue()
{
	delete m_selectFileDialogue;
}

void InspectorScrollableDialogue::SetObjectData(ToolMain* appPtr)
{
	m_app = appPtr;

	if (m_app) {
		int i = m_app->GetIndexFromID(m_app->m_selectedObject);
		if (i != -1) {
			SceneObject selectedObj = m_app->m_sceneGraph[i];

			m_chunkIDEdit.SetWindowText(NumberValueToCString(L"%d", selectedObj.chunk_ID));
			m_meshPathEdit.SetWindowText(CString(selectedObj.model_path.c_str()));
			m_texPathEdit.SetWindowText(CString(selectedObj.tex_diffuse_path.c_str()));
			m_posxEdit.SetWindowText(NumberValueToCString("%f", selectedObj.posX));
			m_posyEdit.SetWindowText(NumberValueToCString("%f", selectedObj.posY));
			m_poszEdit.SetWindowText(NumberValueToCString("%f", selectedObj.posZ));
			m_rotxEdit.SetWindowText(NumberValueToCString("%f", selectedObj.rotX));
			m_rotyEdit.SetWindowText(NumberValueToCString("%f", selectedObj.rotY));
			m_rotzEdit.SetWindowText(NumberValueToCString("%f", selectedObj.rotZ));
			m_scaxEdit.SetWindowText(NumberValueToCString("%f", selectedObj.scaX));
			m_scayEdit.SetWindowText(NumberValueToCString("%f", selectedObj.scaY));
			m_scazEdit.SetWindowText(NumberValueToCString("%f", selectedObj.scaZ));
			m_renderCheck.SetCheck(selectedObj.render);
			m_enableCollisionCheck.SetCheck(selectedObj.collision);
			m_collisionMeshPathEdit.SetWindowText(CString(selectedObj.collision_mesh.c_str()));
			m_collectableCheck.SetCheck(selectedObj.collectable);
			m_destructableCheck.SetCheck(selectedObj.destructable);
			m_healthAmountEdit.SetWindowText(NumberValueToCString("%d", selectedObj.health_amount));
			m_editorRenderCheck.SetCheck(selectedObj.editor_render);
			m_editorTextureVisCheck.SetCheck(selectedObj.editor_texture_vis);
			m_editorNormalsVisCheck.SetCheck(selectedObj.editor_normals_vis);
			m_editorCollisionVisCheck.SetCheck(selectedObj.editor_collision_vis);
			m_editorPivotVisCheck.SetCheck(selectedObj.editor_pivot_vis);
			m_pivotXEdit.SetWindowText(NumberValueToCString("%f", selectedObj.pivotX));
			m_pivotYEdit.SetWindowText(NumberValueToCString("%f", selectedObj.pivotY));
			m_pivotZEdit.SetWindowText(NumberValueToCString("%f", selectedObj.pivotZ));
			m_snapToGroundCheck.SetCheck(selectedObj.snapToGround);
			m_aiNodeCheck.SetCheck(selectedObj.AINode);
			m_audioFilePathEdit.SetWindowText(CString(selectedObj.audio_path.c_str()));
			m_audioVolEdit.SetWindowText(NumberValueToCString("%f", selectedObj.volume));
			m_audioPitchEdit.SetWindowText(NumberValueToCString("%f", selectedObj.pitch));
			m_audioPanEdit.SetWindowText(NumberValueToCString("%f", selectedObj.pan));
			m_oneShotCheck.SetCheck(selectedObj.one_shot);
			m_playOnInitCheck.SetCheck(selectedObj.play_on_init);
			m_playInEditorCheck.SetCheck(selectedObj.play_in_editor);
			m_minDistEdit.SetWindowText(NumberValueToCString("%d", selectedObj.min_dist));
			m_maxDistEdit.SetWindowText(NumberValueToCString("%d", selectedObj.max_dist));
			m_cameraCheck.SetCheck(selectedObj.camera);
			m_pathNodeCheck.SetCheck(selectedObj.path_node);
			m_pathNodeStartCheck.SetCheck(selectedObj.path_node_start);
			m_pathNodeEndCheck.SetCheck(selectedObj.path_node_end);
			m_parentIDEdit.SetWindowText(NumberValueToCString("%d", selectedObj.parent_id));
			m_editorWireframeCheck.SetCheck(selectedObj.editor_wireframe);
			m_nameEdit.SetWindowText(CString(selectedObj.name.c_str()));

			m_lightTypeCombo.SetCurSel(selectedObj.light_type);
			m_lightColourREdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_diffuse_r));
			m_lightColourGEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_diffuse_g));
			m_lightColourBEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_diffuse_b));
			m_specularColourREdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_specular_r));
			m_specularColourGEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_specular_g));
			m_specularColourBEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_specular_b));
			m_spotlightAngleEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_spot_cutoff));
			m_attConstantEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_constant));
			m_attLinearEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_linear));
			m_attQuadraticEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_quadratic));
		}
	}
}

void InspectorScrollableDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Name, m_nameEdit);
	DDX_Control(pDX, IDC_ChunkID, m_chunkIDEdit);
	DDX_Control(pDX, IDC_PositionX, m_posxEdit);
	DDX_Control(pDX, IDC_PositionY, m_posyEdit);
	DDX_Control(pDX, IDC_PositionZ, m_poszEdit);
	DDX_Control(pDX, IDC_Pitch, m_rotxEdit);
	DDX_Control(pDX, IDC_Yaw, m_rotyEdit);
	DDX_Control(pDX, IDC_Roll, m_rotzEdit);
	DDX_Control(pDX, IDC_ScaleX, m_scaxEdit);
	DDX_Control(pDX, IDC_ScaleY, m_scayEdit);
	DDX_Control(pDX, IDC_ScaleZ, m_scazEdit);
	DDX_Control(pDX, IDC_ParentID, m_parentIDEdit);
	DDX_Control(pDX, IDC_MeshPath, m_meshPathEdit);
	DDX_Control(pDX, IDC_TexturePath, m_texPathEdit);
	DDX_Control(pDX, IDC_CollisionCheck, m_enableCollisionCheck);
	DDX_Control(pDX, IDC_CollisionMeshPath, m_collisionMeshPathEdit);
	DDX_Control(pDX, IDC_Collectable, m_collectableCheck);
	DDX_Control(pDX, IDC_Destructable, m_destructableCheck);
	DDX_Control(pDX, IDC_HealthAmount, m_healthAmountEdit);
	DDX_Control(pDX, IDC_Render, m_renderCheck);
	DDX_Control(pDX, IDC_Camera, m_cameraCheck);
	DDX_Control(pDX, IDC_EditorRender, m_editorRenderCheck);
	DDX_Control(pDX, IDC_EditorWireframe, m_editorWireframeCheck);
	DDX_Control(pDX, IDC_EditorTextureVis, m_editorTextureVisCheck);
	DDX_Control(pDX, IDC_EditorNormalsVis, m_editorNormalsVisCheck);
	DDX_Control(pDX, IDC_EditorCollisionsVis, m_editorCollisionVisCheck);
	DDX_Control(pDX, IDC_EditorPivotVis, m_editorPivotVisCheck);
	DDX_Control(pDX, IDC_LightType, m_lightTypeCombo);
	DDX_Control(pDX, IDC_DiffuseR, m_lightColourREdit);
	DDX_Control(pDX, IDC_DiffuseG, m_lightColourGEdit);
	DDX_Control(pDX, IDC_DiffuseB, m_lightColourBEdit);
	DDX_Control(pDX, IDC_SpecularR, m_specularColourREdit);
	DDX_Control(pDX, IDC_SpecularG, m_specularColourGEdit);
	DDX_Control(pDX, IDC_SpecularB, m_specularColourBEdit);
	DDX_Control(pDX, IDC_SpotAngle, m_spotlightAngleEdit);
	DDX_Control(pDX, IDC_AttConstant, m_attConstantEdit);
	DDX_Control(pDX, IDC_AttLinear, m_attLinearEdit);
	DDX_Control(pDX, IDC_AttQuadratic, m_attQuadraticEdit);
	DDX_Control(pDX, IDC_PivotPosX, m_pivotXEdit);
	DDX_Control(pDX, IDC_PivotPosY, m_pivotYEdit);
	DDX_Control(pDX, IDC_PivotPosZ, m_pivotZEdit);
	DDX_Control(pDX, IDC_SnapToGround, m_snapToGroundCheck);
	DDX_Control(pDX, IDC_AINode, m_aiNodeCheck);
	DDX_Control(pDX, IDC_PathNode, m_pathNodeCheck);
	DDX_Control(pDX, IDC_PathNodeStart, m_pathNodeStartCheck);
	DDX_Control(pDX, IDC_PathNodeEnd, m_pathNodeEndCheck);
	DDX_Control(pDX, IDC_AudioFilePath, m_audioFilePathEdit);
	DDX_Control(pDX, IDC_AudioVol, m_audioVolEdit);
	DDX_Control(pDX, IDC_AudioPitch, m_audioPitchEdit);
	DDX_Control(pDX, IDC_AudioPan, m_audioPanEdit);
	DDX_Control(pDX, IDC_OneShot, m_oneShotCheck);
	DDX_Control(pDX, IDC_PlayOnInit, m_playOnInitCheck);
	DDX_Control(pDX, IDC_PlayInEditor, m_playInEditorCheck);
	DDX_Control(pDX, IDC_MinDist, m_minDistEdit);
	DDX_Control(pDX, IDC_MaxDist, m_maxDistEdit);
}

void InspectorScrollableDialogue::MakeNewSelectorWindow()
{
	if (IsWindow(m_selectFileDialogue->GetSafeHwnd()))
		m_selectFileDialogue->DestroyWindow();

	m_selectFileDialogue->Create(IDD_SelectFile);	//Start up modeless
	m_selectFileDialogue->ShowWindow(SW_SHOW);	//show modeless
}


BOOL InspectorScrollableDialogue::OnInitDialog()
{
	CDialog::OnInitDialog();

	// save the original size
	GetWindowRect(m_rcOriginalRect);

	m_selectFileDialogue = new SelectFileDialogue();

	m_lightTypeCombo.AddString(L"0");
	m_lightTypeCombo.AddString(L"1");
	m_lightTypeCombo.AddString(L"2");

	return TRUE;
}

void InspectorScrollableDialogue::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int delta = 0;	// change in scoll pos
	int maxPos = m_rcOriginalRect.Height() - m_currentHeight;

	// scrolling message handler
	switch (nSBCode)
	{
	case SB_LINEUP:
		if (m_scrollPos <= 0)
			return;

		delta = -min(max(maxPos / 20, 5), m_scrollPos);
		break;

	case SB_LINEDOWN:
		if (m_scrollPos >= maxPos)
			return;

		delta = min(max(maxPos / 20, 5), maxPos - m_scrollPos);
		break;

	case SB_PAGEUP:
		if (m_scrollPos <= 0)
			return;

		delta = -min(max(maxPos / 10, 5), m_scrollPos);
		break;

	case SB_PAGEDOWN:
		if (m_scrollPos >= maxPos)
			return;

		delta = min(max(maxPos / 10, 5), maxPos - m_scrollPos);
		break;

	case SB_THUMBPOSITION:
		delta = (int)nPos - m_scrollPos;
		break;

	default:
		return;
	}

	m_scrollPos += delta;
	SetScrollPos(SB_VERT, m_scrollPos, TRUE);
	ScrollWindow(0, -delta);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void InspectorScrollableDialogue::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	m_currentHeight = cy;

	SCROLLINFO si{};
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = m_rcOriginalRect.Height();
	si.nPage = cy;
	si.nPos = 0;
	SetScrollInfo(SB_VERT, &si, TRUE);
}

BOOL InspectorScrollableDialogue::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nMaxPos = m_rcOriginalRect.Height() - m_currentHeight;

	if (zDelta < 0)
	{
		if (m_scrollPos < nMaxPos)
		{
			zDelta = min(max(nMaxPos / 20, 5), nMaxPos - m_scrollPos);

			m_scrollPos += zDelta;
			SetScrollPos(SB_VERT, m_scrollPos, TRUE);
			ScrollWindow(0, -zDelta);
		}
	}
	else
	{
		if (m_scrollPos > 0)
		{
			zDelta = -min(max(nMaxPos / 20, 5), m_scrollPos);

			m_scrollPos += zDelta;
			SetScrollPos(SB_VERT, m_scrollPos, TRUE);
			ScrollWindow(0, -zDelta);
		}
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void InspectorScrollableDialogue::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_bDragging = TRUE;
	SetCapture();

	m_dragPoint = point;

	CDialog::OnRButtonDown(nFlags, point);
}

void InspectorScrollableDialogue::OnRButtonUp(UINT nFlags, CPoint point)
{
	EndDrag();

	CDialog::OnRButtonUp(nFlags, point);
}

void InspectorScrollableDialogue::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		int nDelta = m_dragPoint.y - point.y;
		m_dragPoint = point;

		int nNewPos = m_scrollPos + nDelta;

		if (nNewPos < 0)
			nNewPos = 0;
		else if (nNewPos > m_rcOriginalRect.Height() - m_currentHeight)
			nNewPos = m_rcOriginalRect.Height() - m_currentHeight;

		nDelta = nNewPos - m_scrollPos;
		m_scrollPos = nNewPos;

		SetScrollPos(SB_VERT, m_scrollPos, TRUE);
		ScrollWindow(0, -nDelta);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void InspectorScrollableDialogue::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);

	EndDrag();
}

void InspectorScrollableDialogue::EndDrag()
{
	m_bDragging = FALSE;
	ReleaseCapture();
}

void InspectorScrollableDialogue::ResetInspector()
{

	if (m_app) {
		int i = m_app->GetIndexFromID(m_app->m_selectedObject);
		if (i != -1) {
			SceneObject selectedObj = m_app->m_sceneGraph[i];

			m_chunkIDEdit.SetWindowText(NumberValueToCString(L"%d", selectedObj.chunk_ID));
			m_meshPathEdit.SetWindowText(CString(selectedObj.model_path.c_str()));
			m_texPathEdit.SetWindowText(CString(selectedObj.tex_diffuse_path.c_str()));
			m_posxEdit.SetWindowText(NumberValueToCString("%f", selectedObj.posX));
			m_posyEdit.SetWindowText(NumberValueToCString("%f", selectedObj.posY));
			m_poszEdit.SetWindowText(NumberValueToCString("%f", selectedObj.posZ));
			m_rotxEdit.SetWindowText(NumberValueToCString("%f", selectedObj.rotX));
			m_rotyEdit.SetWindowText(NumberValueToCString("%f", selectedObj.rotY));
			m_rotzEdit.SetWindowText(NumberValueToCString("%f", selectedObj.rotZ));
			m_scaxEdit.SetWindowText(NumberValueToCString("%f", selectedObj.scaX));
			m_scayEdit.SetWindowText(NumberValueToCString("%f", selectedObj.scaY));
			m_scazEdit.SetWindowText(NumberValueToCString("%f", selectedObj.scaZ));
			m_renderCheck.SetCheck(selectedObj.render);
			m_enableCollisionCheck.SetCheck(selectedObj.collision);
			m_collisionMeshPathEdit.SetWindowText(CString(selectedObj.collision_mesh.c_str()));
			m_collectableCheck.SetCheck(selectedObj.collectable);
			m_destructableCheck.SetCheck(selectedObj.destructable);
			m_healthAmountEdit.SetWindowText(NumberValueToCString("%d", selectedObj.health_amount));
			m_editorRenderCheck.SetCheck(selectedObj.editor_render);
			m_editorTextureVisCheck.SetCheck(selectedObj.editor_texture_vis);
			m_editorNormalsVisCheck.SetCheck(selectedObj.editor_normals_vis);
			m_editorCollisionVisCheck.SetCheck(selectedObj.editor_collision_vis);
			m_editorPivotVisCheck.SetCheck(selectedObj.editor_pivot_vis);
			m_pivotXEdit.SetWindowText(NumberValueToCString("%f", selectedObj.pivotX));
			m_pivotYEdit.SetWindowText(NumberValueToCString("%f", selectedObj.pivotY));
			m_pivotZEdit.SetWindowText(NumberValueToCString("%f", selectedObj.pivotZ));
			m_snapToGroundCheck.SetCheck(selectedObj.snapToGround);
			m_aiNodeCheck.SetCheck(selectedObj.AINode);
			m_audioFilePathEdit.SetWindowText(CString(selectedObj.audio_path.c_str()));
			m_audioVolEdit.SetWindowText(NumberValueToCString("%f", selectedObj.volume));
			m_audioPitchEdit.SetWindowText(NumberValueToCString("%f", selectedObj.pitch));
			m_audioPanEdit.SetWindowText(NumberValueToCString("%f", selectedObj.pan));
			m_oneShotCheck.SetCheck(selectedObj.one_shot);
			m_playOnInitCheck.SetCheck(selectedObj.play_on_init);
			m_playInEditorCheck.SetCheck(selectedObj.play_in_editor);
			m_minDistEdit.SetWindowText(NumberValueToCString("%d", selectedObj.min_dist));
			m_maxDistEdit.SetWindowText(NumberValueToCString("%d", selectedObj.max_dist));
			m_cameraCheck.SetCheck(selectedObj.camera);
			m_pathNodeCheck.SetCheck(selectedObj.path_node);
			m_pathNodeStartCheck.SetCheck(selectedObj.path_node_start);
			m_pathNodeEndCheck.SetCheck(selectedObj.path_node_end);
			m_parentIDEdit.SetWindowText(NumberValueToCString("%d", selectedObj.parent_id));
			m_editorWireframeCheck.SetCheck(selectedObj.editor_wireframe);
			m_nameEdit.SetWindowText(CString(selectedObj.name.c_str()));

			m_lightTypeCombo.SetCurSel(selectedObj.light_type);
			m_lightColourREdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_diffuse_r));
			m_lightColourGEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_diffuse_g));
			m_lightColourBEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_diffuse_b));
			m_specularColourREdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_specular_r));
			m_specularColourGEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_specular_g));
			m_specularColourBEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_specular_b));
			m_spotlightAngleEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_spot_cutoff));
			m_attConstantEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_constant));
			m_attLinearEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_linear));
			m_attQuadraticEdit.SetWindowText(NumberValueToCString("%f", selectedObj.light_quadratic));
		}
		else {
			((InspectorDialogue*)m_parent)->DestroyWindow();
		}
	}
}

void InspectorScrollableDialogue::Confirm()
{
	CString name, chunkID,
		posx, posy, posz,
		rotx, roty, rotz,
		scax, scay, scaz,
		parentID,
		meshPath, m_texPath, collisionMeshPath,
		healthAmount,
		lightColourR, lightColourG, lightColourB,
		specularColourR, specularColourG, specularColourB,
		spotlightAngle,
		attConstant, attLinear, attQuadratic,
		pivotX, pivotY, pivotZ,
		audioFilePath,
		audioVol, audioPitch, audioPan,
		minDist, maxDist,
		lightType;

	m_nameEdit.GetWindowText(name);
	m_chunkIDEdit.GetWindowText(chunkID);

	m_posxEdit.GetWindowText(posx);
	m_posyEdit.GetWindowText(posy);
	m_poszEdit.GetWindowText(posz);
	m_rotxEdit.GetWindowText(rotx);
	m_rotyEdit.GetWindowText(roty);
	m_rotzEdit.GetWindowText(rotz);
	m_scaxEdit.GetWindowText(scax);
	m_scayEdit.GetWindowText(scay);
	m_scazEdit.GetWindowText(scaz);

	m_parentIDEdit.GetWindowText(parentID);

	m_meshPathEdit.GetWindowText(meshPath);
	m_texPathEdit.GetWindowText(m_texPath);

	m_collisionMeshPathEdit.GetWindowText(collisionMeshPath);

	m_healthAmountEdit.GetWindowText(healthAmount);

	m_lightTypeCombo.GetLBText(m_lightTypeCombo.GetCurSel(), lightType);
	m_lightColourREdit.GetWindowText(lightColourR);
	m_lightColourGEdit.GetWindowText(lightColourG);
	m_lightColourBEdit.GetWindowText(lightColourB);
	m_specularColourREdit.GetWindowText(specularColourR);
	m_specularColourGEdit.GetWindowText(specularColourG);
	m_specularColourBEdit.GetWindowText(specularColourB);
	m_spotlightAngleEdit.GetWindowText(spotlightAngle);
	m_attConstantEdit.GetWindowText(attConstant);
	m_attLinearEdit.GetWindowText(attLinear);
	m_attQuadraticEdit.GetWindowText(attQuadratic);

	m_pivotXEdit.GetWindowText(pivotX);
	m_pivotYEdit.GetWindowText(pivotY);
	m_pivotZEdit.GetWindowText(pivotZ);
	
	m_audioFilePathEdit.GetWindowText(audioFilePath);
	m_audioVolEdit.GetWindowText(audioVol);
	m_audioPitchEdit.GetWindowText(audioPitch);
	m_audioPanEdit.GetWindowText(audioPan);

	m_minDistEdit.GetWindowText(minDist);
	m_maxDistEdit.GetWindowText(maxDist);

	if (m_app) {
		SceneObject editData{};

		editData.ID = 0;
		editData.chunk_ID = _ttoi(chunkID);

		if (meshPath.Find(L".cmo") == meshPath.GetLength() - 4 || meshPath == "")
			editData.model_path = std::string(CT2CA(meshPath));

		if (m_texPath.Find(L".dds") == m_texPath.GetLength() - 4 || m_texPath == "")
			editData.tex_diffuse_path = std::string(CT2CA(m_texPath));

		editData.posX = _ttof(posx);
		editData.posY = _ttof(posy);
		editData.posZ = _ttof(posz);
		editData.rotX = _ttof(rotx);
		editData.rotY = _ttof(roty);
		editData.rotZ = _ttof(rotz);
		editData.scaX = _ttof(scax);
		editData.scaY = _ttof(scay);
		editData.scaZ = _ttof(scaz);
		editData.render = m_renderCheck.GetCheck();
		editData.collision = m_enableCollisionCheck.GetCheck();

		if (collisionMeshPath.Find(L".cmo") == collisionMeshPath.GetLength() - 4 || collisionMeshPath == "")
			editData.collision_mesh = std::string(CT2CA(collisionMeshPath));

		editData.collectable = m_collectableCheck.GetCheck();
		editData.destructable = m_destructableCheck.GetCheck();
		editData.health_amount = _ttoi(healthAmount);
		editData.editor_render = m_editorRenderCheck.GetCheck();
		editData.editor_texture_vis = m_editorTextureVisCheck.GetCheck();
		editData.editor_normals_vis = m_editorNormalsVisCheck.GetCheck();
		editData.editor_collision_vis = m_editorCollisionVisCheck.GetCheck();
		editData.editor_pivot_vis = m_editorPivotVisCheck.GetCheck();
		editData.pivotX = _ttof(pivotX);
		editData.pivotY = _ttof(pivotY);
		editData.pivotZ = _ttof(pivotZ);
		editData.snapToGround = m_snapToGroundCheck.GetCheck();
		editData.AINode = m_aiNodeCheck.GetCheck();

		if (audioFilePath.Find(L".wav") == audioFilePath.GetLength() - 4 || audioFilePath == "")
			editData.audio_path = std::string(CT2CA(audioFilePath));

		editData.volume = _ttof(audioVol);
		editData.pitch = _ttof(audioPitch);
		editData.pan = _ttof(audioPan);
		editData.one_shot = m_oneShotCheck.GetCheck();
		editData.play_on_init = m_playOnInitCheck.GetCheck();
		editData.play_in_editor = m_playInEditorCheck.GetCheck();
		editData.min_dist = _ttoi(minDist);
		editData.max_dist = _ttoi(maxDist);
		editData.camera = m_cameraCheck.GetCheck();
		editData.path_node = m_pathNodeCheck.GetCheck();
		editData.path_node_start = m_pathNodeStartCheck.GetCheck();
		editData.path_node_end = m_pathNodeEndCheck.GetCheck();
		editData.parent_id = _ttoi(parentID);
		editData.editor_wireframe = m_editorWireframeCheck.GetCheck();
		editData.name = std::string(CT2CA(name));

		editData.light_type = _ttoi(lightType);
		editData.light_diffuse_r = _ttof(lightColourR);
		editData.light_diffuse_g = _ttof(lightColourG);
		editData.light_diffuse_b = _ttof(lightColourB);
		editData.light_specular_r = _ttof(specularColourR);
		editData.light_specular_g = _ttof(specularColourG);
		editData.light_specular_b = _ttof(specularColourB);
		editData.light_spot_cutoff = _ttof(spotlightAngle);
		editData.light_constant = _ttof(attConstant);
		editData.light_linear = _ttof(attLinear);
		editData.light_quadratic = _ttof(attQuadratic);

		m_app->UpdateObjectByID(editData, m_app->m_selectedObject);
	}
}

void InspectorScrollableDialogue::OnClickedBnSelectMesh()
{
	if (m_selectFileDialogue) {
		MakeNewSelectorWindow();
		m_selectFileDialogue->SetObjectData(&m_meshPathEdit, L".cmo");
	}
}


void InspectorScrollableDialogue::OnClickedBnSelectTexture()
{
	if (m_selectFileDialogue) {
		MakeNewSelectorWindow();
		m_selectFileDialogue->SetObjectData(&m_texPathEdit, L".dds");
	}
}


void InspectorScrollableDialogue::OnClickedBnSelectCollisionMesh()
{
	if (m_selectFileDialogue) {
		MakeNewSelectorWindow();
		m_selectFileDialogue->SetObjectData(&m_collisionMeshPathEdit, L".cmo");
	}
}


void InspectorScrollableDialogue::OnClickedBnSelectAudioFile()
{
	if (m_selectFileDialogue) {
		MakeNewSelectorWindow();
		m_selectFileDialogue->SetObjectData(&m_audioFilePathEdit, L".wav");
	}
}
