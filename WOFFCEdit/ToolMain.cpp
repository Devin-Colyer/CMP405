#include "ToolMain.h"
#include "MFCFrame.h"
#include "resource.h"
#include <vector>
#include <sstream>

//
//ToolMain Class
ToolMain::ToolMain()
{

	m_currentChunk = 0;		//default value
	m_selectedObject = -1;	//initial selection ID
	m_sceneGraph.clear();	//clear the vector for the scenegraph
	m_databaseConnection = NULL;

	//zero input commands
	m_toolInputCommands.forward =
		m_toolInputCommands.back =
		m_toolInputCommands.left =
		m_toolInputCommands.right =
		m_toolInputCommands.flyUp =
		m_toolInputCommands.flyDown = false;

	m_toolInputCommands.yawRight =
		m_toolInputCommands.yawLeft =
		m_toolInputCommands.pitchUp =
		m_toolInputCommands.pitchDown = false;

	m_toolInputCommands.mouseX = m_toolInputCommands.mouseY = 0;
	m_toolInputCommands.inWindowMouseX = m_toolInputCommands.inWindowMouseY = 0;
	m_toolInputCommands.originalMouseX = m_toolInputCommands.originalMouseY = 0;
	m_toolInputCommands.lmbDown = m_toolInputCommands.rmbDown = false;
	
}


ToolMain::~ToolMain()
{
	sqlite3_close(m_databaseConnection);		//close the database connection
}


int ToolMain::getCurrentSelectionID()
{

	return m_selectedObject;
}

void ToolMain::onActionInitialise(HWND handle, int width, int height)
{
	//window size, handle etc for directX
	m_width		= width;
	m_height	= height;
	m_toolHandle = handle;

	CMyFrame* myFrame = (CMyFrame*)CWnd::FromHandle(m_toolHandle);
	CRect dxViewRect;
	myFrame->GetWindowRect(dxViewRect);

	m_d3dRenderer.Initialize(handle, m_width, m_height, dxViewRect);

	//database connection establish
	int rc;
	rc = sqlite3_open_v2("database/test.db",&m_databaseConnection, SQLITE_OPEN_READWRITE, NULL);

	if (rc) 
	{
		TRACE("Can't open database");
		//if the database cant open. Perhaps a more catastrophic error would be better here
	}
	else 
	{
		TRACE("Opened database successfully");
	}

	onActionLoad();
}

