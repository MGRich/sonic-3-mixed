#ifndef SCENE_LEVELSELECT_H
#define SCENE_LEVELSELECT_H

#define PUBLIC
#define PRIVATE
#define PROTECTED
#define STATIC
#define VIRTUAL


#include <Engine/IScene.h>

class Scene_LevelSelect : public IScene {
public:
    int selected = 0;

    Scene_LevelSelect(IApp* app, IGraphics* g);
    void Init();
    void Update();
    void Render();
};

#endif /* SCENE_LEVELSELECT_H */
