// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaRevol : public MallaInd
{
  protected:
    // (el número de vértices, M , es el número de tuplas del vector)

    // Método que crea las tablas vertices y triangulos
    void crearMallaRevol(
      const std::vector<Tupla3f> & perfil_original, // vértices del perfil original
      const unsigned nperfiles,                     // número de perfiles
      const bool crear_tapas,                       // true para crear tapas
      const bool cerrar_malla,                      // true para cerrar la malla
      const bool crear_texturas                     // true para crear tabla de texturas
    );

  public:
    MallaRevol(const std::string& nombre_arch);

    // crea una malla de revolucion
    // constructor: crea una malla de revolución (lee PLY y llama a crearMallaRevol)
    MallaRevol(
      const std::string & nombre_arch,      // nombre del archivo ply
      const unsigned nperfiles,             // número de perfiles
      const bool crear_tapas,               // true para crear tapas
      const bool cerrar_malla,               // true para cerrar la malla
      const bool crear_texturas              // true para crear tabla de texturas
    );

};

// clases mallas indexadas por revolución de un perfil generado proceduralmente
class Cilindro : public MallaRevol
{
  public:
  // Constructor: crea el perfil original y llama a crearMalla
  // la base tiene el centro en el origen, el radio y la altura son 1
  Cilindro(
    const unsigned int num_verts_per,  // número de vértices del perfil original (M )
    const unsigned nperfiles,   // número de perfiles (N )
    const bool crear_tapas,     // true para crear tapas
    const bool cerrar_malla,     // true para cerrar la malla
    const bool crear_texturas   // true para crear tabla de texturas
  ) ;
} ;

class Cono : public MallaRevol
{
  public:
  // Constructor: crea el perfil original y llama a crearMalla
  // la base tiene el centro en el origen, el radio y altura son 1
  Cono(
    const unsigned int num_verts_per,  // número de vértices del perfil original (M )
    const unsigned nperfiles,   // número de perfiles (N )
    const bool crear_tapas,     // true para crear tapas
    const bool cerrar_malla,     // true para cerrar la malla
    const bool crear_texturas   // true para crear tabla de texturas
  ) ;
} ;

class Esfera : public MallaRevol
{
  public:
  // Constructor: crea el perfil original y llama a crearMalla
  // La esfera tiene el centro en el origen, el radio es la unidad
  Esfera(
    const unsigned int num_verts_per,  // número de vértices del perfil original (M )
    const unsigned nperfiles,   // número de perfiles (N )
    const bool crear_tapas,     // true para crear tapas
    const bool cerrar_malla,     // true para cerrar la malla
    const bool crear_texturas   // true para crear tabla de texturas
  ) ;
} ;

#endif
