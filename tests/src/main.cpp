#include <iostream>
#include <object.hpp>
#include <transform.hpp>
#include <md_geometry.hpp>
#include <vector>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class TestObj : public mdge::Object {
public:
	void Update() override {
		mdge::Transform *transform = GetComponent<mdge::Transform>();
		std::cout << "Position: " << transform->Position().x() << " " << transform->Position().y() << " "
				  << transform->Position().z() << "\n";
	}
};

int main() {
	mdge::Object *go = new TestObj;
	go->AddComponent<mdge::Transform>(new mdge::Transform);
	go->GetComponent<mdge::Transform>()->Position() = MDGE_TRANSFORM_UP;
	go->Update();
	delete go;

	Assimp::Importer imp;
	const aiScene *scene =
			imp.ReadFile("C:/Users/borbg/Documents/Projects/MDGE/3d_objects/cube.obj", aiProcess_Triangulate);
	if (!scene) {
		std::cerr << "Failed to load file\n";
		return -1;
	}

	for (int i = 0; i < scene->mMeshes[0]->mNumVertices; i++) {
		std::cout << scene->mMeshes[0]->mVertices[i].x << "\t" << scene->mMeshes[0]->mVertices[i].y << "\t"
				  << scene->mMeshes[0]->mVertices[i].z << "\n";
	}

	return 0;
}