void ToolMain::onActionLoad()
{
	//load current chunk and objects into lists
	if (!m_sceneGraph.empty())		//is the vector empty
	{
		m_sceneGraph.clear();		//if not, empty it
	}

	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	sqlite3_stmt *pResultsChunk;

	//OBJECTS IN THE WORLD
	//prepare SQL Text
	sqlCommand = "SELECT * from Objects";				//sql command which will return all records from the objects table.
	//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0 );
	
	//loop for each row in results until there are no more rows.  ie for every row in the results. We create and object
	while (sqlite3_step(pResults) == SQLITE_ROW)
	{	
		SceneObject newSceneObject;
		newSceneObject.ID = sqlite3_column_int(pResults, 0);
		newSceneObject.chunk_ID = sqlite3_column_int(pResults, 1);
		newSceneObject.model_path		= reinterpret_cast<const char*>(sqlite3_column_text(pResults, 2));
		newSceneObject.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 3));
		newSceneObject.posX = sqlite3_column_double(pResults, 4);
		newSceneObject.posY = sqlite3_column_double(pResults, 5);
		newSceneObject.posZ = sqlite3_column_double(pResults, 6);
		newSceneObject.rotX = sqlite3_column_double(pResults, 7);
		newSceneObject.rotY = sqlite3_column_double(pResults, 8);
		newSceneObject.rotZ = sqlite3_column_double(pResults, 9);
		newSceneObject.scaX = sqlite3_column_double(pResults, 10);
		newSceneObject.scaY = sqlite3_column_double(pResults, 11);
		newSceneObject.scaZ = sqlite3_column_double(pResults, 12);
		newSceneObject.render = sqlite3_column_int(pResults, 13);
		newSceneObject.collision = sqlite3_column_int(pResults, 14);
		newSceneObject.collision_mesh = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 15));
		newSceneObject.collectable = sqlite3_column_int(pResults, 16);
		newSceneObject.destructable = sqlite3_column_int(pResults, 17);
		newSceneObject.health_amount = sqlite3_column_int(pResults, 18);
		newSceneObject.editor_render = sqlite3_column_int(pResults, 19);
		newSceneObject.editor_texture_vis = sqlite3_column_int(pResults, 20);
		newSceneObject.editor_normals_vis = sqlite3_column_int(pResults, 21);
		newSceneObject.editor_collision_vis = sqlite3_column_int(pResults, 22);
		newSceneObject.editor_pivot_vis = sqlite3_column_int(pResults, 23);
		newSceneObject.pivotX = sqlite3_column_double(pResults, 24);
		newSceneObject.pivotY = sqlite3_column_double(pResults, 25);
		newSceneObject.pivotZ = sqlite3_column_double(pResults, 26);
		newSceneObject.snapToGround = sqlite3_column_int(pResults, 27);
		newSceneObject.AINode = sqlite3_column_int(pResults, 28);
		newSceneObject.audio_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 29));
		newSceneObject.volume = sqlite3_column_double(pResults, 30);
		newSceneObject.pitch = sqlite3_column_double(pResults, 31);
		newSceneObject.pan = sqlite3_column_int(pResults, 32);
		newSceneObject.one_shot = sqlite3_column_int(pResults, 33);
		newSceneObject.play_on_init = sqlite3_column_int(pResults, 34);
		newSceneObject.play_in_editor = sqlite3_column_int(pResults, 35);
		newSceneObject.min_dist = sqlite3_column_double(pResults, 36);
		newSceneObject.max_dist = sqlite3_column_double(pResults, 37);
		newSceneObject.camera = sqlite3_column_int(pResults, 38);
		newSceneObject.path_node = sqlite3_column_int(pResults, 39);
		newSceneObject.path_node_start = sqlite3_column_int(pResults, 40);
		newSceneObject.path_node_end = sqlite3_column_int(pResults, 41);
		newSceneObject.parent_id = sqlite3_column_int(pResults, 42);
		newSceneObject.editor_wireframe = sqlite3_column_int(pResults, 43);
		newSceneObject.name = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 44));

		newSceneObject.light_type = sqlite3_column_int(pResults, 45);
		newSceneObject.light_diffuse_r = sqlite3_column_double(pResults, 46);
		newSceneObject.light_diffuse_g = sqlite3_column_double(pResults, 47);
		newSceneObject.light_diffuse_b = sqlite3_column_double(pResults, 48);
		newSceneObject.light_specular_r = sqlite3_column_double(pResults, 49);
		newSceneObject.light_specular_g = sqlite3_column_double(pResults, 50);
		newSceneObject.light_specular_b = sqlite3_column_double(pResults, 51);
		newSceneObject.light_spot_cutoff = sqlite3_column_double(pResults, 52);
		newSceneObject.light_constant = sqlite3_column_double(pResults, 53);
		newSceneObject.light_linear = sqlite3_column_double(pResults, 54);
		newSceneObject.light_quadratic = sqlite3_column_double(pResults, 55);
	

		//send completed object to scenegraph
		m_sceneGraph.push_back(newSceneObject);
	}

	//THE WORLD CHUNK
	//prepare SQL Text
	sqlCommand = "SELECT * from Chunks";				//sql command which will return all records from  chunks table. There is only one tho.
														//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResultsChunk, 0);


	sqlite3_step(pResultsChunk);
	m_chunk.ID = sqlite3_column_int(pResultsChunk, 0);
	m_chunk.name = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 1));
	m_chunk.chunk_x_size_metres = sqlite3_column_int(pResultsChunk, 2);
	m_chunk.chunk_y_size_metres = sqlite3_column_int(pResultsChunk, 3);
	m_chunk.chunk_base_resolution = sqlite3_column_int(pResultsChunk, 4);
	m_chunk.heightmap_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 5));
	m_chunk.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 6));
	m_chunk.tex_splat_alpha_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 7));
	m_chunk.tex_splat_1_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 8));
	m_chunk.tex_splat_2_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 9));
	m_chunk.tex_splat_3_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 10));
	m_chunk.tex_splat_4_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 11));
	m_chunk.render_wireframe = sqlite3_column_int(pResultsChunk, 12);
	m_chunk.render_normals = sqlite3_column_int(pResultsChunk, 13);
	m_chunk.tex_diffuse_tiling = sqlite3_column_int(pResultsChunk, 14);
	m_chunk.tex_splat_1_tiling = sqlite3_column_int(pResultsChunk, 15);
	m_chunk.tex_splat_2_tiling = sqlite3_column_int(pResultsChunk, 16);
	m_chunk.tex_splat_3_tiling = sqlite3_column_int(pResultsChunk, 17);
	m_chunk.tex_splat_4_tiling = sqlite3_column_int(pResultsChunk, 18);


	//Process REsults into renderable
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	//build the renderable chunk 
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);

}

