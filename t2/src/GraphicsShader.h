

#ifndef GV_GRAPHICS_SHADER_ITEM_H
#define GV_GRAPHICS_SHADER_ITEM_H

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>

class GraphicsShader
{
public:

    /**
     * Construtor padrao.
     */
    GraphicsShader( );

    /**
     * Destrutor.
     */
    virtual ~GraphicsShader( );

    /**
     * Seta string para compilacao do vertex program.
     * @param[in] vertexProgramString string de vertex program para ser compilada.
     */
    void setVertexProgram( const char* vertexProgramString, int lenght );

    /**
     * Seta string para compilacao do fragment program.
     * @param[in] fragmentProgramString string de fragment program para ser compilada.
     */
    void setFragmentProgram( const char* fragmentProgramString, int lenght );

    /**
     * Define o programa de controle da tesselacao
     * @param tesselationControlProgramString - codigo do programa
     * @param lenght - tamanho da string com o codigo
     */
    void setTesselationControlProgram( const char* tesselationControlProgramString, int lenght );

    /**
     * Define o programa de avaliacao da tesselacao
     * @param tesselationEvaluationProgramString - codigo do programa
     * @param lenght - tamanho da string com o codigo
     */
    void setTesselationEvaluationProgram( const char* tesselationEvaluationProgramString, int lenght );

    /**
     * Seta string para compilacao do geometry program.
     * @param[in] geometryProgramString string de geometry program para ser compilada.
     */
    void setGeometryProgram( const char* geometryProgramString, int lenght );

    /**
     * Retorna string de compilacao do vertex program.
     * @return - string do programa do vertex shader.
     */
    const char* getVertexProgram( );

    /**
     * Retorna string de compilacao do fragment program.
     * @return  - string do programa do fragment shader.
     */
    const char* getFragmentProgram( );

    /**
     * Retorna string de compilacao do geometry program.
     * @return - string do programa do geometry shader.
     */
    const char* getGeometryProgram( );

    /**
     * Retorna string de compilacao do tesselation control program.
     * @return - string do programa do tesselation control shader.
     */
    const char* getTesselationControlProgram( );

    /**
     * Retorna string de compilacao do tesselation evaluation program.
     * @return - string do programa do tesselation evaluation shader.
     */
    const char* getTesselationEvaluationProgram( );

    /**
     * Return the shader index.
     * @return - shader index.
     */
    unsigned int getShaderIndex( )const;

    /**
     * Seta parametros de geometria
     * @param[in] inputType Index do opengl indicandl tipo de geometria que o geometry programa recebe
     * @param[in] outputType Index do opengl indicandl tipo de geometria que o geometry programa gera
     * @param[in] nVertexOut maximo numero de vertices que programa de geometria pode gerar
     */
    void setGeometryParameters( unsigned int inputType, unsigned int outputType, unsigned int nVertexOut );

    /**
     * Retorna parametros de geometria
     * @param[out] inputType Index do opengl indicandl tipo de geometria que o geometry programa recebe
     * @param[out] outputType Index do opengl indicandl tipo de geometria que o geometry programa gera
     * @param[out] nVertexOut maximo numero de vertices que programa de geometria pode gerar
     */
    void getGeometryParameters( unsigned int& inputType, unsigned int& outputType, unsigned int& nVertexOut );

    /**
     * Compila shader.
     */
    void compileShader( );

    /**
     * Deleta shader dentro do opengl.
     */
    void deleteShader( );

    /**
     * Carrega shader no opengl.
     */
    void load( );

    /**
     * Descarrega shader do opengl
     */
    void unload( );

    /**
     * Funcao virtual, carrega no shader todas as variaveis uniformes utilizadas pelo programa
     * Shaders que utilizam variaveis uniformes devem sobrescrever esta funcao.
     */
    virtual void loadVariables( );

    /**
     * Retorna se shader esta alocado.
     * @return - true se tiver alocado e falso caso contrario.
     */
    bool isAllocated( );

protected:

    /**
     * Armazena string de vertex program para ser compilada.
     */
    char* _vertexProgramString;

    /**
     * Armazena string de fragment program para ser compilada.
     */
    char* _fragmentProgramString;

    /**
     * Armazena string de geometry program para ser compilada.
     */
    char* _geometryProgramString;

    /**
     * Armazena string de tessalation control program para ser compilada.
     */
    char* _tesselationControlProgramString;

    /**
     * Armazena string de tessalation evaluation program para ser compilada.
     */
    char* _tesselationEvaluationProgramString;


    /**
     * Tamanho dos shaders.
     */
    int _lenghtVertex;
    int _lenghtGeometry;
    int _lenghtFragment;
    int _lenghtTesselationCrontrolShader;
    int _lenghtTesselationEvaluationShader;

    /**
     * Armazena o indice do vertex program gerado.
     */
    unsigned int _vertexProgram;

    /**
     * Flag que indica se vertex program foi definido.
     */
    bool _isVertexProgramSet;

    /**
     * Armazena o indice do fragment program gerado.
     */
    unsigned int _fragmentProgram;

    /**
     * Flag que indica se vertex program foi definido.
     */
    bool _isFragmentProgramSet;

    /**
     * Armazena o indice do geometry program gerado.
     */
    unsigned int _geometryProgram;

    /**
     * Flag que indica se vertex program foi definido.
     */
    bool _isGeometryProgramSet;

    /**
     * Armazena o indice do tesseletion control program gerado.
     */
    unsigned int _tesselationControlProgram;

    /**
     * Flag que indica se tesseletion control program foi definido.
     */
    bool _isTesselationControlProgramSet;

    /**
     * Armazena o indice do tesseletion evaluantion program gerado.
     */
    unsigned int _tesselationEvaluationProgram;

    /**
     * Flag que indica se tesseletion evaluantion program foi definido.
     */
    bool _isTesselationEvaluationProgramSet;

    /**
     * Armazena o indice do Shader program gerado.
     */
    unsigned int _glShader;

    /**
     * Tipo de geometria que o shader recebe.
     */
    unsigned int _geometryInputType;

    /**
     * Tipo de geometria que o shader gera.
     */
    unsigned int _geometryOutputType;

    /**
     * Numero maximo de vertices que shader gera.
     */
    unsigned int _nVertexOut;

private:

    /**
     * Indica se shader esta atualmente alocado.
     */
    bool _isAllocated;



    /** 
     * Funcoes auxialiares para debugar o shader - imprime mensagens de erro 
     */
    void printInfoLog( GLenum obj, int shader );
};


#endif
