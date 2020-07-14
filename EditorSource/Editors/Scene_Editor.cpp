#include "Scene_Editor.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponent.h"
#include "EditorSource/FileSystem/GameObjects/StaticModel_Component.h"
#include "EditorSource/FileSystem/GameObjects/Camera_Component.h"
#include "EditorSource/FileSystem/ResourceTypes/ResourceTYPEs.h"

#include "GFXSource/GFX_Core.h"
#include "EditorSource/RenderContext/Game_RenderGraph.h"

#include "Status_Window.h"
#include "Properties_Window.h"
using namespace TuranAPI;
using namespace GFX_API;


namespace TuranEditor {

	class Create_StaticModelComp : public IMGUI_WINDOW {
		Resource_Identifier* SCENE;
		StaticModel_Component* COMPONENT;
		Resource_Identifier* SELECTED_MODEL = nullptr;
		unsigned int selected_model_index;
		vector<unsigned int> selected_matinst_indexes;
		string COMP_NAME;
	public:
		Create_StaticModelComp(Resource_Identifier* scene);
		virtual void Run_Window() override;
	};

	class Create_CameraComp : public IMGUI_WINDOW {
		Resource_Identifier* SCENE;
		Camera_Component* COMPONENT;
		int selected_AspectRatio_Index;
		string COMP_NAME;
	public:
		Create_CameraComp(Resource_Identifier* scene);
		virtual void Run_Window() override;
	};

	


	Scene_Editor::Scene_Editor(Resource_Identifier* SCENE) : IMGUI_WINDOW("Scene Editor"), SCENE_to_EDIT(SCENE) {
		if (!SCENE) {
			TuranAPI::LOG_CRASHING("Scene_Resource* that's given to Scene_Editor is nullptr!\n");
			return;
		}
		IMGUI_REGISTERWINDOW(this);
	}

