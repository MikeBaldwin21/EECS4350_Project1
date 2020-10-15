#version 330 core
layout (location = 0) in vec4 aPos;

uniform mat4 modelingTransform = {{1.0, 0.0, 0.0, 0.0},
                                  {0.0, 1.0, 0.0, 0.0},
								  {0.0, 0.0, 1.0, 0.0},
								  {0.0, 0.0, 0.0, 1.0}};
uniform mat4 projectionTransform = {{1.0, 0.0, 0.0, 0.0}, 
                                    {0.0, 1.0, 0.0, 0.0}, 
									{0.0, 0.0, 1.0, 0.0}, 
									{0.0, 0.0, 0.0, 1.0}};
uniform vec3 eyePosition = { 0.0, 0.0, 10.0};
uniform int materialID = 0;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, aPos.w);

	// do the projection thing now...
	gl_Position = projectionTransform * modelingTransform * aPos/aPos.w;
}