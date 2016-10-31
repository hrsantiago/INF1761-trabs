#include "GraphicsShader.h"
#include <iostream>
#include <stdio.h>
#include <string.h>



GraphicsShader::GraphicsShader( )
{
    _vertexProgramString = 0;
    _fragmentProgramString = 0;
    _geometryProgramString = 0;
    _tesselationControlProgramString = 0;
    _tesselationEvaluationProgramString = 0;

    _vertexProgram = 0;
    _fragmentProgram = 0;
    _geometryProgram = 0;
    _tesselationControlProgram = 0;
    _tesselationEvaluationProgram = 0;
    _glShader = 0;
    _geometryInputType = 0;
    _geometryOutputType = 0;
    _nVertexOut = 64;

    _isAllocated = false;
    _isVertexProgramSet = false;
    _isFragmentProgramSet = false;
    _isGeometryProgramSet = false;
    _isTesselationControlProgramSet = false;
    _isTesselationEvaluationProgramSet = false;
}



GraphicsShader::~GraphicsShader( )
{
    deleteShader( );
    delete [] _vertexProgramString;
    delete [] _fragmentProgramString;
    delete [] _geometryProgramString;
    delete [] _tesselationControlProgramString;
    delete [] _tesselationEvaluationProgramString;
}



void GraphicsShader::setVertexProgram( const char* vertexProgramString, int lenght )
{
    delete []_vertexProgramString;
    _vertexProgramString = new char[lenght + 1];
    memcpy( _vertexProgramString, vertexProgramString, lenght * sizeof ( char ) );

    if (_vertexProgramString != 0)
    {
        _isVertexProgramSet = true;
        _lenghtVertex = lenght;
    }
    else
    {
        _isVertexProgramSet = false;
    }
}



void GraphicsShader::setFragmentProgram( const char* fragmentProgramString, int lenght )
{
    delete []_fragmentProgramString;
    _fragmentProgramString = new char[lenght + 1];
    memcpy( _fragmentProgramString, fragmentProgramString, lenght * sizeof ( char ) );

    if (_fragmentProgramString != 0)
    {
        _isFragmentProgramSet = true;
        _lenghtFragment = lenght;
    }
    else
    {
        _isFragmentProgramSet = false;
    }
}



void GraphicsShader::setTesselationControlProgram( const char* tesselationControlProgramString, int lenght )
{
    delete []_tesselationControlProgramString;
    _tesselationControlProgramString = new char[lenght + 1];
    memcpy( _tesselationControlProgramString, tesselationControlProgramString, lenght * sizeof ( char ) );

    if (_tesselationControlProgramString != 0)
    {
        _isTesselationControlProgramSet = true;
        _lenghtTesselationCrontrolShader = lenght;
    }
    else
    {
        _isTesselationControlProgramSet = false;
    }
}



void GraphicsShader::setTesselationEvaluationProgram( const char* tesselationEvaluationProgramString, int lenght )
{
    delete []_tesselationEvaluationProgramString;
    _tesselationEvaluationProgramString = new char[lenght + 1];
    memcpy( _tesselationEvaluationProgramString, tesselationEvaluationProgramString, lenght * sizeof ( char ) );

    if (_tesselationControlProgramString != 0)
    {
        _isTesselationEvaluationProgramSet = true;
        _lenghtTesselationEvaluationShader = lenght;
    }
    else
    {
        _isTesselationEvaluationProgramSet = false;
    }
}



void GraphicsShader::setGeometryProgram( const char* geometryProgramString, int lenght )
{
    delete []_geometryProgramString;
    _geometryProgramString = new char[lenght + 1];
    memcpy( _geometryProgramString, geometryProgramString, lenght * sizeof ( char ) );


    if (_geometryProgramString != 0)
    {
        _isGeometryProgramSet = true;
        _lenghtGeometry = lenght;
    }
    else
    {
        _isGeometryProgramSet = false;
    }
}



const char* GraphicsShader::getVertexProgram( )
{
    return _vertexProgramString;
}



const char* GraphicsShader::getFragmentProgram( )
{
    return _fragmentProgramString;
}



