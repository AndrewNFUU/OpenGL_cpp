#include <freeglut.h>
#include <GL/gl.h>
#include <cmath>

const double PI = 3.14;

bool animation = true;

bool isJumping = false;
bool up = true;
double jumpDirection = 0.0;
double jumpSpeed = 0.08;
double jumpLimit = 1.5;

double eyeX = 5.0;
double eyeY = 5.0;
double eyeZ = 2.0;

double bigX = 0.5;
double bigY = 0.5;
double bigZ = 0.5;

double zoomFactor = 1.0;

double centerX = 0.0;
double centerY = 0.0;
double centerZ = 0.0;

double upX = 0.0;
double upY = 0.0;
double upZ = 1.0;

double rotateAngle = 45.0;
double orbitAngle = 0.0;
double rotationSpeed = 0.5;
double rotationOrbit = 2.0;

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_FLAT);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLoadIdentity(); // clear matrix

    gluLookAt(eyeX, eyeY, eyeZ,
              centerX, centerY, centerZ,
              upX, upY, upZ);

    glPushMatrix();

    glTranslated(bigX, bigY, bigZ);
    glRotated(rotateAngle, 0.0, 0.0, 1.0);
    glScaled(zoomFactor, zoomFactor, zoomFactor);

    glColor3d(1.0, 0.0, 0.0);

    // draw cube manually
    glBegin(GL_QUADS);

    // front face
    glVertex3d(-0.5, -0.5, 0.5);
    glVertex3d(0.5, -0.5, 0.5);
    glVertex3d(0.5, 0.5, 0.5);
    glVertex3d(-0.5, 0.5, 0.5);

    // back face
    glVertex3d(-0.5, -0.5, -0.5);
    glVertex3d(0.5, -0.5, -0.5);
    glVertex3d(0.5, 0.5, -0.5);
    glVertex3d(-0.5, 0.5, -0.5);

    // top face
    glVertex3d(-0.5, 0.5, -0.5);
    glVertex3d(0.5, 0.5, -0.5);
    glVertex3d(0.5, 0.5, 0.5);
    glVertex3d(-0.5, 0.5, 0.5);

    // bottom face
    glVertex3d(-0.5, -0.5, -0.5);
    glVertex3d(0.5, -0.5, -0.5);
    glVertex3d(0.5, -0.5, 0.5);
    glVertex3d(-0.5, -0.5, 0.5);

    // right face
    glVertex3d(0.5, -0.5, -0.5);
    glVertex3d(0.5, 0.5, -0.5);
    glVertex3d(0.5, 0.5, 0.5);
    glVertex3d(0.5, -0.5, 0.5);

    // left face
    glVertex3d(-0.5, -0.5, -0.5);
    glVertex3d(-0.5, 0.5, -0.5);
    glVertex3d(-0.5, 0.5, 0.5);
    glVertex3d(-0.5, -0.5, 0.5);

    glEnd();

    glPopMatrix();

    glPushMatrix();

        double smallX = 1.5 * cos(orbitAngle * PI / 180.0);
        double smallY = 1.5 * sin(orbitAngle * PI / 180.0);
        double smallZ = 0.3 + jumpDirection;

        glTranslated(smallX, smallY, smallZ);
        glRotated(rotationSpeed, 0.0, 1.0, 0.0);
        glScaled(0.5, 0.5, 0.5);
        glColor3d(0.0, 0.0, 1.0);
        glutWireCube(1.0);

    glPopMatrix();

    glutSwapBuffers();
}


void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, 1.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y) {

    switch (key) {

    case 'w':
        bigZ += rotationSpeed;
        glutPostRedisplay();
        break;

    case 's':
        bigZ -= rotationSpeed;
        glutPostRedisplay();
        break;

    case 'q':
        bigX -= rotationSpeed;
        glutPostRedisplay();
        break;

    case 'a':
        bigX += rotationSpeed;
        glutPostRedisplay();
        break;

    case 'e':
        bigY -= rotationSpeed;
        glutPostRedisplay();
        break;

    case 'd':
        bigY += rotationSpeed;
        glutPostRedisplay();
        break;

    case ' ':
        animation = !animation;
        break;

    case 27:
        exit(0);
    }
}


void specialKeyDown(int key, int x, int y) {

    switch (key) {

    case GLUT_KEY_SHIFT_L:
        zoomFactor = 2.5;
        glutPostRedisplay();
        break;

    case GLUT_KEY_CTRL_L:
        isJumping = true;
        break;

    case GLUT_KEY_RIGHT:
        centerY += 2.0;
        upY += 2.0;
        break;

    case GLUT_KEY_LEFT:
        centerY -= 2.0;
        upY -= 2.0;
        break;

    case GLUT_KEY_UP:
        eyeZ += 5.0;
        upZ += 10.5;
        break;

    case GLUT_KEY_DOWN:
        eyeZ -= 5.0;
        upZ -= 10.5;
        break;
    }
    glutPostRedisplay();
}

void specialKeyUp(int key, int x, int y) {
    switch (key) {

    case GLUT_KEY_SHIFT_L:
        zoomFactor = 1.0;
        glutPostRedisplay();
        break;

    case GLUT_KEY_CTRL_L:
        if (glutGetModifiers() != GLUT_ACTIVE_CTRL) {
            isJumping = false;
            jumpDirection = 0.0;
            glutPostRedisplay();
        }
        break;
    }
}

void update(int value) {

    if (!animation) {
        rotateAngle += rotationSpeed;
        orbitAngle += rotationOrbit;
        if (orbitAngle >= 360) {
            orbitAngle = 0.0;
        }
    }

    if (isJumping) {
        if (up) {
            jumpDirection += jumpSpeed;
            if (jumpDirection >= jumpLimit) {
                up = false;
            }
        }
        else {

            jumpDirection -= jumpSpeed;
            if (jumpDirection <= 0) {
                jumpDirection = 0.0;
                up = true;
                isJumping = false;
            }
        }
    }

    glutPostRedisplay(); // Schedule display update
    glutTimerFunc(10, update, 0); // Call update again in 10 ms
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3d Animation");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    init();

    update(5);
    glutMainLoop();

    return 0;
}