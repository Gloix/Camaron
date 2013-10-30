#ifndef _SCENE_HELPERS_H_
#define _SCENE_HELPERS_H_

#include "Rendering/RModel/rmodel.h"

class RModel;
class SimpleShader;

//singleton class. Provides visual helpers inside the rendered scene.
//Since it is a singleton class, it only supports single OpenGL context Applications.
class SceneHelpers;
class SceneHelpers{
protected:
    SceneHelpers();
    ~SceneHelpers();
    static SceneHelpers* theInstance;
    static bool StartingShowAxesValue;
    GLuint vao[1];
    static const int vertex_count = 18;
    static const int float_count = vertex_count*3;

    SimpleShader* simpleShader;
    bool showAxes;
    static void Initialize();
    void InitializeByRModel(RModel* model);
    RModel* lastModel;
public:

    //Clean up and force the instance to be re-generated on next getInstance call
    static void reset();
    static SceneHelpers* getInstance();

    void drawHelpers(RModel* model);
    void setAxesVisible(bool value);

};

#endif
