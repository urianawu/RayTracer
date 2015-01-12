// =============================================================================
// Template Program for VIZA654/CSCE646 at Texas A&M University
// Created by Ariel Chisholm
// 09.01.2009
//
// This file is supplied with an associated makefile. Put both files in the same
// directory, navigate to that directory from the Linux shell, and type 'make'.
// This will create a program called 'ppmview' that you can run by entering
// 'ppmview' as a command in the shell. The program will display a small white
// pixmap. Clicking on the pixmap display window will close the program.
//
// This template program is designed to help you get jump-started on your first
// ppmview assignment. If you are new to programming in Linux, there is an
// excellent introduction to makefile structure and the gcc compiler here:
//
// http://www.cs.txstate.edu/labs/tutorials/tut_docs/Linux_Prog_Environment.pdf
//
// For viewing and editing code, Gedit is a good all-purpose text editor
// available on the lab's Linux systems. You can find it on the main GNOME
// menu under Debian>Apps>Editors.
// =============================================================================

// =============================================================================
// [Project 3]
// [You Wu]
// [10/10/2013]
// [It runs on MacOS 10.8.5. Put both files in the same directory, navigate to that directory from the Terminal, and type 'make'.This will create a program called 'ppmview'.]

//
// Always put a comment block like this at the top of your source files. You can
// see in the prior comment block my own use of this structure to document the
// origin and purpose of this template file.
// =============================================================================

// =============================================================================
// For all your projects, please adopt a consistent, readable programming style.
// Specifically pay attention to your maximum line lengths and indentation
// levels. A variable maximum line length or indentation level will almost
// always make your code unnecessarily difficult to read.
//
// Google (you may have heard of them) wisely mandates in their own C++ style
// guide a maximum line length of 80 characters, and a indentation level of two
// spaces (no tabs, just as in this document). This a very good coding style
// and I recommend you adopt it unless you have already developed a readable and
// consistent style of your own.
// =============================================================================
#include <cstdlib>
#include <iostream>
#include <sstream>
//#include <GLUT/glut.h>
//#include <openGL/glu.h>
#include <cstring>
#include "ppmview.h"

using namespace std;



int ppmview::readPPM(char * filename)
{
    
    width = 100; height = 100; // These values must be initialized for the OpenGL
    
    int A = 0;  //ascii flag
    
    FILE * imageFile;
    imageFile = fopen (filename,"r");
    
    if (imageFile == NULL) {
        perror ("Error opening file");
    }
    
    char a[80];
    fscanf(imageFile,"%s",a);
    while (a[0] == '#') {
        char comments[100];
        fgets(comments,100,imageFile);
        fscanf(imageFile,"%s",a);
    }
    
    //check magic number
    if ((strcmp(a,"P3") != 0) && (strcmp(a, "P6") != 0)) {
        while (a[0] == '#') {
            char comments[100];
            fgets(comments,100,imageFile);
            fscanf(imageFile,"%s",a);
        }
        cout<<"Its not ppm file"<<endl;
        readPGM(filename);
    }else {
        //cout<<"Its ppm file"<<endl;
        if (strcmp(a, "P3") == 0) {
            A = 1;
        }
        fscanf(imageFile,"%s",a);
        while (a[0] == '#') {
            char comments[100];
            fgets(comments,100,imageFile);
            fscanf(imageFile,"%s",a);
        }
        width = atoi(a);
        fscanf(imageFile, "%d %d", &height, &maxcolor);
        
        //grab the last newline character after the maxcolor byte:
        fgetc(imageFile);
        
        pixmap = new unsigned char[width * height * 3];
        int y, x, pixel;
        
        if (A == 0) {
            for(y = height - 1; y >= 0; y--) {
                for(x = 0; x < width; x++) {
                    
                    pixel = ((y) * width + x) * 3;
                    pixmap[pixel] = fgetc(imageFile);
                    pixel++;
                    pixmap[pixel] = fgetc(imageFile);
                    pixel++;
                    pixmap[pixel] = fgetc(imageFile);
                }
            }
        }else {
            int buffer[width * height *3];
            int i;
            for (i = 0; i < width*height*3; i++) {
                fscanf(imageFile,"%d",&buffer[i]);
            }
            
            i = 0;
            for(y = height - 1; y >= 0; y--) {
                for(x = 0; x < width; x++) {
                    pixel = ((y) * width + x) * 3;
                    pixmap[pixel] = buffer[i];
                    pixel++;
                    i++;
                    pixmap[pixel] = buffer[i];
                    pixel++;
                    i++;
                    pixmap[pixel] = buffer[i];
                    i++;

                }
            }
            
        }
        
    }
    fclose(imageFile);
    return A;
}