const char* GraphicsShader::getGeometryProgram( )
{
    return _geometryProgramString;
}



const char* GraphicsShader::getTesselationControlProgram( )
{
    return _tesselationControlProgramString;
}



const char* GraphicsShader::getTesselationEvaluationProgram( )
{
    return _tesselationEvaluationProgramString;
}



unsigned int GraphicsShader::getShaderIndex( ) const
{
    return _glShader;
}



void GraphicsShader::setGeometryParameters( unsigned int inputType, unsigned int outputType, unsigned int nVertexOut )
{
    _geometryInputType = inputType;
    _geometryOutputType = outputType;
    _nVertexOut = nVertexOut;
}



void GraphicsShader::getGeometryParameters( unsigned int& inputType, unsigned int& outputType, unsigned int& nVertexOut )
{
    inputType = _geometryInputType;
    outputType = _geometryOutputType;
    nVertexOut = _nVertexOut;
}



void GraphicsShader::compileShader( )
{
    if (_isAllocated)
    {
        deleteShader( );
        _isAllocated = false;
    }

    GLenum err = glewInit( );
    if (err != GLEW_OK)
    {
        std::cerr << "Error: " << glewGetErrorString( err ) << std::endl;
    }
    // Cria um programa que ira utilizar todos os programs criados.
    _glShader = glCreateProgram( );

    err = glGetError( );
    if (err != GL_NO_ERROR)
    {
        printf( "Erro na criacao do programa: %s\n", gluErrorString( err ) );
    }

    // vertex
    if (_isVertexProgramSet)
    {        
        printf( "\n\nVertex Shader:\n%s", _vertexProgramString );

        _vertexProgram = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( _vertexProgram, 1, ( const char** ) &_vertexProgramString, &_lenghtVertex );
        glCompileShader( _vertexProgram );

        printInfoLog( GL_COMPILE_STATUS, _vertexProgram );
        glAttachShader( _glShader, _vertexProgram );
    }

    err = glGetError( );
    if (err != GL_NO_ERROR)
    {
        printf( "Erro vertexShader: %s\n", gluErrorString( err ) );
    }

    if (_isTesselationControlProgramSet)
    {
        printf( "\n\nTesselation Control Shader:\n%s", _tesselationControlProgramString );

        _tesselationControlProgram = glCreateShader( GL_TESS_CONTROL_SHADER );
        glShaderSource( _tesselationControlProgram, 1, ( const char** ) &_tesselationControlProgramString, &_lenghtTesselationCrontrolShader );
        glCompileShader( _tesselationControlProgram );

        printInfoLog( GL_COMPILE_STATUS, _tesselationControlProgram );
        glAttachShader( _glShader, _tesselationControlProgram );
    }

    err = glGetError( );
    if (err != GL_NO_ERROR)
    {
        printf( "Erro tesselationControlShader: %s\n", gluErrorString( err ) );
    }

    if (_isTesselationEvaluationProgramSet)
    {
        printf( "\n\nTesselation Evaluation Shader:\n%s", _tesselationEvaluationProgramString );

        _tesselationEvaluationProgram = glCreateShader( GL_TESS_EVALUATION_SHADER );
        glShaderSource( _tesselationEvaluationProgram, 1, ( const char** ) &_tesselationEvaluationProgramString, &_lenghtTesselationEvaluationShader );
        glCompileShader( _tesselationEvaluationProgram );

        printInfoLog( GL_COMPILE_STATUS, _tesselationEvaluationProgram );
        glAttachShader( _glShader, _tesselationEvaluationProgram );
    }


    err = glGetError( );
    if (err != GL_NO_ERROR)
    {
        printf( "Erro tessalationEvaluationShader: %s\n", gluErrorString( err ) );
    }

    // fragment
    if (_isFragmentProgramSet)
    {
        printf( "\n\nFragment Shader:\n%s", _fragmentProgramString );

        _fragmentProgram = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( _fragmentProgram, 1, ( const char** ) &_fragmentProgramString, &_lenghtFragment );
        glCompileShader( _fragmentProgram );

        printInfoLog( GL_COMPILE_STATUS, _fragmentProgram );
        glAttachShader( _glShader, _fragmentProgram );
    }


    err = glGetError( );
    if (err != GL_NO_ERROR)
    {
        printf( "Erro fragmentShader: %s\n", gluErrorString( err ) );
    }

    // geometry
    if (_isGeometryProgramSet)
    {
        printf( "\n\nGeometry Shader:\n%s", _geometryProgramString );
        _geometryProgram = glCreateShader( GL_GEOMETRY_SHADER );
        glShaderSource( _geometryProgram, 1, ( const char** ) &_geometryProgramString, &_lenghtGeometry );
        glCompileShader( _geometryProgram );

        printInfoLog( GL_COMPILE_STATUS, _geometryProgram );
        glAttachShader( _glShader, _geometryProgram );

        err = glGetError( );
        if (err != GL_NO_ERROR)
        {
            printf( "Erro geometryShader: %s\n", gluErrorString( err ) );
        }
        // Define o tipo de entrada de saida do geometry program
        glProgramParameteri( _glShader, GL_GEOMETRY_INPUT_TYPE, GL_TRIANGLES );
        glProgramParameteri( _glShader, GL_GEOMETRY_OUTPUT_TYPE, _geometryOutputType );
        glProgramParameteri( _glShader, GL_GEOMETRY_VERTICES_OUT, _nVertexOut );
    }


    err = glGetError( );
    if (err != GL_NO_ERROR)
    {
        printf( "Erro geometryShader: %s\n", gluErrorString( err ) );
    }

    // Linka os shaders
    glLinkProgram( _glShader );

    GLchar ErrorLog[1024] = { 0 };
    GLint success = 0;
    glGetProgramiv( _glShader, GL_LINK_STATUS, &success );
    if (success == GL_FALSE)
    {
        glGetProgramInfoLog( _glShader, sizeof (ErrorLog ), NULL, ErrorLog );
        fprintf( stderr, "Error linking shader program: '%s'\n", ErrorLog );
    }

    glValidateProgram( _glShader );
    glGetProgramiv( _glShader, GL_VALIDATE_STATUS, &success );
    if (success == GL_FALSE)
    {
        glGetProgramInfoLog( _glShader, sizeof (ErrorLog ), NULL, ErrorLog );
        fprintf( stderr, "Invalid shader program: '%s'\n", ErrorLog );
    }

    err = glGetError( );
    if (err != GL_NO_ERROR)
    {
        printf( "Erro na compilacao do shader: %s\n", gluErrorString( err ) );
    }
    _isAllocated = true;
}



