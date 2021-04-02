/* sierpinski gasket with vertex arrays */

#include "Angel.h"

using namespace std;

const int NumTriangles = 2;
const int NumVertices  = 3 * NumTriangles;

/*Vec3 is used for squares*/
vec3 points[NumVertices] = {
    vec3(-0.5, -0.5, 0.0), vec3(-0.5, 0.5, 0.0), vec3(0.5, -0.5, 0.0),
    vec3(0.5, 0.5, 0.0),   vec3(0.5, -0.5, 0.0), vec3(-0.5, 0.5, 0.0)
};

vec3 colors[NumVertices] = {
    vec3(0.0, 1.0, 0.0) /* Green, but choose any 6 colours */,vec3(1.0,0.0,1.0), vec3(1.0,0.0,0.5),
    vec3(0.5,0.5,0.5), vec3(0.5,1.0,0.5),vec3(0.4,0.6,0.0)
};
//----------------------------------------------------------------------------

void init(void)
{
  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );

  // First, we create an empty buffer of the size we need by passing
  //   a NULL pointer for the data values
  glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
    NULL, GL_STATIC_DRAW );

  // Next, we load the real data in parts.  We need to specify the
  //   correct byte offset for placing the color data after the point
  //   data in the buffer.  Conveniently, the byte offset we need is
  //   the same as the size (in bytes) of the points array, which is
  //   returned from "sizeof(points)".
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vshader24.glsl", "fshader24.glsl" );
  glUseProgram( program );

  // Initialize the vertex position attribute from the vertex shader
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(0) );

  // Likewise, initialize the vertex color attribute.  Once again, we
  //    need to specify the starting offset (in bytes) for the color
  //    data.  Just like loading the array, we use "sizeof(points)"
  //    to determine the correct value.
  GLuint vColor = glGetAttribLocation( program, "vColor" );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(sizeof(points)) );

  // glEnable( GL_DEPTH_TEST );

  glClearColor( 1.0, 1.0, 1.0, 1.0 ); /* white background */
}

//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 033: // ASCII code for Esc
        exit(EXIT_SUCCESS);
        break;
    }
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(256, 256);
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("CITS3003 Lab 1 Q3 Triangle Scene");

    glewInit(); // optional for Linux

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
