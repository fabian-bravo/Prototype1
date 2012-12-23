#include "engine.hpp"
#include "../global.hpp"


Engine::Engine(){

}

int 			Engine::initDisplay(){
	// initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;

    _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(_display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(_display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(_display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(_app->window, 0, 0, format);

    _surface = eglCreateWindowSurface(_display, config, _app->window, NULL);

    static const EGLint ctx_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
    };

    _context = eglCreateContext(_display, config, EGL_NO_CONTEXT, ctx_attribs);

    if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE) {
        LOGW("Engine > initDisplay > Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(_display, _surface, EGL_WIDTH, &w);
    eglQuerySurface(_display, _surface, EGL_HEIGHT, &h);

    LOGI("Engine > initDisplay > Screen width = %d, screen height = %d", w, h);

    _width = w;
    _height = h;
    _state.angle = 0;

    // Initialize GL state.
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);


    //INIT draw handler
    _drawHandler    = new DrawHandler();


    return 0;
}

void 				Engine::drawFrame() {
    if (_display == NULL) {
        // No display.
        return;
    }

    // Just fill the screen with a color.
    glClearColor(((float)_state.x)/_width, _state.angle,
            ((float)_state.y)/_height, 1);
    glClear(GL_COLOR_BUFFER_BIT);


    _drawHandler->drawObjects();

    eglSwapBuffers(_display, _surface);
}

void 				Engine::termDisplay() {
    if (_display != EGL_NO_DISPLAY) {
        eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (_context != EGL_NO_CONTEXT) {
            eglDestroyContext(_display, _context);
        }
        if (_surface != EGL_NO_SURFACE) {
            eglDestroySurface(_display, _surface);
        }
        eglTerminate(_display);
    }
    _animating 	= 0;
    _display 	= EGL_NO_DISPLAY;
    _context 	= EGL_NO_CONTEXT;
    _surface 	= EGL_NO_SURFACE;
}

int 				Engine::handleInput(AInputEvent* event) {
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        _animating = 1;
        _state.x = AMotionEvent_getX(event, 0);
        _state.y = AMotionEvent_getY(event, 0);
        return 1;
    }
    return 0;
}

void 				Engine::handleCMD(int32_t cmd) {
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            _app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)_app->savedState) = _state;
            _app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (_app->window != NULL) {
                initDisplay();
                drawFrame();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            termDisplay();
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (_accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(_sensorEventQueue, _accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(_sensorEventQueue, _accelerometerSensor, (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (_accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(_sensorEventQueue,_accelerometerSensor);
            }
            // Also stop animating.
            _animating = 0;
            drawFrame();
            break;
    }
}

/*
	GETTERS AND SETTERS
 */

android_app* 		Engine::getAndroidApp() const{
	return _app;
}

void				Engine::setAndroidApp( android_app* app ){
	_app	= app;
}

ASensorManager* 	Engine::getSensorManager() const{
	return _sensorManager;
}
    	
void				Engine::setSensorManager( ASensorManager* sensor ){
	_sensorManager	= sensor;
}
    
ASensor* 			Engine::getAccelerometerSensor() const{
	return _accelerometerSensor;
}

void				Engine::setAccelerometerSensor( ASensor* sensor ){
	_accelerometerSensor	= sensor;
}

ASensorEventQueue*  Engine::getSensorEventQueue() const{
	return _sensorEventQueue;
}

void				Engine::setSensorEventQueue( ASensorEventQueue* eventQueue ){
	_sensorEventQueue		= eventQueue;
}

EGLDisplay			Engine::getDisplay() const{
	return _display;
}
        
void                Engine::setDisplay( EGLDisplay display ){
	_display		= display;
}
    	
EGLSurface			Engine::getSurface() const{
	return _surface;
}

void                Engine::setSurface( EGLSurface surface ){
	_surface		= surface;
}

EGLContext			Engine::getContext() const{
	return _context;
}
        
void                Engine::setContext( EGLContext context ){
	_context		= context;
}

saved_state         Engine::getSavedState() const{
	return _state;
}

void                Engine::setSavedState( saved_state state ){
	_state			= state;
}

int32_t				Engine::getWidth() const{
	return _width;
}

int32_t				Engine::getHeight() const{
	return _height;
}

int 				Engine::isAnimating() const{
	return _animating;
}

void 				Engine::setAnimating( int animating ){
	_animating		= animating;
}