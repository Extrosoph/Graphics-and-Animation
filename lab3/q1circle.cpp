// Two-Dimensional Sierpinski Gasket
// Generated using randomly selected vertices and bisection

#include "Angel.h"

using namespace std;

const int NumPoints = 500000;
// GLuint sinAngle, cosAngle;
float angle = 0.001 * glutGet(GLUT_ELAPSED_TIME);
GLuint multipliers;

vec2 points[NumPoints];
vec3 colors[NumPoints];
//----------------------------------------------------------------------------

void
init( void )
{
    // Specifiy the vertices of a triangle
    vec2 vertices[3] = {
        vec2( -1.0, -1.0 ), vec2( 0.0, 1.0 ), vec2( 1.0, -1.0 )
    };

    // Select an arbitrary initial point inside of the triangle
    points[0] = vec2( 0.25, 0.50 );
    colors[0] = vec3(1.0, 0.4, 0.3);

    // compute and store N-1 new points
    for ( int i = 1; i < NumPoints; ++i ) {
        int j = rand() % 3;   // pick a vertex at random

        // Compute the point halfway between the selected vertex
        //   and the previous point
        points[i] = ( points[i - 1] + vertices[j] ) / 2.0;
        colors[i] = colors[i-1];
        vec3 newcolor = vec3(0.0, 0.0, 0.0);
        newcolor[j] = 1.0;
        colors[i] = colors[i-1] * 0.8 + newcolor*0.2;
    }

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
                  NULL, GL_STATIC_DRAW);
                  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
                  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vrotate2d.glsl", "fshader24.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(points)));


    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------
void idle(void)
{
  glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    float angle = 0.001 * glutGet(GLUT_ELAPSED_TIME);
    mat2 multipliers_mat = mat2( vec2(cos(angle), -sin(angle)),
                             vec2(sin(angle),  cos(angle))  );
    glUniformMatrix2fv( multipliers, 1, GL_TRUE, multipliers_mat );
    glDrawArrays(GL_POINTS, 0, NumPoints);
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_DOUBLE);
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
