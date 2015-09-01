#version 300 es


in highp vec3 colorPosition;
out highp vec4 color;
void main(){
    color = vec4(colorPosition,1.0f);
}