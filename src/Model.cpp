#include "Model.h"

void Model::load(const char *filename)
{
        std::string line;
        std::vector<std::string> lines;
        std::ifstream in(filename);

        if(!in.is_open())
        {
            printf("cannot load model%s\n",filename);
            return;
        }

        while(!in.eof())
        {
            std::getline(in,line);
            lines.push_back(line);
        }

        in.close();

        float a,b,c;
        char str[40];
        for(std::string &line:lines)
        {
            if(line[0]=='v')
            {
                if(line[1]==' ')
                {
                    sscanf(line.c_str(),"v %f %f %f",&a,&b,&c);
                    vertices.push_back(new float[3]{a+offsetX,b+offsetY,c+offsetZ});
                }
                else if(line[1]=='t')
                {
                    sscanf(line.c_str(),"vt %f %f",&a,&b);
                    texcoords.push_back(new float[2]{a,b});
                }
                else if(line[1]=='n')
                {
                    sscanf(line.c_str(),"vn %f %f %f",&a,&b,&c);
                    normals.push_back(new float[3]{a,b,c});
                }
            }
            else if(line[0]=='f')
            {
                int edge = count_char(line, ' ');
                int count_slash = count_char(line, '/');
                if (count_slash == 0)
                {
                    if (edge == 3)
                        add_face_3v(line);
                    else
                        add_face_4v(line);
                }
                else if (count_slash == edge)
                {
                    if (edge == 3)
                        add_face_3vt(line);
                    else
                        add_face_4vt(line);
                }
                else if (count_slash == edge * 2)
                {
                    if (has_double_slash(line))
                    {
                        if (edge == 3)
                            add_face_3vn(line);
                        else
                            add_face_4vn(line);
                    }
                    else
                    {
                        if (edge == 3)
                            add_face_3vtn(line);
                        else
                            add_face_4vtn(line);
                    }
                }

            }
            else if (line[0] == 'm' && line[1] == 't')
            {
                sscanf(line.c_str(), "mtllib %s", &str);
                std::string file = prefix + str;
                load_material(file.c_str());
            }
            else if (line[0] == 'u' && line[1] == 's')
            {
                sscanf(line.c_str(), "usemtl %s", &str);
                std::string material = str;
                if (map_material.find(material) != map_material.end())
                    faces.push_back(Face(-1, NULL, NULL, map_material[material]));
            }

        }

        bool has_texcoord = false;

        list= glGenLists(1);
        glNewList(list,GL_COMPILE);
        for(Face &face: faces)
        {
            if (face.edge == -1)
            {
                has_texcoord = false;
                glLightfv(GL_LIGHT0, GL_AMBIENT, materials[face.normal].ambient);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, materials[face.normal].diffuse);
                glLightfv(GL_LIGHT0, GL_SPECULAR, materials[face.normal].specular);

                if (materials[face.normal].texture != 0)
                {
                    has_texcoord = true;
                    glBindTexture(GL_TEXTURE_2D, materials[face.normal].texture);
                }

                continue;
            }


            if(face.normal!=-1)
                glNormal3fv(normals[face.normal]);
            else
                glDisable(GL_LIGHTING);

            if(has_texcoord)
            {
                glBegin(GL_POLYGON);

                for (int i = 0; i < face.edge; i++)
                {
                    glTexCoord2fv(texcoords[face.texcoords[i]]);
                    glVertex3fv(vertices[face.vertices[i]]);
                }
                glEnd();
            }
            else
            {
                glBegin(GL_POLYGON);

                for (int i = 0; i < face.edge; i++)
                    glVertex3fv(vertices[face.vertices[i]]);

                glEnd();
            }
            if(face.normal==-1)
                glEnable(GL_LIGHTING);
        }
        glEndList();

//        printf("Model: %s\n",filename);
//        printf("vertices: %d\n",vertices.size());
//        printf("Texcoordinates: %d\n",texcoords.size());
//        printf("Normals: %d\n",normals.size());
//        printf("Faces: %d\n",faces.size());
//        printf("Materials: %d\n", materials.size());


        for (Material &material : materials)
        {
            delete material.ambient;
            delete material.diffuse;
            delete material.specular;
        }

        materials.clear();
        map_material.clear();


        for(std::string f:lines)
                f.erase();
        lines.clear();

        for(float *f:vertices)
            delete f;
        vertices.clear();

        for(float*f:texcoords)
            delete f;
        texcoords.clear();
        for(float*f:normals)
            delete f;
        normals.clear();

        faces.clear();
}

void Model::draw()
{
    glCallList(list);
}


