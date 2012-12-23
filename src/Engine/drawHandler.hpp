#include <stdio.h>
#include <stdlib.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "../global.hpp"

class DrawHandler{
	public :
									DrawHandler();
		GLuint						LoadShader( GLenum type, const char* shaderSrc );
		void						drawObjects();
		//void						push( DrawableObject* object );
		//void						removeObject( DrawableObject* object );
		GLuint						getProgramObject();
	private :
		//std::list<DrawableObject> 	drawableObjects;
		 GLuint						programObject;
};