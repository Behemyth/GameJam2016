//put global includes in 'BasicIncludes'

#include "BasicIncludes.h"
#include "rand.h"
#include "Camera.h"
#include "Input.h"
#include "Object.h"
#include "navMesh.h"
#include "Landscape.h"
#include "Character.h"
#include "Skybox.h"

//Function List
void Update(double);
void GetPositions();
void DecrementTimers();
void Draw();
void CameraInput();
void MouseInput();
void InitializeWindow();
void Terminate();
void Run();

//Variables
GLFWwindow* mainThread;
glm::uvec2 SCREEN_SIZE;
Camera camera = Camera();
glm::vec2 mouseChangeDegrees;
double deltaTime;
double physicsTimer;
bool runPhysics;
double timeMod;
int seed;
bool wireframeToggle;
double wireframeTimer;
std::vector<Object*> objects;

irrklang::ISoundEngine* engine;
irrklang::ISound* music;

Character* playa=NULL;

void Terminate() {
	glfwTerminate();

	exit(0);
}

void TogglePhysics(){
	if (physicsTimer <= 0){
		runPhysics = !runPhysics;
		physicsTimer = 0.35f;
	}
}
void ToggleWireFrame(){
	if (wireframeTimer <= 0){

		if (wireframeToggle){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (!wireframeToggle){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		}

		wireframeToggle = !wireframeToggle;
		wireframeTimer = 0.35f;
	}
}
void InitializeWindow() {

	engine = irrklang::createIrrKlangDevice();
	engine->setSoundVolume(1.0f);
	music = engine->play3D("Looking Shady.mp3",
		irrklang::vec3df(0, 0, 0), true, false, true);
	if (music){

		music->setMinDistance(1.0f*KILOMETER);
		music->setPosition(irrklang::vec3df(0, 0, 0));
		music->setVolume(0.35f);
	}


	wireframeToggle = false;
	seed = time(NULL);
	runPhysics = false;
	physicsTimer = 0;
	wireframeTimer = 0;

	if (!glfwInit()) {
		Terminate();
	}

	//set screen size
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SCREEN_SIZE = glm::uvec2(mode->width, mode->height);
	//SCREEN_SIZE = glm::uvec2(1280, 720);

	//basic aa done for us ;D
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);


	//can change the screen setting later


	//if (window == FULLSCREEN) {

	//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//	mainThread = glfwCreateWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "Space Trip 2: Superstitious Trip", glfwGetPrimaryMonitor(), NULL);

	//}
	//else if (window == WINDOWED) {
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		mainThread = glfwCreateWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "Space Trip 2: Superstitious Trip", glfwGetPrimaryMonitor(), NULL);
	//glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//mainThread = glfwCreateWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "Space Trip 2: Superstitious Trip", NULL, NULL);

	//}
	//else if (BORDERLESS) {

	//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	//	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	//	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	//	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	//	mainThread = glfwCreateWindow(mode->width, mode->height, "LifeSim", glfwGetPrimaryMonitor(), NULL);

	//}


	if (!mainThread) {
		glfwTerminate();
		throw std::runtime_error("GLFW window failed");
	}

	glfwMakeContextCurrent(mainThread);

	// initialise GLEW
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("glewInit failed");
	}

	glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/

	glfwSetInputMode(mainThread, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(mainThread, SCREEN_SIZE.x / 2.0, SCREEN_SIZE.y / 2.0);

	//Discard all the errors
	while (glGetError() != GL_NO_ERROR) {}

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthMask(GL_TRUE);  // turn on
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mouseChangeDegrees = glm::vec2(0);

	// setup camera 
	camera.setViewportAspectRatio(SCREEN_SIZE.x / (float)SCREEN_SIZE.y);

	camera.setPosition(glm::vec3(-METER*2.0f, METER*2.0f, METER*2.0f));
	camera.offsetOrientation(45.0f, 45.0f);

	//unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
	//threads = new ThreadPool(concurentThreadsSupported);


	//for keyboard controls
	glfwSetKeyCallback(mainThread, InputKeyboardCallback);
	SetInputWindow(mainThread);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	timeMod *= (1.0f + (yoffset / 15.0));
	if (timeMod<0.005){
		timeMod = 0.005;
	}
}

