layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];
uniform int layer;

in VertexData
{
    vec2 TexCoord;
    vec3 FragPos;
    vec3 Normal;
} inData[];

out vec4 FragPos;
 
void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face + 6 * layer; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            FragPos = vec4(inData[i].FragPos,1);
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  