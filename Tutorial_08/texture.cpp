#include "gl_framework.hpp"
#include <fstream>
#include <cstdlib>
#include "texture.hpp"

GLuint LoadTexture( const char * filename, int width, int height )
{
    GLuint texture;
    unsigned char header[54];// 54 Byte header of BMP
    int pos;
    unsigned int w,h;
    unsigned int size; //w*h*3
    unsigned char * data; // Data in RGB FORMAT
    FILE * file;
    
    file = fopen( filename, "rb" ); 
    if ( file == NULL ) return 0;  // if file is empty 
    if (fread(header,1,54,file)!=54)
      {
	printf("Incorrect BMP file\n");
	return 0;
      }

    // Read  MetaData
    pos = *(int*)&(header[0x0A]);
    size = *(int*)&(header[0x22]);
    w = *(int*)&(header[0x12]);
    h = *(int*)&(header[0x16]);

    //Just in case metadata is missing
    if(size == 0) 
      size = w*h*3;
    if(pos == 0)
      pos = 54;

    data = new unsigned char [size];

    fread( data, size, 1, file ); // read the file
    fclose( file );
    //////////////////////////

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
   

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    free( data );
    return texture;// return the texture id
}
void FreeTexture( GLuint texture )
{
  glDeleteTextures( 1, &texture ); // delete the texture
}
