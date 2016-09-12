/* 
 * File:   ImageLoader.cpp
 * Author: jcoelho
 * 
 * Created on September 11, 2016, 2:34 PM
 */

#include "imageloader.h"
#include <cassert>
#include <cstdlib>
#include <cmath>

#define ROUND(_) (int)floor( (_) + 0.5 )

typedef unsigned char BYTE;
typedef unsigned short int USHORT;
typedef unsigned short int WORD;
typedef long int LONG;
typedef unsigned long int DWORD;



ImageLoader::ImageLoader( )
{
}



bool ImageLoader::readBMP( float*& data, int& w, int& h, const char* path )
{
    BYTE *linedata;

    USHORT bfType; /* "BM" = 19788           */
    LONG biWidth; /* image width in pixels  */
    LONG biHeight; /* image height in pixels */
    WORD biBitCount; /* bitmap color depth     */
    DWORD bfSize;

    USHORT ushortSkip; /* dado lixo USHORT */
    DWORD dwordSkip; /* dado lixo DWORD  */
    LONG longSkip; /* dado lixo LONG   */
    WORD wordSkip; /* dado lixo WORD   */

    LONG i, j, k, l, linesize, got;
    FILE* filePtr = fopen( path, "rb" );
    assert( filePtr );
    /* verifica se eh uma imagem bmp */
    getuint( &bfType, filePtr );
    assert( bfType == 19778 );

    /* pula os 12 bytes correspondentes a bfSize, Reserved1 e Reserved2 */
    getdword( filePtr, &bfSize );
    getuint( &ushortSkip, filePtr ); /* Reserved1, deve ter valor 0 */
    assert( ushortSkip == 0 );
    getuint( &ushortSkip, filePtr ); /* Reserved2, deve ter valor 0 */
    assert( ushortSkip == 0 );

    /* pula os 4 bytes correspondentes a bfOffBits, que deve ter valor 54 */
    getdword( filePtr, &dwordSkip );
    assert( dwordSkip == 54 );

    /* pula os 4 bytes correspondentes a biSize, que deve ter valor 40 */
    getdword( filePtr, &dwordSkip );
    assert( dwordSkip == 40 );

    /* pega largura e altura da imagem */
    getlong( filePtr, &biWidth );
    getlong( filePtr, &biHeight );

    /* verifica que o numero de quadros eh igual a 1 */
    getword( filePtr, &wordSkip );
    assert( wordSkip == 1 );

    /* Verifica se a imagem eh de 24 bits */
    getword( filePtr, &biBitCount );
    if (biBitCount != 24)
    {
        fprintf( stderr, "imgReadBMP: Not a bitmap 24 bits file.\n" );
        fclose( filePtr );
        return false;
    }

    /* pula os demais bytes do infoheader */
    getdword( filePtr, &dwordSkip );
    assert( dwordSkip == 0 );
    getdword( filePtr, &dwordSkip );
    getlong( filePtr, &longSkip );
    getlong( filePtr, &longSkip );
    getdword( filePtr, &dwordSkip );
    getdword( filePtr, &dwordSkip );

    w = biWidth;
    h = biHeight;

    data = new float[ 3 * w * h];

    /* a linha deve terminar em uma fronteira de dword */
    linesize = 3 * w;
    if (linesize & 3)
    {
        linesize |= 3;
        linesize++;
    }

    /* aloca espaco para a area de trabalho */
    linedata = ( BYTE * ) malloc( linesize );
    if (linedata == NULL)
    {
        fprintf( stderr, "get24bits: Not enough memory.\n" );
        return 0;
    }

    /* pega as componentes de cada pixel */
    for (k = 0, i = 0; i < h; i++)
    {
        got = ( unsigned long int ) fread( linedata, linesize, 1, filePtr );
        if (got != 1)
        {
            free( linedata );
            fprintf( stderr, "get24bits: Unexpected end of file.\n" );
        }
        for (l = 1, j = 0; j < w; j++, l = l + 3)
        {
            data[k++] = ( float ) ( linedata[l + 1] / 255. );
            data[k++] = ( float ) ( linedata[l ] / 255. );
            data[k++] = ( float ) ( linedata[l - 1] / 255. );
        }
    }

    free( linedata );
    fclose( filePtr );
    return true;
}



