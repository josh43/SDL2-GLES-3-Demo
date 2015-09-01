//
//  JObjectLoader.cpp
//  ___PROJECTNAME___
//
//  Created by McCloskey, Joshua R - (josh43) on 8/6/15.
//
//

#include "JObjectLoader.h"
void JMesh::loadToGPU(GLuint shaderProgramID){
    
    glUseProgram(shaderProgramID);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
   // std::vector<glm::vec3> positions;
    //for(int i =0 ;i < this->verts.size();i++){
     //   positions.push_back(verts[i].position);
   // }
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*verts.size(),&verts[0],GL_STATIC_DRAW);
   
    
    glEnableVertexAttribArray(0); // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

    
    glEnableVertexAttribArray(1); // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(glm::vec3)));
    
    glEnableVertexAttribArray(2); //texCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(glm::vec3)*2));
    
    
    glGenBuffers(1,&ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GL_UNSIGNED_INT) * indices.size(),&indices[0],GL_STATIC_DRAW);
    

   glBindVertexArray(0);
    
    bindPoints[VERTEX] = 0;
    bindPoints[NORMAL] = 1;
    bindPoints[TEXCOORD] = 2;
    

    //glUseProgram(1);
   // glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT,0);

    
    assertBuffers();
    assertAttributeLocations(shaderProgramID);
    //assertBufferData();
    
}
void JMesh::assertBuffers(){
    assert(glIsBuffer(vbo));
    assert(glIsBuffer(ibo));
    assert(glIsVertexArray(vao));
    
}
void JMesh::assertBufferData(){
   
    
}
void JMesh::assertAttributeLocations(GLuint programID){

    GLint activeAttribs, activeShaders;
    glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &activeAttribs);
   // assert(activeAttribs>8);
    glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &activeAttribs);

    glGetProgramiv(programID, GL_ATTACHED_SHADERS, &activeShaders);
    assert(activeShaders == 2);
    GLint toDelete;
    glGetProgramiv(programID, GL_DELETE_STATUS, &toDelete); // If the program is about to be deleted
    assert(toDelete == GL_FALSE);//                            it will be flagged as gl_true we dont want that
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    GLuint locs[3];
    GLuint posLocation = glGetAttribLocation(programID, "position");
    GLuint normalLocation = glGetAttribLocation(programID, "normal");
    GLuint texLocation = glGetAttribLocation(programID, "texCoord");
    
    char * name1 = new char[1000];
    char * name2 = new char[1000];
    char * name3 = new char[1000];
    int actual[3];
    int sizeOfVar[3];
    GLenum typeOfVar[3];
    // Might not actually be the same
    glGetActiveAttrib(programID, posLocation, 1000, &actual[0], &sizeOfVar[0], &typeOfVar[0], name1);
    glGetActiveAttrib(programID, normalLocation, 1000, &actual[1], &sizeOfVar[1], &typeOfVar[1], name2);
    glGetActiveAttrib(programID, texLocation, 1000, &actual[2], &sizeOfVar[2], &typeOfVar[2], name3);
    std::string pos = "position";
    std::string nor = "normal";
    
    std::string tex = "texCoord";
    
   //// assert(typeOfVar[0] == GL_FLOAT_VEC3);
   // assert(typeOfVar[1] == GL_FLOAT_VEC2);
   // assert(typeOfVar[2] == GL_FLOAT_VEC3);

   
//    assert(pos.compare(name1)==0);
  //  assert(nor.compare(name2)==0);
    //assert(tex.compare(name3)==0);
    
    
    
    delete name1;
    delete name2;
    delete name3;
}
void JMesh::drawMe(GLuint programID,GLuint theTexID){
    // This method is highly inefficient and I should consider something else
    // BUT NAAAAAA I NO WANNA :|||
   

    GLint texLoc = glGetUniformLocation(programID,"material_texture_diffuse1");
    glUniform1i(GL_TEXTURE_2D,0);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, theTexID );
    
    
    GLuint vertSize = sizeof(Vertex);
    assert(vertSize = (sizeof(glm::vec3) + sizeof(glm::vec3)+sizeof(glm::vec2)));
    glBindVertexArray(vao);
    assert(glIsVertexArray(vao));


    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT,0);

    glBindVertexArray(0);
}
void Character::loadMeshToGPU(GLuint shaderID){
    for(auto m1 : jMeshes)
        m1->loadToGPU(shaderID);
}
Character::Character(std::vector<HelperMesh> &meshes){
    for(auto mesh : meshes){
        JMesh * toAdd = new JMesh();
        
        toAdd->verts.reserve(mesh.indices.size());
        for(int i =0; i < mesh.positions.size(); i ++){
            Vertex vert;
            vert.position =std::move(mesh.positions[i]);
            vert.normal =std::move(mesh.normals[i]);
            vert.texCoord =std::move(mesh.textureCoords[i]);
            toAdd->verts.push_back(vert);
        }
        toAdd->indices = std::move(mesh.indices);
        jMeshes.push_back(toAdd);
        
    }
}
void Character::drawMe(GLuint programID,GLuint theTexID){
    for(auto mesh : jMeshes){
        mesh->drawMe(programID,theTexID);
    }
}
Character::~Character(){
    if(jMeshes[0] != nullptr){
        for(auto mesh : jMeshes){
            delete mesh;
        }
        jMeshes.clear();
    }
}

