#version 300 es
layout (location = 0) in highp vec3 position;


uniform mediump mat4 MVP;
out highp vec3 pos;
void main(){
    pos = position;
    gl_Position = MVP * vec4(position,1.0f);
}