void ToolMain::onActionSave()
{
	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	

	//OBJECTS IN THE WORLD Delete them all
	//prepare SQL Text
	sqlCommand = "DELETE FROM Objects";	 //will delete the whole object table.   Slightly risky but hey.
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0);
	sqlite3_step(pResults);

	//Populate with our new objects
	std::wstring sqlCommand2;
	int numObjects = m_sceneGraph.size();	//Loop thru the scengraph.

	for (int i = 0; i < numObjects; i++)
	{
		std::stringstream command;
		command << "INSERT INTO Objects " 
			<<"VALUES(" << m_sceneGraph.at(i).ID << ","
			<< m_sceneGraph.at(i).chunk_ID  << ","
			<< "'" << m_sceneGraph.at(i).model_path <<"'" << ","
			<< "'" << m_sceneGraph.at(i).tex_diffuse_path << "'" << ","
			<< m_sceneGraph.at(i).posX << ","
			<< m_sceneGraph.at(i).posY << ","
			<< m_sceneGraph.at(i).posZ << ","
			<< m_sceneGraph.at(i).rotX << ","
			<< m_sceneGraph.at(i).rotY << ","
			<< m_sceneGraph.at(i).rotZ << ","
			<< m_sceneGraph.at(i).scaX << ","
			<< m_sceneGraph.at(i).scaY << ","
			<< m_sceneGraph.at(i).scaZ << ","
			<< m_sceneGraph.at(i).render << ","
			<< m_sceneGraph.at(i).collision << ","
			<< "'" << m_sceneGraph.at(i).collision_mesh << "'" << ","
			<< m_sceneGraph.at(i).collectable << ","
			<< m_sceneGraph.at(i).destructable << ","
			<< m_sceneGraph.at(i).health_amount << ","
			<< m_sceneGraph.at(i).editor_render << ","
			<< m_sceneGraph.at(i).editor_texture_vis << ","
			<< m_sceneGraph.at(i).editor_normals_vis << ","
			<< m_sceneGraph.at(i).editor_collision_vis << ","
			<< m_sceneGraph.at(i).editor_pivot_vis << ","
			<< m_sceneGraph.at(i).pivotX << ","
			<< m_sceneGraph.at(i).pivotY << ","
			<< m_sceneGraph.at(i).pivotZ << ","
			<< m_sceneGraph.at(i).snapToGround << ","
			<< m_sceneGraph.at(i).AINode << ","
			<< "'" << m_sceneGraph.at(i).audio_path << "'" << ","
			<< m_sceneGraph.at(i).volume << ","
			<< m_sceneGraph.at(i).pitch << ","
			<< m_sceneGraph.at(i).pan << ","
			<< m_sceneGraph.at(i).one_shot << ","
			<< m_sceneGraph.at(i).play_on_init << ","
			<< m_sceneGraph.at(i).play_in_editor << ","
			<< m_sceneGraph.at(i).min_dist << ","
			<< m_sceneGraph.at(i).max_dist << ","
			<< m_sceneGraph.at(i).camera << ","
			<< m_sceneGraph.at(i).path_node << ","
			<< m_sceneGraph.at(i).path_node_start << ","
			<< m_sceneGraph.at(i).path_node_end << ","
			<< m_sceneGraph.at(i).parent_id << ","
			<< m_sceneGraph.at(i).editor_wireframe << ","
			<< "'" << m_sceneGraph.at(i).name << "'" << ","

			<< m_sceneGraph.at(i).light_type << ","
			<< m_sceneGraph.at(i).light_diffuse_r << ","
			<< m_sceneGraph.at(i).light_diffuse_g << ","
			<< m_sceneGraph.at(i).light_diffuse_b << ","
			<< m_sceneGraph.at(i).light_specular_r << ","
			<< m_sceneGraph.at(i).light_specular_g << ","
			<< m_sceneGraph.at(i).light_specular_b << ","
			<< m_sceneGraph.at(i).light_spot_cutoff << ","
			<< m_sceneGraph.at(i).light_constant << ","
			<< m_sceneGraph.at(i).light_linear << ","
			<< m_sceneGraph.at(i).light_quadratic

			<< ")";
		std::string sqlCommand2 = command.str();
		rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand2.c_str(), -1, &pResults, 0);
		sqlite3_step(pResults);	
	}
	MessageBox(NULL, L"Objects Saved", L"Notification", MB_OK);
}

