/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "../src/global.hpp"
#include "../src/Engine/engine.hpp"


static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    Engine* engine = (Engine*)app->userData;
    return engine->handleInput( event );
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    Engine* engine = (Engine*)app->userData;
    return engine->handleCMD( cmd );
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
    Engine *engine   = new Engine();

    // Make sure glue isn't stripped.
    app_dummy();

    state->userData = engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine->setAndroidApp( state );

    // Prepare to monitor accelerometer
    engine->setSensorManager( ASensorManager_getInstance() );
    engine->setAccelerometerSensor( ASensorManager_getDefaultSensor(engine->getSensorManager(), ASENSOR_TYPE_ACCELEROMETER) );
    engine->setSensorEventQueue( ASensorManager_createEventQueue(engine->getSensorManager(), state->looper, LOOPER_ID_USER, NULL, NULL) );

    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine->setSavedState( *(struct saved_state*)state->savedState );
    }

    // loop waiting for stuff to do.

    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine->isAnimating() ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                if (engine->getAccelerometerSensor() != NULL) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine->getSensorEventQueue() , &event, 1) > 0) {
                        
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine->termDisplay();
                return;
            }
        }

        if (engine->isAnimating()) {
            // Done with events; draw next animation frame.
            engine->getSavedState().angle += .01f;
            if (engine->getSavedState().angle > 1) {
                engine->getSavedState().angle = 0;
            }

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine->drawFrame();
        }
    }
}
//END_INCLUDE(all)
