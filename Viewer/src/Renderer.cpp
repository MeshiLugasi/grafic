#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include "Scene.h"



#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))
const glm::vec3 color = { 150,200,250 };

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height)
{
	InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer_;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;

	color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
	color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
	color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	int x;
	int y;
	int e;
	int max_x;
	int reflectFlag;
	int deltaX, deltaY;

	if (p1.x > p2.x) deltaX = p1.x - p2.x;
	else deltaX = p2.x - p1.x;

	if (p1.y > p2.y) deltaY = p1.y - p2.y;
	else deltaY = p2.y - p1.y;

	bool switchFlag = false;

	if (deltaX > deltaY)
	{
		if (p2.x > p1.x)
		{
			x = p1.x;
			y = p1.y;
			max_x = p2.x;

			if (p2.y > p1.y) reflectFlag = 1;
			else reflectFlag = -1;
		}
		else
		{ //(p1.x >= p2.x)
			x = p2.x;
			y = p2.y;
			max_x = p1.x;

			if (p1.y > p2.y) reflectFlag = 1;
			else reflectFlag = -1;
		}
	}
	else
	{
		int tmpNum;

		if (p2.y > p1.y)
		{
			x = p1.y;
			y = p1.x;
			max_x = p2.y;

			if (p2.x > p1.x) reflectFlag = 1;
			else reflectFlag = -1;
		}
		else
		{
			x = p2.y;
			y = p2.x;
			max_x = p1.y;

			if (p1.x > p2.x) reflectFlag = 1;
			else reflectFlag = -1;
		}
		tmpNum = deltaX;
		deltaX = deltaY;
		deltaY = tmpNum;
		switchFlag = true;
	}

	e = 2 * deltaY - deltaX;

	while (max_x >= x)
	{

		if (switchFlag) PutPixel(y, x, color);
		else PutPixel(x, y, color);

		if (e > 0)
		{
			y += reflectFlag;
			e -= 2 * deltaX;
		}
		e += 2 * deltaY;
		x++;
	}



}
//
//void Renderer::CreateBuffers(int w, int h)
//{
//	CreateOpenGLBuffer(); //Do not remove this line.
//	color_buffer_ = new float[3 * w * h];
//	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
//
//	color_temp_buffer = new bool[w * h];
//	//ClearColorTempbuffer();
//
//	z_buffer_ = new float[w * h];
//}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex_);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc_);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc_);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

