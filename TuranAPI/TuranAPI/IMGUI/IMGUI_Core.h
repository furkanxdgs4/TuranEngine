#pragma once
#include "TuranAPI/API_includes.h"
#include "IMGUI_GFX.h"


namespace TuranAPI {
	namespace IMGUI {
		class TURANAPI IMGUI {
		public:
			static IMGUI_GFX* GFX_IMGUI;
			static bool Is_IMGUI_Open;

			static bool Check_IMGUI_Version();
			static void* Create_Context(void* gpu_window_context);
			static void Set_Current_Context(void* context);
			static void Destroy_IMGUI_Resources();
			static void Platform_Settings();
			static void Set_as_MainViewport();

			static bool Show_DemoWindow();
			static bool Show_MetricsWindow();

			//IMGUI FUNCTIONALITY!
			static void New_Frame();
			static void Render_Frame();
			static bool Create_Window(const string &title, bool& should_close, const bool& has_menubar = false);
			static void End_Window();
			static void Text(const string &text);
			static bool Button(const string &button_name);
			static bool Checkbox(const string &name, bool* variable);
			static bool Input_Text(const string& name, string* text);
			//Create a menubar for a IMGUI window!
			static bool Begin_Menubar();
			static void End_Menubar();
			//Create a menu button! Returns if it is clicked!
			static bool Begin_Menu(const string &name);
			static void End_Menu();
			//Create a item for a menu! Shortcut argument is just use for the future support, nothing functional for now.
			static bool Menu_Item(const string& name, const string& Shortcut = "");
			//Show a paragraph text!
			static void Paragraph_Text(const string& name, const string& text);
			//Write a paragraph text!
			static void Input_Paragraph_Text(const string& name, string* text);
			//Put the next item to the same line with last created item
			//Use between after last item's end - before next item's begin!
			static void Same_Line();
			static bool Begin_Tree(const string& name);
			static void End_Tree();
			//Create a select list that extends when clicked and get the selected_index in one-line of code!
			//Returns if any item is selected in the list! Selected item's index is the selected_index's pointer's value!
			static bool SelectList_OneLine(const string& name, int* selected_index, const vector<string>* item_names);
			static void Selectable(const string& name, bool* is_selected);
			//Create a box of selectable items in one-line of code!
			//Returns if any item is selected in the list! Selected item's index is the selected_index's pointer's value!
			static bool Selectable_ListBox(const string& name, int* selected_index, vector<string>* item_names);
			//Create a box of checkable items in one-line of code! Nice feature to edit a list
			static void CheckListBox(const string& name, vector<bool>* items_status, vector<string>* item_names);
			//Display a texture that is in the GPU memory, for example a Render Target or a Texture
			static void Display_Texture(void* TEXTURE_GL_ID, const unsigned int& Display_WIDTH, const unsigned int& Display_HEIGHT, bool should_Flip_Vertically = false);
			
			//Add here Unsigned Int, Unsigned Short & Short, Unsigned Char & Char sliders too!

			static bool Slider_Int(const string& name, int* data, int min, int max);
			static bool Slider_Float(const string& name, float* data, float min, float max);
			static bool Slider_Vec2(const string& name, vec2* data, float min, float max);
			static bool Slider_Vec3(const string& name, vec3* data, float min, float max);
			static bool Slider_Vec4(const string& name, vec4* data, float min, float max);
		};

	}
}