void Run() {


	SetKey(GLFW_KEY_ESCAPE, std::bind(&Terminate));
	//SetKey(GLFW_KEY_SPACE, std::bind(&TogglePhysics));
	//SetKey(GLFW_KEY_E, std::bind(&ToggleWireFrame));

	deltaTime = 1.0 / 60.0;
	InitializeWindow();

	glfwSetScrollCallback(mainThread, ScrollCallback);

	Skybox* hand = new Skybox(&camera,-2.0f*METER,"skybox.png");
	Object* handP = hand;
	objects.push_back(handP);

	Skybox* filter = new Skybox(&camera, 0.4f*METER, "red.png");
	Object* filterP = filter;
	objects.push_back(filterP);

	filterP->isGhost = true;

	NavMesh* navM = new NavMesh("Level1.obj");
	Object* nObj = navM;
	objects.push_back(nObj);

	Landscape* landM = new Landscape("Level1Real.obj");
	Object* landMP = landM;
	objects.push_back(landMP);

	Character* mainC = new Character(5, 4, 4, "MainSheet.png", false, navM, 1.5f, NULL, NULL, "finalsound.wav", engine);
	Object* mainCP = mainC;
	objects.push_back(mainCP);

	Character* carM = new Character(5, 4, 4, "CatSheet.png", true, navM, 1.0f, mainC, "sound1back.wav", "sound1.wav", engine);
	Object* nCar = carM;
	objects.push_back(nCar);

	Character* carM1 = new Character(5, 4, 4, "SaltSheet.png", true, navM, 1.0f, mainC, "sound2back.wav", "sound2.wav", engine);
	Object* nCar1 = carM1;
	objects.push_back(nCar1);

	Character* carM2 = new Character(5, 4, 4, "MirrorSheet.png", true, navM, 1.0f, mainC, "sound3back.wav", "sound3.wav", engine);
	Object* nCar2 = carM2;
	objects.push_back(nCar2);

	Character* carM3 = new Character(5, 4, 4, "CrowSheet.png", true, navM, 1.0f, mainC, "sound4back.wav", "sound4.wav", engine);
	Object* nCar3 = carM3;
	objects.push_back(nCar3);


	Character* carM4 = new Character(5, 4, 4, "MirrorSheet2.png", true, navM, 1.0f, mainC, "sevendaysr.mp3", "sevendays.mp3", engine);
	Object* nCar4 = carM4;
	objects.push_back(nCar4);

	Character* carM5 = new Character(5, 4, 4, "MirrorSheet3.png", true, navM, 1.0f, mainC, "deathr.mp3", "sound6.mp3", engine);
	Object* nCar5 = carM5;
	objects.push_back(nCar5);

	Character* carM6 = new Character(5, 4, 4, "CatSheet2.png", true, navM, 1.0f, mainC, "sound5back.wav", "sound5.wav", engine);
	Object* nCar6 = carM6;
	objects.push_back(nCar6);

	Character* carM7 = new Character(5, 4, 4, "CrowSheet2.png", true, navM, 1.0f, mainC, "urgleback.wav", "urgleburgle.wav", engine);
	Object* nCar7 = carM7;
	objects.push_back(nCar7);

	Character* carM8 = new Character(5, 4, 4, "SaltSheet2.png", true, navM, 1.0f, mainC, "sound6back.wav", "sound6.wav", engine);
	Object* nCar8 = carM8;
	objects.push_back(nCar8);

	bool flop = false;

	playa = mainC;
	//timer info for loop
	double t = 0.0f;
	double currentTime = glfwGetTime();
	double accumulator = 0.0f;

	glfwPollEvents();		//stop loop when glfw exit is called
	glfwSetCursorPos(mainThread, SCREEN_SIZE.x / 2.0f, SCREEN_SIZE.y / 2.0f);


	//THIS IS TO SPEED UP TIME
	timeMod = 1.0f;


	while (!glfwWindowShouldClose(mainThread)) {
		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;
		//std::cout << "FPS:: " <<1.0f / frameTime << std::endl;

		//setting up timers
		if (frameTime > 0.25) {
			frameTime = 0.25;
		}
		currentTime = newTime;
		accumulator += frameTime;

		//# of updates based on accumulated time

		while (accumulator >= deltaTime) {

			MouseInput();//update mouse change
			glfwPollEvents(); //executes all set input callbacks

			if (mainC->amount<9 && !engine->isCurrentlyPlaying(carM->bSound) && !engine->isCurrentlyPlaying(carM->fSound) &&
				!engine->isCurrentlyPlaying(carM1->bSound) && !engine->isCurrentlyPlaying(carM1->fSound) &&
				!engine->isCurrentlyPlaying(carM2->bSound) && !engine->isCurrentlyPlaying(carM2->fSound) &&
				!engine->isCurrentlyPlaying(carM3->bSound) && !engine->isCurrentlyPlaying(carM3->fSound) &&
				!engine->isCurrentlyPlaying(carM4->bSound) && !engine->isCurrentlyPlaying(carM4->fSound) &&
				!engine->isCurrentlyPlaying(carM5->bSound) && !engine->isCurrentlyPlaying(carM5->fSound) &&
				!engine->isCurrentlyPlaying(carM6->bSound) && !engine->isCurrentlyPlaying(carM6->fSound) &&
				!engine->isCurrentlyPlaying(carM7->bSound) && !engine->isCurrentlyPlaying(carM7->fSound) &&
				!engine->isCurrentlyPlaying(carM8->bSound) && !engine->isCurrentlyPlaying(carM8->fSound)){
				music->setIsPaused(false);
				filterP->isGhost = true;


				CameraInput(); //bypasses input system for direct camera manipulation
			}

			camera.ExtractPosition(mainCP->GetPosition());

			if (!(!engine->isCurrentlyPlaying(carM->bSound) && !engine->isCurrentlyPlaying(carM->fSound) &&
				!engine->isCurrentlyPlaying(carM1->bSound) && !engine->isCurrentlyPlaying(carM1->fSound) &&
				!engine->isCurrentlyPlaying(carM2->bSound) && !engine->isCurrentlyPlaying(carM2->fSound) &&
				!engine->isCurrentlyPlaying(carM3->bSound) && !engine->isCurrentlyPlaying(carM3->fSound) &&
				!engine->isCurrentlyPlaying(carM4->bSound) && !engine->isCurrentlyPlaying(carM4->fSound) &&
				!engine->isCurrentlyPlaying(carM5->bSound) && !engine->isCurrentlyPlaying(carM5->fSound) &&
				!engine->isCurrentlyPlaying(carM6->bSound) && !engine->isCurrentlyPlaying(carM6->fSound) &&
				!engine->isCurrentlyPlaying(carM7->bSound) && !engine->isCurrentlyPlaying(carM7->fSound) &&
				!engine->isCurrentlyPlaying(carM8->bSound) && !engine->isCurrentlyPlaying(carM8->fSound))){
				music->setIsPaused(true);
				glm::vec3 tempRight = camera.right();
				tempRight.y = 0.0f;
				tempRight = glm::normalize(tempRight);

				if (flop){
					camera.ExtractPosition(mainCP->GetPosition() + tempRight*METER*0.01f);
					flop = !flop;
				}
				else{
					camera.ExtractPosition(mainCP->GetPosition()  -tempRight*METER*0.01f);
					flop = !flop;
				}
				if (!mainC->forward){
					filterP->isGhost = false;
				}
			}


			if (!engine->isCurrentlyPlaying(carM->bSound) && !engine->isCurrentlyPlaying(carM->fSound) &&
				!engine->isCurrentlyPlaying(carM1->bSound) && !engine->isCurrentlyPlaying(carM1->fSound) &&
				!engine->isCurrentlyPlaying(carM2->bSound) && !engine->isCurrentlyPlaying(carM2->fSound) &&
				!engine->isCurrentlyPlaying(carM3->bSound) && !engine->isCurrentlyPlaying(carM3->fSound) &&
				!engine->isCurrentlyPlaying(carM4->bSound) && !engine->isCurrentlyPlaying(carM4->fSound) &&
				!engine->isCurrentlyPlaying(carM5->bSound) && !engine->isCurrentlyPlaying(carM5->fSound) &&
				!engine->isCurrentlyPlaying(carM6->bSound) && !engine->isCurrentlyPlaying(carM6->fSound) &&
				!engine->isCurrentlyPlaying(carM7->bSound) && !engine->isCurrentlyPlaying(carM7->fSound) &&
				!engine->isCurrentlyPlaying(carM8->bSound) && !engine->isCurrentlyPlaying(carM8->fSound)){
				music->setIsPaused(false);
				filterP->isGhost = true;

				Update(deltaTime*timeMod); //updates all objects based on the constant deltaTime.
			}
			else{
				music->setIsPaused(true);
			}
			GetPositions(); //transforms bullet matrices to opengl

			DecrementTimers();

			t += deltaTime;
			accumulator -= deltaTime;
		}


		//draw
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (mainC->finally){
		glfwSwapBuffers(mainThread);
		//final sound

		while (engine->isCurrentlyPlaying(mainC->fSound)){
				glfwPollEvents(); //executes all set input callbacks
		}
		Terminate();
		}

		Draw();
		glfwSwapBuffers(mainThread);
	}



}

