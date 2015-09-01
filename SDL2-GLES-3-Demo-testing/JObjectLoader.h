//
//  JObjectLoader.h
//  ___PROJECTNAME___
//
//  Created by McCloskey, Joshua R - (josh43) on 8/6/15.
//
//

#ifndef _____PROJECTNAME_____JObjectLoader__
#define _____PROJECTNAME_____JObjectLoader__



#endif /* defined(_____PROJECTNAME_____JObjectLoader__) */

#include <vector>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include <iostream>
#include <fstream>
#include <string>
#include "glm/glm.hpp"
#include <map>


struct HelperMesh;

struct Vertex{
    
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    // glm::vec3 *tangent;
};
class JMesh{
public:
   
    enum locations{
        VERTEX,
        NORMAL,
        TANGENT,
        TEXCOORD,
        BONES,
    };
    void assertBuffers();
    void assertBufferData();
    void drawMe(GLuint programID,GLuint texID);
    std::map<locations, GLuint> bindPoints;
    void loadToGPU(GLuint shadeID);
    void assertAttributeLocations(GLuint shadeID);
    std::vector<Vertex> verts;
    std::vector<GLuint> indices;
    GLuint vao,vbo,ibo, texID;
};
class Character{
    
public:
    Character(std::vector<HelperMesh> &meshes);
    ~Character();
    inline const std::vector<JMesh *> &getMeshes(){
        return jMeshes;
    }
    void drawMe(GLuint programID,GLuint texID);
    void loadMeshToGPU(GLuint shadeID);
    std::string characterName;

private:
    std::vector<JMesh *> jMeshes;
   
    
    
};

struct GameCharacter{
    std::string charName;
    glm::mat4 modelMatrix;
};
struct HelperMesh{
    std::string meshName;
    std::string materialName;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec2> textureCoords;
    std::vector<GLuint> indices;
    
};
class JObjectLoader{
    enum{
        VERTEX,
        NORMAL,
        TANGENT,
        TEX_COORD,
        INDEX,
        OBJECT,
        MATERIAL,
        COUNT
    };
    static inline int getTypeID(std::string & input){
        if(input == "v")
            return VERTEX;
        if(input == "vt")
            return TEX_COORD;
        if(input == "vn")
            return NORMAL;
        if(input == "f")
            return INDEX;
        if(input == "o")
            return OBJECT;
        if(input =="usemtl")
            return MATERIAL;
        
        return -1;
    }
    static void addToIndices(std::string & s1, std::string &s2, std::string &s3, std::vector<GLuint> & vec);
    static HelperMesh getMesh(std::ifstream &stream);

    static void loadMaterials(std::ifstream &stream, GLuint texture);
public:
    
    static Character * loadMesh(const std::string &fileName);
    
};