void Renderer::CreateOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width_, viewport_height_, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width_, viewport_height_);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width_, viewport_height_, GL_RGB, GL_FLOAT, color_buffer_);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc_);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width_ / 2; // screen center
	int half_height = viewport_height_ / 2;//screen center
	//int thickness = 10;
	int numOfLines = 200;
	int radius = 150;
	glm::ivec2 srcp;
	glm::ivec2 dstp;
	srcp.x = half_width + 100;
	srcp.y = half_height + 100;

	//dstp.x = half_width ;
	//dstp.y = half_height ;

	int x0 = half_width;
	int y0 = half_height;
	int r = 200;
	int a = 24;

	int x1;
	int y1;

	float min_x = 10000000;
	float min_y = 1000000;
	float max_x = -1000000;
	float max_y = -10000000;
	float max_z = -1000000;
	float min_z = 10000000;

	//draw lines - working
	//for (int i = 0; i < a; i++) {
	//	x1 = x0 + r * sin((2 * M_PI * i) / a);
	//	y1 = y0 + r * cos((2 * M_PI * i) / a);
	//	DrawLine(glm::ivec2(x0, y0), glm::ivec2(x1, y1), glm::vec3(0, 0, 1));
	//}
	//

	if (scene.GetModelCount() > 0)  // for now were assume there is only one model
	{
		//MeshModel model = scene.GetModel(0);//original line
		MeshModel model = scene.GetModel(scene.GetModelCount() - 1);
		for (int i = 0; i < model.GetFacesCount(); i++) {

			int vertexIndex0 = model.faces_[i].GetVertexIndex(0);
			int vertexIndex1 = model.faces_[i].GetVertexIndex(1);
			int vertexIndex2 = model.faces_[i].GetVertexIndex(2);
			glm::vec3 vec0(model.vertices_[vertexIndex0 - 1]);
			glm::vec3 vec1(model.vertices_[vertexIndex1 - 1]);
			glm::vec3 vec2(model.vertices_[vertexIndex2 - 1]);
			glm::vec4 v0 = glm::vec4(vec0, 1);
			glm::mat4 m = model.local_transform();
			//v0 = m * v0;
			glm::vec4 v1(vec1.x, vec1.y, vec1.z, 1);
			//v1 = m*v1;
			glm::vec4 v2(vec2.x, vec2.y, vec2.z, 1);
			//v2 = m*v2;

			glm::mat4 m2 = scene.world_transform();
			//glm::mat4 m2(1.0f);
			v0 = m2 * m * v0;
			v1 = m2 * m * v1;
			v2 = m2 * m * v2;
			//scene.set_anglex(100);

			if (vec0.x < vec1.x && vec0.x < vec2.x && vec0.x < min_x)
				min_x = vec0.x;
			if (vec1.x < vec0.x && vec1.x < vec2.x && vec1.x < min_x)
				min_x = vec1.x;
			if (vec2.x < vec1.x && vec2.x < vec0.x && vec2.x < min_x)
				min_x = vec2.x;

			if (vec0.y < vec1.y && vec0.y < vec2.y && vec0.y < min_y)
				min_y = vec0.y;
			if (vec1.y < vec0.y && vec1.y < vec2.y && vec1.y < min_y)
				min_y = vec1.y;
			if (vec2.y < vec1.y && vec2.y < vec0.y && vec2.y < min_y)
				min_y = vec2.y;

			if (vec0.x > vec1.x && vec0.x > vec2.x && vec0.x > max_x)
				max_x = vec0.x;
			if (vec1.x > vec0.x && vec1.x > vec2.x && vec1.x > max_x)
				max_x = vec1.x;
			if (vec2.x > vec1.x && vec2.x > vec0.x && vec2.x > max_x)
				max_x = vec2.x;

			if (vec0.y > vec1.y && vec0.y > vec2.y && vec0.y > max_y)
				max_y = vec0.y;
			if (vec1.y > vec0.y && vec1.y > vec2.y && vec1.y > max_y)
				max_y = vec1.y;
			if (vec2.y > vec1.y && vec2.y > vec0.y && vec2.y > max_y)
				max_y = vec2.y;

			if (vec0.z < vec1.z && vec0.z < vec2.z && vec0.z < min_z)
				min_z = vec0.z;
			if (vec1.z < vec0.z && vec1.z < vec2.z && vec1.z < min_z)
				min_z = vec1.z;
			if (vec2.z < vec1.z && vec2.z < vec0.z && vec2.z < min_z)
				min_z = vec2.z;

			if (vec0.z > vec1.z && vec0.z > vec2.z && vec0.z > max_z)
				max_z = vec0.z;
			if (vec1.z > vec0.z && vec1.z > vec2.z && vec1.z > max_z)
				max_z = vec1.z;
			if (vec2.z > vec1.z && vec2.z > vec0.z && vec2.z > max_z)
				max_z = vec2.z;



			/*if (v0.x < v1.x && v0.x < v2.x && v0.x < min_x)
				min_x = v0.x;
			if (v1.x < v0.x && v1.x < v2.x && v1.x < min_x)
				min_x = v1.x;
			if (v2.x < v1.x && v2.x < v0.x && v2.x < min_x)
				min_x = v2.x;

			if (v0.y < v1.y && v0.y < v2.y && v0.y < min_y)
				min_y = v0.y;
			if (v1.y < v0.y && v1.y < v2.y && v1.y < min_y)
				min_y = v1.y;
			if (v2.y < v1.y && v2.y < v0.y && v2.y < min_y)
				min_y = v2.y;

			if (v0.x > v1.x && v0.x > v2.x && v0.x > max_x)
				max_x = v0.x;
			if (v1.x > v0.x && v1.x > v2.x && v1.x > max_x)
				max_x = v1.x;
			if (v2.x > v1.x && v2.x > v0.x && v2.x > max_x)
				max_x = v2.x;

			if (v0.y > v1.y && v0.y > v2.y && v0.y > max_y)
				max_y = v0.y;
			if (v1.y > v0.y && v1.y > v2.y && v1.y > max_y)
				max_y = v1.y;
			if (v2.y > v1.y && v2.y > v0.y && v2.y > max_y)
				max_y = v2.y;

			if (v0.z < v1.z && v0.z < v2.z && v0.z < min_z)
				min_z = v0.z;
			if (v1.z < v0.z && v1.z < v2.z && v1.z < min_z)
				min_z = vec1.z;
			if (v2.z < v1.z && v2.z < v0.z && v2.z < min_z)
				min_z = v2.z;

			if (v0.z > v1.z && v0.z > v2.z && v0.z > max_z)
				max_z = v0.z;
			if (v1.z > v0.z && v1.z > v2.z && v1.z > max_z)
				max_z = v1.z;
			if (v2.z > v1.z && v2.z > v0.z && v2.z > max_z)
				max_z = v2.z;*/


			Model_painting(v0, v1, v2);




			DrawLine(glm::ivec2(v0.x, v0.y), glm::ivec2(v1.x, v1.y), glm::vec3(0, 1, 1));
			DrawLine(glm::ivec2(v0.x, v0.y), glm::ivec2(v2.x, v2.y), glm::vec3(0, 1, 1));
			DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(v2.x, v2.y), glm::vec3(0, 1, 1));
			//bounding_box(scene);
				/*DrawLine(glm::ivec2(model.vertices_[vertexIndex0-1].x *x0 +x0, model.vertices_[vertexIndex0-1].y * y0 +y0), glm::ivec2(model.vertices_[vertexIndex1-1].x *x0+x0, model.vertices_[vertexIndex1-1].y*y0+y0), glm::vec3(0, 1, 1));
				DrawLine(glm::ivec2(model.vertices_[vertexIndex0-1].x *x0 + x0, model.vertices_[vertexIndex0-1].y*y0 +y0), glm::ivec2(model.vertices_[vertexIndex2-1].x*x0 + x0, model.vertices_[vertexIndex2-1].y*y0 +y0), glm::vec3(0, 1, 1));
				DrawLine(glm::ivec2(model.vertices_[vertexIndex1-1].x*x0+x0, model.vertices_[vertexIndex1-1].y*y0+y0), glm::ivec2(model.vertices_[vertexIndex2-1].x*x0+x0, model.vertices_[vertexIndex2-1].y*y0+y0), glm::vec3(0, 1, 1));*/
		}
		glm::mat4 m = model.local_transform();
		glm::mat4 m2 = scene.world_transform();


		glm::vec4 top_left_near(min_x, max_y, max_z, 1);
		glm::vec4 top_left_far(min_x, max_y, min_z, 1);
		glm::vec4 top_right_near(max_x, max_y, max_z, 1);
		glm::vec4 top_right_far(max_x, max_y, min_z, 1);
		glm::vec4 bottom_left_near(min_x, min_y, max_z, 1);
		glm::vec4 bottom_left_far(min_x, min_y, min_z, 1);
		glm::vec4 bottom_right_near(max_x, min_y, max_z, 1);
		glm::vec4 bottom_right_far(max_x, min_y, min_z, 1);

		top_left_near = m2 * m * top_left_near;
		top_left_far = m2 * m * top_left_far;
		top_right_near = m2 * m * top_right_near;
		top_right_far = m2 * m * top_right_far;
		bottom_left_near = m2 * m * bottom_left_near;
		bottom_left_far = m2 * m * bottom_left_far;
		bottom_right_near = m2 * m * bottom_right_near;
		bottom_right_far = m2 * m * bottom_right_far;


		DrawLine(top_left_far, top_right_far, glm::vec3(0, 1, 1));
		DrawLine(top_left_far, top_left_near, glm::vec3(0, 1, 1));
		DrawLine(top_left_near, top_right_near, glm::vec3(0, 1, 1));
		DrawLine(top_left_near, top_right_far, glm::vec3(0, 1, 1));

		DrawLine(bottom_left_far, bottom_right_far, glm::vec3(0, 1, 1));
		DrawLine(bottom_left_far, bottom_left_near, glm::vec3(0, 1, 1));
		DrawLine(bottom_left_near, bottom_right_near, glm::vec3(0, 1, 1));
		DrawLine(bottom_right_near, bottom_right_far, glm::vec3(0, 1, 1));

		DrawLine(bottom_left_far, top_left_far, glm::vec3(0, 1, 1));
		DrawLine(bottom_left_near, top_left_near, glm::vec3(0, 1, 1));


		DrawLine(bottom_right_far, top_right_far, glm::vec3(0, 1, 1));
		DrawLine(bottom_right_near, top_right_near, glm::vec3(0, 1, 1));


	}



	//DrawLine(glm::ivec2(meshinun1, y0), glm::ivec2(x1, y1), glm::vec3(0, 0, 1));
	//
	//DrawLine(srcp, dstp, glm::vec3(1, 0, 1));
	/*for(int i = 0; i < numOfLines ; i++)
		dstp.x = int(srcp.x + radius * sin(((2 * M_PI * i))/numOfLines));
		dstp.y = int(srcp.y + radius * cos(((2 * M_PI * i)) / numOfLines));
		DrawLine(srcp, dstp, glm::vec3(1, 0, 1));
	}*/

	//original code - change at 18.11.20
	/*for(int i = 0; i < viewport_width_; i++)
	{
		for (int j = half_height - thickness; j < half_height + thickness; j++)
		{
			PutPixel(i, j, glm::vec3(1, 1, 0));
		}
	}

	for (int i = 0; i < viewport_height_; i++)
	{
	{
		for (int j = half_width - thickness; j < half_width + thickness; j++)
		{
			PutPixel(j, i, glm::vec3(1, 0, 1));
		}
	}*/

	//lm::ivec2 p1 = vertices_[GetModel(0).GetFace(0).GetVertexIndex(0)];


