#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "Renderer.h"
#include <iostream>

Scene::Scene() :
	active_camera_index_(0),
	active_model_index_(0)
{

}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models_.push_back(mesh_model);
	PrintModel();
}


void Scene::PrintModel() const
{
	if (mesh_models_.size() > 0)
	{
		for(int i = 0; i< GetModel(GetModelCount() - 1).GetVerticesount(); i++)
		{
			std::cout << "v " << GetModel(GetModelCount() - 1).GetVertexes().at(i).x << " " <<
				GetModel(GetModelCount() - 1).GetVertexes().at(i).y << " " <<
				GetModel(GetModelCount() - 1).GetVertexes().at(i).z << std::endl;
		}
		
		std::cout << "# " << GetModel(GetModelCount() - 1).GetVerticesount() << " verticies" << std::endl;

		for(int i = 0; i < GetModel(GetModelCount() - 1).GetNormalsCount(); i++)
		{
			std::cout << "vn " << GetModel(GetModelCount() - 1).GetNormals().at(i).x << " " <<
				GetModel(GetModelCount() - 1).GetNormals().at(i).y << " " <<
				GetModel(GetModelCount() - 1).GetNormals().at(i).z << std::endl;
		}

		std::cout << "# " << GetModel(GetModelCount() - 1).GetNormalsCount() << " normals" << std::endl;
		
		for(int i =0; i< GetModel(GetModelCount() - 1).GetFacesCount() ;i++)
		{
			/*std::cout << "f" << std::endl;
			std::cout << GetModel(0).GetFace(i).GetVertexIndex(0) << std::endl;
			std::cout << GetModel(0).GetFace(i).GetVertexIndex(1) << std::endl;
			std::cout << GetModel(0).GetFace(i).GetVertexIndex(2) << std::endl;
			std::cout<< "\n";*/

			//std:: cout << "f  " <<" "<< GetModel(0).GetFace(i).GetVertexIndex(0) <<" "<< GetModel(0).GetFace(i).GetVertexIndex(1) << " "<< GetModel(0).GetFace(i).GetVertexIndex(2) << std::endl;
			std:: cout << "f  " << GetModel(GetModelCount() - 1).GetFace(i).GetVertexIndex(0) << "//" << GetModel(GetModelCount() - 1).GetFace(i).GetNormalIndex(0) << " " <<
				GetModel(GetModelCount() - 1).GetFace(i).GetVertexIndex(1) << "//" << GetModel(GetModelCount() - 1).GetFace(i).GetNormalIndex(1) << " " <<
				GetModel(GetModelCount() - 1).GetFace(i).GetVertexIndex(2) << "//" << GetModel(GetModelCount() - 1).GetFace(i).GetNormalIndex(2) << std::endl;
		}

		std::cout << "# " << GetModel(GetModelCount() - 1).GetFacesCount()  <<" faces" << std::endl;
	}
	

	//std::cout<<GetModel(0).GetFace(0).GetVertexIndex(1)<<std::endl;
	//std::cout << GetModel(0).GetFace(3).GetVertexIndex(1) << std::endl;
	//std::cout << GetModel(0).GetFace(3).GetNormalIndex(1) << std::endl;
	//std::cout << GetModel(0).GetNormals().at(0).x << std::endl;
	//std::cout << GetModel(0).GetVertexes().at(0).x << std::endl;
	//glm::ivec2 meshinum1 = GetModel(0).GetVertexes().at(0);
	//glm::ivec2 meshinum2 = GetModel(0).GetVertexes().at(1);
	
}


int Scene::GetModelCount() const
{
	return mesh_models_.size();
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models_[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models_[active_model_index_];
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       


void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras_.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras_.size();
}

Camera& Scene::GetCamera(int index)
{
	return *cameras_[index];
}

Camera& Scene::GetActiveCamera()
{
	return *cameras_[active_camera_index_];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index_ = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index_;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index_ = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index_;
}

//float anglex = 0;
//float angley = 0;
//float anglez = 0;
//float scaleX = 0;
//float scaleY = 0;
//float scaleZ = 0;
//float translateX = 0;
//float translateY = 0;
//float translateZ = 0;


float Scene::Get_anglex()
{
	return  anglex;
}
void Scene::set_anglex(float x)
{
	anglex = x;
}
void Scene::set_angley(float y)
{
	angley = y;
}
void Scene::set_anglez(float z)
{
	anglez = z;
}
void Scene::set_scalex(float x)
{
	scaleX = x;
}

void Scene::set_scaley(float y)
{
	scaleY = y;
}

void Scene::set_scalez(float z)
{
	scaleZ = z;
}
void Scene::set_translatex(float x)
{
	translateX = x;
}
void Scene::set_translatey(float y)
{
	translateY = y;
}
void Scene::set_translatez(float Z)
{
	translateZ = Z;
}







glm::mat4  Scene::rotate_mat() const
{
	glm::mat4 rotateOnX = glm::rotate(glm::mat4(1.0f), anglex, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotateOnY = glm::rotate(glm::mat4(1.0f), angley, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotateOnZ = glm::rotate(glm::mat4(1.0f), anglez, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 rotate = rotateOnX * rotateOnY * rotateOnZ;
	return rotate;
}

glm::mat4  Scene::scale_mat() const
{
	glm::mat4 scale = glm::scale(glm::vec3{ scaleX,scaleY,scaleZ });

	return scale;
}

glm::mat4  Scene::translate_mat() const
{
	glm::mat4 translate = glm::translate(glm::vec3{ translateX,translateY,translateZ });

	return translate;
}

glm::mat4 Scene::world_transform() const
{
	glm::mat4 transform = translate_mat() * scale_mat() * rotate_mat();
	return transform;
}


