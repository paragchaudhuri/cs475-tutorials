#version 330

in vec3 normal;
in vec4 eye;
in vec4 COLOR;

uniform mat4 viewMatrix;

out vec4 frag_color;

void main () 
{
  // Defining Materials
  vec4 diffuse = vec4(0.5, 0.0, 0.0, 1.0); 
  vec4 ambient = vec4(0.1, 0.0, 0.0, 1.0);
  vec4 specular = vec4(1.0, 0.5, 0.5, 1.0);
  float shininess = 10.0;
  vec4 spec = vec4(0.0); 

  // Defining Light 
  vec4 lightPos = vec4(1.0, 1.0, 1.0, 0.0);
  vec3 lightDir = vec3(viewMatrix * lightPos);  // Transforms with camera
  lightDir = normalize( vec3(lightDir));  

  //Diffuse
  vec3 n = normalize(vec3(normal));	     
  float dotProduct = dot(n, lightDir);
  float intensity =  max( dotProduct, 0.0);

  // Compute specular component only if light falls on vertex
  if(intensity > 0.0)
  {
	vec3 e = normalize(vec3(eye));
	vec3 h = normalize(lightDir + e );
   	float intSpec = max(dot(h,n), 0.0);	
        spec = specular * pow(intSpec, shininess);
  }  

  vec4 color = max((intensity * diffuse  + spec)*COLOR, ambient); // All
 //vec4 color = intensity * diffuse; // Only Diffuse  
  frag_color = color;

}
