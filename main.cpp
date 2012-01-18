/*
Left mouse: rotate;
Right mouse:   menu;
ESC to quit
*/

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#define MAX_TRIANGLES (10)

struct Point {float x[3];   float n[3]; };
struct Triangle {   Point v[3];  };

//Triangle triangleList[MAX_TRIANGLES];
//int triangleCount = 0;
void InitGeometry();

/* Viewer state */
float sphi=90.0, stheta=45.0;
float sdepth = 5;
float zNear=1.0, zFar=100.0;
float aspect = 5.0/4.0;
float xcam = 0, ycam = 0;

long xsize, ysize;
int downX, downY;
bool leftButton = false, middleButton = false;

int i,j;
GLfloat light0Position[] = { 4, 4, 4, 1.0};

int displayMenu, mainMenu;
enum {WIREFRAME, FLATSHADED, SMOOTHSHADED, LIGHTED};
int displayMode = WIREFRAME;

void MyIdleFunc(void) { glutPostRedisplay();} /* things to do while idle */
void RunIdleFunc(void) {   glutIdleFunc(MyIdleFunc); }
void PauseIdleFunc(void) {   glutIdleFunc(NULL); }

GLfloat off[] = {0.0, 0.0, 0.0, 0.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
GLfloat deep_blue[] = {0.1, 0.5, 0.8, 1.0};
GLfloat shiny[] = {50.0};
GLfloat dull[] = {0.0};


GLfloat k;
GLfloat k2;
GLfloat mat_red_diffuse[] = { 0.9, 0.0, 0.1, 1.0 };
GLfloat mat_blue_diffuse[] = { 0.0, 0.1, 0.7, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 80.0 };
GLfloat knots1[4] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat knots2[6] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
GLfloat pts1[2][3][4], pts2[2][3][4], pts3[3][3][4];
GLUnurbsObj *nurb;
GLfloat R = 2.5; // didysis spindulys
GLfloat r = 1.5; // mazasis spindulys

void DrawFrames(void) {

    /* draw Smoth */
    glColor3f (1,0,0);

    glPushMatrix();
        glTranslatef (-0.5,-0.5,-0.5);
        glutSolidCube (0.1);

        glPushMatrix();
            glTranslatef (0.05,0,0);
            glRotatef(90,0,1,0);
            glutSolidCone (0.05,1,3,3);
        glPopMatrix();

        glPushMatrix();
            glColor3f (0,1,0);
            glTranslatef (0,0.05,0);
            glRotatef(-90,1,0,0);
            glutSolidCone (0.05,1,4,4);
        glPopMatrix();

        glPushMatrix();

            glColor3f (0,0,1);
            glTranslatef (0,0,0.05);
            glutSolidCone (0.05,1,4,4);
        glPopMatrix();
    glPopMatrix();
    //glTranslatef (1,1,1);
    glutWireCube (1);

    glPushMatrix();
        glColor3f (0,1,0);
        glTranslatef (-0.25,0.25,-0.25);
        glutWireCube(0.5);
    glPopMatrix();

    glPushMatrix();
        glTranslatef (-0.25,-0.25,-0.25);
        glutWireCube(0.5);
    glPopMatrix();

    glPushMatrix();
        glColor3f (1,0,0);
        glTranslatef (0.25,0.25,-0.25);
        glutWireCube(0.5);
    glPopMatrix();

    glPushMatrix();
        glTranslatef (0.25,-0.25,-0.25);
        glutWireCube(0.5);
    glPopMatrix();
}

void DrawQuad(float size){
    glBegin( GL_QUADS );
    glVertex3f( size, 0.0, 0.0 );
    glVertex3f( size, size, 0.0 );
    glVertex3f( 0.0, size, 0.0 );
    glVertex3f( 0.0, 0.0, 0.0 );
    glEnd();
}

void DrawFigure(float size) {
    //kaire
    glPushMatrix();
    glRotatef(-90,0,1,0);
    DrawQuad (size);
    glPopMatrix();
    //desine
    glPushMatrix();
    glRotatef(-90,0,1,0);
    glTranslatef(0.0,0.0,size);
    DrawQuad (size);
    glPopMatrix();
    //galas
    glPushMatrix();
    glRotatef(-90,0,1,0);
    glRotatef(-90,1,0,0);
    glTranslatef(0.0,-size,0.0);
    DrawQuad (size);
    glPopMatrix();
    //kairys izambainis
    glPushMatrix();
    glTranslatef(-0.5*size,0.5*size,0.0);
    glRotatef(-90,0,1,0);
    glRotatef(45,1,0,0);
    glScalef(0.8,0.5,1);
    glTranslatef(0.25,0.375,0.0);
    DrawQuad (size);
    glPopMatrix();
    //desinys izambainis
    glPushMatrix();
    glTranslatef(-0.5*size,0.5*size,0.0);
    glRotatef(-90,0,1,0);
    glRotatef(-45,1,0,0);
    glScalef(0.8,0.5,1);
    glTranslatef(0.25,0.375,0.0);
    DrawQuad (size);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-size,0.0,0.0);
    glBegin( GL_POLYGON );
    glVertex3f( 0.0, 0.0, 0.0 );
    glVertex3f( size, 0.0, 0.0 );
    glVertex3f( size, size, 0.0 );
    glVertex3f( size/2, size/2, 0.0 );
    glVertex3f( 0.0, size, 0.0 );
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-size,0.0,size);
    glBegin( GL_POLYGON );
    glVertex3f( 0.0, 0.0, 0.0 );
    glVertex3f( size, 0.0, 0.0 );
    glVertex3f( size, size, 0.0 );
    glVertex3f( size/2, size/2, 0.0 );
    glVertex3f( 0.0, size, 0.0 );
    glEnd();
    glPopMatrix();
}
void DrawNurbsCylinder(void) {
  /* cilindro ketvirtis */
  //glScalef(0.25,0.25,0.25);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      gluNurbsSurface(nurb, 4, knots1, 6, knots2,
        3 * 4, 4, &pts1[0][0][0],
        2, 3, GL_MAP2_VERTEX_4);
}
void DrawNurbsCylinder2(void) {
  /* cilindro ketvirtis */
  //glScalef(0.25,0.25,0.25);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      gluNurbsSurface(nurb, 4, knots1, 6, knots2,
        3 * 4, 4, &pts2[0][0][0],
        2, 3, GL_MAP2_VERTEX_4);
}

