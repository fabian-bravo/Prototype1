#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "../global.hpp"
#include "drawHandler.hpp"

struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

class Engine {
	public 		:
    	//Getters and Setters
    	android_app* 		   getAndroidApp() const;
    	void				   setAndroidApp( android_app* app );

    	ASensorManager*		   getSensorManager() const;
    	void				   setSensorManager( ASensorManager* sensor );
    	ASensor*			   getAccelerometerSensor() const;
    	void				   setAccelerometerSensor( ASensor* sensor );
    	ASensorEventQueue* 	   getSensorEventQueue() const;
    	void				   setSensorEventQueue( ASensorEventQueue* eventQueue );

    	EGLDisplay			   getDisplay() const;
        void                   setDisplay( EGLDisplay display );
    	EGLSurface			   getSurface() const;
        void                   setSurface( EGLSurface surface );
    	EGLContext			   getContext() const;
        void                   setContext( EGLContext context );

        saved_state            getSavedState() const;
        void                   setSavedState( saved_state state );

    	int32_t				   getWidth() const;
    	int32_t				   getHeight() const;

        int                    isAnimating() const;
        void                   setAnimating( int animating );

        DrawHandler            getDrawHandler() const;
        void                   setDrawHandler( DrawHandler * drawHanldler );

        //Object functions
                               Engine();
        int                    initDisplay();
        void                   drawFrame();
        void                   termDisplay();
        int                    handleInput();
        int                    handleInput(AInputEvent* event);
        void                   handleCMD(int32_t cmd);

	private 	: 
		//Android Data
		struct android_app*    _app;
		ASensorManager*        _sensorManager;
    	const ASensor*         _accelerometerSensor;
    	ASensorEventQueue*     _sensorEventQueue;

    	//OpenGL Data
    	int                    _animating;
    	EGLDisplay             _display;
    	EGLSurface             _surface;
    	EGLContext             _context;

    	//Display Data
    	int32_t                _width;
    	int32_t                _height;

    	saved_state            _state;
        DrawHandler*           _drawHandler;
};