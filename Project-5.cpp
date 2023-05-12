
//Name: Harry Patel
//Course: CSCI 172-Computer Graphics
//Project: Model Loader

#include <string.h>
#include <algorithm>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <iostream>

#include <math.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <SOIL.h>
using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

//This parameter is utilized inside the load. Texture Since there are 6 sides, we will utilize 6 photos. Each will enter their own set of photographs.
GLuint tex;
//zoom and rotate variables
float rotX, rotY, rotCamX, rotCamY, moveX, moveY, zoom = 0;
float prev_x, prev_y = 0.0;

//Texture coordinates
class v2{
    public:
    float a2;
    float b2;
    v2(float a, float b){
        a2 = a;
        b2 = b;
    }
};

//Quads vectors
class v4{
    public:
    float a4;
    float b4;
    float c4;
    float d4;
};

//Triangle vectors
class v3{
    public:
    float a3;
    float b3;
    float c3;
    v3(float a, float b, float c){
        a3 = a;
        b3 = b;
        c3 = c;
    }
};

//Faces with only vertexes
class faceOnlyVertex{
    public:
    float a3;
    float b3;
    float c3;
    faceOnlyVertex(float a, float b, float c){
        a3 = a;
        b3 = b;
        c3 = c;
    }
};


//Faces with vertexes and textures for Quads
class faceVertexTexture4{
    public:
    float a4;
    float b4;
    float c4;
    float d4;
    float t1;
    float t2;
    float t3;
    float t4;
    faceVertexTexture4(float a, float b, float c, float d, float e, float f, float g, float h){
        a4 = a;
        b4 = b;
        c4 = c;
        d4 = d;
        t1 = e;
        t2 = f;
        t3 = g;
        t4 = h;
    }
};
//Faces with vertexes and textures for triangles
class faceVertexTexture3{
    public:
    int a3;
    int b3;
    int c3;
    int t1;
    int t2;
    int t3;
    faceVertexTexture3(int a, int b, int c, int d, int e, int f){
        a3 = a;
        b3 = b;
        c3 = c;
        t1 = d;
        t2 = e;
        t3 = f;
    }
};

//Hexagons Vertexes and Textures
class faceVertexTexture6{
    public:
    float v1, v2, v3, v4, v5, v6, t1, t2, t3, t4, t5, t6;

    //This is for faces
    faceVertexTexture6(float vert1, float vert2, float vert3, float vert4, float vert5, float vert6,
                            float tex1, float tex2, float tex3, float tex4, float tex5, float tex6){
        v1 = vert1;
        v2 = vert2;
        v3 = vert3;
        v4 = vert4;
        v5 = vert5;
        v6 = vert6;
        t1 = tex1;
        t2 = tex2;
        t3 = tex3;
        t4 = tex4;
        t5 = tex5;
        t6 = tex6;
    }
};

//Pentagons Vertexes and Textures
class faceVertexTexture5{
    public:
    float v1, v2, v3, v4, v5, t1, t2, t3, t4, t5;

    //Faces
    faceVertexTexture5(float vert1, float vert2, float vert3, float vert4, float vert5,
                            float tex1, float tex2, float tex3, float tex4, float tex5){
        v1 = vert1;
        v2 = vert2;
        v3 = vert3;
        v4 = vert4;
        v5 = vert5;
        t1 = tex1;
        t2 = tex2;
        t3 = tex3;
        t4 = tex4;
        t5 = tex5;
    }
};



//Triangles Faces with vertexes, textures, and normals
class faceVertexTextureNorms3{
   public:
    float v1, v2, v3, t1, t2, t3, n1, n2, n3;   //Vertexes

    //faces
    faceVertexTextureNorms3(float vert1, float vert2, float vert3, float tex1, float tex2, float tex3, float norm1, float norm2, float norm3){
        v1 = vert1;
        v2 = vert2;
        v3 = vert3;
        t1 = tex1;
        t2 = tex2;
        t3 = tex3;
        n1 = norm1;
        n2 = norm2;
        n3 = norm3;
    }
};

//Quads Faces with vertexes, textures, and normals
class faceVertexTextureNorms4{
   public:
    float v1, v2, v3, v4, t1, t2, t3, t4, n1, n2, n3, n4;

    //faces
    faceVertexTextureNorms4(float vert1, float vert2, float vert3, float vert4, float tex1, float tex2, float tex3, float tex4, float norm1, float norm2, float norm3, float norm4){
        v1 = vert1;
        v2 = vert2;
        v3 = vert3;
        v4 = vert4;
        t1 = tex1;
        t2 = tex2;
        t3 = tex3;
        t4 = tex4;
        n1 = norm1;
        n2 = norm2;
        n3 = norm3;
        n4 = norm4;
    }
};

//Pentagons Faces with vertexes, textures, and normals
class faceVertexTextureNorms5{
   public:
    float v1, v2, v3, v4, v5, t1, t2, t3, t4, t5, n1, n2, n3, n4, n5;

    //faces
    faceVertexTextureNorms5(float vert1, float vert2, float vert3, float vert4, float vert5,
                            float tex1, float tex2, float tex3, float tex4, float tex5,
                            float norm1, float norm2, float norm3, float norm4, float norm5){
        v1 = vert1;
        v2 = vert2;
        v3 = vert3;
        v4 = vert4;
        v5 = vert5;
        t1 = tex1;
        t2 = tex2;
        t3 = tex3;
        t4 = tex4;
        t5 = tex5;
        n1 = norm1;
        n2 = norm2;
        n3 = norm3;
        n4 = norm4;
        n5 = norm5;
    }
};


//Hexagons Faces with vertexes, textures, and normals
class faceVertexTextureNorms6{
   public:
    float v1, v2, v3, v4, v5, v6, t1, t2, t3, t4, t5, t6, n1, n2, n3, n4, n5, n6;   //Vertexes

    //faces
    faceVertexTextureNorms6(float vert1, float vert2, float vert3, float vert4, float vert5, float vert6,
                            float tex1, float tex2, float tex3, float tex4, float tex5, float tex6,
                            float norm1, float norm2, float norm3, float norm4, float norm5, float norm6){
        v1 = vert1;
        v2 = vert2;
        v3 = vert3;
        v4 = vert4;
        v5 = vert5;
        v6 = vert6;
        t1 = tex1;
        t2 = tex2;
        t3 = tex3;
        t4 = tex4;
        t5 = tex5;
        t6 = tex6;
        n1 = norm1;
        n2 = norm2;
        n3 = norm3;
        n4 = norm4;
        n5 = norm5;
        n6 = norm6;
    }
};

//This list will be referenced in order to work on some functions, similar to tex
GLuint list;

v3 subtractVertexes(v3 ver1, v3 ver2){
    v3 subVertex = v3(0, 0, 0);
    subVertex.a3 = ver1.a3 - ver2.a3;
    subVertex.b3 = ver1.b3 - ver2.b3;
    subVertex.c3 = ver1.c3 - ver2.c3;
    return subVertex;
}

v3 crossProduct(v3 ver1, v3 ver2){
    v3 crossP = v3(0, 0, 0);
    crossP.a3 = ver1.b3 * ver2.c3 - ver1.c3 * ver2.b3;
    crossP.b3 = ver1.c3 * ver2.a3 - ver1.a3 * ver2.c3;
    crossP.c3 = ver1.a3 * ver2.b3 - ver1.b3 * ver2.a3;
    return crossP;
}

//We should now be able to get the normal
v3 normalize(v3 ver1, v3 ver2, v3 ver3){
    v3 norm = v3(0, 0, 0);  //This is our new normal

    //A->B = B - A
    v3 temp1 = subtractVertexes(ver2, ver1);
    //A->C = C - A
    v3 temp2 = subtractVertexes(ver3, ver1);
    //A->B X A->C
    norm = crossProduct(temp1, temp2);

    return norm;
}
//Triangles to Hexagons
class Model{
    public:
    //Vertexes
    vector<faceOnlyVertex> faceVertexes;          //"f" values that only have vertexes