void DrawNurbsTorus(void) {
  /* toro 1/16 dalis */
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      gluNurbsSurface(nurb, 6, knots2, 6, knots2,
        3 * 4, 4, &pts3[0][0][0],
        3, 3, GL_MAP2_VERTEX_4);
}

void DrawWireframe(void) {
  /* draw Wireframe */
  glDisable(GL_LIGHT1);
  glEnable(GL_LIGHT0);
//  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red_diffuse);
//  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
  glMaterialfv(GL_FRONT, GL_SPECULAR, off);
  glMaterialfv(GL_FRONT, GL_SHININESS, dull);
  bool drawFrames = true;
  float sceneSize = 2.0;
  glRotatef(90,1,0,0);
  glRotatef(215,0,1,0);
  if (drawFrames)
  {
      glPushMatrix();
          glScalef(sceneSize,sceneSize,sceneSize);
          glTranslatef(0,0,0.5);
          DrawFrames();
      glPopMatrix();
  }
  glPushMatrix();
      glRotatef(90,1,0,0);
      DrawFigure(1.0);
  glPopMatrix();
  //kairys cilindras
  glPushMatrix();
      glTranslatef(-0.25,-0.25,0.25);
      glRotatef(45,0,1,0);
      glTranslatef(-0.71,0.25,0.0);
      glTranslatef(0,-0.125*R,-0.125*R);
      glScalef(0.25,0.25,0.25);
      glScalef(0.71,0.5,0.5);
      glRotatef(90,0,0,1);
      DrawNurbsCylinder ();
  glPopMatrix();

  //desinys cilindras
  glPushMatrix();
      glTranslatef(0.25,-0.25,1.25);
      glRotatef(-45,0,1,0);
      glTranslatef(-0.71,0.25,0.0);
      glTranslatef(0,-0.125*R,-0.125*R);
      glScalef(0.25,0.25,0.25);
      glScalef(0.71,0.5,0.5);
      glRotatef(90,0,0,1);
      glScalef(1,-1,1);
      DrawNurbsCylinder ();
  glPopMatrix();

  //kairys torusas
  glPushMatrix();
    glTranslatef(-0.5,0.125,1);
    glTranslatef(0,-0.125*R,-0.125*2.5);
    glRotatef(180,0,1,0);
    glScalef(0.25,0.25,0.25);
    glScalef(0.71,0.5,0.5);
    DrawNurbsTorus ();
  glPopMatrix();

  //desinys torusas
  glPushMatrix();
    glTranslatef(-0.5,0.125,1);
    glTranslatef(0,-0.125*R,-0.125*2.5);
    glScalef(-1,1,1);
    glRotatef(180,0,1,0);
    glScalef(0.25,0.25,0.25);
    glScalef(0.71,0.5,0.5);
    DrawNurbsTorus ();
  glPopMatrix();

  //kairys stacias cilindras
  glPushMatrix();
      glTranslatef(0.25,-0.6,1.25);
      glTranslatef(-0.75,0,-0.25);
      glTranslatef(0,0,-0.125*2);
      glRotatef(-90,0,1,0);
      glScalef(0.25,0.25,0.25);
      glScalef(0.5,0.8,0.5);
      glRotatef(180,0,0,1);
      DrawNurbsCylinder2 ();
  glPopMatrix();

  //desinys stacias cilindras
  glPushMatrix();
      glTranslatef(0.25,-0.6,1.25);
      glTranslatef(-0.75,0,-0.25);
      glTranslatef(0,0,-0.125*2);
      glScalef(1,1,-1);
      glRotatef(90,0,1,0);
      glScalef(0.25,0.25,0.25);
      glScalef(0.5,0.8,0.5);
      glRotatef(180,0,0,1);
      DrawNurbsCylinder2 ();
  glPopMatrix();

  //glutWireTorus(1.0, 2.0, 30, 30);
}

