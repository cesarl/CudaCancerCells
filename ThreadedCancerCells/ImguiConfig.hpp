#pragma once

#include "Globals.hpp"
#include <gl/GL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <imgui/stb_image.h>
#include <imgui/imgui.h>
#include <chrono>

namespace ImguiConf
{

	static void ImImpl_RenderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count)
	{
		if (cmd_lists_count == 0)
			return;

		// We are using the OpenGL fixed pipeline to make the example code simpler to read!
		// A probable faster way to render would be to collate all vertices from all cmd_lists into a single vertex buffer.
		// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		// Setup texture
		glBindTexture(GL_TEXTURE_2D, TCC::fontTex);
		glEnable(GL_TEXTURE_2D);

		// Setup orthographic projection matrix
		const float width = ImGui::GetIO().DisplaySize.x;
		const float height = ImGui::GetIO().DisplaySize.y;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, width, height, 0.0f, -1.0f, +1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Render command lists
		for (int n = 0; n < cmd_lists_count; n++)
		{
			const ImDrawList* cmd_list = cmd_lists[n];
			const unsigned char* vtx_buffer = (const unsigned char*)cmd_list->vtx_buffer.begin();
			glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer));
			glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + 8));
			glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + 16));

			int vtx_offset = 0;
			const ImDrawCmd* pcmd_end = cmd_list->commands.end();
			for (const ImDrawCmd* pcmd = cmd_list->commands.begin(); pcmd != pcmd_end; pcmd++)
			{
				glScissor((int)pcmd->clip_rect.x, (int)(height - pcmd->clip_rect.w), (int)(pcmd->clip_rect.z - pcmd->clip_rect.x), (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
				glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
				vtx_offset += pcmd->vtx_count;
			}
		}
		glDisable(GL_SCISSOR_TEST);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	static void scroll_callback(double xoffset, double yoffset)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel = (yoffset != 0.0f) ? yoffset > 0.0f ? 1 : -1 : 0;           // Mouse wheel: -1,0,+1
	}

	static void key_callback(int key, int action)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (action == GLUT_KEY_DOWN)
			io.KeysDown[key] = true;
		if (action == GLUT_KEY_UP)
			io.KeysDown[key] = false;
		io.KeyCtrl = glutGetModifiers() == GLUT_ACTIVE_CTRL;
		io.KeyShift = glutGetModifiers() == GLUT_ACTIVE_SHIFT;
	}

	static void char_callback(unsigned int c)
	{
		if (c > 0 && c <= 255)
			ImGui::GetIO().AddInputCharacter((char)c);
	}

	void InitImGui()
	{
		int w = TCC::windowWidth, h = TCC::windowHeight;

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)w, (float)h);        // Display size, in pixels. For clamping windows positions.
		io.DeltaTime = 1.0f / 60.0f;                          // Time elapsed since last frame, in seconds (in this sample app we'll override this every frame because our timestep is variable)
		io.PixelCenterOffset = 0.0f;                        // Align OpenGL texels
		io.KeyMap[ImGuiKey_Tab] = 9;             // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
		io.KeyMap[ImGuiKey_LeftArrow] = GLUT_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLUT_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLUT_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLUT_KEY_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLUT_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLUT_KEY_END;
		io.KeyMap[ImGuiKey_Delete] = 127;
		io.KeyMap[ImGuiKey_Backspace] = 8;
		io.KeyMap[ImGuiKey_Enter] = 10;
		io.KeyMap[ImGuiKey_Escape] = 27;
		io.KeyMap[ImGuiKey_A] = 97;
		io.KeyMap[ImGuiKey_C] = 99;
		io.KeyMap[ImGuiKey_V] = 118;
		io.KeyMap[ImGuiKey_X] = 120;
		io.KeyMap[ImGuiKey_Y] = 121;
		io.KeyMap[ImGuiKey_Z] = 122;

		io.RenderDrawListsFn = ImImpl_RenderDrawLists;

		// Load font texture
		glGenTextures(1, &TCC::fontTex);
		glBindTexture(GL_TEXTURE_2D, TCC::fontTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		const void* png_data;
		unsigned int png_size;
		ImGui::GetDefaultFontData(NULL, NULL, &png_data, &png_size);
		int tex_x, tex_y, tex_comp;
		void* tex_data = stbi_load_from_memory((const unsigned char*)png_data, (int)png_size, &tex_x, &tex_y, &tex_comp, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_x, tex_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
		stbi_image_free(tex_data);
	}

	void UpdateImGui()
	{
		ImGuiIO& io = ImGui::GetIO();

		static double time = glutGet(GLUT_ELAPSED_TIME);;
		const double current_time = glutGet(GLUT_ELAPSED_TIME);
		float dif = (float)(current_time - time);
		io.DeltaTime = dif == 0.0f ? 0.000000001f : dif / 1000.0f;
		time = current_time;
		// Setup inputs
		// (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())

		io.MousePos = ImVec2((float)TCC::mouse_x, (float)TCC::mouse_y);                           // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
		io.MouseDown[0] = TCC::lMouse;
		io.MouseDown[1] = TCC::rMouse;

		// Start the frame
		ImGui::NewFrame();
	}
}