//std::vector<glm::vec3> aviad = GetVertexes();

//	int num = scene.GetModelCount();
//	if (num>0){
//	MeshModel model = scene.GetActiveModel();
//DrawLine(glm::ivec2(model.GetVertex(0)), glm::ivec2(model.GetVertex(1)), glm::vec3(1, 1, 1));
//}

		//glm::vec2 point1, point2;
	//const Face& face = model.GetFace(face_index);		// getting Face to draw
	//DrawLine(model.GetVertex(face.GetVertexIndex(0)), model.GetVertex(face.GetVertexIndex(1)), color);
}

int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}
//
//void Renderer::bounding_box(const Scene& scene)
//{
//
//	if (scene.GetModelCount() > 0)  // for now were assume there is only one model
//	{
//		//MeshModel model = scene.GetModel(0);//original line
//		float min_x = 10000000;
//		float min_y = 1000000;
//		float max_x = -1000000;
//		float max_y = -10000000;
//		float max_z = -1000000;
//		float min_z = 10000000;
//
//
//		MeshModel model = scene.GetModel(scene.GetModelCount() - 1);
//		for (int i = 0; i < model.GetFacesCount(); i++) {
//
//			int vertexIndex0 = model.faces_[i].GetVertexIndex(0);
//			int vertexIndex1 = model.faces_[i].GetVertexIndex(1);
//			int vertexIndex2 = model.faces_[i].GetVertexIndex(2);
//			glm::vec3 vec0(model.vertices_[vertexIndex0 - 1]);
//			glm::vec3 vec1(model.vertices_[vertexIndex1 - 1]);
//			glm::vec3 vec2(model.vertices_[vertexIndex2 - 1]);
//			if (vec0.x < vec1.x && vec0.x < vec2.x && vec0.x < min_x)
//				min_x = vec0.x;
//			if (vec1.x < vec0.x && vec1.x < vec2.x && vec1.x < min_x)
//				min_x = vec1.x;
//			if (vec2.x < vec1.x && vec2.x < vec0.x && vec2.x < min_x)
//				min_x = vec2.x;
//
//			if (vec0.y < vec1.y && vec0.y < vec2.y && vec0.y < min_y)
//				min_y = vec0.y;
//			if (vec1.y < vec0.y && vec1.y < vec2.y && vec1.y < min_y)
//				min_y = vec1.y;
//			if (vec2.y < vec1.y && vec2.y < vec0.y && vec2.y < min_y)
//				min_y = vec2.y;
//
//			if (vec0.x > vec1.x && vec0.x > vec2.x && vec0.x > max_x)
//				max_x = vec0.x;
//			if (vec1.x > vec0.x && vec1.x > vec2.x && vec1.x > max_x)
//				max_x = vec1.x;
//			if (vec2.x > vec1.x && vec2.x > vec0.x && vec2.x > max_x)
//				max_x = vec2.x;
//
//			if (vec0.y > vec1.y && vec0.y > vec2.y && vec0.y > max_y)
//				max_y = vec0.y;
//			if (vec1.y > vec0.y && vec1.y > vec2.y && vec1.y > max_y)
//				max_y = vec1.y;
//			if (vec2.y > vec1.y && vec2.y > vec0.y && vec2.y > max_y)
//				max_y = vec2.y;
//
//			if (vec0.z < vec1.z && vec0.z < vec2.z && vec0.z < min_z)
//				min_z = vec0.z;
//			if (vec1.z < vec0.z && vec1.z < vec2.z && vec1.z < min_z)
//				min_z = vec1.z;
//			if (vec2.z < vec1.z && vec2.z < vec0.z && vec2.z < min_z)
//				min_z = vec2.z;
//
//			if (vec0.z > vec1.z && vec0.z > vec2.z && vec0.z > max_z)
//				max_z = vec0.z;
//			if (vec1.z > vec0.z && vec1.z > vec2.z && vec1.z > max_z)
//				max_z = vec1.z;
//			if (vec2.z > vec1.z && vec2.z > vec0.z && vec2.z > max_z)
//				max_z = vec2.z;
//
//
//			glm::vec4 top_left_near(min_x, max_y, max_z,1);
//			glm::vec4 top_left_far(min_x, max_y, min_z, 1);
//			glm::vec4 top_right_near(max_x, max_y, max_z, 1);
//			glm::vec4 top_right_far(max_x, max_y, min_z, 1);
//			glm::vec4 bottom_left_near(min_x, min_y, max_z, 1);
//			glm::vec4 bottom_left_far(min_x, min_y, min_z, 1);
//			glm::vec4 bottom_right_near(max_x, min_y, max_z, 1);
//			glm::vec4 bottom_right_far(max_x, min_y, min_z, 1);
//
//
//			/*DrawLine(glm::ivec2(max_x, max_x), glm::ivec2(max_y, max_y), glm::vec3(0, 1, 1));
//			DrawLine(glm::ivec2(min_x, min_x), glm::ivec2(max_y, max_y), glm::vec3(0, 1, 1));
//			DrawLine(glm::ivec2(max_x, max_x), glm::ivec2(min_y, min_y), glm::vec3(0, 1, 1));
//			DrawLine(glm::ivec2(min_x, min_x), glm::ivec2(min_y, min_y), glm::vec3(0, 1, 1));*/
//
//		}
//	}
//}
void Renderer::Model_painting(const glm::vec4& vec0, const glm::vec4& vec1, const glm::vec4& vec2) {

	float min_x = 10000000;
	float min_y = 1000000;
	float max_x = -1000000;
	float max_y = -10000000;
	float max_z = -1000000;
	float min_z = 10000000;

	if (vec0.x < vec1.x && vec0.x < vec2.x && vec0.x < min_x)
		min_x = vec0.x;
	if (vec1.x < vec0.x && vec1.x < vec2.x && vec1.x < min_x)
		min_x = vec1.x;
	if (vec2.x < vec1.x && vec2.x < vec0.x && vec2.x < min_x)
		min_x = vec2.x;

	if (vec0.y < vec1.y && vec0.y < vec2.y && vec0.y < min_y)
		min_y = vec0.y;
	if (vec1.y < vec0.y && vec1.y < vec2.y && vec1.y < min_y)
		min_y = vec1.y;
	if (vec2.y < vec1.y && vec2.y < vec0.y && vec2.y < min_y)
		min_y = vec2.y;

	if (vec0.x > vec1.x && vec0.x > vec2.x && vec0.x > max_x)
		max_x = vec0.x;
	if (vec1.x > vec0.x && vec1.x > vec2.x && vec1.x > max_x)
		max_x = vec1.x;
	if (vec2.x > vec1.x && vec2.x > vec0.x && vec2.x > max_x)
		max_x = vec2.x;

	if (vec0.y > vec1.y && vec0.y > vec2.y && vec0.y > max_y)
		max_y = vec0.y;
	if (vec1.y > vec0.y && vec1.y > vec2.y && vec1.y > max_y)
		max_y = vec1.y;
	if (vec2.y > vec1.y && vec2.y > vec0.y && vec2.y > max_y)
		max_y = vec2.y;

	if (vec0.z < vec1.z && vec0.z < vec2.z && vec0.z < min_z)
		min_z = vec0.z;
	if (vec1.z < vec0.z && vec1.z < vec2.z && vec1.z < min_z)
		min_z = vec1.z;
	if (vec2.z < vec1.z && vec2.z < vec0.z && vec2.z < min_z)
		min_z = vec2.z;

	if (vec0.z > vec1.z && vec0.z > vec2.z && vec0.z > max_z)
		max_z = vec0.z;
	if (vec1.z > vec0.z && vec1.z > vec2.z && vec1.z > max_z)
		max_z = vec1.z;
	if (vec2.z > vec1.z && vec2.z > vec0.z && vec2.z > max_z)
		max_z = vec2.z;

	for (int i = min_x; i <= max_x; i++) {
		for (int j = min_y; j < max_y; j++)
		{
			
			
			//if (Inside(vec0, vec1, vec2, i, j,max_x,max_y)==true)
			//{
				PutPixel(i, j, glm::vec3(1, 1, 1));
			//}
		}
	}

	/*glm::vec4 top_left_near(min_x, max_y, max_z, 1);
	glm::vec4 top_left_far(min_x, max_y, min_z, 1);
	glm::vec4 top_right_near(max_x, max_y, max_z, 1);
	glm::vec4 top_right_far(max_x, max_y, min_z, 1);
	glm::vec4 bottom_left_near(min_x, min_y, max_z, 1);
	glm::vec4 bottom_left_far(min_x, min_y, min_z, 1);
	glm::vec4 bottom_right_near(max_x, min_y, max_z, 1);
	glm::vec4 bottom_right_far(max_x, min_y, min_z, 1);*/

}
bool Renderer:: Inside(const glm::vec4& vec0, const glm::vec4& vec1, const glm::vec4& vec2,int i,int j,float max_x,float max_y) {
	float m1, m2, m3, n1, n2, n3, d1=-2, d2=-2, d3=-2, m_of_Cutting_point, n_of_Cutting_point,x1=100000000, x2 = 100000000, x3 = 100000000;
		;
	m1 = (vec0.y - vec1.y) / (vec0.x - vec1.x);
	m2 = (vec0.y - vec2.y) / (vec0.x - vec2.x);
	m3 = (vec1.y - vec2.y) / (vec1.x - vec2.x);

	n1 = vec0.y - m1 * vec0.x;
	n2= vec0.y - m2 * vec0.x;
	n3 = vec1.y - m3 * vec1.x;

	d1 = abs(-m1 * i + j - n1) / (sqrt(m1 * m1 + 1));
	d2 = abs(-m2 * i + j - n2) / (sqrt(m2 * m2 + 1));
	d3 = abs(-m3 * i + j - n3) / (sqrt(m3 * m3 + 1));
	//	d1 = j- m1 * i -n1;
	//d2 = j-m2 * i -n2;
	//d3 = j- m3 * i - n3;

	/*m_of_Cutting_point = (max_y+1000 - j) / (max_x+1000 - i);
	n_of_Cutting_point = j - m_of_Cutting_point * i;*/
	//x1 = (n_of_Cutting_point - n1) / (m_of_Cutting_point - m1);
	//x2 = (n_of_Cutting_point - n2) / (m_of_Cutting_point - m2);
	//x3 = (n_of_Cutting_point - n3) / (m_of_Cutting_point - m3);
	/*if (m_of_Cutting_point == m1 && m_of_Cutting_point == m2 && m_of_Cutting_point == m3)
		return false;
	return true;*/

	

	if (d1 >= 0 && d2 >= 0 && d3 >= 0) return true;
	else
		return false;



}

void Renderer::clear_z_buffer()
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			z_buffer_[Z_INDEX(viewport_height_, i, j)] = 1.0f;
		}
	}
}
void Renderer::set_z_value_for_pixel(float value, int i, int j)
{
	if (i < 0)return;
	if (i >= viewport_width_)return;
	if (j < 0)return;
	if (j >= viewport_height_)return;
	z_buffer_[Z_INDEX(viewport_width_, i, j)] = value;

}
bool Renderer::check_for_z_value_in_pixel(float search_value, const int i, const int j)
{
	if (i < 0)return false;
	if (i >= viewport_width_)return false;
	if (j < 0)return false;
	if (j >= viewport_height_)return false;
	return (search_value <= z_buffer_[Z_INDEX(viewport_width_, i, j)]);
}