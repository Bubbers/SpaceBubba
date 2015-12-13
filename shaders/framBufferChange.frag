#version 130

precision highp float;

out vec4 fragmentColor;
uniform sampler2DRect textureImage;


void main() {
    vec3 color = texture(textureImage, gl_FragCoord.xy).xyz;
	fragmentColor = vec4(color, 1.0);
}