	void Scene_Editor::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name.c_str(), Is_Window_Open, true)) {
			IMGUI->End_Window();
			return;
		}

		//Even if there is a importing proccess, show the contents!
		int selected_list_item_index = -1;
		
		if (!IMGUI->Begin_Menubar()) {
			IMGUI->End_Menubar();
			IMGUI->End_Window();
			return;
		}
		
		if (IMGUI->Begin_Menu("Add")) {
			if (IMGUI->Menu_Item("Static Model Component")) { new Create_StaticModelComp(SCENE_to_EDIT); }
			if (IMGUI->Menu_Item("Camera Component")) { new Create_CameraComp(SCENE_to_EDIT); }

			IMGUI->End_Menu();
		}
		IMGUI->End_Menubar();

		Scene_Resource* SCENE_DATA = (Scene_Resource*)SCENE_to_EDIT->DATA;
		//Find every game component here and create component_names list to show!
		if (component_names.size() != SCENE_DATA->ADDED_COMPONENTs.size()) {
			component_names.clear();
			for (unsigned int i = 0; i < SCENE_DATA->ADDED_COMPONENTs.size(); i++) {
				GameComponent* Game_Component = SCENE_DATA->ADDED_COMPONENTs[i];
				component_names.push_back(Game_Component->NAME);
			}
		}

		//Show selected content's properties!
		if (IMGUI->Selectable_ListBox("Game Component List", &selected_list_item_index, &component_names)) {
			new GameComponentProperties_Window(SCENE_DATA->ADDED_COMPONENTs[selected_list_item_index]);
		}



		/*
		if (RenderGraph_forScene == nullptr) {
			RenderGraph_forScene = new Game_RenderGraph;
		}
		else {
			unsigned int RT_ID = RenderGraph_forScene->Get_FinalColor_Texture()->Get_ID();
			IMGUI->Display_Texture(&RT_ID, 960, 540, true);
		}*/



		IMGUI->End_Window();
	}



	Create_CameraComp::Create_CameraComp(Resource_Identifier* scene) : IMGUI_WINDOW("Create Camera Component"), SCENE(scene) { COMPONENT = new Camera_Component(vec3(0, 0, 0)); }
	void Create_CameraComp::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name.c_str(), Is_Window_Open)) {
			IMGUI->End_Window();
			return;
		}

		//IMGUI->Input_Text("Camera Name: ", &COMP_NAME);

		if (IMGUI->Button("Create")) {
			COMPONENT->NAME = COMP_NAME;
			Scene_Resource* SCENE_DATA = (Scene_Resource*)SCENE->DATA;
			SCENE_DATA->ADDED_COMPONENTs.push_back(COMPONENT);
			EDITOR_FILESYSTEM->Save_Resource(SCENE->ID);
			IMGUI->End_Window();
			Is_Window_Open = false;
			IMGUI_DELETEWINDOW(this);
		}


		IMGUI->End_Window();
	}





	Create_StaticModelComp::Create_StaticModelComp(Resource_Identifier* scene) : IMGUI_WINDOW("Create StaticModel Component"), SCENE(scene) {
		if (EDITOR_FILESYSTEM->Get_SpecificAssetType(RESOURCETYPEs::EDITOR_STATICMODEL).size() > 0) {
			COMPONENT = nullptr;
			IMGUI_REGISTERWINDOW(this);
		}
		else {
			TuranAPI::LOG_NOTCODED("There is no Static Model in File List! I didn't code an exact way to show this in Editor!\n", true);
		}
	}
	void Create_StaticModelComp::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name.c_str(), Is_Window_Open)) {
			IMGUI->End_Window();
			return;
		}


		vector<string> Models_Names;						//Get Model names to choose one of them
		vector<Resource_Identifier*> ALL_MODELASSETs = EDITOR_FILESYSTEM->Get_SpecificAssetType(RESOURCETYPEs::EDITOR_STATICMODEL);
		vector<Resource_Identifier*> ALL_MATINSTs = EDITOR_FILESYSTEM->Get_SpecificAssetType(RESOURCETYPEs::GFXAPI_MATINST);
		if (!ALL_MODELASSETs.size()) {
			IMGUI->Text("There is no Static Model, so you can't create a Static Model Component!");
			IMGUI->End_Window();
			return;
		}
		if (!ALL_MATINSTs.size()) {
			IMGUI->Text("There is no Material Instance, so you can't create a Static Model Component!");
			IMGUI->End_Window();
			return;
		}
		for (unsigned int i = 0; i < ALL_MODELASSETs.size(); i++) {
			Resource_Identifier* MODEL = ALL_MODELASSETs[i];
			Models_Names.push_back(MODEL->Get_Name());
		}
		vector<string> MatInst_Names;						//Get MatInst names to choose one of them
		for (unsigned int i = 0; i < ALL_MATINSTs.size(); i++) {
			Resource_Identifier* MatInst = ALL_MATINSTs[i];
			MatInst_Names.push_back(MatInst->Get_Name());
		}

		IMGUI->Input_Text("Component Name", &COMP_NAME);



		//If this is the first frame of window!
		if (!SELECTED_MODEL) {
			SELECTED_MODEL = ALL_MODELASSETs[0];
			COMPONENT = new StaticModel_Component;
			COMPONENT->MODEL = SELECTED_MODEL;
			Static_Model* MODEL_DATA = (Static_Model*)SELECTED_MODEL->DATA;
			COMPONENT->MATERIALs.resize(MODEL_DATA->Material_Number);
		}
		//General window loop!
		if (IMGUI->SelectList_OneLine("Model", &selected_model_index, &Models_Names)) {
			SELECTED_MODEL = ALL_MODELASSETs[selected_model_index];
			delete COMPONENT;
			COMPONENT = new StaticModel_Component;
			COMPONENT->MODEL = SELECTED_MODEL;
			Static_Model* MODEL_DATA = (Static_Model*)SELECTED_MODEL->DATA;
			COMPONENT->MATERIALs.resize(MODEL_DATA->Material_Number);
		}




		selected_matinst_indexes.resize(COMPONENT->MATERIALs.size());
		if (IMGUI->Begin_Tree("Material Instances")) {
			for (unsigned int i = 0; i < COMPONENT->MATERIALs.size(); i++) {
				if (IMGUI->Begin_Tree("number->string isn't supported yet!")) {
					unsigned int selected_matinst_index = selected_matinst_indexes[i];
					IMGUI->SelectList_OneLine("Material Index: number->string isn't supported yet!", &selected_matinst_index, &MatInst_Names);
					IMGUI->End_Tree();
				}
			}
			IMGUI->End_Tree();
		}




		if (IMGUI->Button("Create")) {
			for (unsigned int i = 0; i < COMPONENT->MATERIALs.size(); i++) {
				int selected_matinst_index = selected_matinst_indexes[i];
				COMPONENT->MATERIALs[i] = ALL_MATINSTs[selected_matinst_index];
			}
			COMPONENT->NAME = COMPONENT->MODEL->Get_Name();
			Scene_Resource* SCENE_DATA = (Scene_Resource*)SCENE->DATA;
			SCENE_DATA->ADDED_COMPONENTs.push_back(COMPONENT);
			EDITOR_FILESYSTEM->Save_Resource(SCENE->ID);
			Is_Window_Open = false;
			IMGUI_DELETEWINDOW(this);
		}


		IMGUI->End_Window();
	}



	Scene_Create_Window::Scene_Create_Window() : IMGUI_WINDOW("Scene Create") {
		IMGUI_REGISTERWINDOW(this);
	}
	void Scene_Create_Window::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name.c_str(), Is_Window_Open, false)) {
			IMGUI->End_Window();
			return;
		}

		IMGUI->Input_Text("Scene Name", &Scene_NAME);
		IMGUI->Input_Text("Output Folder", &Scene_Folder);
		if (IMGUI->Button("Create")) {
			Scene_Resource* SCENE = new Scene_Resource;
			Resource_Identifier* RESOURCE = new Resource_Identifier;
			RESOURCE->PATH = Scene_Folder;
			RESOURCE->PATH.append(Scene_NAME);
			RESOURCE->PATH.append(".scenecont");
			RESOURCE->TYPE = RESOURCETYPEs::EDITOR_SCENE;
			RESOURCE->DATA = SCENE;
			EDITOR_FILESYSTEM->Add_anAsset_toFileList(RESOURCE);
			//Add_anAsset_toFileList gave an ID to Resource, so we can save it now!
			EDITOR_FILESYSTEM->Save_Resource(RESOURCE->ID);
		}

		IMGUI->End_Window();
	}
}