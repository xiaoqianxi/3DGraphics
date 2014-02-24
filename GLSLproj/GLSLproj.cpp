// GLSLproj.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 
 
#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#include "Camera.h"
#include "SFML/Graphics/Shader.hpp"
#include "vec3f.h"
#include "imageloader.h"
////////////////////////////////////////////////////////////
///Entrypoint of application 
//////////////////////////////////////////////////////////// 


int main() 
{ 
    // Create the main window 
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL"); 
   
    // Create a clock for measuring time elapsed     
    sf::Clock Clock; 
      
	Camera camera;
    camera.Init();

    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.3f, 0.3f, 0.3f, 0.f); //background colour
    glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); 
	
    //Add ambient light
    GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color(0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	 //Add positioned light
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {100.0f, 0.0f, 100.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    //// Setup a perspective projection & Camera position 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    
    //set up a 3D Perspective View volume
    //gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar 
 
    //set up a  orthographic projection same size as window
    //this mease the vertex coordinates are in pixel space
    glOrtho(0,800,0,600,0,1); // use pixel coordinates
   
 
       
	//load first texture
    sf::Image img;
   
    if (!img.loadFromFile("Grass.bmp"))
    {
        return 1;
         
    }
	
	
	bool isWireframe = false;
    // Start game loop 
    while (App.isOpen()) 
    { 
        // Process events 
        sf::Event Event; 
        while (App.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                App.close(); 
   
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                App.close(); 

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::B)){
				if(!isWireframe){
                    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					isWireframe = true;
				}
				else{
					glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
					isWireframe = false;
				}
            }

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::D)){
                camera.MoveLeftRight(1);
            }
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A)){
                camera.MoveLeftRight(-1);
            }
 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::W)){
                camera.MoveForwardBack(1);
            }
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::S)){
                camera.MoveForwardBack(-1);
            }
 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Right)){
                camera.TurnRightLeft(1);
            }
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Left)){
                camera.TurnRightLeft(-1);
            }
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up)){
                camera.TurnUpDown(1);
            }
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Down)){
                camera.TurnUpDown(-1);
 
            }
             
    
        } 
           
        //Prepare for drawing 
        // Clear color and depth buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   
        // Apply some transformations 
        glMatrixMode(GL_MODELVIEW); 
        glLoadIdentity(); 
		//camera.ViewingTransform();
		//gluLookAt(0,0,10,
		//	      0,0,0,
		//		  0,0,1);
        double angle=Clock.getElapsedTime().asMilliseconds();
        //glTranslated(+400,+300,0); //shift to original position
        //glRotated(angle/10, 0, 0, 1); // rotate
        //glTranslated(-400,-300,0);// shift centre to origin

        GLuint texture;
        glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D,texture);
		glTexImage2D(
            GL_TEXTURE_2D, 
            0,  //mip-map level
            GL_RGBA, //We want the internal texture to have RGBA components
            img.getSize().x, img.getSize().y,// size of texture
            0, //border (0=no border, 1=border)
            GL_RGBA, //format of the external texture data
            GL_UNSIGNED_BYTE, 
            img.getPixelsPtr() //pointer to array of pixel data 
        );
        
		glEnable(GL_TEXTURE_2D);
	    glBindTexture(GL_TEXTURE_2D, texture); 

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int offset = 100;
        int count = 0;

        for(int z = 0; z < 100; z++) {
            //Makes OpenGL draw a triangle at every three consecutive vertices
			
            glBegin(GL_TRIANGLE_STRIP);
			glColor3f(1.0f, 1.0f, 1.0f);
            for(int x = 0; x < 100; x++) {
				if(x%2 == 0)glTexCoord2d(0.0,0.0);
				else glTexCoord2d(1.0,0.0);
				glVertex3f(x*offset, z*offset, 0);

				if(x%2 == 0)glTexCoord2d(0.0,1.0);
				else glTexCoord2d(1.0,1.0);
				glVertex3f(x*offset, (z+1)*offset, 0);
             
            }
            glEnd();
        }
		
        
        // Finally, display rendered frame on screen 
        App.display(); 
    } 
   
    return EXIT_SUCCESS; 
}

