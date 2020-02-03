#include "IMGUI_Core.h"
using namespace TuranAPI::IMGUI;

#include "Includes/imgui.h"
#include "Includes/imgui_stdlib.h"

#include "TuranAPI/FileSystem/Resource_Types/Texture_Resource.h"

IMGUI_GFX* IMGUI::GFX_IMGUI = nullptr;
bool IMGUI::Is_IMGUI_Open = true;

bool IMGUI::Check_IMGUI_Version() {
	//Check version here, I don't care here for now!
	return IMGUI_CHECKVERSION();
}

void* IMGUI::Create_Context(void* gpu_window_context) {
	//Create Context here!
	void* Context = ImGui::CreateContext();
	if (Context == nullptr) {
		cout << "Error: Context is nullptr after creation!\n";
	}

	//Set Input Handling settings here! 
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


	//Set color style to dark by default for now!
	ImGui::StyleColorsDark();


	//Set context's GFX_API settings!
	if (GFX_IMGUI == nullptr) {
		cout << "GFX_IMGUI isn't initialized, initializing failed for IMGUI!\n";
		return nullptr;
	}
	GFX_IMGUI->Initialize(gpu_window_context);
	return Context;
}

void IMGUI::Set_as_MainViewport() {
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
	ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
}

void IMGUI::Set_Current_Context(void* context) {
	ImGui::SetCurrentContext((ImGuiContext*)context);
}

void IMGUI::Destroy_IMGUI_Resources() {
	cout << "IMGUI resources are being destroyed!\n";
	GFX_IMGUI->Destroy_IMGUI_GFX_Resources();
	ImGui::DestroyContext();
	Is_IMGUI_Open = false;
}

bool IMGUI::Show_DemoWindow() {
	bool x = true;
	ImGui::ShowDemoWindow(&x);
	return x;
}

bool IMGUI::Show_MetricsWindow() {
	bool x = true;
	ImGui::ShowMetricsWindow(&x);
	return x;
}

	//IMGUI FUNCTIONALITY!

void IMGUI::New_Frame() {
	GFX_IMGUI->GFX_New_Frame();
	ImGui::NewFrame();
}

void IMGUI::Render_Frame() {
	ImGui::Render();
	GFX_IMGUI->Render_IMGUI(ImGui::GetDrawData());
}

void IMGUI::Platform_Settings() {
	GFX_IMGUI->Set_Platform_Settings();
}

bool IMGUI::Create_Window(const string &title, bool &should_close, const bool& has_menubar) {
	ImGuiWindowFlags window_flags = 0;
	window_flags |= (has_menubar ? ImGuiWindowFlags_MenuBar : 0);
	return ImGui::Begin(title.c_str(), &should_close, window_flags);
}

void IMGUI::End_Window() {
	ImGui::End();
}

void IMGUI::Text(const string &text) {
	ImGui::Text(text.c_str());
}

bool IMGUI::Button(const string &button_name) {
	return ImGui::Button(button_name.c_str());
}

bool IMGUI::Checkbox(const string &name, bool* variable) {
	return ImGui::Checkbox(name.c_str(), variable);
}

bool IMGUI::Input_Text(const string &name, string* text) {
	return ImGui::InputText(name.c_str(), text, ImGuiInputTextFlags_EnterReturnsTrue);
}

bool IMGUI::Begin_Menubar() {
	return ImGui::BeginMenuBar();
}

void IMGUI::End_Menubar() {
	ImGui::EndMenuBar();
}

bool IMGUI::Begin_Menu(const string &name) {
	return ImGui::BeginMenu(name.c_str());
}

void IMGUI::End_Menu() {
	ImGui::EndMenu();
}

bool IMGUI::Menu_Item(const string& name, const string& shortcut) {
	return ImGui::MenuItem(name.c_str(), shortcut.c_str());
}

void IMGUI::Paragraph_Text(const string& name, const string& text) {
	cout << "Nothing functional for now in Paragraph_Text!\n";
	this_thread::sleep_for(chrono::seconds(5));
}

