#pragma once
#include <string>
#include "Face.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	 glm::mat4 rotate_mat() const;
	 glm::mat4 scale_mat()const;
	 glm::mat4 translate_mat()const;
	 glm::mat4 MeshModel::local_transform() const;
	 void MeshModel::set_anglex(float x);
	 void MeshModel::set_angley(float y);
	 void MeshModel::set_anglez(float z);
	 float MeshModel::Get_anglex();
	 float MeshModel::Get_angley();
	 float MeshModel::Get_anglez();
	 void MeshModel::set_scalex(int x);
	 void MeshModel::set_scaley(int y);
	 void MeshModel::set_scalez(int z);
	 void MeshModel::set_translatey(int y);
	 void MeshModel::set_translatex(int x);
	 void MeshModel::set_translatez(int z);
	 



	//
	//int MeshModel::GetVertices_Count() const;


	int MeshModel::GetVerticesount() const;//
	int MeshModel::GetNormalsCount() const;
	const glm::ivec2& GetVertex(int index) const;
	const std::vector<glm::vec3> GetVertexes() const;
	const std::vector<glm::vec3> GetNormals() const;
	//
public:
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;
	float angleX;
	float angleY;
	float angleZ;


};
