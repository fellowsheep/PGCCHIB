#version 410

in vec2 texture_coords;

uniform sampler2D sprite;
uniform float offsetx;
uniform float offsety;

uniform float weight;

out vec4 frag_color; 

void main () {
    vec4 texel = mix (texture (sprite, 
        vec2(texture_coords.x + offsetx,
             texture_coords.y + offsety)), vec4(0,0,1,1), weight);
    if(texel.a < 0.5) {
        discard;
    }
    frag_color = texel;
}
