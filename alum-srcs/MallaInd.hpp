// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector
#include "Objeto3D.hpp"   // declaración de 'Objeto3D'

// ---------------------------------------------------------------------
// funciones auxiliares
Tupla3f calcularCentroCajaEnglobante(const std::vector<Tupla3f>& vertices);

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{
  protected:
    std::vector<Tupla3f> tabla_vertices ; // tabla de vértices (num_ver entradas)
    std::vector<Tupla3i> tabla_caras;     // tabla de triángulos (num_tri entradas)
    std::vector<Tupla3f> color_vertices;  // tabla de colores de los vértices

    std::vector<Tupla3f> nor_caras;       // tabla de normales de caras
    std::vector<Tupla3f> nor_vertices;    // tabla de normales de vértices
    std::vector<Tupla2f> tabla_texturas;  // tabla de texturas

    unsigned num_vertices;                // número de vértices (nv)
    unsigned num_caras;                   // número de triángulos (nt)
    unsigned int tam_vertices, tam_caras; // tamaño en bytes de las tablas
    unsigned int tam_texturas;
    bool usar_texturas;

    GLuint  // identificadores VBO
      id_vbo_vertices = 0,
      id_vbo_caras = 0,
      id_vbo_col_ver = 0,
      id_vbo_nor_ver = 0,
      id_vbo_tex = 0;

    bool vbo_creado;
    bool normales_creadas;

    // calculo de las normales de esta malla
    void calcular_normales();

    // crear VBO de caras y VBO de vertices
    void crearVBOs();
    // visualizar con 'draw elements', en modo inmediato
    void visualizarDE_MI( ContextoVis & cv );
    // visualizar con 'draw elements', en modo diferido (con VBOS)
    void visualizarDE_VBOs( ContextoVis & cv );
    // Visualizar con glBegin/glEnd en modo inmediato
    void visualizarDE_MI_Plano(ContextoVis & cv);

  public:
    // crea una malla vacía (nombre: "malla indexada nueva vacía")
    MallaInd() ;
    // crea una malla vacía con un nombre concreto:
    MallaInd( const std::string & nombreIni );
    // visualizar el objeto con OpenGL
    virtual void visualizarGL( ContextoVis & cv ) ;
    // Fijar color de la figura
    virtual void fijarColorNodo(const Tupla3f& color);
    // Recalcular el centro del objeto
    virtual void calcularCentroOC();
} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
  public:
    Cubo();
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
  public:
    Tetraedro();
};


#endif
