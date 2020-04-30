#include "Scene_Editor.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponent.h"
#include "EditorSource/FileSystem/GameObjects/StaticModel_Component.h"
#include "EditorSource/FileSystem/GameObjects/Camera_Component.h"

#include "GFXSource/GFX_Core.h"
#include "EditorSource/RenderContext/Game_RenderGraph.h"

#include "Status_Window.h"
#include "Properties_Window.h"
using namespace TuranAPI;
using namespace GFX_API;


namespace TuranEditor {

	class Create_StaticModelComp : public IMGUI_WINDOW {
		Scene_Resource* SCENE;
		StaticModel_Component* COMPONENT;
		Static_Model_Data* SELECTED_MODEL = nullptr;
		unsigned int selected_model_index;
		Vector<unsigned int> selected_matinst_indexes;
		String COMP_NAME;
	public:
		Create_StaticModelComp(Scene_Resource* scene);
		virtual void Run_Window() override;
	};

	class Create_CameraComp : public IMGUI_WINDOW {
		Scene_Resource* SCENE;
		Camera_Component* COMPONENT;
		int selected_AspectRatio_Index;
		String COMP_NAME;
	public:
		Create_CameraComp(Scene_Resource* scene);
		virtual void Run_Window() override;
	};




	Scene_Editor::Scene_Editor() : IMGUI_WINDOW("Scene Editor"), item_names(LASTUSEDALLOCATOR, 10, 100), component_names(LASTUSEDALLOCATOR, 10, 100) {
		IMGUI_REGISTERWINDOW(this);
	}

