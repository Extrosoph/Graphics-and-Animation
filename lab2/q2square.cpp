/* sierpinski gasket with vertex arrays */

#include "Angel.h"

using namespace std;

const int NumTriangles = 2;
const int NumVertices  = 3 * NumTriangles;
GLint timeParam;

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
  GLuint program = InitShader( "vrotate2d.glsl", "fshader24.glsl" );
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
  /*[This is a uniform variable which means that it doesn't change during the
  drawing of a primitive, hence no array/buffer is needed unlike vPosition and
  vColor.]*/
  timeParam = glGetUniformLocation(program, "time");
}

//----------------------------------------------------------------------------
void idle(void)
{
  glutPostRedisplay();
}


//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    /*[glUniform1f sets a uniform parameter containing a single float to a value.
    Here the value is that returned by glutGet(GLUT_ELAPSED_TIME) which is the
    time (in milliseconds) since glutInit was called. See glutGet in the
    freeglut API (now in the links to the left of this page). ]*/
    glUniform1f( timeParam, glutGet(GLUT_ELAPSED_TIME) );
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glutSwapBuffers();
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
  /* [Calling glutPostRedisplay tells GLUT that the window needs to be redisplayed.
   Here we call it in the idle function because there is constant motion, so we
   want to redisplay whenever GLUT/OpenGL has nothing else to do and is idle.
   The actual redisplay will happen at some point after the idle function returns,
   when GLUT is ready to redraw the window. More generally, if you only require
   redrawing when a mouse event causes an object to move, or similar, you should
   only call glutPostRedisplay() when such a change occurs. Calling it multiple
   times is fine if multiple changes occur before a redraw happens - it just
   sets a variable that GLUT uses to remember that a redraw is required.] */

    // glutIdleFunc();glutIdleFunc();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
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