    //Triangles
    vector<faceVertexTexture3> faceVertTexTri;        //"f" values that only have 3 vertexes and textures
    vector<faceVertexTexture3> faceVertNormTri;        //This is for the case that there are no texture coordinates
    vector<faceVertexTextureNorms3> faceVertTexNormTri;   //Triangle faces that have all 3


    //Quads
    vector<faceVertexTexture4> faceVertTexQuad;
    vector<faceVertexTexture4> faceVertNormQuad;
    vector<faceVertexTextureNorms4> faceVertTexNormQuad;

    //Pentagons
    vector<faceVertexTexture5> faceVertTexPent;
    vector<faceVertexTexture5> faceVertNormPent;
    vector<faceVertexTextureNorms5> faceVertTexNormPent;

    //Hexagons
    vector<faceVertexTexture6> faceVertTexHex;
    vector<faceVertexTexture6> faceVertNormHex;
    vector<faceVertexTextureNorms6> faceVertTexNormHex;


    //Triangles
    vector<v2> v2TexCoords; //2D
    vector<v3> v3Vectors;
    vector<v3> v3Normals;

    //Quads
    vector<v4> v4Vertexes;
    vector<v4> v4Normals;

    const char *fileName;

    Model(const char *file){
        fileName = file;
    }
    //This will read a file then insert the corresponding values to them.
    void readFile(){
        string line;            //current line
        vector<string> lines;   //all lines reading

        ifstream in(fileName);  //read the file
        if(!in.is_open())       //if file doesn't exist then print error.
        {
            cout << "Cannot Load Model " << fileName << endl;
            return;
        }
        while(!in.eof())        //read the entire file using while loop.
        {
            getline(in, line);      //store each line in the variable line.
            lines.push_back(line);  //push in the vector.
        }
        in.close(); //Close file.

        //populate the vectors
        float a, b, c, ver1, ver2, ver3, ver4, ver5, ver6, tex1, tex2, tex3, tex4, tex5, tex6, norm1, norm2, norm3, norm4, norm5, norm6; //We will be working with v3 vectors
        for(string &line : lines)  //check all lines
        {
            cout << line << endl;
            if(line[0] == 'v')  //If first letter is v then its a vector, normal, or texture coordinate
            {
                if(line[1] == ' ')  // check if the line is a vector
                {
                    sscanf(line.c_str(), "v %f %f %f", &a, &b, &c); // populate the variables a,b,c with the vector values.
                    v3Vectors.push_back(v3(a, b, c));   //Add a new v3 variable with the following values
                }
                else if(line[1] == 't')  //check if the line is a texture coordinate
                {
                    sscanf(line.c_str(), "vt %f %f", &a, &b); // populate the variables a,b,c with the float values.
                    v2TexCoords.push_back(v2(a, b)); //Add a new v2 variable into the texture coordinates
                }
                else    //if not other two then its vector coordinates
                {
                    sscanf(line.c_str(), "vn %f %f %f", &a, &b, &c); // populate the variables a,b,c with the float values
                    v3Normals.push_back(v3(a, b, c));   //Add new normal coordinates to the list of normals
                }
            }
            else if(line[0] == 'f')  //If it is not a vertex then check the face
            {
                //Check if the face is a triangle or a quad
                int numOfSlash = 0;
                for (int i = 0; i < line.size(); i++){
                    if(line[i] == '/')
                    {
                        numOfSlash++;
                    }
                }
                //Only Vertexes
                if(numOfSlash == 0)
                {
                    sscanf(line.c_str(), "f %f %f %f", &ver1, &ver2, &ver3);
                    faceVertexes.push_back(faceOnlyVertex(ver1-1, ver2-1, ver3-1));
                }
                //Triangles vertexes and textures
                else if(numOfSlash == 3){
                    sscanf(line.c_str(), "f %f/%f %f/%f %f/%f", &ver1, &tex1, &ver2, &tex2, &ver3, &tex3);
                    faceVertTexTri.push_back(faceVertexTexture3(ver1-1, ver2-1, ver3-1, tex1-1, tex2-1, tex3-1));
                }
                //Quads vertexes and textures
                else if(numOfSlash == 4){
                    sscanf(line.c_str(), "f %f/%f %f/%f %f/%f %f/%f", &ver1, &tex1, &ver2, &tex2, &ver3, &tex3, &ver4, &tex4);
                    faceVertTexQuad.push_back(faceVertexTexture4(ver1-1, ver2-1, ver3-1, ver4-1, tex1-1, tex2-1, tex3-1, tex4-1));
                }
                //Pentagons vertexes and textures
                else if(numOfSlash == 5){
                    sscanf(line.c_str(), "f %f/%f %f/%f %f/%f %f/%f %f/%f", &ver1, &tex1, &ver2, &tex2, &ver3, &tex3, &ver4, &tex4, &ver5, &tex5);
                    faceVertTexPent.push_back(faceVertexTexture5(ver1-1, ver2-1,  ver3-1, ver4-1, ver5-1, tex1-1, tex2-1, tex3-1, tex4-1, tex5-1));
                }
                //Hexagons
                else if(numOfSlash == 6){
                    //condition for texture coordinates
                    if((line.find('/', line.find('/')+1)) - line.find('/') == 1){
                        sscanf(line.c_str(), "f %f//%f %f//%f %f//%f", &ver1, &norm1, &ver2, &norm2, &ver3, &norm3);
                        faceVertNormTri.push_back(faceVertexTexture3(ver1-1, ver2-1, ver3-1, norm1-1, norm2-1, norm3-1));
                    }
                    else{
                        sscanf(line.c_str(), "f %f/%f/%f %f/%f/%f %f/%f/%f", &ver1, &tex1, &norm1, &ver2, &tex2, &norm2, &ver3, &tex3, &norm3);
                        faceVertTexNormTri.push_back(faceVertexTextureNorms3(ver1-1, ver2-1, ver3-1, tex1-1, tex2-1, tex3-1, norm1-1, norm2-1, norm3-1));
                    }
                }
                //Quads
                else if(numOfSlash == 8){
                    //condition for texture coordinates
                    if((line.find('/', line.find('/')+1)) - line.find('/') == 1){
                        sscanf(line.c_str(), "f %f//%f %f//%f %f//%f %f//%f", &ver1, &norm1, &ver2, &norm2, &ver3, &norm3, &ver4, &norm4);
                        faceVertNormQuad.push_back(faceVertexTexture4(ver1-1, ver2-1, ver3-1, ver4-1, norm1-1, norm2-1, norm3-1, norm4-1));
                    }
                    else{
                        sscanf(line.c_str(), "f %f/%f/%f %f/%f/%f %f/%f/%f %f/%f/%f", &ver1, &tex1, &norm1, &ver2, &tex2, &norm2, &ver3, &tex3, &norm3, &ver4, &tex4, &norm4);
                        faceVertTexNormQuad.push_back(faceVertexTextureNorms4(ver1-1, ver2-1, ver3-1, ver4-1, tex1-1, tex2-1, tex3-1, tex4-1, norm1-1, norm2-1, norm3-1, norm4-1));
                    }
                }
                //Pentagons
                else if(numOfSlash == 10){
                    //condition for texture coordinates
                    if((line.find('/', line.find('/')+1)) - line.find('/') == 1){
                        sscanf(line.c_str(), "f %f//%f %f//%f %f//%f %f//%f %f//%f", &ver1, &norm1, &ver2, &norm2, &ver3, &norm3, &ver4, &norm4, &ver5, &norm5);
                        faceVertNormPent.push_back(faceVertexTexture5(ver1-1, ver2-1, ver3-1, ver4-1, ver5-1, norm1-1, norm2-1, norm3-1, norm4-1, norm5-1));
                    }
                    else{
                        sscanf(line.c_str(), "f %f/%f/%f %f/%f/%f %f/%f/%f %f/%f/%f  %f/%f/%f", &ver1, &tex1, &norm1, &ver2, &tex2, &norm2, &ver3, &tex3, &norm3, &ver4, &tex4, &norm4, &ver5, &tex5, &norm5);
                        faceVertTexNormPent.push_back(faceVertexTextureNorms5(ver1-1, ver2-1, ver3-1, ver4-1, ver5-1, tex1-1, tex2-1, tex3-1, tex4-1, tex5-1, norm1-1, norm2-1, norm3-1, norm4-1, norm5-1));
                    }
                }
                //Triangles
                else if(numOfSlash == 12){
                    //condition for texture coordinates
                    if((line.find('/', line.find('/')+1)) - line.find('/') == 1){
                        sscanf(line.c_str(), "f %f//%f %f//%f %f//%f %f//%f %f//%f %f//%f", &ver1, &norm1, &ver2, &norm2, &ver3, &norm3, &ver4, &norm4, &ver5, &norm5, &ver6, &norm6);
                        faceVertNormHex.push_back(faceVertexTexture6(ver1-1, ver2-1, ver3-1, ver4-1, ver5-1, ver6-1, norm1-1, norm2-1, norm3-1, norm4-1, norm5-1, norm6-1));
                    }
                    else{
                        sscanf(line.c_str(), "f %f/%f/%f %f/%f/%f %f/%f/%f %f/%f/%f  %f/%f/%f", &ver1, &tex1, &norm1, &ver2, &tex2, &norm2, &ver3, &tex3, &norm3, &ver4, &tex4, &norm4, &ver5, &tex5, &norm5, &ver6, &tex6, &norm6);
                        faceVertTexNormHex.push_back(faceVertexTextureNorms6(ver1-1, ver2-1, ver3-1, ver4-1, ver5-1, ver6-1, tex1-1, tex2-1, tex3-1, tex4-1, tex5-1, tex6-1, norm1-1, norm2-1, norm3-1, norm4-1, norm5-1, norm6-1));
                    }
                }
            }
        }
    }