bool ImageLoader::writeBMP( float* data, int w, int h, const char* path )
{
    unsigned char *filedata;
    DWORD bfSize;
    int i, k, l;
    int linesize, put;

    if (!data)
        return false;

    /* cria um novo arquivo binario */
    FILE* filePtr = fopen( path, "wb" );
    assert( filePtr );

    /* a linha deve terminar em uma double word boundary */
    linesize = w * 3;
    if (linesize & 3)
    {
        linesize |= 3;
        linesize++;
    }

    /* calcula o tamanho do arquivo em bytes */
    bfSize = 14 + /* file header size */
        40 + /* info header size */
        h * linesize; /* image data  size */

    /* Preenche o cabe�alho -> FileHeader e InfoHeader */
    putuint( 19778, filePtr ); /* type = "BM" = 19788                             */
    putdword( filePtr, bfSize ); /* bfSize -> file size in bytes                    */
    putuint( 0, filePtr ); /* bfReserved1, must be zero                       */
    putuint( 0, filePtr ); /* bfReserved2, must be zero                       */
    putdword( filePtr, 54 ); /* bfOffBits -> offset in bits to data             */

    putdword( filePtr, 40 ); /* biSize -> structure size in bytes                 */
    putlong( filePtr, w ); /* biWidth -> image width in pixels                  */
    putlong( filePtr, h ); /* biHeight -> image height in pixels                */
    putword( filePtr, 1 ); /* biPlanes, must be 1                               */
    putword( filePtr, 24 ); /* biBitCount, 24 para 24 bits -> bitmap color depth */
    putdword( filePtr, 0 ); /* biCompression, compression type -> no compression */
    putdword( filePtr, 0 ); /* biSizeImage, nao eh usado sem compressao          */
    putlong( filePtr, 0 ); /* biXPelsPerMeter                                   */
    putlong( filePtr, 0 ); /* biYPelsPerMeter                                   */
    putdword( filePtr, 0 ); /* biClrUsed, numero de cores na palheta             */
    putdword( filePtr, 0 ); /* biClrImportant, 0 pq todas sao importantes        */

    /* aloca espacco para a area de trabalho */
    filedata = ( unsigned char * ) malloc( linesize );
    assert( filedata );

    /* a linha deve ser zero padded */
    for (i = 0; i < ( linesize - ( 3 * w ) ); i++)
        filedata[linesize - 1 - i] = 0;

    for (k = 0; k < h; k++)
    {
        l = 1;
        /* coloca as componentes BGR no buffer */
        for (i = 0; i < w; i++)
        {
            int r, g, b;
            int pos = ( 3 * k * w ) + ( 3 * i );

            r = ROUND( 255 * data[pos + 0] );
            g = ROUND( 255 * data[pos + 1] );
            b = ROUND( 255 * data[pos + 2] );

            r = ( unsigned char ) ( r < 256 ) ? r : 255;
            g = ( unsigned char ) ( g < 256 ) ? g : 255;
            b = ( unsigned char ) ( b < 256 ) ? b : 255;

            filedata[l - 1] = ( unsigned char ) ( b < 256 ) ? b : 255;
            filedata[l + 0] = ( unsigned char ) ( g < 256 ) ? g : 255;
            filedata[l + 1] = ( unsigned char ) ( r < 256 ) ? r : 255;
            l += 3;
        }

        /* joga para o arquivo */
        put = ( int ) fwrite( filedata, linesize, 1, filePtr );
        if (put != 1)
        {
            fprintf( stderr, "put24bits: Disk full." );
            free( filedata );
            return 0;
        }
    }

    /* operacao executada com sucesso */
    fprintf( stdout, "imgWriteBMP: %s successfuly generated\n", path );
    free( filedata );
    fclose( filePtr );
    return 1;
}



