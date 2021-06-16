#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <iostream>
#include <InitShader.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"

/**
 * Fields
 */
bool show_demo_window = false;
bool Local_World_Mode = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene, Renderer& renderer);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
 	std::cout << "test" << std::endl;//example
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene,renderer);
		RenderFrame(window, scene, renderer, io);
    }

	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene, Renderer& renderer)
{



	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");
	
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					//std::cout << scene.GetModelCount() << std::endl;
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)'=
		
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed
	
	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static float rotateX = 0.0f;
		static float rotateY = 0.0f;
		static float rotateZ = 0.0f;
		static float scaleX = 100;
		static float scaleY = 100;
		static float scaleZ = 0.0f;
		static float TranslateX = 640;
		static float TranslateY = 400;
		static float TranslateZ = 100;
		bool Local_World_Mode;
		//
		static float rotateX_world = 0.0f;
		static float rotateY_world = 0.0f;
		static float rotateZ_world = 0.0f;
		static float scaleX_world = 1.0f;
		static float scaleY_world = 1.0f;
		static float scaleZ_world = 1.0f;
		static float TranslateX_world = 0.0f;
		static float TranslateY_world = 0.0f;
		static float TranslateZ_world = 0.0f;
		//

		static int counter = 0;
		static float xCounter = 0;
		static float yCounter = 0;
		static float zCounter = 0;
		
		
		//if(){}
		ImGui::Begin("transform in world!");                          // Create a window called "Hello, world!" and append into it.

		//ImGui::Checkbox("World/Local Mode", &Local_World_Mode);

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color


		//fix gui for new function tools
		//rotate
		ImGui::SliderFloat("Rotate X world:", &rotateX_world, 0.0f, 360.0f);
		if (scene.GetModelCount() > 0) 
		{
			//scene.GetActiveModel().set_anglex(rotateX);
			scene.set_anglex(rotateX_world);
			//renderer.Render(scene);
		}
		ImGui::SliderFloat("Rotate Y world:", &rotateY_world, 0.0f, 360.0f);
		if (scene.GetModelCount() > 0) 
		{
			//scene.GetActiveModel().set_anglex(rotateX);
			scene.set_angley(rotateY_world);
			//renderer.Render(scene);
		}
		ImGui::SliderFloat("Rotate Z world:", &rotateZ_world, 0.0f, 360.0f);
		if (scene.GetModelCount() > 0)
		{
			//scene.GetActiveModel().set_anglex(rotateX);
			scene.set_anglez(rotateZ_world);
			//renderer.Render(scene);
		}
		
		//scale
		ImGui::SliderFloat("Scale X world:", &scaleX_world, 0.1f, 2.0f);
		if (scene.GetModelCount() > 0)
		{
			//scene.GetActiveModel().set_scalex(yCounter);
			scene.set_scalex(scaleX_world);
			//renderer.Render(scene);
		}

		ImGui::SliderFloat("Scale Y world:", &scaleY_world, 0.1f, 2.0f);
		if (scene.GetModelCount() > 0)
		{
			//scene.GetActiveModel().set_scaley(yCounter);
			scene.set_scaley(scaleY_world);
			//renderer.Render(scene);
		}

		ImGui::SliderFloat("Scale Z world:", &scaleZ_world, 0.1f, 2.0f);
		if (scene.GetModelCount() > 0)
		{
			//scene.GetActiveModel().set_scalez(yCounter);
			scene.set_scalez(scaleZ_world);
			//renderer.Render(scene);
		}
		//
		//Translate
		ImGui::SliderFloat("Translate X world:", &TranslateX_world, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0) {
			//scene.GetActiveModel().set_translatex(zCounter);
			scene.set_translatex(TranslateX_world);

			//renderer.Render(scene);
		}

		ImGui::SliderFloat("Translate Y world:", &TranslateY_world, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0) {
			//scene.GetActiveModel().set_translatex(zCounter);
			scene.set_translatey(TranslateY_world);
			//renderer.Render(scene);
		}

		ImGui::SliderFloat("Translate Z world:", &TranslateZ_world, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0) {
			//scene.GetActiveModel().set_translatex(zCounter);
			scene.set_translatez(TranslateZ_world);
			//renderer.Render(scene);
		}


		

		//ImGui::Text("Rotate X:");
		//ImGui::Text("Rotate X:");
		//ImGui::Text("X = %f", xCounter);
		//if (ImGui::Button("X +1"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	xCounter++;
		//ImGui::SameLine();
		//if (ImGui::Button("X +10"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	xCounter += 10;
		//ImGui::SameLine();
		//if (ImGui::Button("X +45"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	xCounter += 45;
		//if (scene.GetModelCount() > 0) {
		//	scene.GetActiveModel().set_anglex(xCounter);
		//	renderer.Render(scene);
		//	
		//}
		//if (ImGui::Button("X -1"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	xCounter--;
		//ImGui::SameLine();
		//if (ImGui::Button("X -10"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	xCounter -= 10;
		//ImGui::SameLine();
		//if (ImGui::Button("Y -45"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	xCounter -= 45;

		////ImGui::Text("Rotate Y:");
		//ImGui::Text("Rotate Y:");
		//ImGui::Text("Y = %f", yCounter);
		//if (ImGui::Button("Y +1"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	yCounter++;
		//ImGui::SameLine();
		//if (ImGui::Button("Y +10"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	yCounter+=10;
		//ImGui::SameLine();
		//if (ImGui::Button("Y +45"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	yCounter+=45;
		//if (scene.GetModelCount() > 0) {
		//	scene.GetActiveModel().set_scalex(yCounter);
		//	renderer.Render(scene);

		//}

		//if (ImGui::Button("Y -1"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	yCounter--;
		//ImGui::SameLine();
		//if (ImGui::Button("Y -10"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	yCounter-=10;
		//ImGui::SameLine();
		//if (ImGui::Button("Y -45"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	yCounter-=45;

		////ImGui::Text("Rotate Z:");
		//ImGui::Text("Rotate Z:");
		//ImGui::Text("Z = %f", zCounter);
		//if (ImGui::Button("Z +1"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	zCounter++;
		//ImGui::SameLine();
		//if (ImGui::Button("Z +10"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	zCounter += 10;
		//ImGui::SameLine();
		//if (ImGui::Button("Z +45"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	zCounter += 45;	
		//if (scene.GetModelCount() > 0) {
		//	scene.GetActiveModel().set_translatex(zCounter);
		//	renderer.Render(scene);
		//}

		//if (ImGui::Button("Z -1"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	zCounter--;
		//ImGui::SameLine();
		//if (ImGui::Button("Z -10"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	zCounter -= 10;
		//ImGui::SameLine();
		//if (ImGui::Button("Z -45"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	zCounter -= 45;

		//
		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		

		ImGui::End();//
		

		ImGui::Begin("transform in local!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//fix gui for new function tools
		//rotate
		ImGui::SliderFloat("Rotate X :", &rotateX, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0)
		{
			//scene.GetActiveModel().set_anglex(rotateX);
			scene.GetModel(scene.GetModelCount() - 1).set_anglex(rotateX);
			renderer.Render(scene);
		}
		ImGui::SliderFloat("Rotate Y:", &rotateY, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0)
		{
			//scene.GetActiveModel().set_anglex(rotateX);
			scene.GetModel(scene.GetModelCount() - 1).set_angley(rotateY);
			renderer.Render(scene);
		}
		ImGui::SliderFloat("Rotate Z:", &rotateZ, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0)
		{
			//scene.GetActiveModel().set_anglex(rotateX);
			scene.GetModel(scene.GetModelCount() - 1).set_anglez(rotateZ);
			renderer.Render(scene);
		}

		//scale
		ImGui::SliderFloat("Scale X:", &scaleX, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0)
		{
			//scene.GetActiveModel().set_scalex(yCounter);
			scene.GetModel(scene.GetModelCount() - 1).set_scalex(scaleX);
			renderer.Render(scene);
		}

		ImGui::SliderFloat("Scale Y:", &scaleY, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0)
		{
			//scene.GetActiveModel().set_scaley(yCounter);
			scene.GetModel(scene.GetModelCount() - 1).set_scaley(scaleY);
			renderer.Render(scene);
		}

		ImGui::SliderFloat("Scale Z:", &scaleZ, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0)
		{
			//scene.GetActiveModel().set_scalez(yCounter);
			scene.GetModel(scene.GetModelCount() - 1).set_scalez(scaleZ);
			renderer.Render(scene);
		}
		//
		//Translate
		ImGui::SliderFloat("Translate X:", &TranslateX, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0) {
			//scene.GetActiveModel().set_translatex(zCounter);
			scene.GetModel(scene.GetModelCount() - 1).set_translatex(TranslateX);
			renderer.Render(scene);
		}

		ImGui::SliderFloat("Translate Y:", &TranslateY, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0) {
			//scene.GetActiveModel().set_translatex(zCounter);
			scene.GetModel(scene.GetModelCount() - 1).set_translatey(TranslateY);
			renderer.Render(scene);
		}

		ImGui::SliderFloat("Translate Z:", &TranslateZ, 0.0f, 1000.0f);
		if (scene.GetModelCount() > 0) {
			//scene.GetActiveModel().set_translatex(zCounter);
			scene.GetModel(scene.GetModelCount() - 1).set_translatez(TranslateZ);
			renderer.Render(scene);
		}

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);



		ImGui::End();//
	}
	

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}