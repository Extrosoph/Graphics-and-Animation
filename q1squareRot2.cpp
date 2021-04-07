/* sierpinski gasket with vertex arrays */

#include "Angel.h"

using namespace std;

const int NumTriangles = 12;
const int NumVertices  = 8;
const int NumElements  = 3 * NumTriangles;

// Perspective projection
mat4 projection = Frustum(-0.2, 0.2, -0.2, 0.2, 0.2, 2.0);

// Move the scene backwards relative to the camera
mat4 view = Translate(0.0, 0.0, -1.5);

int numDisplayCalls = 0;
char lab[] = "Lab5";
char programName[] = "q3callbacks";

GLint windowWidth;
GLint windowHeight;


// Each vertex now appears only once, so we have only 8 rather than 36
vec3 points[NumVertices] = {
   vec3( -0.5, -0.5, -0.5 ), vec3( -0.5, -0.5,  0.5 ),
   vec3( -0.5,  0.5, -0.5 ), vec3( -0.5,  0.5,  0.5 ),

   vec3(  0.5, -0.5, -0.5 ), vec3(  0.5, -0.5,  0.5 ),
   vec3(  0.5,  0.5, -0.5 ), vec3(  0.5,  0.5,  0.5 )
};

// The following builds triangles from the 8 vertices above,
// using numbers 0-7 to refer to the element positions in the array
GLuint elements[NumElements] = {
    1, 5, 3,
    7, 3, 5,

    0, 4, 2,
    6, 2, 4,

    4, 6, 5,
    7, 5, 6,

    0, 2, 1,
    3, 1, 2,

    2, 3, 6,
    7, 6, 3,

    0, 1, 4,
    5, 4, 1
};


// We only need to give 8 colors, one for each vertex.
vec3 colors[NumVertices] = {
  vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0),
  vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 1.0),

  vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 1.0),
  vec3(1.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0),
};

GLint multipliers;
//----------------------------------------------------------------------------

void init(void)
{

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  GLuint buffer[2];
  glGenBuffers( 2, buffer );

  glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

  // ADDED: load the element index data
  GLuint elementBufferId;
  glGenBuffers(1, &elementBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vrotate2d.glsl", "fshader24.glsl" );
  glUseProgram( program );

  // Initialize the vertex position attribute from the vertex shader
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  // Next, we set up buffer[1] for the vertex color coordinates
  glBindBuffer( GL_ARRAY_BUFFER, buffer[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW );

  GLuint vColor = glGetAttribLocation( program, "vColor" );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glClearColor( 1.0, 1.0, 1.0, 1.0 );
  multipliers = glGetUniformLocation(program, "matrix");
  glEnable( GL_DEPTH_TEST );
}
//----------------------------------------------------------------------------
void idle(void)
{
  glutPostRedisplay();
}

//----------------------------------------------------------------------------

void drawCube(mat4 model) {
  glUniformMatrix4fv(multipliers, 1, GL_TRUE, model);
  glDrawElements(GL_TRIANGLES, NumElements, GL_UNSIGNED_INT, NULL);
}

//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    float time = glutGet(GLUT_ELAPSED_TIME);
    float angleDegrees = ((0.001 * time)/3.1416)*180;

    // mat4 rotX = mat4( 1.0, 0.0, 0.0, 0.0,
    //                  0.0, cos(angle), -sin(angle), 0.0,
    //                  0.0, sin(angle), cos(angle), 0.0,
    //                  0.0, 0.0, 0.0, 1.0 );

   //  mat4 rotY= mat4( cos(angle), 0.0, sin(angle), 0.0,
   //                   0.0, 1.0, 0.0, 0.0,
   //                   -sin(angle), 0.0, cos(angle), 0.0,
   //                   0.0, 0.0, 0.0, 1.0 );
   //
   //  mat4 rotZ = mat4( cos(angle), -sin(angle), 0.0, 0.0,
   //                   sin(angle), cos(angle), 0.0, 0.0,
   //                   0.0, 0.0, 1.0, 0.0,
   //                   0.0, 0.0, 0.0, 1.0 );
   //
   // mat4 shrinkX = mat4( vec4(0.25, 0.0, 0.0, 0.0),
   //                  vec4(0.0,  1.0, 0.0, 0.0),
   //                  vec4(0.0,  0.0, 1.0, 0.0),
   //                  vec4(0.0,  0.0, 0.0, 1.0));

    mat4 first = projection * view * Translate(0.5, 0.5, 0.0) * Scale(0.3) * RotateX(angleDegrees) * RotateY(angleDegrees);
    mat4 second = projection * view * Translate(-0.5, 0.5, 0.0) * Scale(0.3) * RotateX(angleDegrees) * RotateY(angleDegrees);
    mat4 third =   projection * view * Translate(0.5, -0.5, 0.0) * Scale(0.3) * RotateX(angleDegrees) * RotateY(angleDegrees);
    mat4 last =  projection * view * Translate(-0.5, -0.5, 0.0) * Scale(0.3) * RotateX(angleDegrees) * RotateY(angleDegrees);
    drawCube(first);
    drawCube(second);
    drawCube(third);
    drawCube(last);
    numDisplayCalls += 1;


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

void myreshape(int width, int height)
{
  windowWidth = width;
  windowHeight = height;
   glViewport(0, 0, windowWidth, windowHeight);
   projection = Frustum(-0.2*(float)width/(float)height,
                        0.2*(float)width/(float)height,
                        -0.2, 0.2, 0.2, 2.0);
}

//----------------------------------------------------------------------------

void mymouse(int xPixel, int yPixel)
{
      GLfloat x = (GLfloat)xPixel/(GLfloat)windowWidth - 0.5;
      GLfloat y = 0.5 - (GLfloat)yPixel/(GLfloat)windowHeight;

      view = Translate(x, y, -1.5);
}

//----------------------------------------------------------------------------

void timer(int unused)
{

    char title[256];
    sprintf(title, "%s %s: %d Frames Per Second @ %d x %d",
            lab, programName, numDisplayCalls, windowWidth, windowHeight );

    glutSetWindowTitle(title);

    numDisplayCalls = 0;
    glutTimerFunc(1000, timer, 1);
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
    glutReshapeFunc(myreshape);
    glutMotionFunc(mymouse);
    glutTimerFunc(1000, timer, 1);
    glutMainLoop();

    return 0;
}