void DrawFlatShaded(void) {
  /* draw FlatShaded */
  glDisable(GL_LIGHT1);
  glEnable(GL_LIGHT0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
  glMaterialfv(GL_FRONT, GL_SPECULAR, off);
  glMaterialfv(GL_FRONT, GL_SHININESS, dull);
  glutSolidTorus(1.0, 2.0, 30, 30);
  //glutSolidCube(3.0);
}

void DrawSmoothShaded(void) {
  /* draw Smoth */
    glRotatef(180,1,0,0);
  glDisable(GL_LIGHT1);
  glEnable(GL_LIGHT0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
  glMaterialfv(GL_FRONT, GL_SPECULAR, off);
  glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
  glutSolidTorus(1.0, 2.0, 30, 30);
}

void DrawLighted(void) {
  /* draw Lighted by Light1 */
  GLfloat ambientIntensity[4]={0.2,0.2,0.2,1.0};
  GLfloat diffuseIntensity[4]={1.0,0.0,0.0,1.0};
  GLfloat pos[4]={1.5,1.0,1.0,0.0};
  glDisable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambientIntensity);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseIntensity);
  glLightfv(GL_LIGHT1, GL_POSITION, pos);
  glutSolidTorus(1.0, 2.0, 30, 30);
}

void DrawWireTorus(void) {
  /* vielinis toras */
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_blue_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  glutWireTorus(r - 0.01, R, 30, 30);
}

