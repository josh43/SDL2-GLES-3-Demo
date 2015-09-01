#version 300 es


in highp vec3 pos;
out highp vec4 color;
void main(){
    
    color = vec4(pos,1.0f);

}