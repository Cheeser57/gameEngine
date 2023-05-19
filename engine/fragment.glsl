#version 330

uniform sampler2D textureMap0;

in vec2 iTexCoord;
out vec4 pixelColor;

void main(void) {
	pixelColor=texture(textureMap0,iTexCoord);
}