void ReshapeCallback(int width, int height) {
  xsize = width;
  ysize = height;
  aspect = (float)xsize/(float)ysize;
  glViewport(0, 0, xsize, ysize);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glutPostRedisplay();}

void SetDisplayMenu(int value) {
  displayMode = value;
  switch(value) {
    case WIREFRAME:
        glShadeModel(GL_FLAT);
        break;
    case FLATSHADED:
        glShadeModel(GL_FLAT);
        break;
    case SMOOTHSHADED:
        glShadeModel(GL_SMOOTH);
        break;
    case LIGHTED:
        glShadeModel(GL_SMOOTH);
        break;
  }
  glutPostRedisplay();
}

void SetMainMenu(int value) {switch(value) {case 99: exit(0); break;}}

void DisplayCallback(void) {

    // Cilindro ketvircio kontroliniai taskai ir svoriai

    pts1[0][0][0] = R;    pts1[0][0][1] = -0.5;   pts1[0][0][2] = 0.0;  pts1[0][0][3] = 1.0;
    pts1[0][1][0] = k*R;  pts1[0][1][1] = -k*0.5; pts1[0][1][2] = k*R;  pts1[0][1][3] = k;
    pts1[0][2][0] = k*R/2;  pts1[0][2][1] = -0.5;   pts1[0][2][2] = R;    pts1[0][2][3] = 1.0;

    pts1[1][0][0] = R;    pts1[1][0][1] = 2.0;    pts1[1][0][2] = R;  pts1[1][0][3] = 1.0;
    pts1[1][1][0] = k*R;  pts1[1][1][1] = k*2.0;  pts1[1][1][2] = k*R;  pts1[1][1][3] = k;
    pts1[1][2][0] = k*R/2;  pts1[1][2][1] = 2.0;    pts1[1][2][2] = R;    pts1[1][2][3] = 1.0;
    //Stacio cilindro ketvircio kontroliniai taskai ir svoriai

    pts2[0][0][0] = r;    pts2[0][0][1] = -2.0;   pts2[0][0][2] = 0.0;  pts2[0][0][3] = 1.0;
    pts2[0][1][0] = k*r;  pts2[0][1][1] = -k*2.0; pts2[0][1][2] = k*r;  pts2[0][1][3] = k;
    pts2[0][2][0] = k*r/2;  pts2[0][2][1] = -2.0;   pts2[0][2][2] = r;    pts2[0][2][3] = 1.0;

    pts2[1][0][0] = r;    pts2[1][0][1] = 2.0;    pts2[1][0][2] = 0.0;  pts2[1][0][3] = 1.0;
    pts2[1][1][0] = k*r;  pts2[1][1][1] = k*2.0;  pts2[1][1][2] = k*r;  pts2[1][1][3] = k;
    pts2[1][2][0] = k*r/2;  pts2[1][2][1] = 2.0;    pts2[1][2][2] = r;    pts2[1][2][3] = 1.0;
    // Toro 1/16 dalies kontroliniai taskai ir svoriai

    // GLfloat R = 3.0; // didysis spindulys
    // GLfloat r = 1.0; // mazasis spindulys
    //apacia
    pts3[0][0][0] = R-r;     pts3[0][0][1] = 0.0; pts3[0][0][2] = 0.0;     pts3[0][0][3] = 1.0;
    pts3[0][1][0] = k*(R-r); pts3[0][1][1] = 0.0; pts3[0][1][2] = k*(R-r); pts3[0][1][3] = k;
    pts3[0][2][0] = 0.0;     pts3[0][2][1] = 0.0; pts3[0][2][2] = R-r;     pts3[0][2][3] = 1.0;
    //vidurys
    pts3[1][0][0] = k*(R-r); pts3[1][0][1] = k*r; pts3[1][0][2] = 0.0;     pts3[1][0][3] = k;
    pts3[1][1][0] = k2*(R-r); pts3[1][1][1] = k2*r; pts3[1][1][2] = k2*(R-r); pts3[1][1][3] = k2;
    pts3[1][2][0] = 0.0;     pts3[1][2][1] = k*r; pts3[1][2][2] = k*(R-r); pts3[1][2][3] = k;
    //virsus
    pts3[2][0][0] = R;       pts3[2][0][1] = r;   pts3[2][0][2] = 0.0;     pts3[2][0][3] = 1.0;
    pts3[2][1][0] = k*R;     pts3[2][1][1] = k*r; pts3[2][1][2] = k*R;     pts3[2][1][3] = k;
    pts3[2][2][0] = 0.0;     pts3[2][2][1] = r;   pts3[2][2][2] = R;       pts3[2][2][3] = 1.0;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(64.0, aspect, zNear, zFar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0,0.0,-sdepth);
  glRotatef(-stheta, 1.0, 0.0, 0.0);
  glRotatef(sphi, 0.0, 0.0, 1.0);
  switch (displayMode) {
    case WIREFRAME: DrawWireframe();    break;
    case FLATSHADED: DrawFlatShaded();     break;
    case SMOOTHSHADED: DrawSmoothShaded();     break;
    //case LIGHTED: DrawLighted();     break;
    case LIGHTED:    DrawNurbsTorus();    DrawWireTorus();    break;   }
  glutSwapBuffers();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void KeyboardCallback(unsigned char ch, int x, int y) {
  switch (ch) {
      case ',':
          sdepth++;
          //sceneSize++;
          break;
      case '.':
          sdepth--;
          //sceneSize--;
          break;
      case 'R':
          R = R + 0.25;
          r = r + 0.15;
          break;
      case 'r':
          R = R - 0.25;
          r = r - 0.15;
          break;
      case 27:
          exit(0);
          break;
  }
  glutPostRedisplay();
}

void MouseCallback(int button, int state, int x, int y) {
  downX = x; downY = y;
  leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));
  middleButton = ((button == GLUT_MIDDLE_BUTTON) &&  (state == GLUT_DOWN));
  glutPostRedisplay();}

