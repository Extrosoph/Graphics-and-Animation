#version 150

in  vec3 vPosition;
in  vec3 vColor;
out vec4 color;

uniform float time;      /* in milliseconds */

vec4 rotation(int opt, vec3 vPosition, float angle){

  switch (opt) {
  	case 0:
    // X rotation
    return vec4(
    vPosition.x,
    vPosition.y*cos(angle) - vPosition.z*sin(angle),
    vPosition.y*sin(angle) + vPosition.z*cos(angle),
    1.0);

      break;
  	case 1:
  	 // Y rotation
  	 return vec4(
  	 vPosition.x*cos(angle) + vPosition.z*sin(angle),
  	 vPosition.y,
  	 -vPosition.x*sin(angle) + vPosition.z*cos(angle),
  	 1.0);

      break;
  	case 2:
     
        // Z rotation
	  return vec4(
	  vPosition.x*cos(angle) - vPosition.y*sin(angle),
	  vPosition.x*sin(angle) + vPosition.y*cos(angle),
	  0.0,
	  1.0);
      break;
      }
}


//Increasing the angle result in increasing speed of rotation
void main()
{
  float angle = 0.001*time;

	//first number determines rotation
  gl_Position = rotation(2, vPosition, angle);

  color = vec4( vColor, 1.0 );
}






