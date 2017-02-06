#ifndef _SCENE_H
#define _SCENE_H

#include "image.h"

class Scene : public Image {
private:
    Scene* subScene;

public:
    Scene();
    virtual void update() = 0;
};

#endif
