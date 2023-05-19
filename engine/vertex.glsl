#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec2 texCoord;

out vec2 iTexCoord;

void main(void) {
    iTexCoord = texCoord;
    gl_Position=P*V*M*vertex;
}
