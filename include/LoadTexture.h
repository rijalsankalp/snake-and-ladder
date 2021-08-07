#ifndef LOADTEXTURE_H
#define LOADTEXTURE_H
#include <vector>
#include <fstream>
#include <iostream>


class LoadTexture
{
    public:
        LoadTexture();
        bool loadPPMImageFlipped(unsigned &, unsigned &, std::vector<unsigned char> &);
        virtual ~LoadTexture();

    protected:

    private:
};

#endif // LOADTEXTURE_H
