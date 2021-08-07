#include "LoadTexture.h"

using namespace std;

LoadTexture::LoadTexture(){}

bool LoadTexture::loadPPMImageFlipped(unsigned &width, unsigned &height, std::vector<unsigned char> &imgData)
{
    ifstream input("Image/dice.ppm", ifstream::in | ifstream::binary);

    if(!input)
    {
      cerr << "Can not find texture data file " << endl;
      return false;
    }

    input.unsetf(std::ios_base::skipws);

    string line;
    input >> line >> std::ws;

    if (line != "P6")
    {
      cerr << "File is not PPM P6 raw format" << endl;
      input.close();
      return false;
    }

    width = 0;
    height = 0;
    unsigned depth = 0;
    unsigned readItems = 0;
    unsigned char lastCharBeforeBinary;

    while (readItems < 3)
    {
        input >> std::ws;
        if(input.peek() != '#')
        {
            if (readItems == 0)
                input >> width;
            if (readItems == 1)
                input >> height;
            if (readItems == 2)
                input >> depth >> lastCharBeforeBinary;

            readItems++;
        }
        else
        {
            std::getline(input, line);
        }
    }

    if(depth >= 256)
    {
        cerr << "Only 8-bit PPM format is supported" << endl;
        input.close();
      return false;
    }

    unsigned byteCount = width * height * 3;
    imgData.resize(byteCount);
    input.read((char*)&imgData[0], byteCount*sizeof(unsigned char));

    unsigned char tmpData;
    for(unsigned y=0; y < height / 2; y++)
    {
        int sourceIndex = y * width * 3;
        int targetIndex = (height-1-y) * width *3;
        for(unsigned x=0; x < width*3; x++)
        {
            tmpData = imgData[targetIndex];
            imgData[targetIndex] = imgData[sourceIndex];
            imgData[sourceIndex] = tmpData;
            sourceIndex++;
            targetIndex++;
        }
    }
    input.close();
    return true;
}

LoadTexture::~LoadTexture(){}