	void Scene_Editor::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name, Is_Window_Open, false)) {
			IMGUI->End_Window();
			return;
		}

		IMGUI->Text("Scene Editor isn't coded!");

		IMGUI->End_Window();

		/*
		//Search for a Scene content!
		if (SCENE_to_EDIT == nullptr) {
			//Create a window that doesn't have Menubar
			if (!IMGUI->Create_Window(Window_Name, Is_Window_Open, false)) {
				IMGUI->End_Window();
				return;
			}

			//Even if there is a importing proccess, show the contents!
			int selected_list_item_index = -1;
			for (unsigned int Scene_Resource_Index = 0; Scene_Resource_Index < Scene.ALL_SCENEs.size(); Scene_Resource_Index++) {
				item_names.push_back(ALL_SCENEs[Scene_Resource_Index]->NAME);
			}
			//Select a Scene resource to edit!
			if (IMGUI->Selectable_ListBox("Scene Resources List", &selected_list_item_index, &item_names)) {
				SCENE_to_EDIT = Scene.ALL_SCENEs[selected_list_item_index];
			}

			IMGUI->End_Window();

			return;
		}

		//There is scene!
		else {
			//Create a window that has Menubar
			if (!IMGUI->Create_Window(Window_Name, Is_Window_Open, true)) {
				IMGUI->End_Window();
				return;
			}
			//Even if there is a importing proccess, show the contents!
			int selected_list_item_index = -1;

			if (!IMGUI->Begin_Menubar()) {
				IMGUI->End_Menubar();
			}
			if (IMGUI->Begin_Menu("Add")) {

				if (IMGUI->Menu_Item("Static Model Component")) { new Create_StaticModelComp(SCENE_to_EDIT); }
				if (IMGUI->Menu_Item("Camera Component")) { new Create_CameraComp(SCENE_to_EDIT); }

				IMGUI->End_Menu();
			}
			IMGUI->End_Menubar();

			//Find every game component here and create component_names list to show!
			for (unsigned int i = 0; i < SCENE_to_EDIT->ADDED_COMPONENTs.size(); i++) {
				GameComponent* Game_Component = SCENE_to_EDIT->ADDED_COMPONENTs[i];
				component_names.push_back(Game_Component->NAME);
			}

			//Show selected content's properties!
			if (IMGUI->Selectable_ListBox("Game Component List", &selected_list_item_index, &component_names)) {
				new GameComponentProperties_Window(SCENE_to_EDIT->ADDED_COMPONENTs[selected_list_item_index]);
			}



			/*
			if (RenderGraph_forScene == nullptr) {
				RenderGraph_forScene = new Game_RenderGraph;
			}
			else {
				unsigned int RT_ID = RenderGraph_forScene->Get_FinalColor_Texture()->Get_ID();
				IMGUI->Display_Texture(&RT_ID, 960, 540, true);
			}

		

			IMGUI->End_Window();
		}*/
	}



	Create_CameraComp::Create_CameraComp(Scene_Resource* scene) : IMGUI_WINDOW("Create Camera Component"), SCENE(scene) { COMPONENT = new Camera_Component(vec3(0, 0, 0)); }
	void Create_CameraComp::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name, Is_Window_Open)) {
			IMGUI->End_Window();
			return;
		}

		//IMGUI->Input_Text("Camera Name: ", &COMP_NAME);

		if (IMGUI->Button("Create")) {
			COMPONENT->NAME = COMP_NAME;
			SCENE->ADDED_COMPONENTs.push_back(COMPONENT);
			SCENE->Write_ToDisk(true);
			IMGUI->End_Window();
			Is_Window_Open = false;
			IMGUI_DELETEWINDOW(this);
			return;
		}


		IMGUI->End_Window();
	}





	Create_StaticModelComp::Create_StaticModelComp(Scene_Resource* scene) : IMGUI_WINDOW("Create StaticModel Component"), SCENE(scene), selected_matinst_indexes(LASTUSEDALLOCATOR, 10, 100) {
		COMPONENT = new StaticModel_Component(SELECTED_MODEL = CONTENTMANAGER->Read_ALLStaticModelAssets()->Get(0));
	}
	void Create_StaticModelComp::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name, Is_Window_Open)) {
			IMGUI->End_Window();
			return;
		}


		Vector<String> Models_Names(LASTUSEDALLOCATOR, 10, 100);						//Get Model names to choose one of them
		for (unsigned int i = 0; i < CONTENTMANAGER->Read_ALLStaticModelAssets()->size(); i++) {
			Static_Model_Data* MODEL = CONTENTMANAGER->Read_ALLStaticModelAssets()->Get(i);
			Models_Names.push_back(MODEL->NAME);
		}
		Vector<String> MatInst_Names(LASTUSEDALLOCATOR, 10, 100);						//Get MatInst names to choose one of them
		for (unsigned int i = 0; i < CONTENTMANAGER->Read_ALLMaterialInstances()->size(); i++) {
			Material_Instance* MatInst = CONTENTMANAGER->Read_ALLMaterialInstances()->Get(i);
			MatInst_Names.push_back(MatInst->NAME);
		}

		//IMGUI->Input_Text("Name", &COMP_NAME);
		if (IMGUI->SelectList_OneLine("Model", &selected_model_index, &Models_Names)) {
			SELECTED_MODEL = CONTENTMANAGER->Read_ALLStaticModelAssets()->Get(selected_model_index);
			delete COMPONENT;
			COMPONENT = new StaticModel_Component(SELECTED_MODEL);
		}
		//selected_matinst_indexes.resize(COMPONENT->MATERIALs.size());
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
				COMPONENT->MATERIALs[i] = CONTENTMANAGER->Read_ALLMaterialInstances()->Get(selected_matinst_index);
			}
			COMPONENT->NAME = COMP_NAME;
			SCENE->ADDED_COMPONENTs.push_back(COMPONENT);
			SCENE->Write_ToDisk(true);
			Is_Window_Open = false;
			IMGUI_DELETEWINDOW(this);
			return;
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
		if (!IMGUI->Create_Window(Window_Name, Is_Window_Open, false)) {
			IMGUI->End_Window();
			return;
		}

		IMGUI->Input_Text("Scene Name", &Scene_NAME);
		IMGUI->Input_Text("Output Folder", &Scene_Folder);
		if (IMGUI->Button("Create")) {
			Scene_Resource* SCENE = new Scene_Resource;
			SCENE->PATH = Scene_Folder;
			SCENE->PATH.append(Scene_NAME);
			SCENE->PATH.append(".scenecont");
			SCENE->NAME = Scene_NAME;
			SCENE->Write_ToDisk(true);
			EDITOR_FILESYSTEM->Add_anAsset_toFileList(SCENE);
		}

		IMGUI->End_Window();
	}
}