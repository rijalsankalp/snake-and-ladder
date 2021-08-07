#ifndef MODEL_H
#define MODEL_H

#include"Header.h"

class Model
{
    public:
        void load(const char *filename);
        void draw();
        void sendOffset(float x,float y, float z);

    private:
        static int count_char(std::string &str, char ch);


        static bool has_double_slash(std::string &str);

        class Material {
            public:
                float *ambient;
                float *diffuse;
                float *specular;
                GLuint texture;

                Material(float *ambient, float *diffuse, float *specular) {
                    this->ambient = ambient;
                    this->diffuse = diffuse;
                    this->specular = specular;
                    this->texture = 0;
                }
            };

        class Face{
            public:
                int edge;
                int *vertices;
                int *texcoords;
                int normal;

                Face(int edge,int *vertices,int *texcoords,int normal=-1){
                    this->edge=edge;
                    this->vertices=vertices;
                    this->texcoords=texcoords;
                    this->normal=normal;
                }
            };

        std::string prefix="../../Models/";
        std::vector<Material> materials;
        std::map<std::string, int> map_material;

        std::vector<float*> vertices;
        std::vector<float*> texcoords;
        std::vector<float*> normals;
        std::vector<Face> faces;
        GLuint list;



        float offsetX=0,offsetY=0,offsetZ=0;





        void load_material(const char *filename);
        void add_face_3v(std::string &line);
        void add_face_3vt(std::string &line);
        void add_face_3vn(std::string &line);
        void add_face_3vtn(std::string &line);
        void add_face_4v(std::string &line);
        void add_face_4vt(std::string &line);
        void add_face_4vn(std::string &line);
        void add_face_4vtn(std::string &line);


};

#endif // MODEL_H
