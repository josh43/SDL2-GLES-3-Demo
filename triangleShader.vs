#version 300 es
layout(location = 0) in highp vec3 position;

out highp vec3 colorPosition;
void main(){
    colorPosition = position;
    gl_Position = vec4(position,1.0f);
}