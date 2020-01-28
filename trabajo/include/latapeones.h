#ifndef IG_LATA_PEONES_HPP
#define IG_LATA_PEONES_HPP

#include "grafo-escena.h" // declaración de 'grafo-escena'
#include "malla-revol.h"

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class VariasLatasPeones : public NodoGrafoEscena
{
  public:
    VariasLatasPeones();
};


class LataPeones : public NodoGrafoEscena
{
  public:
    LataPeones();
};

class Lata : public NodoGrafoEscena
{
  public:
    Lata(const std::string & textureJPG, const std::string nombre, const int id);
};


#endif
