// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "objeto3d.h"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{

   protected:
      // COMPLETAR: incluir aquí las variables y métodos privados que sean
      // necesarios para una malla indexada
      // ......

      std::vector<Tupla3f> vertices ;
      std::vector<Tupla3i> triangulos ;
      // extra
      //std::vector<Tupla3f> varistas;

      std::vector<Tupla3f> col_ver ;  // colores de los vértices
      std::vector<Tupla3f> nor_ver ;  // normales de vértices
      std::vector<Tupla3f> nor_tri ;  // normales de triangulos
      std::vector<Tupla2f> cc_tt_ver ; // coordenadas de textura de los vértices

      // identificadores de VBO
      GLuint
         id_vbo_col, // colores
         id_vbo_nor, // normales
         id_vbo_cct, // coord. de textura
         id_vbo_ver, // posiciones o coordenadas de vértices
         id_vbo_tri; // triángulos (índices)

      bool vbos_creados = false ;

      // identificadores de 'vertex array object' (VAO)
      // (si no se han creado todavía, es 0)
      GLuint id_vao_md = 0; // VAO del modo diferido, (0 si no inicializado aún)



      // VAOs para modo diferido
      void crearActivarVAO_MD(  ) ;

      void crearActivarVAO_MD_geom(  ) ;

      // crea todos los VBOS (solo la primera vez)
      void crearVBOs();

      // resgistrar las tablas en los distintos modos de envío
      void registrarTablas_MI();
      void registrarTablas_MD();

      void registrarTablas_MD_geom();

      // visualizar con begin/end (modo inmediato)
      void visualizarGL_MI_BE( ContextoVis & cv );

      // visualizar con 'draw elements', en modo inmediato
      void visualizarGL_MI_DE( ContextoVis & cv );

      // visualizar con 'draw elements', en modo diferido, usando 'id_vao_md'
      void visualizarGL_MD_VAO( ContextoVis & cv );

      // visualizar en modo aristas
      //void visualizarGL_modoAristas( ContextoVis & cv );

      // normales de triángulos y vértices
      void calcularNormales();

      // calculo de las normales de triángulos (solo si no están creadas ya)
      void calcularNormalesTriangulos() ;



   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd() ;

      // crea una malla vacía con un nombre concreto:
      MallaInd( const std::string & nombreIni );

      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) ;

      // modo visualización CuboColores
      void modoMulticolor();

      // modo visualización ajedrez
      void modoAjedrez();

      // modo visualización random
      void modoRandom();

      void visualizarNormales();


} ;
// ---------------------------------------------------------------------
// Clase para mallas obtenidas de un archivo 'ply'
// es un tipo de malla indexada que define un nuevo constructor
// que recibe el nombre del archivo ply como parámetro

class MallaPLY : public MallaInd
{
   public:
      MallaPLY( const std::string & nombre_arch ) ;
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

// ---------------------------------------------------------------------

class CuboColores : public MallaInd
{
   public:
      CuboColores();
};

// ---------------------------------------------------------------------

class Cubo24 : public MallaInd
{
  public:
    Cubo24();
};


#endif
