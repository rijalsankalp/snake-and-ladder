#ifndef DICE_H
#define DICE_H
#include <vector>
#include <GL/glut.h>
#include <cstdlib>
#include "LoadTexture.h"
#include "dice_coord.h"

class Dice
{
    public:
        Dice();
        void imgTex();
        void draw();
        void roll(int &);
        void updateCoord(int);
        virtual ~Dice();

    private:
        dice_coord dice;
        GLuint tex;
};

#endif // DICE_H