void ToolMain::onActionSaveTerrain()
{
	m_d3dRenderer.SaveDisplayChunk(&m_chunk);
}

void ToolMain::AddObjectToScene(CString modelPath, CString m_texPath, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scaX, float scaY, float scaZ)
{
	SceneObject newSceneObject;

	if (m_sceneGraph.size() > 0) {
		newSceneObject.ID = m_sceneGraph.back().ID + 1;
		newSceneObject.chunk_ID = m_sceneGraph.back().chunk_ID;
	}
	else {
		newSceneObject.ID = 1;
		newSceneObject.chunk_ID = 0;
	}

	newSceneObject.model_path = CT2CA(modelPath);
	newSceneObject.tex_diffuse_path = CT2CA(m_texPath);
	newSceneObject.posX = posX;
	newSceneObject.posY = posY;
	newSceneObject.posZ = posZ;
	newSceneObject.rotX = rotX;
	newSceneObject.rotY = rotY;
	newSceneObject.rotZ = rotZ;
	newSceneObject.scaX = scaX;
	newSceneObject.scaY = scaY;
	newSceneObject.scaZ = scaZ;
	newSceneObject.render = false;
	newSceneObject.collision = false;
	newSceneObject.collision_mesh = "";
	newSceneObject.collectable = false;
	newSceneObject.destructable = false;
	newSceneObject.health_amount = 0;
	newSceneObject.editor_render = true;
	newSceneObject.editor_texture_vis = true;
	newSceneObject.editor_normals_vis = true;
	newSceneObject.editor_collision_vis = true;
	newSceneObject.editor_pivot_vis = true;
	newSceneObject.pivotX = 0;
	newSceneObject.pivotY = 0;
	newSceneObject.pivotZ = 0;
	newSceneObject.snapToGround = false;
	newSceneObject.AINode = false;
	newSceneObject.audio_path = "";
	newSceneObject.volume = 0;
	newSceneObject.pitch = 0;
	newSceneObject.pan = 0;
	newSceneObject.one_shot = false;
	newSceneObject.play_on_init = false;
	newSceneObject.play_in_editor = false;
	newSceneObject.min_dist = 0;
	newSceneObject.max_dist = 0;
	newSceneObject.camera = false;
	newSceneObject.path_node = false;
	newSceneObject.path_node_start = false;
	newSceneObject.path_node_end = false;
	newSceneObject.parent_id = 0;
	newSceneObject.editor_wireframe = false;
	newSceneObject.name = newSceneObject.model_path.substr(newSceneObject.model_path.rfind('/'), newSceneObject.model_path.rfind(".cmo"));

	newSceneObject.light_type = 1;
	newSceneObject.light_diffuse_r = 2;
	newSceneObject.light_diffuse_g = 3;
	newSceneObject.light_diffuse_b = 4;
	newSceneObject.light_specular_r = 5;
	newSceneObject.light_specular_g = 6;
	newSceneObject.light_specular_b = 7;
	newSceneObject.light_spot_cutoff = 8;
	newSceneObject.light_constant = 9;
	newSceneObject.light_linear = 0;
	newSceneObject.light_quadratic = 1;


	//send completed object to scenegraph
	m_sceneGraph.push_back(newSceneObject);
	m_d3dRenderer.SetRebuildDisplayList(true);
}

