#include <iostream>
#include <object.hpp>
#include <transform.hpp>

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
}