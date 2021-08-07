#include "Dice.h"

Dice::Dice()
{
    dice.x = -1;
    dice.y = 1;
    dice.z = 1;
}

void Dice::imgTex()
{
    unsigned width;
    unsigned height;
    std::vector<unsigned char>imageData;

    LoadTexture texture;

    if(!texture.loadPPMImageFlipped(width, height, imageData))
    {
        std::cout<<"Loading image failed"<<std::endl;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures( 1, &tex);                //letting OpenGL name the texture to avoid repetition by user
    glBindTexture( GL_TEXTURE_2D, tex);     //naming the pattern of the texture

    //@GL_TEXTURE_MAG_FILTER if required enlarge texture to fit in specified coordinate
    //@GL_TEXTURE_MIN_FILTER if required reduce texture to fit in specified coordinate
    //@GL_LINEAR calculates pixel color as linear cobn of overlapping tex colors

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //@GL_REPLACE replaces the object value with the texture value
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    /*specifying parameters for the 2D texture space::
    /texture array is being defined for 2D-surface
    /texture array is not reduction of some large reduction pattern
    /color of texture be RGB
    /width of image
    /height of image
    /no border required
    /data format = RGB
    /data type = unsigned byte
    /data = imagedata
    */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &imageData[0]);

    glBindTexture(GL_TEXTURE_2D, tex); //designating "tex" as the current pattern
}

void Dice::draw()
{
    //draw
    glBegin(GL_QUADS);

    //front
    glTexCoord2f(0.01, 0.99);
    glVertex3f(dice.x, dice.y, dice.z); //A

    glTexCoord2d(0.01, 0.5);
    glVertex3f(dice.x, -dice.y, dice.z); //B

    glTexCoord2d(0.325, 0.5);
    glVertex3f(-dice.x, -dice.y, dice.z); //c

    glTexCoord2d(0.325, 0.99);
    glVertex3f(-dice.x, dice.y, dice.z); //D

    //back
    glTexCoord2f(0.99, 0.5);
    glVertex3f(dice.x, -dice.y, -dice.z); //F

    glTexCoord2f(0.99, 0.99);
    glVertex3f(dice.x, dice.y, -dice.z); //E

    glTexCoord2f(0.666, 0.99);
    glVertex3f(-dice.x, dice.y, -dice.z); //H

    glTexCoord2f(0.666, 0.5);
    glVertex3f(-dice.x, -dice.y, -dice.z); //G

    //right

    glTexCoord2f(0.66, 0.5);
    glVertex3f(-dice.x, dice.y, dice.z); //D

    glTexCoord2f(0.66, 0.01);
    glVertex3f(-dice.x, -dice.y, dice.z); //C

    glTexCoord2f(0.99, 0.01);
    glVertex3f(-dice.x, -dice.y, -dice.z); //G

    glTexCoord2f(0.99, 0.5);
    glVertex3f(-dice.x, dice.y, -dice.z); //H

    //left

    glTexCoord2f(0.33, 0.5);
    glVertex3f(dice.x, dice.y, -dice.z); //E

    glTexCoord2f(0.33, 0.01);
    glVertex3f(dice.x, -dice.y, -dice.z); //F

    glTexCoord2f(0.66, 0.01);
    glVertex3f(dice.x, -dice.y, dice.z); //B

    glTexCoord2f(0.66, 0.5);
    glVertex3f(dice.x, dice.y, dice.z); //A

    //top

    glTexCoord2f(0.01, 0.5);
    glVertex3f(dice.x, dice.y, -dice.z); //E

    glTexCoord2f(0.01, 0.01);
    glVertex3f(dice.x, dice.y, dice.z); //A

    glTexCoord2f(0.33, 0.01);
    glVertex3f(-dice.x, dice.y, dice.z); //D

    glTexCoord2f(0.33, 0.5);
    glVertex3f(-dice.x, dice.y, -dice.z); //H

    //bottom
    glTexCoord2f(0.33, 0.5);
    glVertex3f(dice.x, -dice.y, dice.z); //B

    glTexCoord2f(0.33, 0.99);
    glVertex3f(dice.x, -dice.y, -dice.z); //F

    glTexCoord2f(0.66, 0.99);
    glVertex3f(-dice.x, -dice.y, -dice.z); //G

    glTexCoord2f(0.66, 0.5);
    glVertex3f(-dice.x, -dice.y, dice.z); //C

    glEnd();
}

void Dice::roll(int &a)
{
    a = rand()%6 +1;
}

void Dice::updateCoord(int pos)
{
    switch(pos)
    {
    case 1:
        {
            glRotatef(0, 0, 0, 0);
            break;
        }
    case 2:
        {
            glRotatef(180, 0, -1, 1);
            break;
        }
    case 3:
        {
            glRotatef(180, 0, -1, 1);
            glRotatef(180, 0, 1, 1);
            break;
        }
    case 4:
        {
            glRotatef(180, 0, 1, 1);
            break;
        }
    case 5:
        {
            glRotatef(180, -1, 0, 1);
            break;
        }
    case 6:
        {
            glRotatef(180, 1, 0, 1);
            break;
        }
    }
}

Dice::~Dice(){}
