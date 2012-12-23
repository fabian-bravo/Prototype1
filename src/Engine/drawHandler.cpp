#include "drawHandler.hpp"

			DrawHandler::DrawHandler(){
	//Basic shaders
	GLbyte		vShaderStr[]		= 
		"attribute vec4		vPosition; 					\n"
		"void main()									\n"
		"{												\n"
		"	gl_Position		= vPosition;				\n"
		"}												\n";

	GLbyte		fShaderStr[]		=
		"precision mediump float;						\n"
		"void main()									\n"
		"{												\n"
		" 	gl_FragColor	= vec4(1.0, 0.0, 0.0, 1.0);	\n"
		"}												\n";

	GLuint vertexShader;
	GLuint fragmentShader;
	GLint linked;

	vertexShader		= LoadShader( GL_VERTEX_SHADER, vShaderStr );
	fragmentShader		= LoadShader( GL_FRAGMENT_SHADER, fShaderStr );

	programObject		= glCreateProgram();

	if( programObject == 0 ){
		//Failed to created program object, should stop game execution
		return;
	}

	glAttachShader( programObject, vertexShader );
	glAttachShader( programObject, fragmentShader );

	glBindAttribLocation( programObject, 0, "vPosition" );

	glLinkProgram( programObject );

	glGetProgramiv( programObject, GL_LINK_STATUS, &linked );

	if( !linked ){
		GLint infoLen = 0;

		glGetProgramiv( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if( infoLen > 1 ){
			char * infoLog 		= malloc( sizeof( char ) * infoLen );

			glGetProgramInfoLog( programObject, infoLen, NULL, infoLog );
			LOGW( "DrawHandler > Constructor > Error linking program : \n %s \n", infoLog );

			free( infoLog );
		}

		glDeleteProgram( programObject );
		return;
	}
}

GLuint 		DrawHandler::LoadShader( GLuint type, const char* shaderSrc ){
	GLuint 		shader;
	GLint 		compiled;

	shader 		= glCreateShader( type );

	if( shader == 0 ){
		return 0;
	}

	glShaderSource( shader, 1, &shaderSrc, NULL );

	glCompileShader( shader );

	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );

	if( !compiled ){
		GLint 		infoLen	= 0;

		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if( infoLen > 1 ){
			char * infoLog 		= malloc( sizeof( char ) * infoLen );

			glGetShaderInfoLog( shader, infoLen, NULL, infoLog );
			LOGW( "DrawHandler > LoadShader > Error compiling shader:\n%s\n", infoLog );

			free( infoLog );
		}

		glDeleteShader( shader );

		return 0;
	}

	return shader;
}

void 		DrawHandler::drawObjects(  ){
	if( ! programObject ){
		return;
	}

	GLfloat vVertices[]		= {	 0.0f,  0.5f, 0.0f,
								-0.5f, -0.5f, 0.0f,
								 0.5f, -0.5f, 0.0f	};

	glUseProgram( programObject );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
	glEnableVertexAttribArray( 0 );

	glDrawArrays( GL_TRIANGLES, 0, 3 );
}

// void		DrawHandler::push( DrawableObject* object ){

// }

// void		DrawHandler::removeObject( DrawableObject* object ){

// }

GLuint		DrawHandler::getProgramObject(){
	return programObject;
}