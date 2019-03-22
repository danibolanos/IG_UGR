// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************


#include <cassert>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include "MallaPLY.hpp"

using namespace std ;

// *****************************************************************************

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre(string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla

   std::vector<float> vertices_ply ; // coordenadas de vértices
   std::vector<int>   caras_ply ;    // índices de vértices de triángulos

	ply::read( nombre_arch.c_str(), vertices_ply, caras_ply );

	tam_vertices = vertices_ply.size() / 3;
	tam_caras = caras_ply.size() / 3;

	for(int i=0; i < tam_vertices; i++)
		tabla_vertices.push_back({vertices_ply[3*i], vertices_ply[3*i+1], vertices_ply[3*i+2]});

	for(int i=0; i < tam_caras; i++)
		tabla_caras.push_back({caras_ply[3*i], caras_ply[3*i+1], caras_ply[3*i+2]});

   // calcular la tabla de normales
   //calcular_normales();
}

// *****************************************************************************
