#include "MeshModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals)//
{

}


MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}

int MeshModel::GetFacesCount() const
{
	return faces_.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}

//vec
const std::vector<glm::vec3> MeshModel::GetVertexes() const
{

	return vertices_;

}

int MeshModel::GetNormalsCount() const
{
	return normals_.size();
}
//

int MeshModel::GetVerticesount() const
{
	return vertices_.size();
}

const glm::ivec2& MeshModel::GetVertex(int index) const
{
	return vertices_[index];

}

const std::vector<glm::vec3> MeshModel::GetNormals() const
{
	return normals_;
}

//glm::mat4x4 rotateOnX = glm::mat4x4{ 1,0,0,0,
//									0,0,0,0,
//									0,0,0,0,
//									0,0,0,0 };
//
//glm::mat4x4 rotateOnY = glm::mat4x4{ 0,0,0,0,
//									1,0,0,0,
//									0,0,0,0,
//									0,0,0,0 };
//
//glm::mat4x4 rotateOnZ = glm::mat4x4{ 0,0,0,0,
//									0,0,0,0,
//									1,0,0,0,
//									0,0,0,0 };

float angleX =0;
float angleY = 0;
float angleZ = 0;
int scalex = 0;
int scaley = 0;
int scalez = 0;
int translatex = 0;
int translatey = 0;
int translatez = 0;

float MeshModel::Get_anglex()
{
	return  angleX;
}
void MeshModel::set_anglex(float x)
{
	angleX = x;
}

float MeshModel::Get_angley()
{
	return  angleY;
}
void MeshModel::set_angley(float y)
{
	angleY = y;
}

float MeshModel::Get_anglez()
{
	return  angleZ;
}
void MeshModel::set_anglez(float z)
{
	angleZ = z;
}

void MeshModel::set_scalex(int x)
{
	scalex = x;
}

void MeshModel::set_scaley(int y)
{
	scaley = y;
}

void MeshModel::set_scalez(int z)
{
	scalez = z;
}
void MeshModel::set_translatex(int x)
{
	translatex = x;
}

void MeshModel::set_translatey(int y)
{
	translatey = y;
}

void MeshModel::set_translatez(int z)
{
	translatez = z;
}







glm::mat4  MeshModel::rotate_mat() const
{
	glm::mat4 rotateOnX = glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotateOnY = glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotateOnZ = glm::rotate(glm::mat4(1.0f), angleZ, glm::vec3(0.0f, 0.0f, 1.0f));
	 glm::mat4 rotate = rotateOnX * rotateOnY * rotateOnZ;
	 return rotate;
}

glm::mat4  MeshModel::scale_mat() const
{
	glm::mat4 scale = glm::scale(glm::vec3{ scalex, scaley, scalez });

	return scale;
}

glm::mat4  MeshModel::translate_mat() const
{
	glm::mat4 translate = glm::translate(glm::vec3{ translatex, translatey ,translatez });

	return translate;
}

glm::mat4 MeshModel::local_transform() const
{
	glm::mat4 transform = translate_mat() * scale_mat() * rotate_mat()  ;
	return transform;
}




//glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
//glm::mat4 ViewTranslate = glm::translate(
//	glm::mat4(1.0f),
//	glm::vec3(0.0f, 0.0f, -Translate)
//);
//glm::mat4 ViewRotateX = glm::rotate(ViewTranslate,a,glm::vec3(-1.0f, 0.0f, 0.0f)
//trans


//normal
//const Face& MeshModel::GetFace(int index) const
//{
//	return faces_[index];
//}
//
//int MeshModel::GetFacesCount() const
//{
//	return faces_.size();
//}
//
//const std::string& MeshModel::GetModelName() const
//{
//	return model_name_;
//}
//