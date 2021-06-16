#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void Model_painting(const glm::vec4& p1, const glm::vec4& p2, const glm::vec4& p3);
	bool Inside(const glm::vec4& vec0, const glm::vec4& vec1, const glm::vec4& vec2,int i,int j,float max_x,float max_y);
	void clear_z_buffer();
	void set_z_value_for_pixel(float value, int i, int j);
	bool check_for_z_value_in_pixel(float search_value, const int i, const int j);

private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);

	void CreateBuffers(int w, int h);
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();

	float* color_buffer_;
	bool color_temp_buffer;
	float* z_buffer_;
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
};