void ToolMain::UpdateObjectByID(SceneObject newData, int ID)
{
	int i = GetIndexFromID(ID);
	if (i != -1) {
		newData.ID = ID;
		m_sceneGraph[i] = newData;

		m_d3dRenderer.SetRebuildDisplayList(true);
	}

	/*m_sceneGraph[i].chunk_ID = newData.chunk_ID;
	m_sceneGraph[i].model_path = newData.model_path;
	m_sceneGraph[i].tex_diffuse_path = newData.tex_diffuse_path;
	m_sceneGraph[i].posX = newData.posX;
	m_sceneGraph[i].posY = newData.posY;
	m_sceneGraph[i].posZ = newData.posZ;
	m_sceneGraph[i].rotX = newData.rotX;
	m_sceneGraph[i].rotY = newData.rotY;
	m_sceneGraph[i].rotZ = newData.rotZ;
	m_sceneGraph[i].scaX = newData.scaX;
	m_sceneGraph[i].scaY = newData.scaY;
	m_sceneGraph[i].scaZ = newData.scaZ;
	m_sceneGraph[i].render = newData.render;
	m_sceneGraph[i].collision = newData.collision;
	m_sceneGraph[i].collision_mesh = newData.collision_mesh;
	m_sceneGraph[i].collectable = newData.collectable;
	m_sceneGraph[i].destructable = newData.destructable;
	m_sceneGraph[i].health_amount = newData.health_amount;
	m_sceneGraph[i].editor_render = newData.editor_render;
	m_sceneGraph[i].editor_texture_vis = newData.editor_texture_vis;
	m_sceneGraph[i].editor_normals_vis = newData.editor_normals_vis;
	m_sceneGraph[i].editor_collision_vis = newData.editor_collision_vis;
	m_sceneGraph[i].editor_pivot_vis = newData.editor_pivot_vis;
	m_sceneGraph[i].pivotX = newData.pivotX;
	m_sceneGraph[i].pivotY = newData.pivotY;
	m_sceneGraph[i].pivotZ = newData.pivotZ;
	m_sceneGraph[i].snapToGround = newData.snapToGround;
	m_sceneGraph[i].AINode = newData.AINode;
	m_sceneGraph[i].audio_path = newData.audio_path;
	m_sceneGraph[i].volume = newData.volume;
	m_sceneGraph[i].pitch = newData.pitch;
	m_sceneGraph[i].pan = newData.pan;
	m_sceneGraph[i].one_shot = newData.one_shot;
	m_sceneGraph[i].play_on_init = newData.play_on_init;
	m_sceneGraph[i].play_in_editor = newData.play_in_editor;
	m_sceneGraph[i].min_dist = newData.min_dist;
	m_sceneGraph[i].max_dist = newData.max_dist;
	m_sceneGraph[i].camera = newData.camera;
	m_sceneGraph[i].path_node = newData.path_node;
	m_sceneGraph[i].path_node_start = newData.path_node_start;
	m_sceneGraph[i].path_node_end = newData.path_node_end;
	m_sceneGraph[i].parent_id = newData.parent_id;
	m_sceneGraph[i].editor_wireframe = newData.editor_wireframe;
	m_sceneGraph[i].name = newData.name;

	m_sceneGraph[i].light_type = newData.light_type;
	m_sceneGraph[i].light_diffuse_r = newData.light_diffuse_r;
	m_sceneGraph[i].light_diffuse_g = newData.light_diffuse_g;
	m_sceneGraph[i].light_diffuse_b = newData.light_diffuse_b;
	m_sceneGraph[i].light_specular_r = newData.light_specular_r;
	m_sceneGraph[i].light_specular_g = newData.light_specular_g;
	m_sceneGraph[i].light_specular_b = newData.light_specular_b;
	m_sceneGraph[i].light_spot_cutoff = newData.light_spot_cutoff;
	m_sceneGraph[i].light_constant = newData.light_constant;
	m_sceneGraph[i].light_linear = newData.light_linear;
	m_sceneGraph[i].light_quadratic = newData.light_quadratic;*/
}