int ppmview::readPGM (char * filename)
{
    int A = 0;  //ascii flag
    
    FILE * imageFile;
    imageFile = fopen (filename,"r");
    
    if (imageFile == NULL) {
        perror ("Error opening file");
    }
    
    char a[80];
    fscanf(imageFile,"%s",a);
    while (a[0] == '#') {
        char comments[100];
        fgets(comments,100,imageFile);
        fscanf(imageFile,"%s",a);
    }
    
    //check magic number
    if ((strcmp(a,"P2") != 0) && (strcmp(a, "P5") != 0)) {
        while (a[0] == '#') {
            char comments[100];
            fgets(comments,100,imageFile);
            fscanf(imageFile,"%s",a);
        }
        cout<<"Its not pgm file"<<endl;
    }else {
        cout<<"Its pgm file"<<endl;
        if (strcmp(a, "P2") == 0) {
            A = 1;
        }
        fscanf(imageFile,"%s",a);
        while (a[0] == '#') {
            char comments[100];
            fgets(comments,100,imageFile);
            fscanf(imageFile,"%s",a);
        }
        width = atoi(a);
        fscanf(imageFile, "%d %d", &height, &maxcolor);
        
        //grab the last newline character after the maxcolor byte:
        fgetc(imageFile);
        
        pixmap = new unsigned char[width * height * 3];
        int y, x, pixel;
        
        if (A == 0) {
            for(y = height - 1; y >= 0; y--) {
                for(x = 0; x < width; x++) {
                    
                    pixel = ((y) * width + x) * 3;
                    unsigned char temp;
                    temp = fgetc(imageFile);
                    pixmap[pixel] = temp;
                    pixel++;
                    pixmap[pixel] = temp;
                    pixel++;
                    pixmap[pixel] = temp;
                }
            }
        }else {
            int buffer[width * height];
            int i;
            for (i = 0; i < width*height; i++) {
                fscanf(imageFile,"%d",&buffer[i]);
            }
            
            i = 0;
            for(y = height - 1; y >= 0; y--) {
                for(x = 0; x < width; x++) {
                    pixel = ((y) * width + x) * 3;
                    pixmap[pixel] = buffer[i];
                    pixel++;
                    pixmap[pixel] = buffer[i];
                    pixel++;
                    pixmap[pixel] = buffer[i];
                    i++;
                    
                }
            }
            
        }
        
    }
    fclose(imageFile);
    return A;

    
}

void ppmview::writePPM(char* outfile, int A)
{
    FILE *out;
    out = fopen (outfile,"w");
    
    if (out == NULL) {
        perror ("Error opening file");
    }
    
    if (A == 0) {
        fprintf(out, "P6\n");
        fprintf(out, "%d %d\n", width, height);
        fprintf(out, "255\n");
        
        int y, x, pixel;
        for(y = height - 1; y >= 0; y--) {
            for(x = 0; x < width; x++) {
                pixel = ((y) * width + x) * 3;
                fputc(pixmap[pixel],out);
                pixel++;
                fputc(pixmap[pixel],out);
                pixel++;
                fputc(pixmap[pixel],out);
                
            }
        }
    }else{
        fprintf(out, "P3\n");
        fprintf(out, "%d %d\n", width, height);
        fprintf(out, "255\n");
        
        int y, x, pixel;
        for(y = height - 1; y >= 0; y--) {
            for(x = 0; x < width; x++) {
                pixel = ((y) * width + x) * 3;
                fprintf(out, "%d ",(int)pixmap[pixel]);
                pixel++;
                fprintf(out, "%d ",(int)pixmap[pixel]);
                pixel++;
                fprintf(out, "%d ",(int)pixmap[pixel]);
            }
        }
    }
    fclose(out);
}

void ppmview::writePGM(char* outfile, int A)
{
    FILE *out;
    out = fopen (outfile,"w");
    
    if (out == NULL) {
        perror ("Error opening file");
    }
    
    if (A == 0) {
        fprintf(out, "P5\n");
        fprintf(out, "%d %d\n", width, height);
        fprintf(out, "255\n");
        
        int y, x, pixel;
        for(y = height - 1; y >= 0; y--) {
            for(x = 0; x < width; x++) {
                pixel = ((y) * width + x) * 3;
                fputc(pixmap[pixel],out);
                pixel++;
                //fputc(pixmap[pixel],out);
                pixel++;
                //fputc(pixmap[pixel],out);
                
            }
        }
    }else{
        fprintf(out, "P2\n");
        fprintf(out, "%d %d\n", width, height);
        fprintf(out, "255\n");
        
        int y, x, pixel;
        for(y = height - 1; y >= 0; y--) {
            for(x = 0; x < width; x++) {
                pixel = ((y) * width + x) * 3;
                fprintf(out, "%d ",(int)pixmap[pixel]);
                pixel++;
                //fprintf(out, "%d ",(int)pixmap[pixel]);
                pixel++;
                //fprintf(out, "%d ",(int)pixmap[pixel]);
            }
        }
    }
    fclose(out);

}