void Model::sendOffset(float x,float y, float z)
{
    offsetX=x;
    offsetY=y;
    offsetZ=z;
}



int Model::count_char(std::string &str, char ch)
{
    int c = 0;
    int length = str.length() - 1;
    for (int i = 0; i < length; i++)
    {
        if (str[i] == ch)
            c++;
    }
    return c;
}

bool Model::has_double_slash(std::string &str)
{
    int length = str.length() - 2;
    for (int i = 0; i < length; i++)
    {
        if (str[i] == '/' && str[i + 1] == '/')
            return true;
    }
    return false;
}

void Model::load_material(const char *filename)
{
    std::string line;
    std::vector<std::string> lines;
    std::ifstream in(filename);
    if (!in.is_open())
    {
        printf("Cannot load material %s\n", filename);
        return;
    }
    while (!in.eof())
    {
        std::getline(in, line);
        lines.push_back(line);
    }
    in.close();

    Material *m;
    int count_material = 0;
    char str[40];
    std::string material;
    float *a, *d, *s;

    for (std::string &line : lines)
    {
        if (line[0] == 'n' && line[1] == 'e')
        {
            sscanf(line.c_str(), "newmtl %s", str);
            material = str;
            map_material[material] = count_material;
            count_material++;
            a = new float[4]{0.2f, 0.2f, 0.2f, 1.0f};
            d = new float[4]{0.8f, 0.8f, 0.8f, 1.0f};
            s = new float[4]{0.0f, 0.0f, 0.0f, 1.0f};
            materials.push_back(Material(a, d, s));
            m = &materials[materials.size() - 1];
        }
        else if (line[0] == 'K')
        {
            switch (line[1])
            {
                case 'a':
                    sscanf(line.c_str(), "Ka %f %f %f", &a[0], &a[1], &a[2]);
                    break;
                case 'd':
                    sscanf(line.c_str(), "Kd %f %f %f", &d[0], &d[1], &d[2]);
                    break;
                case 's':
                    sscanf(line.c_str(), "Ks %f %f %f", &s[0], &s[1], &s[2]);
                    break;
            }
        }
        else if (line[0] == 'm' && line[1] == 'a')
        {
            /*
            sscanf(line.c_str(), "map_Kd %s", str);
            std::string file = prefix + str;
            Image img;
            Load_Texture_Swap(&img, file.c_str());
            glGenTextures(1, &(m->texture));
            glBindTexture(GL_TEXTURE_2D, m->texture);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.w, img.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.img);
            glBindTexture(GL_TEXTURE_2D, 0);
            Delete_Image(&img);
            */
        }
    }
}

void Model::add_face_3v(std::string &line)
{
    int v0, v1, v2;
    sscanf(line.c_str(), "f %d %d %d", &v0, &v1, &v2);
    int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
    faces.push_back(Face(3, v, NULL));
}

void Model::add_face_3vt(std::string &line)
{
    int v0, v1, v2, t0, t1, t2;
    sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2);
    int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
    int *t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
    faces.push_back(Face(3, v, t));
}

void Model::add_face_3vn(std::string &line)
{
    int v0, v1, v2, n;
    sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v0, &n, &v1, &n, &v2, &n);
    int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
    faces.push_back(Face(3, v, NULL, n - 1));
}

void Model::add_face_3vtn(std::string &line)
{
    int v0, v1, v2, t0, t1, t2, n;
    sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n);
    int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
    int *t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
    faces.push_back(Face(3, v, t, n - 1));
}

void Model::add_face_4v(std::string &line)
{
    int v0, v1, v2, v3;
    sscanf(line.c_str(), "f %d %d %d %d", &v0, &v1, &v2, &v3);
    int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
    faces.push_back(Face(4, v, NULL));
}

void Model::add_face_4vt(std::string &line)
{
    int v0, v1, v2, v3, t0, t1, t2, t3;
    sscanf(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2, &v3, &t3);
    int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
    int *t = new int[4]{t0 - 1, t1 - 1, t2 - 1, t3 - 1};
    faces.push_back(Face(4, v, t));
}

void Model::add_face_4vn(std::string &line)
{
    int v0, v1, v2, v3, n;
    sscanf(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &v0, &n, &v1, &n, &v2, &n, &v3, &n);
    int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
    faces.push_back(Face(4, v, NULL, n - 1));
}

void Model::add_face_4vtn(std::string &line)
{
    int v0, v1, v2, v3, t0, t1, t2, t3, n;
    sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n, &v3,&t3, &n);
    int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
    int *t = new int[4]{t0 - 1, t1 - 1, t2 - 1, t3 - 1};
    faces.push_back(Face(4, v, t, n - 1));
}