Character  * JObjectLoader::loadMesh(const std::string &fileName){
    std::ifstream stream;
    stream.open(fileName);
    if(stream.is_open() == 0){
        exit(0);
    }
    stream.ignore(150,'\n');
    stream.ignore(150,'\n');
    
    std::string materialLocation;
    std::string info, garbage;
    
    stream >> info >> materialLocation;
    if(info != "mtllib")
        exit(0);
    
    stream.ignore(150,'\n');
    stream >> garbage;
    std::vector<HelperMesh> meshList;

    while(!stream.eof()){
        meshList.push_back(getMesh(stream));
    }
    stream.close();
    
    
    std::ifstream matStream;
    matStream.open(materialLocation);
    if(matStream.is_open() == 0){
        exit(0);
    }
    
    
 
    Character * toReturn = new Character(meshList);
  
    return toReturn;
}

static int getIntR(char c){
    return (int)(c - 48);
}
static std::string removeDotFileExt(std::string &str){
    int i;
    for( i =0; i < str.size(); i++){
        if(str[i] == '.')
            break;
    }
    return str.substr(0,i+1);
    
}
void JObjectLoader::addToIndices(std::string & s1, std::string &s2, std::string &s3, std::vector<GLuint> & vec){
    GLuint index =0;
    for(int i =0; i < s1.size(); i++){
        if(s1[i] == '/'){
            vec.push_back(index-1);
            index = 0;
        }else{
            index *= 10;
            index += getIntR(s1[i]);
        }
    }
    vec.push_back(index-1);
    index =0;
    for(int i =0; i < s2.size(); i++){
        if(s2[i] == '/'){
            vec.push_back(index-1);
            index = 0;
        }else{
            index *= 10;
            index += getIntR(s2[i]);
        }
    }
    vec.push_back(index-1);
    index = 0;
    for(int i =0; i < s3.size(); i++){
        if(s3[i] == '/'){
            vec.push_back(index-1);
            index = 0;
        }else{
            index *= 10;
            index += getIntR(s3[i]);
        }
    }
    vec.push_back(index-1);

}

HelperMesh JObjectLoader::getMesh(std::ifstream & stream){
    std::string garbage, info;
    if(stream.eof()){
        exit(0);
    }
    
    
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec2> textureCoords;
    std::vector<GLuint> indices;
    
    HelperMesh mesh;
    stream >> mesh.meshName;
    stream.ignore(150,'\n');

    
    
    bool nextMesh =false;
    do{
        stream>> info;
        if(stream.eof())
            break;
        switch(getTypeID(info)){
            case VERTEX:{
                glm::vec3 v;
                stream >> v.x >> v.y >> v.z;
                positions.push_back(v);
                stream.ignore(150,'\n');
                break;
            }case NORMAL:{
                glm::vec3 v;
                stream >> v.x >> v.y >> v.z;
                normals.push_back(v);
                stream.ignore(150,'\n');
                break;
            }case TEX_COORD:{
                glm::vec2 v;
                stream >> v.x >> v.y;
                textureCoords.push_back(v);
                stream.ignore(150,'\n');
                break;
            }case INDEX: {
                std::string s1,s2,s3;
                stream >> s1 >> s2 >> s3;
                addToIndices(s1,s2,s3,indices);
                stream.ignore(150,'\n');
                break;
            }case TANGENT:{
                glm::vec3 v;
                stream >> v.x >> v.y >> v.z;
                tangents.push_back(v);
                stream.ignore(150,'\n');
                break;
            }case MATERIAL:{
                stream >> info;
                mesh.materialName = info;
                stream.ignore(150,'\n');
                stream.ignore(150,'\n');
                break;
            }case OBJECT:{
                nextMesh = true;
                break;
            }case -1:{
                // EOF
                nextMesh =true;
                break;
            }
        }
    }while(!stream.eof() && !nextMesh);
    
    std::vector<glm::vec3> newPos, newNormals, newTangents;
    std::vector<glm::vec2> newTexCoords;
    newPos.reserve(indices.size());
    newNormals.reserve(indices.size());
    newTangents.reserve(indices.size());
 //   newTexCoords.reserve(indices.size()/3);
    
    for(int i =0; i < indices.size(); i+=9){
        newPos.push_back(positions[indices[i]]);
        newPos.push_back(positions[indices[i+3]]);
        newPos.push_back(positions[indices[i+6]]);
   
        
        newTexCoords.push_back(textureCoords[indices[i+1]]);
        newTexCoords.push_back(textureCoords[indices[i+4]]);
        newTexCoords.push_back(textureCoords[indices[i+7]]);
        
        newNormals.push_back(normals[indices[i+2]]);
        newNormals.push_back(normals[indices[i+5]]);
        newNormals.push_back(normals[indices[i+8]]);
    }
    std::vector<GLuint> temp;
    for(int i = 0; i < indices.size()/3; i ++){
        temp.push_back(i);
    }
    mesh.indices = std::move(temp);
    mesh.positions = newPos;
    mesh.textureCoords = newTexCoords;
    mesh.normals = newNormals;
    // ERROR NOT EVEN SETTING VERTEs
    
    
    return mesh;
    
}