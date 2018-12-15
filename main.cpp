/*
 *  Metaballs Final Project - Anthony Keydel
 *  
 */
#include "CSCIx229.h"
#include "marchCubes.cpp"
#include "colors.h"


//  Globals
int th=90;       // Azimuth of view angle
int ph=15;       // Elevation of view angle
int mode=0;     // Mode (only 1 for now)
int debug = 0;
int animate = 1;
int wireFrame = 0;
int lighting = 1;
float gTime;
int iColor;
gVertex color;

float fov = 100;
float asp = 1.25;
float dim=5;   // Dimension of view (used for zoom)

float size = 30;

float threshold = 10;
float valA =0;

float ballX, ballY, ballZ;
float ballSize = 3;


/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Display the scene
 */
void display()
{
   if(lighting){
         glEnable(GL_LIGHTING);
   }
   else{
      glDisable(GL_LIGHTING);
   }
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //  Reset previous transforms
   glLoadIdentity();

   float Ex = -2*dim*Sin(th)*Cos(ph);
   float Ey = +2*dim        *Sin(ph);
   float Ez = +2*dim*Cos(th)*Cos(ph);
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   glutWireCube(10.0); 


   glPushMatrix();
   //  Offset
   glTranslated(-5,-5,-5);

    if(animate){
      setMetaBall(0, 5, 5+(4*Sin(gTime)), 5, 3);
      setMetaBall(1, 5+5*Cos(gTime), 4+2*Sin(gTime), 5+5*Sin(gTime), 2);
      setMetaBall(2, 5+4*Sin(gTime*5), 4, 5+4*Cos(gTime*5), 1);
    }
    else{
         setMetaBall(0, ballX, ballY, ballZ, 3);
    }

      switch (mode){
         case 0:{
            //setMetaBall(0, ballX, ballY, ballZ, 3);

         }
      }

   glScalef(10,10,10);
   glBegin(GL_TRIANGLES);

   //Loop through grid points
   for(float i=0.0; i<size; i++){
      for(float j=0.0; j<size; j++){
         for(float k=0.0; k<size; k++){
            valA = marchCubes(i/size, j/size, k/size, 1.0/size, threshold, color);
         }
      }
   }
   glEnd();

   glPopMatrix();


  if(debug){
      //  Draw axes in white
      glColor3f(1,1,1);
      glBegin(GL_LINES);
      glVertex3d(0,0,0);
      glVertex3d(1,0,0);
      glVertex3d(0,0,0);
      glVertex3d(0,1,0);
      glVertex3d(0,0,0);
      glVertex3d(0,0,1);
      glEnd();

      //  Label axes
      glRasterPos3d(1,0,0);
      Print("X");
      glRasterPos3d(0,1,0);
      Print("Y");
      glRasterPos3d(0,0,1);
      Print("Z");
      //  Display parameters
      glWindowPos2i(5,5);
      Print("Grid Density: %.0f, Threshold %.2f \n ",size, threshold);
      Print("x: %.2f, y %.2f, z %.2f,  size: %.2f",ballX,ballY,ballZ,ballSize);
  }

   //  Flush and swap
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when a key is pressed
 * 
 * TODO:
 * Make a series of menues - one button to switch between threshold, ball size, thickness, and other properties, then arrow keys to change them
 * guuuuuuessss im not doing that
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   
   //  Switch modes
    if (ch== 'l'){
       lighting = 1-lighting;
    }
   if(ch=='o'){
      debug = 1-debug;      
   }
   if(ch=='t'){
      animate = 1-animate;      
   }

   //  Increase grid density
   if (ch == '+' || ch =='='){
      if(size<100){
         size+=1;
      }
   }
   //  Decrease grid density
   if (ch == '-'){
      if(size>5){
         size-=1;
      }
   }

   //  Increase threshold
   if (ch == '8' || ch=='y'){
         threshold+=.1;
   }
   //  Decrease threshold
   if (ch == '2' || ch=='h'){
      if(threshold>.1){
         threshold-=.1;
      }
   }

   if(!animate){
         //  Increase ball size
      if (ch == '7' || ch=='u'){
         if(ballSize<50){
            ballSize+=.1;
            SetupMetaballs(ballSize);
         }
      }
      //  Decrease ball size
      else if (ch == '1' || ch=='j'){
         if(ballSize>.05){
            ballSize-=.1;
            SetupMetaballs(ballSize);
         }
      }
   }

         //  Increase fov
   if (ch == '9'){
      fov+=.5;
   }
   //  Decrease fov
   if (ch == '3'){
      fov-=.5;
   }

   if(ch=='c'){
      iColor= (iColor+1)%6;

      color.x = colorList[iColor][0];
      color.y = colorList[iColor][1];
      color.z = colorList[iColor][2];
   }
   if(ch=='v'){
      wireFrame=1-wireFrame;
      if(wireFrame){
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else{
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
   }

   if(!animate){
      if(ch=='w'){
         ballX+=.2;
      }
      if(ch=='s'){
         ballX-=.2;
      }
      if(ch=='a'){
         ballZ-=.2;
      }
      if(ch=='d'){
         ballZ+=.2;
      }
      if(ch=='z'){
         ballY+=.2;      
      }
      if(ch=='x'){
         ballY-=.2;      
      }
   }

   //  Tell GLUT it is necessary to redisplay the scene
   Project(fov,asp,dim);
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += .2;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= .2;

   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   Project(fov,asp,dim);
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (float)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   
   Project(fov,asp,dim);
   glutPostRedisplay();
}
void idle()
{
   gTime = glutGet(GLUT_ELAPSED_TIME)/100.0;
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */


int main(int argc,char* argv[])
{
  //  Initialize GLUT and set parameters
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(960,640);
   glutCreateWindow("Anthony Keydel - Final Project ");

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutIdleFunc(idle);
   glutKeyboardFunc(key);

   glEnable(GL_DEPTH_TEST); 
   glEnable(GL_LIGHTING);
   glEnable(GL_NORMALIZE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glEnable( GL_LIGHT0 ); 
   glLightfv( GL_LIGHT0, GL_AMBIENT,  propertiesAmbient); 
   glLightfv( GL_LIGHT0, GL_DIFFUSE,  propertiesDiffuse); 
   glLightfv( GL_LIGHT0, GL_SPECULAR, propertiesSpecular); 
   glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0); 
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   glMaterialfv(GL_BACK,  GL_AMBIENT,   ambientRed); 
   glMaterialfv(GL_BACK,  GL_DIFFUSE,   diffuseRed); 
   glMaterialfv(GL_FRONT, GL_AMBIENT,   ambientBlue); 
   glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuseBlue); 
   glMaterialfv(GL_FRONT, GL_SPECULAR,  specularPurple); 
   glMaterialf(GL_FRONT, GL_SHININESS, 5.0); 

   ballX = 0;
   ballY = 0;
   ballZ = 0;

   //metaball data

   SetupMetaballs(ballSize);

   Project(fov,asp,dim);

   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}
