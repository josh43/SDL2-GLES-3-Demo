#version 300 es

in highp vec3 theNorm;
in highp vec2 theCoord;

out highp vec4 color;

uniform mediump vec3 sunDirection;
uniform highp sampler2D material_texture_diffuse1;


void main(){
    /*
    mediump float brightNess = dot(sunDirection,theNorm);
    if(brightNess < 0.0f)
        brightNess = .40f;
    else
        brightNess+= .40;
    
    mediump float lightComponent = clamp(brightNess,0.4f,1.0f);
     
    mediump vec4 col1 = texture(material_texture_diffuse1, theCoord);
    if(theCoord.x == 0.0f && theCoord.y == 0.0f){
        col1 = vec4(.8f,1.0f,1.0f,1.0f);
    }
     */
    color = texture(material_texture_diffuse1,theCoord);
    //color =  texture(material_texture_diffuse1,theCoord);
    
}