    void faceNormalizeTriangles(){  //only for triangles
        if(v3Normals.size() == 0){  //condition to check for normal
            //No textures
            for(int i = 0; i < faceVertexes.size(); i++){
                v3 normalsA = normalize(v3Vectors[faceVertexes[i].a3], v3Vectors[faceVertexes[i].b3], v3Vectors[faceVertexes[i].c3]);
                v3 normalsB = normalize(v3Vectors[faceVertexes[i].b3], v3Vectors[faceVertexes[i].c3], v3Vectors[faceVertexes[i].a3]);
                v3 normalsC = normalize(v3Vectors[faceVertexes[i].c3], v3Vectors[faceVertexes[i].a3], v3Vectors[faceVertexes[i].b3]);
                v3Normals.push_back(v3(normalsA.a3, normalsA.b3, normalsA.c3));
                v3Normals.push_back(v3(normalsB.a3, normalsB.b3, normalsB.c3));
                v3Normals.push_back(v3(normalsC.a3, normalsC.b3, normalsC.c3));
                faceVertNormTri.push_back(faceVertexTexture3(faceVertexes[i].a3, faceVertexes[i].b3, faceVertexes[i].c3, i*3, i*3+1, i*3+2));
            }
            //new normals
            while(!faceVertexes.empty()){
                faceVertexes.pop_back();
            }


            //Texture for loop
            for(int i = 0; i < faceVertTexTri.size(); i++){
                v3 normalsA = normalize(v3Vectors[faceVertTexTri[i].a3], v3Vectors[faceVertTexTri[i].b3], v3Vectors[faceVertTexTri[i].c3]);
                v3 normalsB = normalize(v3Vectors[faceVertTexTri[i].b3], v3Vectors[faceVertTexTri[i].c3], v3Vectors[faceVertTexTri[i].a3]);
                v3 normalsC = normalize(v3Vectors[faceVertTexTri[i].c3], v3Vectors[faceVertTexTri[i].a3], v3Vectors[faceVertTexTri[i].b3]);
                v3Normals.push_back(v3(normalsA.a3, normalsA.b3, normalsA.c3));
                v3Normals.push_back(v3(normalsB.a3, normalsB.b3, normalsB.c3));
                v3Normals.push_back(v3(normalsC.a3, normalsC.b3, normalsC.c3));
                faceVertTexNormTri.push_back(faceVertexTextureNorms3(faceVertTexTri[i].a3, faceVertTexTri[i].b3, faceVertTexTri[i].c3, faceVertTexTri[i].t1, faceVertTexTri[i].t2, faceVertTexTri[i].t2, i*3, i*3+1, i*3+2));
            }
            //new normals
            while(!faceVertTexTri.empty()){
                faceVertTexTri.pop_back();
            }
        }
    }