void GraphicsShader::deleteShader( )
{
    if (!_isAllocated)
        return;
    // Desaloca os shaders.
    glDetachShader( _glShader, _vertexProgram );
    glDetachShader( _glShader, _fragmentProgram );
    glDetachShader( _glShader, _geometryProgram );
    glDetachShader( _glShader, _tesselationControlProgram );
    glDetachShader( _glShader, _tesselationEvaluationProgram );

    _vertexProgram = 0;
    _fragmentProgram = 0;
    _geometryProgram = 0;
    _tesselationControlProgram = 0;
    _tesselationEvaluationProgram = 0;

    // Deleta o shader
    glDeleteShader( _glShader );

    _glShader = 0;
}



void GraphicsShader::load( )
{
    if (!_isAllocated)
        return;
    glUseProgram( _glShader );
}



void GraphicsShader::unload( )
{
    glUseProgram( 0 );
}



void GraphicsShader::loadVariables( )
{
}



bool GraphicsShader::isAllocated( )
{
    return _isAllocated;
}



void GraphicsShader::printInfoLog( GLenum obj, int shader )
{
    GLint status;
    glGetShaderiv( shader, obj, &status );
    if (status == GL_FALSE)
    {
        GLint len;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );
        if (len > 0)
        {
            char* menssage = new char[len];
            glGetShaderInfoLog( shader, len, 0, menssage );
            std::cout << menssage << std::endl;
            delete[]menssage;
        }
    }
}