void DecrementTimers(){
	if (physicsTimer>0){
		physicsTimer = physicsTimer - deltaTime;
		wireframeTimer = wireframeTimer - deltaTime;
	}
	if (wireframeTimer>0){
		wireframeTimer = wireframeTimer - deltaTime;
	}
}
void MouseInput() {
	double xPos;
	double yPos;
	glfwGetCursorPos(mainThread, &xPos, &yPos);
	xPos -= (SCREEN_SIZE.x / 2.0);
	yPos -= (SCREEN_SIZE.y / 2.0);

	mouseChangeDegrees.x = (float)(xPos / SCREEN_SIZE.x *camera.fieldOfView().x);
	mouseChangeDegrees.y = (float)(yPos / SCREEN_SIZE.y *camera.fieldOfView().y);

	/*std::cout << "Change in x (mouse): " << mouseChangeDegrees.x << std::endl;
	std::cout << "Change in y (mouse): " << mouseChangeDegrees.y << std::endl;*/

	//camera.offsetOrientation(mouseChangeDegrees.x, mouseChangeDegrees.y);

	glfwSetCursorPos(mainThread, SCREEN_SIZE.x / 2.0f, SCREEN_SIZE.y / 2.0f);
}
void CameraInput() {
	if (playa != NULL){
		float moveSpeed;
		if (glfwGetKey(mainThread, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			moveSpeed = 0.5 * METER * deltaTime;
		}
		else if (glfwGetKey(mainThread, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
			moveSpeed = 0.1 * METER * deltaTime;
		}
		else {
			moveSpeed = 0.3 * METER * deltaTime;
		}

		glm::vec3 tempFor = camera.forward();
		tempFor.y = 0.0f;
		tempFor = glm::normalize(tempFor);

		glm::vec3 tempRight = camera.right();
		tempRight.y = 0.0f;
		tempRight = glm::normalize(tempRight);
		playa->normalizedDirection = glm::vec3(0);


		if (glfwGetKey(mainThread, GLFW_KEY_S) == GLFW_PRESS) {
			
			playa->positionXYZ += -tempFor*moveSpeed;
			if (playa->nm->inMesh(playa->positionXYZ)){
				playa->normalizedDirection += -tempFor*moveSpeed;
			}
			else{
				playa->positionXYZ += tempFor*moveSpeed;
			}
			//camera.offsetPosition(float(moveSpeed) * -camera.forward());
		}
		else if (glfwGetKey(mainThread, GLFW_KEY_W) == GLFW_PRESS) {
			playa->positionXYZ += tempFor*moveSpeed;
			if (playa->nm->inMesh(playa->positionXYZ)){
				playa->normalizedDirection += tempFor*moveSpeed;
			}
			else{
				playa->positionXYZ += -tempFor*moveSpeed;
			}
			//camera.offsetPosition(float(moveSpeed) * camera.forward());
		}
		if (glfwGetKey(mainThread, GLFW_KEY_A) == GLFW_PRESS) {
			playa->positionXYZ += -tempRight*moveSpeed;
			if (playa->nm->inMesh(playa->positionXYZ)){
				playa->normalizedDirection += -tempRight*moveSpeed;
			}
			else{
				playa->positionXYZ += tempRight*moveSpeed;
			}
			//camera.offsetPosition(float(moveSpeed) * -camera.right());
		}
		else if (glfwGetKey(mainThread, GLFW_KEY_D) == GLFW_PRESS) {
			playa->positionXYZ += tempRight*moveSpeed;
			if (playa->nm->inMesh(playa->positionXYZ)){
				playa->normalizedDirection += tempRight*moveSpeed;
			}
			else{
				playa->positionXYZ += -tempRight*moveSpeed;
			}
			//camera.offsetPosition(float(moveSpeed) * camera.right());
		}
		if (glfwGetKey(mainThread, GLFW_KEY_Z) == GLFW_PRESS) {
			//camera.offsetPosition(float(moveSpeed) * -glm::vec3(0, 1, 0));
		}
		else if (glfwGetKey(mainThread, GLFW_KEY_X) == GLFW_PRESS) {
			//camera.offsetPosition(float(moveSpeed) * glm::vec3(0, 1, 0));
		}
	}
}
void GetPositions(){
	for (int i = 0; i < objects.size(); i++){
		objects[i]->UpdatePosition();
	}
}
void Update(double dt) {
	for (int i = 0; i < objects.size(); i++){
		objects[i]->Update(dt);
	}
}
void Draw() {
	for (int i = 0; i < objects.size(); i++){
		objects[i]->Draw(camera);
	}
}
int main(){

	Run();

	Terminate();

	return 0;
}