int ToolMain::GetIndexFromID(int ID)
{
	for (size_t i = 0; i < m_sceneGraph.size(); i++)
	{
		if (m_sceneGraph[i].ID == ID)
			return i;
	}
	return -1;
}

void ToolMain::UpdateSettings(float camMovSpd, float camRotSpd)
{
	m_d3dRenderer.SetCameraSettings(camMovSpd, camRotSpd);
}

float ToolMain::GetCameraMoveSpeed()
{
	return m_d3dRenderer.GetCameraMoveSpeed();
}

float ToolMain::GetCameraRotationSpeed()
{
	return m_d3dRenderer.GetCameraRotationSpeed();
}

void ToolMain::Tick(MSG *msg)
{

	CWnd* myFrame = CWnd::FromHandle(m_toolHandle);
	CRect dxViewRect;
	myFrame->GetWindowRect(dxViewRect);
	//Renderer Update Call
	m_d3dRenderer.SetScreenDimensions(dxViewRect);
	m_d3dRenderer.Tick(&m_toolInputCommands);

	//do we have a selection
	//do we have a mode
	//are we clicking / dragging /releasing
	//has something changed
		//update Scenegraph
		//add to scenegraph
		//resend scenegraph to Direct X renderer

	if (m_d3dRenderer.ShouldRebuildDisplayList())
		m_d3dRenderer.BuildDisplayList(&m_sceneGraph);


	m_selectedObject = m_d3dRenderer.GetSelectedID();
}