    //Function to draw the 3D models.
    void drawModel(){
        //vertexes
        for(int i = 0; i < faceVertexes.size();i++){
            if(!WireFrame)                  //wireframe
            glBegin(GL_TRIANGLES);
            else
            glBegin(GL_LINE_LOOP);
                glVertex3f(v3Vectors[faceVertexes[i].a3].a3,
                           v3Vectors[faceVertexes[i].a3].b3,
                           v3Vectors[faceVertexes[i].a3].c3);

                glVertex3f(v3Vectors[faceVertexes[i].b3].a3,
                           v3Vectors[faceVertexes[i].b3].b3,
                           v3Vectors[faceVertexes[i].b3].c3);

                glVertex3f(v3Vectors[faceVertexes[i].c3].a3,
                           v3Vectors[faceVertexes[i].c3].b3,
                           v3Vectors[faceVertexes[i].c3].c3);
            glEnd();
        }


        //Triangles
        for(int i = 0; i < faceVertTexTri.size();i++){  //The purpose of this is to draw the triangle using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_TRIANGLES);
            else
            glBegin(GL_LINE_LOOP);
                glTexCoord2f(v2TexCoords[faceVertTexTri[i].t1].a2,
                             v2TexCoords[faceVertTexTri[i].t1].b2);
                glVertex3f(v3Vectors[faceVertTexTri[i].a3].a3,
                           v3Vectors[faceVertTexTri[i].a3].b3,
                           v3Vectors[faceVertTexTri[i].a3].c3);


                glTexCoord2f(v2TexCoords[faceVertTexTri[i].t2].a2,
                             v2TexCoords[faceVertTexTri[i].t2].b2);
                glVertex3f(v3Vectors[faceVertTexTri[i].b3].a3,
                           v3Vectors[faceVertTexTri[i].b3].b3,
                           v3Vectors[faceVertTexTri[i].b3].c3);


                glTexCoord2f(v2TexCoords[faceVertTexTri[i].t3].a2,
                             v2TexCoords[faceVertTexTri[i].t3].b2);
                glVertex3f(v3Vectors[faceVertTexTri[i].c3].a3,
                           v3Vectors[faceVertTexTri[i].c3].b3,
                           v3Vectors[faceVertTexTri[i].c3].c3);
            glEnd();
        }


        for(int i = 0; i < faceVertNormTri.size();i++){ //The purpose of this is to draw the triangles using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_TRIANGLES);
            else
            glBegin(GL_LINE_LOOP);
                glNormal3f(v3Normals[faceVertNormTri[i].t1].a3,
                             v3Normals[faceVertNormTri[i].t1].b3,
                             v3Normals[faceVertNormTri[i].t1].c3);
                glVertex3f(v3Vectors[faceVertNormTri[i].a3].a3,
                           v3Vectors[faceVertNormTri[i].a3].b3,
                           v3Vectors[faceVertNormTri[i].a3].c3);


                glNormal3f(v3Normals[faceVertNormTri[i].t2].a3,
                             v3Normals[faceVertNormTri[i].t2].b3,
                             v3Normals[faceVertNormTri[i].t2].c3);
                glVertex3f(v3Vectors[faceVertNormTri[i].b3].a3,
                           v3Vectors[faceVertNormTri[i].b3].b3,
                           v3Vectors[faceVertNormTri[i].b3].c3);


                glNormal3f(v3Normals[faceVertNormTri[i].t3].a3,
                             v3Normals[faceVertNormTri[i].t3].b3,
                             v3Normals[faceVertNormTri[i].t3].c3);
                glVertex3f(v3Vectors[faceVertNormTri[i].c3].a3,
                           v3Vectors[faceVertNormTri[i].c3].b3,
                           v3Vectors[faceVertNormTri[i].c3].c3);
            glEnd();
        }


        for(int i = 0; i < faceVertTexNormTri.size(); i++){ //The purpose of this is to draw the triangles using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_TRIANGLES);
            else
            glBegin(GL_LINE_LOOP);
                glTexCoord2f(v2TexCoords[faceVertTexNormTri[i].t1].a2,
                             v2TexCoords[faceVertTexNormTri[i].t1].b2);
                glNormal3f(v3Normals[faceVertTexNormTri[i].n1].a3,
                             v3Normals[faceVertTexNormTri[i].n1].b3,
                             v3Normals[faceVertTexNormTri[i].n1].c3);
                glVertex3f(v3Vectors[faceVertTexNormTri[i].v1].a3,
                           v3Vectors[faceVertTexNormTri[i].v1].b3,
                           v3Vectors[faceVertTexNormTri[i].v1].c3);


                glTexCoord2f(v2TexCoords[faceVertTexNormTri[i].t2].a2,
                             v2TexCoords[faceVertTexNormTri[i].t2].b2);
                glNormal3f(v3Normals[faceVertTexNormTri[i].n2].a3,
                             v3Normals[faceVertTexNormTri[i].n2].b3,
                             v3Normals[faceVertTexNormTri[i].n2].c3);
                glVertex3f(v3Vectors[faceVertTexNormTri[i].v2].a3,
                           v3Vectors[faceVertTexNormTri[i].v2].b3,
                           v3Vectors[faceVertTexNormTri[i].v2].c3);


                glTexCoord2f(v2TexCoords[faceVertTexNormTri[i].t3].a2,
                             v2TexCoords[faceVertTexNormTri[i].t3].b2);
                glNormal3f(v3Normals[faceVertTexNormTri[i].n3].a3,
                             v3Normals[faceVertTexNormTri[i].n3].b3,
                             v3Normals[faceVertTexNormTri[i].n3].c3);
                glVertex3f(v3Vectors[faceVertTexNormTri[i].v3].a3,
                           v3Vectors[faceVertTexNormTri[i].v3].b3,
                           v3Vectors[faceVertTexNormTri[i].v3].c3);
            glEnd();
        }



        //Quads
        for(int i = 0; i < faceVertTexQuad.size();i++){  //The purpose of this is to draw the quads using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_QUADS);
            else
            glBegin(GL_LINE_LOOP);
                glTexCoord2f(v2TexCoords[faceVertTexQuad[i].t1].a2,
                             v2TexCoords[faceVertTexQuad[i].t1].b2);
                glVertex3f(v3Vectors[faceVertTexQuad[i].a4].a3,
                           v3Vectors[faceVertTexQuad[i].a4].b3,
                           v3Vectors[faceVertTexQuad[i].a4].c3);


                glTexCoord2f(v2TexCoords[faceVertTexQuad[i].t2].a2,
                             v2TexCoords[faceVertTexQuad[i].t2].b2);
                glVertex3f(v3Vectors[faceVertTexQuad[i].b4].a3,
                           v3Vectors[faceVertTexQuad[i].b4].b3,
                           v3Vectors[faceVertTexQuad[i].b4].c3);


                glTexCoord2f(v2TexCoords[faceVertTexQuad[i].t3].a2,
                             v2TexCoords[faceVertTexQuad[i].t3].b2);
                glVertex3f(v3Vectors[faceVertTexQuad[i].c4].a3,
                           v3Vectors[faceVertTexQuad[i].c4].b3,
                           v3Vectors[faceVertTexQuad[i].c4].c3);

                glTexCoord2f(v2TexCoords[faceVertTexQuad[i].t4].a2,
                             v2TexCoords[faceVertTexQuad[i].t4].b2);
                glVertex3f(v3Vectors[faceVertTexQuad[i].d4].a3,
                           v3Vectors[faceVertTexQuad[i].d4].b3,
                           v3Vectors[faceVertTexQuad[i].d4].c3);
            glEnd();
        }

        for(int i = 0; i < faceVertNormQuad.size(); i++){ //The purpose of this is to draw the quads using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_QUADS);
            else
            glBegin(GL_LINE_LOOP);
                glNormal3f(v3Normals[faceVertNormQuad[i].t1].a3,
                             v3Normals[faceVertNormQuad[i].t1].b3,
                             v3Normals[faceVertNormQuad[i].t1].c3);
                glVertex3f(v3Vectors[faceVertNormQuad[i].a4].a3,
                           v3Vectors[faceVertNormQuad[i].a4].b3,
                           v3Vectors[faceVertNormQuad[i].a4].c3);


                glNormal3f(v3Normals[faceVertNormQuad[i].t2].a3,
                             v3Normals[faceVertNormQuad[i].t2].b3,
                             v3Normals[faceVertNormQuad[i].t2].c3);
                glVertex3f(v3Vectors[faceVertNormQuad[i].b4].a3,
                           v3Vectors[faceVertNormQuad[i].b4].b3,
                           v3Vectors[faceVertNormQuad[i].b4].c3);


                glNormal3f(v3Normals[faceVertNormQuad[i].t3].a3,
                             v3Normals[faceVertNormQuad[i].t3].b3,
                             v3Normals[faceVertNormQuad[i].t3].c3);
                glVertex3f(v3Vectors[faceVertNormQuad[i].c4].a3,
                           v3Vectors[faceVertNormQuad[i].c4].b3,
                           v3Vectors[faceVertNormQuad[i].c4].c3);

                glNormal3f(v3Normals[faceVertNormQuad[i].t4].a3,
                             v3Normals[faceVertNormQuad[i].t4].b3,
                             v3Normals[faceVertNormQuad[i].t4].c3);
                glVertex3f(v3Vectors[faceVertNormQuad[i].d4].a3,
                           v3Vectors[faceVertNormQuad[i].d4].b3,
                           v3Vectors[faceVertNormQuad[i].d4].c3);
            glEnd();
        }

        for(int i = 0; i < faceVertTexNormQuad.size();i++){ //The purpose of this is to draw the quads using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_QUADS);
            else
            glBegin(GL_LINE_LOOP);
                glTexCoord2f(v2TexCoords[faceVertTexNormQuad[i].t1].a2,
                             v2TexCoords[faceVertTexNormQuad[i].t1].b2);
                glNormal3f(v3Normals[faceVertTexNormQuad[i].n1].a3,
                             v3Normals[faceVertTexNormQuad[i].n1].b3,
                             v3Normals[faceVertTexNormQuad[i].n1].c3);
                glVertex3f(v3Vectors[faceVertTexNormQuad[i].v1].a3,
                           v3Vectors[faceVertTexNormQuad[i].v1].b3,
                           v3Vectors[faceVertTexNormQuad[i].v1].c3);


                glTexCoord2f(v2TexCoords[faceVertTexNormQuad[i].t2].a2,
                             v2TexCoords[faceVertTexNormQuad[i].t2].b2);
                glNormal3f(v3Normals[faceVertTexNormQuad[i].n2].a3,
                             v3Normals[faceVertTexNormQuad[i].n2].b3,
                             v3Normals[faceVertTexNormQuad[i].n2].c3);
                glVertex3f(v3Vectors[faceVertTexNormQuad[i].v2].a3,
                           v3Vectors[faceVertTexNormQuad[i].v2].b3,
                           v3Vectors[faceVertTexNormQuad[i].v2].c3);


                glTexCoord2f(v2TexCoords[faceVertTexNormQuad[i].t3].a2,
                             v2TexCoords[faceVertTexNormQuad[i].t3].b2);
                glNormal3f(v3Normals[faceVertTexNormQuad[i].n3].a3,
                             v3Normals[faceVertTexNormQuad[i].n3].b3,
                             v3Normals[faceVertTexNormQuad[i].n3].c3);
                glVertex3f(v3Vectors[faceVertTexNormQuad[i].v3].a3,
                           v3Vectors[faceVertTexNormQuad[i].v3].b3,
                           v3Vectors[faceVertTexNormQuad[i].v3].c3);


                glTexCoord2f(v2TexCoords[faceVertTexNormQuad[i].t4].a2,
                             v2TexCoords[faceVertTexNormQuad[i].t4].b2);
                glNormal3f(v3Normals[faceVertTexNormQuad[i].n4].a3,
                             v3Normals[faceVertTexNormQuad[i].n4].b3,
                             v3Normals[faceVertTexNormQuad[i].n4].c3);
                glVertex3f(v3Vectors[faceVertTexNormQuad[i].v4].a3,
                           v3Vectors[faceVertTexNormQuad[i].v4].b3,
                           v3Vectors[faceVertTexNormQuad[i].v4].c3);
            glEnd();
        }

        //Pentagons
        for(int i = 0; i < faceVertTexPent.size();i++){  //The purpose of this is to draw the pentagon using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_POLYGON);
            else
            glBegin(GL_LINE_LOOP);
                glTexCoord2f(v2TexCoords[faceVertTexPent[i].t1].a2,
                             v2TexCoords[faceVertTexPent[i].t1].b2);
                glVertex3f(v3Vectors[faceVertTexPent[i].v1].a3,
                           v3Vectors[faceVertTexPent[i].v1].b3,
                           v3Vectors[faceVertTexPent[i].v1].c3);


                glTexCoord2f(v2TexCoords[faceVertTexPent[i].t2].a2,
                             v2TexCoords[faceVertTexPent[i].t2].b2);
                glVertex3f(v3Vectors[faceVertTexPent[i].v2].a3,
                           v3Vectors[faceVertTexPent[i].v2].b3,
                           v3Vectors[faceVertTexPent[i].v2].c3);


                glTexCoord2f(v2TexCoords[faceVertTexPent[i].t3].a2,
                             v2TexCoords[faceVertTexPent[i].t3].b2);
                glVertex3f(v3Vectors[faceVertTexPent[i].v3].a3,
                           v3Vectors[faceVertTexPent[i].v3].b3,
                           v3Vectors[faceVertTexPent[i].v3].c3);

                glTexCoord2f(v2TexCoords[faceVertTexPent[i].t4].a2,
                             v2TexCoords[faceVertTexPent[i].t4].b2);
                glVertex3f(v3Vectors[faceVertTexPent[i].v4].a3,
                           v3Vectors[faceVertTexPent[i].v4].b3,
                           v3Vectors[faceVertTexPent[i].v4].c3);

                glTexCoord2f(v2TexCoords[faceVertTexPent[i].t5].a2,
                             v2TexCoords[faceVertTexPent[i].t5].b2);
                glVertex3f(v3Vectors[faceVertTexPent[i].v5].a3,
                           v3Vectors[faceVertTexPent[i].v5].b3,
                           v3Vectors[faceVertTexPent[i].v5].c3);
            glEnd();
        }


        for(int i = 0; i < faceVertNormPent.size(); i++){   //The purpose of this is to draw the pentagon using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_POLYGON);
            else
            glBegin(GL_LINE_LOOP);
                glNormal3f(v3Normals[faceVertNormPent[i].t1].a3,
                             v3Normals[faceVertNormPent[i].t1].b3,
                             v3Normals[faceVertNormPent[i].t1].c3);
                glVertex3f(v3Vectors[faceVertNormPent[i].v1].a3,
                           v3Vectors[faceVertNormPent[i].v1].b3,
                           v3Vectors[faceVertNormPent[i].v1].c3);


                glNormal3f(v3Normals[faceVertNormPent[i].t2].a3,
                             v3Normals[faceVertNormPent[i].t2].b3,
                             v3Normals[faceVertNormPent[i].t2].c3);
                glVertex3f(v3Vectors[faceVertNormPent[i].v2].a3,
                           v3Vectors[faceVertNormPent[i].v2].b3,
                           v3Vectors[faceVertNormPent[i].v2].c3);


                glNormal3f(v3Normals[faceVertNormPent[i].t3].a3,
                             v3Normals[faceVertNormPent[i].t3].b3,
                             v3Normals[faceVertNormPent[i].t3].c3);
                glVertex3f(v3Vectors[faceVertNormPent[i].v3].a3,
                           v3Vectors[faceVertNormPent[i].v3].b3,
                           v3Vectors[faceVertNormPent[i].v3].c3);

                glNormal3f(v3Normals[faceVertNormPent[i].t4].a3,
                             v3Normals[faceVertNormPent[i].t4].b3,
                             v3Normals[faceVertNormPent[i].t4].c3);
                glVertex3f(v3Vectors[faceVertNormPent[i].v4].a3,
                           v3Vectors[faceVertNormPent[i].v4].b3,
                           v3Vectors[faceVertNormPent[i].v4].c3);

                glNormal3f(v3Normals[faceVertNormPent[i].t5].a3,
                             v3Normals[faceVertNormPent[i].t5].b3,
                             v3Normals[faceVertNormPent[i].t5].c3);
                glVertex3f(v3Vectors[faceVertNormPent[i].v5].a3,
                           v3Vectors[faceVertNormPent[i].v5].b3,
                           v3Vectors[faceVertNormPent[i].v5].c3);
            glEnd();
        }

        for(int i = 0; i < faceVertTexNormPent.size();i++){ //The purpose of this is to draw the pentagon using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_POLYGON);
            else
            glBegin(GL_LINE_LOOP);
                glTexCoord2f(v2TexCoords[faceVertTexNormPent[i].t1].a2,
                             v2TexCoords[faceVertTexNormPent[i].t1].b2);
                glNormal3f(v3Normals[faceVertTexNormPent[i].n1].a3,
                             v3Normals[faceVertTexNormPent[i].n1].b3,
                             v3Normals[faceVertTexNormPent[i].n1].c3);
                glVertex3f(v3Vectors[faceVertTexNormPent[i].v1].a3,
                           v3Vectors[faceVertTexNormPent[i].v1].b3,
                           v3Vectors[faceVertTexNormPent[i].v1].c3);


                glTexCoord2f(v2TexCoords[faceVertTexNormPent[i].t2].a2,
                             v2TexCoords[faceVertTexNormPent[i].t2].b2);
                glNormal3f(v3Normals[faceVertTexNormPent[i].n2].a3,
                             v3Normals[faceVertTexNormPent[i].n2].b3,
                             v3Normals[faceVertTexNormPent[i].n2].c3);
                glVertex3f(v3Vectors[faceVertTexNormPent[i].v2].a3,
                           v3Vectors[faceVertTexNormPent[i].v2].b3,
                           v3Vectors[faceVertTexNormPent[i].v2].c3);


                glTexCoord2f(v2TexCoords[faceVertTexNormPent[i].t3].a2,
                             v2TexCoords[faceVertTexNormPent[i].t3].b2);
                glNormal3f(v3Normals[faceVertTexNormPent[i].n3].a3,
                             v3Normals[faceVertTexNormPent[i].n3].b3,
                             v3Normals[faceVertTexNormPent[i].n3].c3);
                glVertex3f(v3Vectors[faceVertTexNormPent[i].v3].a3,
                           v3Vectors[faceVertTexNormPent[i].v3].b3,
                           v3Vectors[faceVertTexNormPent[i].v3].c3);


                glTexCoord2f(v2TexCoords[faceVertTexNormPent[i].t4].a2,
                             v2TexCoords[faceVertTexNormPent[i].t4].b2);
                glNormal3f(v3Normals[faceVertTexNormPent[i].n4].a3,
                             v3Normals[faceVertTexNormPent[i].n4].b3,
                             v3Normals[faceVertTexNormPent[i].n4].c3);
                glVertex3f(v3Vectors[faceVertTexNormPent[i].v4].a3,
                           v3Vectors[faceVertTexNormPent[i].v4].b3,
                           v3Vectors[faceVertTexNormPent[i].v4].c3);

                glTexCoord2f(v2TexCoords[faceVertTexNormPent[i].t5].a2,
                             v2TexCoords[faceVertTexNormPent[i].t5].b2);
                glNormal3f(v3Normals[faceVertTexNormPent[i].n5].a3,
                             v3Normals[faceVertTexNormPent[i].n5].b3,
                             v3Normals[faceVertTexNormPent[i].n5].c3);
                glVertex3f(v3Vectors[faceVertTexNormPent[i].v5].a3,
                           v3Vectors[faceVertTexNormPent[i].v5].b3,
                           v3Vectors[faceVertTexNormPent[i].v5].c3);
            glEnd();
        }


        //Hexagons
        for(int i = 0; i < faceVertTexHex.size();i++){   //The purpose of this is to draw the hexagon using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_POLYGON);
            else
            glBegin(GL_LINE_LOOP);
                glTexCoord2f(v2TexCoords[faceVertTexHex[i].t1].a2,
                             v2TexCoords[faceVertTexHex[i].t1].b2);
                glVertex3f(v3Vectors[faceVertTexHex[i].v1].a3,
                           v3Vectors[faceVertTexHex[i].v1].b3,
                           v3Vectors[faceVertTexHex[i].v1].c3);


                glTexCoord2f(v2TexCoords[faceVertTexHex[i].t2].a2,
                             v2TexCoords[faceVertTexHex[i].t2].b2);
                glVertex3f(v3Vectors[faceVertTexHex[i].v2].a3,
                           v3Vectors[faceVertTexHex[i].v2].b3,
                           v3Vectors[faceVertTexHex[i].v2].c3);


                glTexCoord2f(v2TexCoords[faceVertTexHex[i].t3].a2,
                             v2TexCoords[faceVertTexHex[i].t3].b2);
                glVertex3f(v3Vectors[faceVertTexHex[i].v3].a3,
                           v3Vectors[faceVertTexHex[i].v3].b3,
                           v3Vectors[faceVertTexHex[i].v3].c3);

                glTexCoord2f(v2TexCoords[faceVertTexHex[i].t4].a2,
                             v2TexCoords[faceVertTexHex[i].t4].b2);
                glVertex3f(v3Vectors[faceVertTexHex[i].v4].a3,
                           v3Vectors[faceVertTexHex[i].v4].b3,
                           v3Vectors[faceVertTexHex[i].v4].c3);

                glTexCoord2f(v2TexCoords[faceVertTexHex[i].t5].a2,
                             v2TexCoords[faceVertTexHex[i].t5].b2);
                glVertex3f(v3Vectors[faceVertTexHex[i].v5].a3,
                           v3Vectors[faceVertTexHex[i].v5].b3,
                           v3Vectors[faceVertTexHex[i].v5].c3);

                glTexCoord2f(v2TexCoords[faceVertTexHex[i].t6].a2,
                             v2TexCoords[faceVertTexHex[i].t6].b2);
                glVertex3f(v3Vectors[faceVertTexHex[i].v6].a3,
                           v3Vectors[faceVertTexHex[i].v6].b3,
                           v3Vectors[faceVertTexHex[i].v6].c3);
            glEnd();
        }


        for(int i = 0; i < faceVertNormHex.size(); i++){  //The purpose of this is to draw the hexagon using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_POLYGON);
            else
            glBegin(GL_LINE_LOOP);
                glNormal3f(v3Normals[faceVertNormHex[i].t1].a3,
                             v3Normals[faceVertNormHex[i].t1].b3,
                             v3Normals[faceVertNormHex[i].t1].c3);
                glVertex3f(v3Vectors[faceVertNormHex[i].v1].a3,
                           v3Vectors[faceVertNormHex[i].v1].b3,
                           v3Vectors[faceVertNormHex[i].v1].c3);


                glNormal3f(v3Normals[faceVertNormHex[i].t2].a3,
                             v3Normals[faceVertNormHex[i].t2].b3,
                             v3Normals[faceVertNormHex[i].t2].c3);
                glVertex3f(v3Vectors[faceVertNormHex[i].v2].a3,
                           v3Vectors[faceVertNormHex[i].v2].b3,
                           v3Vectors[faceVertNormHex[i].v2].c3);


                glNormal3f(v3Normals[faceVertNormHex[i].t3].a3,
                             v3Normals[faceVertNormHex[i].t3].b3,
                             v3Normals[faceVertNormHex[i].t3].c3);
                glVertex3f(v3Vectors[faceVertNormHex[i].v3].a3,
                           v3Vectors[faceVertNormHex[i].v3].b3,
                           v3Vectors[faceVertNormHex[i].v3].c3);

                glNormal3f(v3Normals[faceVertNormHex[i].t4].a3,
                             v3Normals[faceVertNormHex[i].t4].b3,
                             v3Normals[faceVertNormHex[i].t4].c3);
                glVertex3f(v3Vectors[faceVertNormHex[i].v4].a3,
                           v3Vectors[faceVertNormHex[i].v4].b3,
                           v3Vectors[faceVertNormHex[i].v4].c3);

                glNormal3f(v3Normals[faceVertNormHex[i].t5].a3,
                             v3Normals[faceVertNormHex[i].t5].b3,
                             v3Normals[faceVertNormHex[i].t5].c3);
                glVertex3f(v3Vectors[faceVertNormHex[i].v5].a3,
                           v3Vectors[faceVertNormHex[i].v5].b3,
                           v3Vectors[faceVertNormHex[i].v5].c3);

                glNormal3f(v3Normals[faceVertNormHex[i].t6].a3,
                             v3Normals[faceVertNormHex[i].t6].b3,
                             v3Normals[faceVertNormHex[i].t6].c3);
                glVertex3f(v3Vectors[faceVertNormHex[i].v6].a3,
                           v3Vectors[faceVertNormHex[i].v6].b3,
                           v3Vectors[faceVertNormHex[i].v6].c3);
            glEnd();
        }

        for(int i = 0; i < faceVertTexNormHex.size();i++){ //The purpose of this is to draw the hexagon using vertices, normals, and textures.
            if(!WireFrame)
            glBegin(GL_POLYGON);
            else
            glBegin(GL_LINE_LOOP);
                glTexCoord2f(v2TexCoords[faceVertTexNormHex[i].t1].a2,
                             v2TexCoords[faceVertTexNormHex[i].t1].b2);
                glNormal3f(v3Normals[faceVertTexNormHex[i].n1].a3,
                             v3Normals[faceVertTexNormHex[i].n1].b3,
                             v3Normals[faceVertTexNormHex[i].n1].c3);
                glVertex3f(v3Vectors[faceVertTexNormHex[i].v1].a3,
                           v3Vectors[faceVertTexNormHex[i].v1].b3,
                           v3Vectors[faceVertTexNormHex[i].v1].c3);


                glTexCoord2f(v2TexCoords[faceVertTexNormHex[i].t2].a2,
                             v2TexCoords[faceVertTexNormHex[i].t2].b2);
                glNormal3f(v3Normals[faceVertTexNormHex[i].n2].a3,
                             v3Normals[faceVertTexNormHex[i].n2].b3,
                             v3Normals[faceVertTexNormHex[i].n2].c3);
                glVertex3f(v3Vectors[faceVertTexNormHex[i].v2].a3,
                           v3Vectors[faceVertTexNormHex[i].v2].b3,
                           v3Vectors[faceVertTexNormHex[i].v2].c3);


                glTexCoord2f(v2TexCoords[faceVertTexNormHex[i].t3].a2,
                             v2TexCoords[faceVertTexNormHex[i].t3].b2);
                glNormal3f(v3Normals[faceVertTexNormHex[i].n3].a3,
                             v3Normals[faceVertTexNormHex[i].n3].b3,
                             v3Normals[faceVertTexNormHex[i].n3].c3);
                glVertex3f(v3Vectors[faceVertTexNormHex[i].v3].a3,
                           v3Vectors[faceVertTexNormHex[i].v3].b3,
                           v3Vectors[faceVertTexNormHex[i].v3].c3);


                glTexCoord2f(v2TexCoords[faceVertTexNormHex[i].t4].a2,
                             v2TexCoords[faceVertTexNormHex[i].t4].b2);
                glNormal3f(v3Normals[faceVertTexNormHex[i].n4].a3,
                             v3Normals[faceVertTexNormHex[i].n4].b3,
                             v3Normals[faceVertTexNormHex[i].n4].c3);
                glVertex3f(v3Vectors[faceVertTexNormHex[i].v4].a3,
                           v3Vectors[faceVertTexNormHex[i].v4].b3,
                           v3Vectors[faceVertTexNormHex[i].v4].c3);

                glTexCoord2f(v2TexCoords[faceVertTexNormHex[i].t5].a2,
                             v2TexCoords[faceVertTexNormHex[i].t5].b2);
                glNormal3f(v3Normals[faceVertTexNormHex[i].n5].a3,
                             v3Normals[faceVertTexNormHex[i].n5].b3,
                             v3Normals[faceVertTexNormHex[i].n5].c3);
                glVertex3f(v3Vectors[faceVertTexNormHex[i].v5].a3,
                           v3Vectors[faceVertTexNormHex[i].v5].b3,
                           v3Vectors[faceVertTexNormHex[i].v5].c3);

                glTexCoord2f(v2TexCoords[faceVertTexNormHex[i].t6].a2,
                             v2TexCoords[faceVertTexNormHex[i].t6].b2);
                glNormal3f(v3Normals[faceVertTexNormHex[i].n6].a3,
                             v3Normals[faceVertTexNormHex[i].n6].b3,
                             v3Normals[faceVertTexNormHex[i].n6].c3);
                glVertex3f(v3Vectors[faceVertTexNormHex[i].v6].a3,
                           v3Vectors[faceVertTexNormHex[i].v6].b3,
                           v3Vectors[faceVertTexNormHex[i].v6].c3);
            glEnd();
        }
    }
};

