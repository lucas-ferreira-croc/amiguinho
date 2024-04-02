#version 330

out vec4 frag_color;

in vec2 TexCoord;

uniform sampler2D theTexture;

void main()
{
	frag_color = texture(theTexture, TexCoord);
}															     