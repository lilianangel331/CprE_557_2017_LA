//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//
//	Adapted by Lilian Angel for CPRE 557 Final Project

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "Texture.h"
#include "GLSphere.h"
#include "Sphere3D.h"

using namespace std;


// The handle to the window object
GLFWwindow*  window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;

// this is a helper variable to allow us to change the texture blend model
extern int g_change_texture_blend;

int bump_map = 0;
GLAppearance* apperance_0;

GLObjectObj* loadedModel1 = NULL; // the basic / normal model
GLObjectObj* loadedModel2 = NULL; // the morphed model

//GLSphere* environment = NULL;

GLfloat* vector_diff = 0;
GLfloat* vector_result = 0;

// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 87 && action == GLFW_PRESS) // key w
    {
        //cout <<  "key w pressed" << endl;
        glUseProgram(apperance_0->getProgram());
        bump_map = (bump_map+1)%2;
        
        int loc = glGetUniformLocation(apperance_0->getProgram(), "bump_mode");
        glUniform1i(loc, bump_map);
        glUseProgram(0);
    }
}


float getTimeFraction(const float time, const float duration)
{
	// we cast to an int. this results in the number of
	float interval = floor(time / duration);

	// return the current interval time
	float current_interval = time - interval*duration;

	// return the fraction / position in our current timeline
	float fraction = current_interval / duration;

	return fraction;
}


void init_morphing(void)
{
	// allocate memory for the difference vector.
	vector_diff = (GLfloat*)malloc(sizeof(GLfloat) * 3 * loadedModel1->size());

	int size = loadedModel1->size();
	// calculate the difference vector
	for (int i = 0; i<size; i++) {
		vector_diff[i * 3 + 0] = loadedModel1->getVertices()[i].x - loadedModel2->getVertices()[i].x;
		vector_diff[i * 3 + 1] = loadedModel1->getVertices()[i].y - loadedModel2->getVertices()[i].y;
		vector_diff[i * 3 + 2] = loadedModel1->getVertices()[i].z - loadedModel2->getVertices()[i].z;
	}
}

void my_morphing_callback(float duration)
{
	// get the current system time
	float time = glfwGetTime();

	// calcualte the time fraction
	float fraction = getTimeFraction(time, duration);

	// get the number of vertices.
	int size = loadedModel1->size();

	// init the vector the first time we call this function.
	if (vector_result == NULL)
	{
		vector_result = (GLfloat*)malloc(sizeof(GLfloat) * 3 * loadedModel1->size());
	}

	for (int i = 0; i<size; i++) {
		vector_result[i * 3 + 0] = loadedModel1->getVertices()[i].x + fraction * vector_diff[i * 3 + 0];
		vector_result[i * 3 + 1] = loadedModel1->getVertices()[i].y + fraction * vector_diff[i * 3 + 1];
		vector_result[i * 3 + 2] = loadedModel1->getVertices()[i].z + fraction * vector_diff[i * 3 + 2];
	}
	//---------------------------
	// Draw the output
	loadedModel1->updateVertices(vector_result);
}

int main(int argc, const char * argv[])
{
    // Init the GLFW Window
    window = initWindow();
     
    // Init the glew api
    initGlew();
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create some models
    
    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);
    
    
    // create an apperance object.
    apperance_0 = new GLAppearance("displacement_texture.vs", "displacement_texture.fs");
    
    GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(00.0,5.0,20.0, 0.0);
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 7.5;
    light_source._diffuse_intensity = 1.0;
    light_source._attenuation_coeff = 0.0;
    
    // add the light to this apperance object
    apperance_0->addLightSource(light_source);
    
    // Create a material object
    GLMaterial material_0;
    material_0._diffuse_material = glm::vec3(0.8, 0.8, 0.8);
    material_0._ambient_material = glm::vec3(0.8, 0.8, 0.8);
    //material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material_0._shininess = 5.0;
    material_0._transparency = 1.0;
    
    // Add the material to the apperance object
    apperance_0->setMaterial(material_0);
    
    
    //************************************************************************************************
    // Add a texture
    GLMultiTexture* texture = new GLMultiTexture();
    int texid = texture->loadAndCreateTextures("butterflyWing.bmp", "NormalMap.bmp","specMap.bmp");
    apperance_0->setTexture(texture);
    
    //************************************************************************************************
    // Finalize the appearance object
    apperance_0->finalize();
   
	loadedModel1 =  new GLObjectObj("butterfly3.obj");
	loadedModel1->setApperance(*apperance_0);
	loadedModel1->init();

	glm::mat4 tranform = glm::scale(glm::vec3(10.0, 10.0f, 10.0f));
	loadedModel1->setMatrix(tranform);

	loadedModel2 = new GLObjectObj("butterfly4.obj");
	loadedModel2->setApperance(*apperance_0);
	loadedModel2->init();
	loadedModel2->setMatrix(tranform);


	// Environment
	// create an apperance object.
	GLAppearance* apperance1 = new GLAppearance("spherical_mapping.vs", "spherical_mapping.fs");


	// The spotlight object
	GLPointLightSource  light_source2;
	light_source2._lightPos = glm::vec4(20.0, 20.0, 0.0, 1.0);
	light_source2._ambient_intensity = 0.05;
	light_source2._specular_intensity = 0.0;
	light_source2._diffuse_intensity = 1.0;
	light_source2._attenuation_coeff = 0.01;

	// add the spot light to this apperance object
	apperance1->addLightSource(light_source2);

	// Create a material object
	GLMaterial material2;
	material2._diffuse_material = glm::vec3(0.0, 0.0, 1.0);
	material2._ambient_material = glm::vec3(0.0, 0.0, 1.0);
	material2._specular_material = glm::vec3(0.0, 0.0, 0.0);
	material2._shininess = 50.0;

	// Add the material to the apperance object
	apperance1->setMaterial(material2);

	// Add a texture
	GLTexture* texture2 = new GLTexture();
	texture2->loadAndCreateTexture("reflection_map.bmp");
	apperance1->setTexture(texture2);

	apperance1->finalize();

	// create the sphere geometry
	GLSphere3D* environment = new GLSphere3D(0.0, 0.0, 0.0, 1000, 50, 50);
	environment->setApperance(*apperance1);
	environment->init();
	glUseProgram(apperance1->getProgram());

    // If you want to change appearance parameters after you init the object, call the update function
    apperance_0->updateLightSources();
    
    
    // enable the program
    //glUseProgram(apperance_0->getProgram());
    int location = glGetUniformLocation(apperance_0->getProgram(), "texture_delta");
    glUniform1f(location, 0.00f);
    glUniform1f(glGetUniformLocation(apperance_0->getProgram(), "bump_mode"), 0);
    
	init_morphing();
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector.
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 65.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
   
    // Set the keyboard callback so that when we press ESC, it knows what to do.
    glfwSetKeyCallback(window, keyboard_callback);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
		my_morphing_callback(3.0);



        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(trackball.getRotationMatrix());
        
        // draw the objects
        cs->draw();
		loadedModel1->draw();
		//glm::mat4 matrix = glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		//environment->setModelMatrix(matrix);
		environment->draw();
        
        //glUseProgram(apperance_0->getProgram());
        float delta = 0.05f;
        glUniform1f(location, delta);
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();      
    }    
    delete cs;
}