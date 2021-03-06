#include "Converter.h"

#include "Mover.h"
#include "EntityHandle.h"
#include "Camera.h"
#include "BasicRenderSystem.h"
#include "ModelLoader.h"
#include "StaticMeshLoader.h"
#include "ConvexMeshLoader.h"
#include "PlayerInput.h"
#include "RigidBody.h"

#include "PlayerInputSystem.h"
#include "PhysicsSystem.h"

#include <iostream>;
void Mover::init() {
	gameWorld = new World();

	camera = gameWorld->createEntity();
	Camera::createCamera(&camera);
	camera.getComponent<CameraComponent>().viewport = dimensions;

	// Systems
	gameWorld->registerUpdateSystem<PlayerInputSystem>();
	gameWorld->registerUpdateSystem<PhysicsSystem>();
	gameWorld->registerRenderSystem<BasicRenderSystem>(&camera);

	{
		EntityHandle plane = gameWorld->createEntity();

		Transform t = Transform{};
		t.position = glm::vec3(0.0, 0.0, 1.0);
		plane.addComponent(t);

		Model m = ModelLoader::loadModel("models/plane/plane.fbx");
		plane.addComponent(m);

		EntityMotionState* motionState = new EntityMotionState(&plane.getComponent<Transform>());

		btCollisionShape* collider = StaticMeshLoader::loadMeshCollider("models/plane/plane.fbx");
		btRigidBody* bulletRigidBody = new btRigidBody(0.0f, motionState, collider);
		bulletRigidBody->setRestitution(0.2);
		bulletRigidBody->setFriction(0.9);
		RigidBody rigidBody(bulletRigidBody);


		plane.addComponent(rigidBody);
	}

	{
		EntityHandle cube = gameWorld->createEntity();

		Transform t = Transform{};
		t.position = glm::vec3(0.0, 20.0, 1.0);
		cube.addComponent(t);

		Model m = ModelLoader::loadModel("models/cube/cube.fbx");
		cube.addComponent(m);

		EntityMotionState* motionState = new EntityMotionState(&cube.getComponent<Transform>());

		btCollisionShape* collider = ConvexMeshLoader::loadMeshCollider("models/cube/cube.fbx");
		
		btVector3 inertia;
		collider->calculateLocalInertia(5.0f, inertia);

		btRigidBody* bulletRigidBody = new btRigidBody(5.0f, motionState, collider, inertia);
		RigidBody rigidBody(bulletRigidBody);

		cube.addComponent(rigidBody);
	}



	{
		EntityHandle player = gameWorld->createEntity();


		player.addComponent<Transform>(Transform{});

		View view{};
		view.viewCamera = camera;
		player.addComponent<View>(view);

		PlayerInput playerInput{};
		playerInput.input = inputs;
		player.addComponent<PlayerInput>(playerInput);
	}
}


void Mover::update() {
	gameWorld->update();
}

void Mover::render(float interpolation) {
	gameWorld->render();
}