void MotionCallback(int x, int y) {
  if (leftButton){sphi+=(float)(x-downX)/4.0;stheta+=(float)(downY-y)/4.0;} // rotate
  if (middleButton){sdepth += (float)(downY - y) / 10.0;  } // scale
  downX = x;   downY = y;
  glutPostRedisplay();}

void InitGL() {
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Torus surface");
  glEnable(GL_LIGHTING); // new
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_DIFFUSE);
  glLightfv (GL_LIGHT0, GL_POSITION, light0Position);
  glEnable(GL_LIGHT0);
  glutReshapeFunc(ReshapeCallback);
  glutDisplayFunc(DisplayCallback);
  glutKeyboardFunc(KeyboardCallback);
  glutMouseFunc(MouseCallback);
  glutMotionFunc(MotionCallback);
}

void InitMenu() {
  displayMenu = glutCreateMenu(SetDisplayMenu);
  glutAddMenuEntry("Wireframe", WIREFRAME);
  glutAddMenuEntry("Flat Shaded", FLATSHADED);
  glutAddMenuEntry("Smooth Shaded", SMOOTHSHADED);
  glutAddMenuEntry("Lighted", LIGHTED);
  mainMenu = glutCreateMenu(SetMainMenu);
  glutAddSubMenu("Shading", displayMenu);
  glutAddMenuEntry("Exit", 99);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
    k = 1/sqrt(2);
    k2 = 0.5;
    nurb = gluNewNurbsRenderer();
    gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, 5.0);
    gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_FILL);
  glutInit(&argc, argv);
  InitGL();
  InitMenu();
  glutMainLoop();
}