static void resize(int width, int height)
{
     double Ratio;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 500.0f); //this  is to zoom in and zoom out.
 }

//It takes in a file name and will bind the texture accordingly
 void loadTexture(char *fileName){
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
    int width, height;                 // width & height for the image reader
    unsigned char* image;
    image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGB);
    if(!image)  //If there is no image or it cannot find it, then it will print an error
       cout << "Failed to find image" << endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // binding image data
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}




//We are loading the following Models into the sky box.
//cow, elephant, ateneam, cube, trunk, ven-2
Model cow = Model("objects/cow.obj");
Model ele = Model("objects/ele.obj");
Model stat = Model("objects/ateneam-2.obj");
Model cube = Model("objects/cube.obj");
Model deer = Model("objects/trunk.obj");
Model ven = Model("objects/ven-2.obj");

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME


    //Your code here
    //Move the camera and zoom in and out

    glRotatef(rotCamX, 1, 0, 0);   //Rotates X
    glRotatef(rotCamY, 0, 1, 0);   //Rotates Y
    gluLookAt(-moveX, 2 ,10-zoom, -moveX, 2, -zoom,0.0,1.0,0.0);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);    //Disable normalize
    glEnable(GL_LIGHTING);     //Disable lighting so that everything is visible and bright

    //Draw cow model
    glPushMatrix();
        glScalef(0.5, 0.5, 0.5);
        glTranslatef(20, 3, 10);
        glRotatef(rotX, 1, 0, 0);   //Rotates X
        glRotatef(rotY, 0, 1, 0);   //Rotates Y
        glRotatef(270, 0, 1, 0);
        cow.drawModel();
        glTranslatef(-20, -3, -10);
    glPopMatrix();

    //Draw Elephant model
    glPushMatrix();
        glScalef(0.01, 0.01, 0.01);
        glTranslatef(-1000, -20, 500);
        glRotatef(rotX, 1, 0, 0);   //Rotates X
        glRotatef(rotY, 0, 1, 0);   //Rotates Y
        ele.drawModel();
        glTranslatef(1000, 20, -500);
    glPopMatrix();

    //Draw ateneam-2 Model
    glPushMatrix();
        glScalef(0.15, 0.15, 0.15);
        glTranslatef(0, -3, 5);
        glRotatef(rotX, 1, 0, 0);   //Rotates X
        glRotatef(rotY, 0, 1, 0);   //Rotates Y
        stat.drawModel();
        glTranslatef(0, 3, -5);
    glPopMatrix();

    //Draw cube model
    glPushMatrix();
        glTranslatef(0, 0, -15);
        glRotatef(rotX, 1, 0, 0);   //Rotates X
        glRotatef(rotY, 0, 1, 0);   //Rotates Y
        glScalef(1, 1, 1);
        cube.drawModel();
        glTranslatef(0, 0, 15);
    glPopMatrix();

    //Draw Trunk model
    glPushMatrix();
        glScalef(1, 1, 1);
        glTranslatef(-15, 0, 20);
        glRotatef(rotX, 1, 0, 0);   //Rotates X
        glRotatef(rotY, 0, 1, 0);   //Rotates Y
        glScalef(0.02, 0.02, 0.02);
        deer.drawModel();
        glScalef(1, 1, 1);
        glTranslatef(15, 0, -25);
    glPopMatrix();

    //Draw ven model
    glPushMatrix();
        glScalef(1, 1, 1);
        glTranslatef(15, 1, 20);
        glRotatef(rotX, 1, 0, 0);   //Rotates X
        glRotatef(rotY, 0, 1, 0);   //Rotates Y
        glScalef(0.001, 0.001, 0.001);
        ven.drawModel();
        glScalef(1, 1, 1);
        glTranslatef(-15, -01, -25);
    glPopMatrix();



    //This is the skybox
     glPushMatrix();
        glScalef(50.0, 50.0, 50.0); //skybox 50 times bigger on all sides
        glDisable(GL_NORMALIZE);    //Disable normalize
        glDisable(GL_LIGHTING);     //Disable lighting so that everything is visible and bright

        //Front -> Loads in front of us
        loadTexture("images/brick.png");
        glBegin(GL_QUADS);  //GL_QUADS allows us to create a 4 sided polygon that we can use. These are the coordinates that we use and their vertexes
            glTexCoord2f(1.0, 0.0);     //This is the First point (x, y), these also set the current texture's coordinates on a 2D plane
            glVertex3f(-1.0, 1.0, -1.0);    //These specify the vertexes, basically the points that we are using

            glTexCoord2f(1.0, 1.0);     //This is the Second point and it's texture
            glVertex3f(-1.0, -1.0, -1.0);

            glTexCoord2f(0.0, 1.0);     //This is the Third point
            glVertex3f(1.0, -1.0, -1.0);

            glTexCoord2f(0.0, 0.0);     //This is the Fourth point
            glVertex3f(1.0, 1.0, -1.0);
        glEnd();

        //Right -> The image to the right
        loadTexture("images/brick.png");
        glBegin(GL_QUADS);                  //Second Square to be made, creates the right side of the box
            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.0, 1.0, -1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(1.0, -1.0, -1.0);

            glTexCoord2f(0.0, 1.0);     //First point that moves behind us, we look at the "z = 0 to -1.0 side"
            glVertex3f(1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.0, 1.0, 1.0);
        glEnd();

        //Left -> The image to the left
        loadTexture("images/brick.png");
        glBegin(GL_QUADS);                  //Third Square to be made, Creates the left side of the box
            glTexCoord2f(0.0, 0.0);         //This side of the texCoord is the opposite because the quad is being created on the opposite side
            glVertex3f(-1.0, 1.0, -1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(-1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(-1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(-1.0, -1.0, -1.0);
        glEnd();


        //Back -> Loads in the back
        loadTexture("images/brick.png");
        //Fourth Square that is being created
        glBegin(GL_QUADS);                  //This will also complete an entire 360 degree rotation moving left to right
            glTexCoord2f(0.0, 0.0);         //This is also being created opposite to the front because it is created on the opposite side of the front
            glVertex3f(-1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(-1.0, -1.0, 1.0);
            glEnd();

        //Ceiling -> Loads in the top of the skybox
        loadTexture("images/cl.png");
        glBegin(GL_QUADS);             //Fifth square made, creates the top of the box
            glTexCoord2f(0.0, 1.0);
            glVertex3f(1.0, 1.0, -1.0);

            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(-1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(-1.0, 1.0, -1.0);
        glEnd();

        //Floor -> Loads in the floor
        loadTexture("images/c.png");
        glBegin(GL_QUADS);              //Last square made, creates the bottom of the box
            glTexCoord2f(1.0, 1.0);
            glVertex3f(-1, -0.01, 1);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(1, -0.01, 1);

            glTexCoord2f(0.0, 0.0);
            glVertex3f(1, -0.01, -1);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(-1, -0.01, -1);
        glEnd();

        glEnable(GL_NORMALIZE); //Enable Normalization again so that objects inside will render correctly if we want to add more
    glPopMatrix();
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        //Move camera
        case 'w':   //Forward
            if(zoom < 40)
                zoom += 1;
            //Don't allow movement towards the first model
            if(zoom > 5 && zoom < 13 && moveX < 2 && moveX > -2){
                zoom -= 1;
            }
            //Don't allow movement towards the elephant model
            if(zoom > -6 && zoom < 13 && moveX < 14 && moveX > 6){
                zoom -= 1;
            }
            //Don't allow movement towards the cow model
            if(zoom > 0 && zoom < 8 && moveX < -8 && moveX > -12){
                zoom -= 1;
            }
            //Don't allow movement towards the cube model
            if(zoom > 22 && zoom < 28 && moveX < 3 && moveX > -3){
                zoom -= 1;
            }
            //Don't allow movement towards the deer model
            if(zoom > -15 && zoom < -7 && moveX < 17 && moveX > 13){
                zoom -= 1;
            }
            //Don't allow movement towards the ven model
            if(zoom > -14 && zoom < -5 && moveX < -11 && moveX > -18){
                zoom -= 1;
            }
            cout << "zoom: " << zoom << endl;
            break;
        case 'a':   //Left
            if(moveX < 40)
                moveX += 1;
            //Don't allow movement towards the first model
            if(zoom > 5 && zoom < 13 && moveX < 2 && moveX > -2){
                moveX -= 1;
            }
            //Don't allow movement towards the elephant model
            if(zoom > -6 && zoom < 13 && moveX < 14 && moveX > 6){
                moveX -= 1;
            }
            //Don't allow movement towards the cow model
            if(zoom > 0 && zoom < 8 && moveX < -8 && moveX > -12){
                moveX -= 1;
            }
            //Don't allow movement towards the cube model
            if(zoom > 22 && zoom < 28 && moveX < 3 && moveX > -3){
                moveX -= 1;
            }
            //Don't allow movement towards the deer model
            if(zoom > -15 && zoom < -7 && moveX < 17 && moveX > 13){
                moveX -= 1;
            }
            //Don't allow movement towards the ven model
            if(zoom > -14 && zoom < -5 && moveX < -11 && moveX > -18){
                moveX -= 1;
            }
            cout << "moveX: " << moveX << endl;
            break;
        case 's':   //Back
            if(zoom > -30)
                zoom -= 1;
            //Don't allow movement towards the first model
            if(zoom > 5 && zoom < 13 && moveX < 2 && moveX > -2){
                zoom += 1;
            }
            //Don't allow movement towards the elephant model
            if(zoom > -6 && zoom < 13 && moveX < 14 && moveX > 6){
                zoom += 1;
            }
            //Don't allow movement towards the cow model
            if(zoom > 0 && zoom < 8 && moveX < -8 && moveX > -12){
                zoom += 1;
            }
            //Don't allow movement towards the cube model
            if(zoom > 22 && zoom < 28 && moveX < 3 && moveX > -3){
                zoom += 1;
            }
            //Don't allow movement towards the deer model
            if(zoom > -15 && zoom < -7 && moveX < 17 && moveX > 13){
                zoom += 1;
            }
            //Don't allow movement towards the ven model
            if(zoom > -14 && zoom < -5 && moveX < -11 && moveX > -18){
                zoom += 1;
            }
            cout << "zoom: " << zoom << endl;
            break;
        case 'd':   //Right
            if(moveX > -40)
                moveX -= 1;
            //Don't allow movement towards the first model
            if(zoom > 5 && zoom < 13 && moveX < 2 && moveX > -2){
                moveX += 1;
            }
            //Don't allow movement towards the elephant model
            if(zoom > -6 && zoom < 13 && moveX < 14 && moveX > 6){
                moveX += 1;
            }
            //Don't allow movement towards the cow model
            if(zoom > 0 && zoom < 8 && moveX < -8 && moveX > -12){
                moveX += 1;
            }
            //Don't allow movement towards the cube model
            if(zoom > 22 && zoom < 28 && moveX < 3 && moveX > -3){
                moveX += 1;
            }
            //Don't allow movement towards the deer model
            if(zoom > -15 && zoom < -7 && moveX < 17 && moveX > 13){
                moveX += 1;
            }
            //Don't allow movement towards the ven model
            if(zoom > -14 && zoom < -5 && moveX < -11 && moveX > -18){
                moveX += 1;
            }
            cout << "moveX: " << moveX << endl;
            break;
        case 'o':   //Reset Camera
            moveX = 0;
            moveY = 0;
            zoom = 0;
            break;

        //Reset Everything
        case 'e':
            rotX = 0;
            rotY = 0;
            zoom = 0;
            moveX = 0;
            moveY = 0;
            rotCamX = 0;
            rotCamY = 0;
            break;
        case 'r':
            WireFrame = !WireFrame;
            break;
    }
}

//This is for the mouse movement and coordination
void mouseMove(int x,int y)
{
    //If the old y is less than the new one, rotateX by 5
     if(prev_y < y){
        if(rotCamX <= 15){
            rotCamX += 1.5;
        }
     }
     //If the old y is greater than the new one, rotateX by -5
      if(prev_y > y){
        if(rotCamX >= -80){
            rotCamX -= 1.5;
        }
     }
     //If the old x is less than the new one, rotateY by 5
     if(prev_x < x){
        rotCamY += 3;
     }
     //If the old x is greater than the new one, rotateY by -5
      if(prev_x > x){
        rotCamY -= 3;
     }
    //Update the mouse positions
    prev_x = x;
    prev_y = y;
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_RIGHT:
        rotY += 3;
        break;
    case GLUT_KEY_LEFT:
        rotY -= 3;
        break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT));
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project 5: Model Loader");

    //We first have to read the files in order to draw them after
    cow.readFile();
    ele.readFile();
    stat.readFile();
    stat.faceNormalizeTriangles();
    cube.readFile();
    deer.readFile();
    ven.readFile();

    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    glutMotionFunc(mouseMove);
    glutIdleFunc(idle);
    glutMainLoop();;
    return EXIT_SUCCESS;
}