void IMGUI::Input_Paragraph_Text(const string& name, string* text) {
	ImGui::InputTextMultiline(name.c_str(), text);
}

//Puts the next item to the same line with last created item
//Use between after last item's end - before next item's begin!
void IMGUI::Same_Line() {
	ImGui::SameLine();
}

bool IMGUI::Begin_Tree(const string& name) {
	return ImGui::TreeNode(name.c_str());
}

void IMGUI::End_Tree() {
	ImGui::TreePop();
}

bool IMGUI::SelectList_OneLine(const string& name, int* selected_index, const vector<string>* item_names) {
	bool is_new_item_selected = false;
	if (
		ImGui::BeginCombo(name.c_str(), (*item_names)[*selected_index].c_str())	// The second parameter is the index of the label previewed before opening the combo.
		)
	{
		for (int n = 0; n < item_names->size(); n++)
		{
			string item_name = (*item_names)[n];
			bool is_selected = (*selected_index == n);
			if (ImGui::Selectable((*item_names)[n].c_str(), is_selected)) {
				*selected_index = n;
				is_new_item_selected = true;
			}
		}
		ImGui::EndCombo();
	}
	return is_new_item_selected;
}

//If selected, argument "is_selected" is set to its opposite!
void IMGUI::Selectable(const string& name, bool* is_selected){
	ImGui::Selectable(name.c_str(), is_selected);
}

bool IMGUI::Selectable_ListBox(const string& name, int* selected_index, vector<string>* item_names) {
	int already_selected_index = *selected_index;
	bool is_new_selected = false;
	if (ImGui::ListBoxHeader(name.c_str())) {
		for (unsigned int i = 0; i < item_names->size(); i++) {
			bool is_selected = false;
			string item_name = (*item_names)[i];
			Selectable(item_name, &is_selected);
			if (is_selected && (already_selected_index != i)) {
				*selected_index = i;
				is_new_selected = true;
			}
		}

		ImGui::ListBoxFooter();
	}
	return is_new_selected;
}

void IMGUI::CheckListBox(const string& name, vector<bool>* items_status, vector<string>* item_names) {
	if (ImGui::ListBoxHeader(name.c_str())) {
		for (unsigned int i = 0; i < item_names->size(); i++) {
			cout << "Current Index: " << i << endl;
			bool x = (*items_status)[i];
			Checkbox((*item_names)[i], &x);
			((*items_status)[i]) = x;
		}
		ImGui::ListBoxFooter();
	}
}

void IMGUI::Display_Texture(void* TEXTURE_GL_ID, const unsigned int& Display_WIDTH, const unsigned int& Display_HEIGHT, bool should_Flip_Vertically) {
	unsigned int data = *(unsigned int*)TEXTURE_GL_ID;
	if (should_Flip_Vertically) {
		ImGui::Image((void*)(intptr_t)data, ImVec2(Display_WIDTH, Display_HEIGHT), ImVec2(0, 1), ImVec2(1, 0));
	}
	else {
		ImGui::Image((void*)(intptr_t)data, ImVec2(Display_WIDTH, Display_HEIGHT));
	}
}


bool IMGUI::Slider_Int(const string& name, int* data, int min, int max) { return ImGui::SliderInt(name.c_str(), data, min, max); }
bool IMGUI::Slider_Float(const string& name, float* data, float min, float max) { return ImGui::SliderFloat(name.c_str(), data, min, max); }
bool IMGUI::Slider_Vec2(const string& name, vec2* data, float min, float max) { return ImGui::SliderFloat2(name.c_str(), (float*)data, min, max); }
bool IMGUI::Slider_Vec3(const string& name, vec3* data, float min, float max) { return ImGui::SliderFloat3(name.c_str(), (float*)data, min, max); }
bool IMGUI::Slider_Vec4(const string& name, vec4* data, float min, float max) { return ImGui::SliderFloat4(name.c_str(), (float*)data, min, max); }