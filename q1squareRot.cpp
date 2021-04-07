/* sierpinski gasket with vertex arrays */

#include "Angel.h"

using namespace std;

const int NumTriangles = 12;
const int NumVertices  = 3 * NumTriangles;



/*Vec3 is used for squares*/
vec3 points[NumVertices] = {
   vec3( -0.5, -0.5,  0.5 ), vec3(  0.5, -0.5,  0.5 ), vec3( -0.5,  0.5, 0.5 ),
   vec3(  0.5,  0.5,  0.5 ), vec3( -0.5,  0.5,  0.5 ), vec3(  0.5, -0.5, 0.5 ),

   vec3( -0.5, -0.5, -0.5 ), vec3(  0.5, -0.5, -0.5 ), vec3( -0.5,  0.5, -0.5 ),
   vec3(  0.5,  0.5, -0.5 ), vec3( -0.5,  0.5, -0.5 ), vec3(  0.5, -0.5, -0.5 ),

   vec3(  0.5, -0.5, -0.5 ), vec3(  0.5,  0.5, -0.5 ), vec3(  0.5, -0.5,  0.5 ),
   vec3(  0.5,  0.5,  0.5 ), vec3(  0.5, -0.5,  0.5 ), vec3(  0.5,  0.5, -0.5 ),

   vec3( -0.5, -0.5, -0.5 ), vec3( -0.5,  0.5, -0.5 ), vec3( -0.5, -0.5,  0.5 ),
   vec3( -0.5,  0.5,  0.5 ), vec3( -0.5, -0.5,  0.5 ), vec3( -0.5,  0.5, -0.5 ),

   vec3( -0.5,  0.5, -0.5 ), vec3( -0.5,  0.5,  0.5 ), vec3(  0.5,  0.5, -0.5 ),
   vec3(  0.5,  0.5,  0.5 ), vec3(  0.5,  0.5, -0.5 ), vec3( -0.5,  0.5,  0.5 ),

   vec3( -0.5, -0.5, -0.5 ), vec3( -0.5, -0.5,  0.5 ), vec3(  0.5, -0.5, -0.5 ),
   vec3(  0.5, -0.5,  0.5 ), vec3(  0.5, -0.5, -0.5 ), vec3( -0.5, -0.5,  0.5 ),
};

vec3 colors[NumVertices] = {
  vec3( 1.0, 0.5,  0.5 ), vec3(  0.5, 0.2,  0.5 ), vec3( 0.5,  0.5, 0.5 ),
  vec3(  0.5,  0.5,  0.5 ), vec3( 0.1,  0.5,  0.5 ), vec3(  0.5, 0.4, 0.5 ),

  vec3( 0.7, 0.4, 0.3 ), vec3(  0.5, 0.7, 0.7 ), vec3( 0.9,  0.5, 0.8 ),
  vec3(  0.5,  0.5, 0.55 ), vec3( 0.46,  0.5, 0.6 ), vec3(  0.5, 0.33, 0.78 ),

  vec3( 1.0, 0.5,  0.5 ), vec3(  0.5, 0.2,  0.5 ), vec3( 0.5,  0.5, 0.5 ),
  vec3(  0.5,  0.5,  0.5 ), vec3( 0.1,  0.5,  0.5 ), vec3(  0.5, 0.4, 0.5 ),

  vec3( 0.7, 0.4, 0.3 ), vec3(  0.5, 0.7, 0.7 ), vec3( 0.9,  0.5, 0.8 ),
  vec3(  0.5,  0.5, 0.55 ), vec3( 0.46,  0.5, 0.6 ), vec3(  0.5, 0.33, 0.78),

  vec3( 1.0, 0.5,  0.5 ), vec3(  0.5, 0.2,  0.5 ), vec3( 0.5,  0.5, 0.5 ),
  vec3(  0.5,  0.5,  0.5 ), vec3( 0.1,  0.5,  0.5 ), vec3(  0.5, 0.4, 0.5 ),

  vec3( 0.7, 0.4, 0.3 ), vec3(  0.5, 0.7, 0.7 ), vec3( 0.9,  0.5, 0.8 ),
  vec3(  0.5,  0.5, 0.55 ), vec3( 0.46,  0.5, 0.6 ), vec3(  0.5, 0.33, 0.78),
};

mat3 shrinkX =
        mat3( vec3(0.25, 0.0, 0.0),
              vec3(0.0,  1.0, 0.0),
              vec3(0.0,  0.0, 1.0)  );

GLint multipliers;
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

  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vrotate2d.glsl", "fshader24.glsl" );
  glUseProgram( program );

  // Initialize the vertex position attribute from the vertex shader
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(0) );

  GLuint vColor = glGetAttribLocation( program, "vColor" );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(sizeof(points)) );

  glClearColor( 1.0, 1.0, 1.0, 1.0 ); /* white background */
  multipliers = glGetUniformLocation(program, "matrix");
  glEnable( GL_DEPTH_TEST );
}
//----------------------------------------------------------------------------
void idle(void)
{
  glutPostRedisplay();
}


//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    float time = glutGet(GLUT_ELAPSED_TIME);
    float angle = 0.001 * time;

    mat4 rotX = mat4( 1.0, 0.0, 0.0, 0.0,
                     0.0, cos(angle), -sin(angle), 0.0,
                     0.0, sin(angle), cos(angle), 0.0,
                     0.0, 0.0, 0.0, 1.0 );

    mat4 rotY= mat4( cos(angle), 0.0, sin(angle), 0.0,
                     0.0, 1.0, 0.0, 0.0,
                     -sin(angle), 0.0, cos(angle), 0.0,
                     0.0, 0.0, 0.0, 1.0 );

    mat4 rotZ = mat4( cos(angle), -sin(angle), 0.0, 0.0,
                     sin(angle), cos(angle), 0.0, 0.0,
                     0.0, 0.0, 1.0, 0.0,
                     0.0, 0.0, 0.0, 1.0 );

     mat4 shrinkX = mat4( vec4(0.25, 0.0, 0.0, 0.0),
                      vec4(0.0,  1.0, 0.0, 0.0),
                      vec4(0.0,  0.0, 1.0, 0.0),
                      vec4(0.0,  0.0, 0.0, 1.0));

    mat4 current = (rotY * shrinkX) * rotX;
    glUniformMatrix4fv(multipliers, 1, GL_TRUE, current);
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
    glutInitDisplayMode( GLUT_RGBA| GLUT_DEPTH );
    glutInitWindowSize(256, 256);
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("CITS3003 Lab 1 Q3 Triangle Scene");

    glewInit(); // optional for Linux

    init();

    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}
