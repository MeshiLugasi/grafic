#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"

#include <stdio.h>
#include <iostream>
class Scene {
public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	
	void AddCamera(const std::shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;
	void PrintModel() const;


	glm::mat4 rotate_mat() const;
	glm::mat4 scale_mat()const;
	glm::mat4 translate_mat()const;
	glm::mat4 Scene::world_transform() const;
	void Scene::set_anglex(float x);
	
	void Scene::set_angley(float y);
	void Scene::set_anglez(float z);
	float Scene::Get_anglex();
	void Scene::set_scalex(float x);
	void Scene::set_scaley(float y);
	void Scene::set_scalez(float z);
	void Scene::set_translatey(float y);
	void Scene::set_translatex(float x);
	void Scene::set_translatez(float Z);


private:
	std::vector<std::shared_ptr<MeshModel>> mesh_models_;
	std::vector<std::shared_ptr<Camera>> cameras_;

	int active_camera_index_;
	int active_model_index_;

	float anglex;
	float angley;
	float anglez;
	float scaleX;
	float scaleY;
	float scaleZ;
	float translateX;
	float translateY;
	float translateZ;
};