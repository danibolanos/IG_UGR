// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include "MallaRevol.hpp"
#include <cmath>

using namespace std ;

// *****************************************************************************

float norma(Tupla3f v){
  return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
};

// *****************************************************************************

MallaRevol::MallaRevol(const string& nombre_arch)
  : MallaInd(nombre_arch)
{

}

// *****************************************************************************

MallaRevol::MallaRevol(
    const string & nombre_arch,
    const unsigned nperfiles,
    const bool     crear_tapas,
    const bool     cerrar_malla,
    const bool     crear_texturas)
{
  vector<float> vertices;
  vector<Tupla3f> perfil;

  ponerNombre( string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
  ply::read_vertices(nombre_arch.c_str(), vertices);

  // Inicializamos el perfil original
  for (unsigned int i=0; i+2 < vertices.size(); i+=3)
    perfil.push_back({vertices[i], vertices[i+1], vertices[i+2]});

  // Rellena las tablas
  crearMallaRevol(perfil, nperfiles, crear_tapas, cerrar_malla, crear_texturas);

  // calcular la tabla de normales
  calcular_normales();
}

// *****************************************************************************

void MallaRevol::crearMallaRevol(
    const std::vector<Tupla3f> & perfil_original,
    const unsigned nperfiles,
    const bool crear_tapas,
    const bool cerrar_malla,
    const bool crear_texturas )
{
  unsigned n_vert_per = perfil_original.size();
  num_vertices = n_vert_per * nperfiles;
  std::vector<float> dist_perfil;

  // Array de distancias de un perfil: d0=0, dj+1=dj+|pj+1-pj|
  if (crear_texturas){
    dist_perfil.push_back(0.0);
    for (int i = 1; i < n_vert_per; ++i){
      dist_perfil.push_back(dist_perfil[i-1] + norma(perfil_original[i] - perfil_original[i-1]));
    }
  }

  //******************** VÉRTICES ********************
  // Para cada perfil, roto todos los vertices del original y los añado a la tabla de vértices
  for (unsigned int i = 0; i < nperfiles; ++i) {
    unsigned j = 0;

    for (auto vert : perfil_original){
      //Giramos 360/nperfiles * i º respecto del eje Y
      if (cerrar_malla)
        tabla_vertices.push_back (MAT_Rotacion((float)(360*i)/nperfiles, 0, 1, 0)*vert);
      else{
        tabla_vertices.push_back (MAT_Rotacion((float)(360*i)/(nperfiles-1), 0, 1, 0)*vert);

        if (crear_texturas){
          float s, t;
          s = (float)i/(float)(nperfiles-1);
          t = dist_perfil[j] / dist_perfil[n_vert_per-1];
          tabla_texturas.push_back ({s,1-t});
        }
      }

      ++j;
    }
  }

  // Añado si procede los vértices para las tapas
  if (crear_tapas){
    // El primer y último punto se proyecta sobre el eje de rotación, Y, y se añaden.
    tabla_vertices.push_back( {0, perfil_original[0](1), 0} );
    tabla_vertices.push_back( {0, perfil_original[n_vert_per-1](1), 0} );
  }

  //******************** CARAS ********************
  int vert_index;
  int vert_index_nxt_per;
  unsigned int ncaras = (cerrar_malla ? nperfiles : nperfiles-1);

  // Añadimos caras a la tabla de vértices
  for (unsigned int i = 0; i < ncaras; ++i) {
    for (unsigned int j = 0; j < n_vert_per-1; ++j){
      vert_index = j + i*n_vert_per;

      if (cerrar_malla)
        vert_index_nxt_per = (vert_index + n_vert_per) % (nperfiles * n_vert_per);
      else
        vert_index_nxt_per = vert_index + n_vert_per;

      tabla_caras.push_back({vert_index+1, vert_index, vert_index_nxt_per});
      tabla_caras.push_back({vert_index+1, vert_index_nxt_per, vert_index_nxt_per+1});
    }
  }

  // Añadimos las caras de las tapas
  if (crear_tapas){
    // Los dos últimos elementos de tabla_vertices son los proyectados en el eje Y
    int tapa_inf_ver_ind = tabla_vertices.size()-2;
    int tapa_sup_ver_ind = tabla_vertices.size()-1;

    int ver_tapainf_ind, ver_tapasup_ind;
    int ver_inf_sig_per_ind, ver_sup_sig_per_ind;

    for (unsigned int i = 0; i < ncaras; ++i) {
      ver_tapainf_ind = i*n_vert_per;
      ver_tapasup_ind = n_vert_per-1 + i*n_vert_per;

      if (cerrar_malla){
        ver_inf_sig_per_ind = (ver_tapainf_ind + n_vert_per) % (nperfiles * n_vert_per);
        ver_sup_sig_per_ind = (ver_tapasup_ind + n_vert_per) % (nperfiles * n_vert_per);
      }
      else{
        ver_inf_sig_per_ind = ver_tapainf_ind + n_vert_per;
        ver_sup_sig_per_ind = ver_tapasup_ind + n_vert_per;
      }

      tabla_caras.push_back ({ver_tapainf_ind, ver_inf_sig_per_ind, tapa_inf_ver_ind});
      tabla_caras.push_back ({ver_tapasup_ind, ver_sup_sig_per_ind, tapa_sup_ver_ind});
    }
  }
}

// *****************************************************************************

Cilindro::Cilindro( const unsigned num_verts_per,
        const unsigned nperfiles,
        const bool crear_tapas,
        const bool cerrar_malla,
        const bool crear_texturas)
    : MallaRevol ( "malla por revolución de un cilindro" )
{
  const float radio = 1.0;
  const float altura = 1.0;
  vector<Tupla3f> perfil_malla;

  perfil_malla.push_back({radio, altura/2.0, 0.0});
  perfil_malla.push_back({radio, -altura/2.0, 0.0});

  crearMallaRevol (perfil_malla, nperfiles, crear_tapas, cerrar_malla, crear_texturas);
}

// *****************************************************************************

Cono::Cono( const unsigned num_verts_per,
        const unsigned nperfiles,
        const bool crear_tapas,
        const bool cerrar_malla,
        const bool crear_texturas)
    : MallaRevol ( "malla por revolución de un cono" )
{
  const float radio = 1.0;
  const float altura = 1.0;
  vector<Tupla3f> perfil_malla;

  perfil_malla.push_back({radio, -altura/2.0, 0.0});
  perfil_malla.push_back({0.0, altura/2.0, 0.0});

  crearMallaRevol (perfil_malla, nperfiles, crear_tapas, cerrar_malla, crear_texturas);
}

// *****************************************************************************

Esfera::Esfera( const unsigned num_verts_per,
        const unsigned nperfiles,
        const bool crear_tapas,
        const bool cerrar_malla,
        const bool crear_texturas)
    : MallaRevol ( "malla por revolución de una esfera" )
{
   const float radio = 1.0;
   Matriz4f matriz;
   vector<Tupla3f> perfil_malla;

	perfil_malla.push_back({0.0,-radio,0.0});

	float angbase = 180.0/(num_verts_per-1);
	for(int i=0; i < num_verts_per; i++){
		matriz = MAT_Rotacion( angbase*(i), 0, 0, 1 );
		Tupla4f h( perfil_malla[0][0], perfil_malla[0][1], perfil_malla[0][2], 1 );
		Tupla4f rota = matriz*h;
		perfil_malla.push_back({rota[0],rota[1],rota[2]});
	}

  crearMallaRevol (perfil_malla, nperfiles, crear_tapas, cerrar_malla, crear_texturas);
}