int ImageLoader::getuint( unsigned short *uint, FILE *input )
{
    int got;
    unsigned char temp[2];
    unsigned short tempuint;

    got = ( int ) fread( &temp, 1, 2, input );
    if (got != 2) return 0;

    tempuint = ( ( unsigned short ) ( temp[1] ) << 8 ) | ( ( unsigned short ) ( temp[0] ) );

    *uint = tempuint;

    return 1;
}



int ImageLoader::putuint( unsigned short uint, FILE *output )
{
    int put;
    unsigned char temp[2];

    temp[0] = uint & 0xff;
    temp[1] = ( uint >> 8 ) & 0xff;

    put = ( int ) fwrite( &temp, 1, 2, output );
    if (put != 2) return 0;

    return 1;
}



int ImageLoader::getlong( FILE *input, long int *longint )
{
    int got;
    unsigned char temp[4];
    long int templongint;

    got = ( int ) fread( &temp, 1, 4, input );
    if (got != 4) return 0;

    templongint = ( ( long int ) ( temp[3] ) << 24 ) | ( ( long int ) ( temp[2] ) << 16 )
        | ( ( long int ) ( temp[1] ) << 8 ) | ( ( long int ) ( temp[0] ) );

    *longint = templongint;

    return 1;
}



int ImageLoader::putlong( FILE *output, long int longint )
{
    int put;
    unsigned char temp[4];

    temp[0] = ( unsigned char ) longint & 0xff;
    temp[1] = ( unsigned char ) ( longint >> 8 ) & 0xff;
    temp[2] = ( unsigned char ) ( longint >> 16 ) & 0xff;
    temp[3] = ( unsigned char ) ( longint >> 24 ) & 0xff;

    put = ( int ) fwrite( &temp, 1, 4, output );

    if (put != 4) return 0;

    return 1;
}



int ImageLoader::getword( FILE *input, unsigned short int *word )
{
    int got;
    unsigned char temp[2];
    unsigned short int tempword;

    got = ( int ) fread( &temp, 1, 2, input );
    if (got != 2) return 0;

    tempword = ( ( unsigned short int ) ( temp[1] ) << 8 ) | ( ( unsigned short int ) ( temp[0] ) );

    *word = tempword;

    return 1;
}



int ImageLoader::putword( FILE *output, unsigned short int word )
{
    int put;
    unsigned char temp[2];

    temp[0] = word & 0xff;
    temp[1] = ( word >> 8 ) & 0xff;

    put = ( int ) fwrite( &temp, 1, 2, output );
    if (put != 2) return 0;

    return 1;
}



int ImageLoader::getdword( FILE *input, unsigned long int *dword )
{
    int got;
    unsigned char temp[4];
    unsigned long int tempdword;

    got = ( int ) fread( &temp, 1, 4, input );
    if (got != 4) return 0;

    tempdword = ( ( unsigned long int ) ( temp[3] ) << 24 ) | ( ( unsigned long int ) ( temp[2] ) << 16 )
        | ( ( unsigned long int ) ( temp[1] ) << 8 ) | ( ( unsigned long int ) ( temp[0] ) );

    *dword = tempdword;

    return 1;
}



int ImageLoader::putdword( FILE *output, unsigned long int dword )
{
    int put;
    unsigned char temp[4];

    temp[0] = ( unsigned char ) ( dword & 0xff );
    temp[1] = ( unsigned char ) ( ( dword >> 8 ) & 0xff );
    temp[2] = ( unsigned char ) ( ( dword >> 16 ) & 0xff );
    temp[3] = ( unsigned char ) ( ( dword >> 24 ) & 0xff );

    put = ( int ) fwrite( &temp, 1, 4, output );

    if (put != 4) return 0;

    return 1;
}
