/* 
 * File:   ImageLoader.h
 * Author: jcoelho
 *
 * Created on September 11, 2016, 2:34 PM
 */

#ifndef IMAGELOADER_H
#define	IMAGELOADER_H
#include <cstdio>

class ImageLoader
{

public:
    /**
     * Construtor que recebe o nome do arquivo.
     * @param path - nome do arquivo a ser lido ou escrito.
     */
    ImageLoader( );

    /**
     * Le imagem em BMP e salva as informacoes nas variaveis passadas por parametro.
     * @param data - pixels da imagem.
     * @param w - largura da imagem.
     * @param h - altura da imagem.
     * @return - true se o arquivo pode ser lido com sucesso e false caso
     * contrario.
     */
    bool readBMP( float*& data, int& w, int& h, const char* path );

    /**
     * Escreve imagem em formato BMP.
     * @param data - pixels das informacoes.
     * @param w - largura da imagem.
     * @param h - altura da imagem.
     * @return - true se a imagem pode ser escrita com sucesso e false caso
     * contrario.
     */
    bool writeBMP( float* data, int w, int h, const char* path );

private:
    /**
     * Reads an unsigned integer from input.
     */
    static int getuint( unsigned short *uint, FILE *input );

    /**
     * Writes an unsigned integer in output.
     */
    static int putuint( unsigned short uint, FILE *output );

    /**
     * Reads a long integer from input
     */
    static int getlong( FILE *input, long int *longint );

    /**
     * Writes a long integer in output.
     */
    static int putlong( FILE *output, long int longint );

    /**
     * Reads a word from input.
     */
    static int getword( FILE *input, unsigned short int *word );

    /**
     * Writes a word in output.
     */
    static int putword( FILE *output, unsigned short int word );

    /**
     * Reads a double word from input.
     */
    static int getdword( FILE *input, unsigned long int *dword );

    /**
     * Writes a double word in output.
     */
    static int putdword( FILE *output, unsigned long int dword );
};

#endif	/* IMAGELOADER_H */

