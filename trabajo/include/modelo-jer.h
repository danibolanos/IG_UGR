#ifndef IG_MODELO_JER_HPP
#define IG_MODELO_JER_HPP

#include <vector>          // usar std::vector
#include "grafo-escena.h"   // declaración de 'grafo-escena'
#include "malla-revol.h"
#include "malla-ind.h"

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class Generador : public NodoGrafoEscena
{
	public:
		Generador();
		unsigned leerNumParametros() const;
		void actualizarEstadoParametro(const unsigned iParam, const float tSec);
};

class Maquinaria : public NodoGrafoEscena
{
	protected:
    Matriz4f *pm_rot_alpha = nullptr;
    Matriz4f *pm_escy_h = nullptr;
	public:
		Maquinaria();
		unsigned leerNumParametros() const;
		void actualizarEstadoParametro(const unsigned iParam, const float tSec);
};

class Base : public NodoGrafoEscena
{
  protected:
    Generador *inst_generador;
  public:
    Base();
    unsigned leerNumParametros() const;
		void actualizarEstadoParametro(const unsigned iParam, const float tSec);
};

class Brazo : public NodoGrafoEscena
{
  public:
    Brazo();
};

class Giro : public NodoGrafoEscena
{
	public:
		Giro();
};

class Pinza : public NodoGrafoEscena
{
	public:
		Pinza();
};

class GiroPinza : public NodoGrafoEscena
{
	public:
		GiroPinza();
};

class Apoyo : public NodoGrafoEscena
{
  protected:
    Brazo *inst_brazo1, *inst_brazo2;
	 Giro *inst_giro;
	 GiroPinza *inst_pinza1, *inst_pinza2;
  public:
    Apoyo();
    unsigned leerNumParametros() const;
	 void actualizarEstadoParametro(const unsigned iParam, const float tSec);
};

class CilindroTornillos : public NodoGrafoEscena
{
  public:
    CilindroTornillos();
};

class Infinito : public NodoGrafoEscena
{
  public:
    Infinito();
};

class C : public NodoGrafoEscena
{
  protected:
    Base *inst_base;
    Maquinaria *inst_maquinaria;
	  Apoyo *inst_apoyo;
  public:
    // constructor crea los subárboles del nodo raiz y lo añade a la lista de entradas
    C();
    // devuelve el número de parámetros
    unsigned leerNumParametros() const;
    // actualiza el estado del prametro iParam
    void actualizarEstadoParametro( const unsigned iParam, const float tSec );
} ;



#endif