void ToolMain::UpdateInput(MSG* msg)
{

	switch (msg->message)
	{
		//Global inputs,  mouse position and keys etc
	case WM_KEYDOWN:
		m_keyArray[msg->wParam] = true;
		break;

	case WM_KEYUP:
		m_keyArray[msg->wParam] = false;
		break;

	case WM_MOUSEMOVE:
	{
		m_toolInputCommands.inWindowMouseX = GET_X_LPARAM(msg->lParam);
		m_toolInputCommands.inWindowMouseY = GET_Y_LPARAM(msg->lParam);

		POINT cursorPos;
		if (GetCursorPos(&cursorPos)) {
			m_toolInputCommands.mouseX = cursorPos.x;
			m_toolInputCommands.mouseY = cursorPos.y;
		}

		break;
	}
	case WM_LBUTTONDOWN:	//mouse button down
		m_toolInputCommands.lmbDown = true;
		break;

	case WM_LBUTTONUP:	//mouse button up
		m_toolInputCommands.lmbDown = false;
		break;

	case WM_RBUTTONDOWN:	//mouse button down
	{
		m_toolInputCommands.rmbDown = true;

		CWnd* myFrame = CWnd::FromHandle(m_toolHandle);
		CRect dxViewRect;
		myFrame->GetWindowRect(dxViewRect);

		bool mouseInWindowX = (m_toolInputCommands.mouseX > dxViewRect.left && m_toolInputCommands.mouseX < dxViewRect.right),
			mouseInWindowY = (m_toolInputCommands.mouseY > dxViewRect.top && m_toolInputCommands.mouseY < dxViewRect.bottom);

		if (mouseInWindowX && mouseInWindowY && HasFocus()) {
			m_toolInputCommands.useMouseForCam = true;
			ShowCursor(false);

			POINT cursorPos;
			if (GetCursorPos(&cursorPos)) {
				m_toolInputCommands.originalMouseX = cursorPos.x;
				m_toolInputCommands.originalMouseY = cursorPos.y;
			}

			SetCursorPos(dxViewRect.CenterPoint().x, dxViewRect.CenterPoint().y);
		}
		break;
	}
	case WM_RBUTTONUP:	//mouse button up
		m_toolInputCommands.rmbDown = false;
		if (m_toolInputCommands.useMouseForCam) {
			m_toolInputCommands.useMouseForCam = false;
			ShowCursor(true);
			SetCursorPos(m_toolInputCommands.originalMouseX, m_toolInputCommands.originalMouseY);
		}
		break;
	}

	//here we update all the actual app functionality that we want.  This information will either be used int toolmain, or sent down to the renderer (Camera movement etc
	// flight controls
	if (HasFocus()) {
		if (m_keyArray['W'])
			m_toolInputCommands.forward = true;
		else
			m_toolInputCommands.forward = false;

		if (m_keyArray['S'])
			m_toolInputCommands.back = true;
		else
			m_toolInputCommands.back = false;

		if (m_keyArray['A'])
			m_toolInputCommands.left = true;
		else
			m_toolInputCommands.left = false;

		if (m_keyArray['D'])
			m_toolInputCommands.right = true;
		else
			m_toolInputCommands.right = false;

		if (m_keyArray['E'])
			m_toolInputCommands.flyUp = true;
		else
			m_toolInputCommands.flyUp = false;

		if (m_keyArray['Q'])
			m_toolInputCommands.flyDown = true;
		else
			m_toolInputCommands.flyDown = false;
		//WASD

		// arrow key rotation: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		if (m_keyArray[VK_UP])
			m_toolInputCommands.pitchUp = true;
		else
			m_toolInputCommands.pitchUp = false;

		if (m_keyArray[VK_LEFT])
			m_toolInputCommands.yawLeft = true;
		else
			m_toolInputCommands.yawLeft = false;

		if (m_keyArray[VK_DOWN])
			m_toolInputCommands.pitchDown = true;
		else
			m_toolInputCommands.pitchDown = false;

		if (m_keyArray[VK_RIGHT])
			m_toolInputCommands.yawRight = true;
		else
			m_toolInputCommands.yawRight = false;

		if (m_keyArray[VK_SHIFT])
			m_toolInputCommands.slowFlight = true;
		else
			m_toolInputCommands.slowFlight = false;

		if (m_keyArray[VK_DELETE]) {
			int i = GetIndexFromID(m_selectedObject);
			if (i >= 0) {
				m_sceneGraph.erase(m_sceneGraph.begin() + i);
				m_d3dRenderer.SetRebuildDisplayList(true);
			}
		}

	}
}

bool ToolMain::HasFocus()
{
	return(GetFocus() == GetParent(m_toolHandle));
}