// =============================================================================
// OpenGL Display and Mouse Processing Functions.
//
// You can read up on OpenGL and modify these functions, as well as the commands
// in main(), to perform more sophisticated display or GUI behavior. This code
// will service the bare minimum display needs for most assignments.
// =============================================================================
//static void resize(int w, int h)
//{
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(0,(w/2),0,(h/2),0,1);
//    glMatrixMode(GL_MODELVIEW);
//}
//static void display(void)
//{
//    glClear(GL_COLOR_BUFFER_BIT);
//    glRasterPos2i(0,0);
//    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
//    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);
//    glFlush();
//}
//static void processMouse(int button, int state, int x, int y)
//{
//    if(state == GLUT_UP)
//        exit(0);               // Exit on mouse click.
//}
//static void init(void)
//{
//    glClearColor(1,1,1,1); // Set background color.
//}
//
//// =============================================================================
//// main() Program Entry
//// =============================================================================
//int main(int argc, char *argv[])
//{
//    // Check command line arguments and read in the filename, then call readPPM().
//    char * filename;
//    char *outFile;
//    int Aflag;  //ASCII flag
//    int isppm;  //ppm flag
//    
//    filename = argv[1];
//    Aflag = readPPM(filename,isppm);
//    
//    // Check command line arguments and read in the output filename if present.
//    if (argc == 3) {
//        outFile = argv[2];
//        if (isppm = 1) {
//            writePPM(outFile, Aflag);
//        }else
//            writePGM(outFile, Aflag);
//        
//    }
//    
//    // OpenGL commands
//    glutInit(&argc, argv);
//    glutInitWindowPosition(100, 100); // Where the window will display on-screen.
//    glutInitWindowSize(width, height);
//    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
//    glutCreateWindow("ppmview");
//    init();
//    glutReshapeFunc(resize);
//    glutDisplayFunc(display);
//    glutMouseFunc(processMouse);
//    glutMainLoop();
//    
//    
//    return 0;
//}
//
// =============================================================================
// GENERAL RECOMMENDATIONS AND SOLUTIONS TO COMMON PROBLEMS
//
// Use FILE, fopen, fclose, fscanf, fprintf, fgetc, and fputc to construct
// the readPPM and writePPM functions. The isspace() function from ctype.h
// is also very useful. You can learn about basic C file I/O issues in the
// excellent Wikipedia entry here:
//
// http://en.wikipedia.org/wiki/C_file_input/output
//
// Pay special attention to the section on the "The EOF Pitfall". Values
// returned from fgetc should be stored in an int, not a char.
//
// You can make more sophisticated versions of these functions using the C++
// <fstream> library and cin/cout. This is probably overkill for the ppm
// utilities you're creating in this class, unless you are already comfortable
// with the C++ way of doing things.
//
// If you stick with a more C-style approach, I strongly recommend you use
// fgetc and fputc to test for the P6 'magic number' bytes and to skip over
// any comment lines. When you've finished skipping over comments, scan-in
// the image width, height and color depth using this line:
//
//      fscanf(imagefile, "%d %d %d", &width, &height, &maxcolor);
//
// Then use this line to grab the last newline character after the maxcolor byte:
//
//      fgetc(imagefile);
//
// after which I recommend using fgetc to read in color channel byte values.
//
// This method is proven and works well. You should adhere to it in your own
// program unless you thoroughly test your own alternate methods.
//
//
// *** IMPORTANT ***
//
// DON'T DO THIS:
//
//      fscanf(imagefile, "%d %d\n %d\n", &width, &height, &maxcolor);
//
// OR THIS:
//
//      fscanf(imagefile, "%d %d %d\n", &width, &height, &maxcolor);
//
// OR THIS:
//
//      fscanf(imagefile, "\n", &ch);
//
// to scan in the last newline after the maxcolor byte.
//
// fscanf is not required to only read in one char byte for each explicit
// newline scan. This is because C binary I/O allows a newline character to
// sometimes be output as two characters, depending on the target output file
// type and other system conventions, and likewise be identified during scan in
// from two char bytes of an input file. fscanf may scan the newline and also
// the following first red color channel byte, if the hex value for that red
// color channel byte corresponds to a character that fscanf interprets as part
// of the newline. This definitely happens if the first red color channel has
// the hex value 0B (which is the hex value for a vertical tab character), but
// fscanf may be vulnerable to other newline-trailing hex values as well.
// If you must use fscanf to read in the last newline, you need to instead
// explicitely instruct fscanf to read in a single char:
//
// fscanf(imagefile, "%c", &ch);
//
// But fgetc(imagefile) will work fine too.
//
// Don't attempt to search for and skip over white space or comments lines after
// the newline that follows the maxcolor byte - there should never be any such
// chunks of data in a valid ppm file. At best you will add useless code to
// your project; at worst you will generate the same problem as the fscanf issue
// above - you may accidentally scan the first red color channel byte, and
// possibly following bytes as well, if the hex values for these bytes
// correspond to whitespace or comment line characters that you're skipping.
//
//
// For image output I recommend you use fprintf to write out the ppm header
// data:
//
//     fprintf(outfile, "P6\n");
//     fprintf(outfile, "%d %d\n", width, height);
//     fprintf(outfile, "255\n");
//
// and then fputc to output color channel byte values.
// =============================================================================
