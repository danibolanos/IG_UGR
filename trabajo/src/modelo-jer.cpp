
#include "modelo-jer.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase modelo-jer.

// Constructor
C::C(){
  ponerNombre("BrazoMecánico");
  // Añadimos una textura de madera al brazo mecánico
  Textura* madera = new Textura("../recursos/imgs/text-madera.jpg");
  Material* mat_brazo = new Material(madera, 0.3, 0.7, 1.0, 40.0);
  agregar(mat_brazo);

  agregar(MAT_Traslacion(0.8,2.0,-0.15));
  inst_maquinaria = new Maquinaria();
  agregar(inst_maquinaria);
  agregar(MAT_Traslacion(-0.8,-2.0,0.15));
  inst_base = new Base();
  agregar(inst_base);
  agregar(MAT_Rotacion(180, 0, 1, 0));
  agregar(MAT_Traslacion(1.0,2.35,0.0));
  inst_apoyo = new Apoyo();
  agregar(inst_apoyo);
  entradas.back().objeto->ponerIdentificador(89328);

}

unsigned C::leerNumParametros() const {
	return 8;
}

void C::actualizarEstadoParametro(const unsigned iParam, const float tSec){
  if(iParam==0)
		inst_base->actualizarEstadoParametro(iParam,tSec);
  if(iParam==1)
    inst_maquinaria->actualizarEstadoParametro(iParam, tSec);
  if(iParam==2)
    inst_apoyo->actualizarEstadoParametro(0, tSec);
  if(iParam==3)
    inst_apoyo->actualizarEstadoParametro(1, tSec);
  if(iParam==4)
    inst_apoyo->actualizarEstadoParametro(2, tSec);
  if(iParam==5)
    inst_apoyo->actualizarEstadoParametro(3, tSec);
  if(iParam==6)
    inst_apoyo->actualizarEstadoParametro(4, tSec);
  if(iParam==7)
    inst_apoyo->actualizarEstadoParametro(5, tSec);
}

// -----------------------------------------------------------------------------

// *****************************************************************************
// Partes Brazo Mecánico
// *****************************************************************************

//***********GENERADOR****************//

Generador::Generador(){
  Objeto3D *inst_cubo = new Cubo;
  Objeto3D *inst_cil = new CilindroTapas(2, 50);
  agregar(MAT_Rotacion(180.0, {0,1,0}));
  agregar(inst_cil);
  // LightSteelBlue
	inst_cil->ponerColor({0.560784 , 0.560784 , 0.737255});
	agregar(MAT_Escalado(1.0/0.7,1.0/0.2,1.0/0.6));
	agregar(MAT_Traslacion(0,-0.3,-0.2));
	agregar(MAT_Escalado(0.05,0.2,0.05));
	agregar(inst_cubo);
	agregar(MAT_Escalado(1.0/0.05,1.0/0.2,1.0/0.05));
	agregar(MAT_Traslacion(0,0,0.4));
	agregar(MAT_Escalado(0.05,0.2,0.05));
	agregar(inst_cubo);
  // Bronce
  inst_cubo->ponerColor({0.65 , 0.49 , 0.24});
  entradas.back().objeto->ponerNombre("Generador");
  entradas.back().objeto->ponerIdentificador(342432);
}

unsigned Generador::leerNumParametros() const {
	return 1;
}

void Generador::actualizarEstadoParametro(const unsigned iParam, const float tSec){
	*leerPtrMatriz(0)=MAT_Rotacion(180.0-90.0*sin(2*M_PI*0.7*tSec), {0,1,0});
}

//***********MAQUINARIA****************//

Maquinaria::Maquinaria(){
  Objeto3D *inst_peon1 = new MallaRevolPLY("../recursos/plys/peon.ply", 50);
  Objeto3D *inst_peon2 = new MallaRevolPLY("../recursos/plys/peon.ply", 50);
  Objeto3D *inst_peon3 = new MallaRevolPLY("../recursos/plys/peon.ply", 50);
  agregar(MAT_Traslacion(0.0, -0.1, 0.0));
  agregar(MAT_Rotacion(90, {0, 1, 0}));
  agregar(MAT_Traslacion(-0.5,0,0));
  agregar(MAT_Escalado(0.3,0.3,0.3));
  agregar(inst_peon1);
  // Gold
  inst_peon1->ponerColor({0.8 , 0.498039 , 0.196078});
  agregar(MAT_Traslacion(1.7,0,0));
  agregar(MAT_Escalado(0.7,0.7,0.7));
  agregar(inst_peon2);
  // Gold + dark
  inst_peon2->ponerColor({0.9 , 0.598039 , 0.196078});
  agregar(MAT_Traslacion(1.7,0,0));
  agregar(MAT_Escalado(0.6,0.6,0.6));
  agregar(inst_peon3);
  // Gold ++ dark
  inst_peon3->ponerColor({0.9 , 0.698039 , 0.196078});
  entradas.back().objeto->ponerNombre("Maquinaria");
  entradas.back().objeto->ponerIdentificador(984298);
}

unsigned Maquinaria::leerNumParametros() const {
	return 1;
}

void Maquinaria::actualizarEstadoParametro(const unsigned iParam, const float tSec){
	*leerPtrMatriz(0)=MAT_Traslacion(0.0, -0.1-0.2*sin(2*M_PI*0.7*tSec), 0.0);
}

//***********BASE****************//

Base::Base()
{
  Objeto3D *inst_cubo1 = new Cubo;
  Objeto3D *inst_cubo2 = new Cubo;
  Objeto3D *inst_cubo3 = new Cubo;
  Objeto3D *inst_cil1 = new CilindroTapas(2, 50);
  Objeto3D *inst_cil2 = new CilindroTapas(2, 50);
  agregar(MAT_Escalado(3.0,0.2,3.0));
	agregar(inst_cubo1);
  // DarkSlateGrey
  inst_cubo1->ponerColor({0.184314 , 0.309804 , 0.309804});
	agregar(MAT_Escalado(1.0/3.0,1.0/0.2,1.0/3.0));
	agregar(MAT_Traslacion(0,0.2,0));
	agregar(MAT_Escalado(2.7,0.1,2.7));
	agregar(inst_cubo2);
  // SteelBlue
  inst_cubo2->ponerColor({0.137255 , 0.419608 , 0.556863});
	agregar(MAT_Escalado(1.0/2.7,1.0/0.1,1.0/2.7));
	agregar(MAT_Traslacion(0,0.1,0));
	agregar(MAT_Escalado(2.5,0.1,2.5));
	agregar(inst_cil1);
  // DarkSlateGrey
	inst_cil1->ponerColor({0.184314 , 0.309804 , 0.309804});
	agregar(MAT_Escalado(1.0/2.5,1.0/0.1,1.0/2.5));
	agregar(MAT_Traslacion(-1.0,1.0,0));
	agregar(MAT_Escalado(1.0,2.0,1.0));
	agregar(inst_cil2);
  // FireBrick
	inst_cil2->ponerColor({0.556863 , 0.137255 , 0.137255});
	agregar(MAT_Escalado(1.0/1.0,1.0/2.0,1.0/1.0));
	agregar(MAT_Traslacion(1.7,-0.2,0));
	agregar(MAT_Escalado(0.7,0.8,0.7));
	agregar(inst_cubo3);
  // FireBrick + dark
	inst_cubo3->ponerColor({0.656863 , 0.137255 , 0.137255});
	agregar(MAT_Escalado(1.0/0.7,1.0/0.8,1.0/0.7));
	agregar(MAT_Traslacion(0.8,0,0));
	agregar(MAT_Rotacion(90, 0, 0, 1));
	agregar(MAT_Escalado(0.7,0.2,0.6));
  inst_generador = new Generador();
	agregar(inst_generador);
  entradas.back().objeto->ponerNombre("Base");
  entradas.back().objeto->ponerIdentificador(939283);
}

unsigned Base::leerNumParametros() const {
	return 1;
}

void Base::actualizarEstadoParametro(const unsigned iParam, const float tSec){
		inst_generador->actualizarEstadoParametro(iParam,tSec);
}

//***********APOYO****************//

Apoyo::Apoyo(){
  Objeto3D *inst_cil = new CilindroTapas(2, 50);
  Objeto3D *inst_cubo = new Cubo;
  agregar(MAT_Rotacion(0.0, {0,1,0}));
  agregar(MAT_Escalado(0.9,0.1,0.9));
	agregar(inst_cil);
	// FireBrick ++ dark
	inst_cil->ponerColor({0.856863 , 0.137255 , 0.137255});
	agregar(MAT_Escalado(1.0/0.9,1.0/0.1,1.0/0.9));
	agregar(MAT_Traslacion(0,0.75,0));
	agregar(MAT_Escalado(0.7,0.7,0.7));
	agregar(inst_cubo);
	agregar(MAT_Escalado(1.0/0.7,1.0/0.7,1.0/0.7));
	// FireBrick + dark
	inst_cubo->ponerColor({0.656863 , 0.137255 , 0.137255});
  inst_brazo1 = new Brazo();
  agregar(MAT_Rotacion(15.0, {1,0,0}));
  agregar(inst_brazo1);
  inst_brazo2 = new Brazo();
  agregar(MAT_Traslacion(0,0,-2.4));
  agregar(MAT_Rotacion(-15.0, {1,0,0}));
  agregar(inst_brazo2);
  inst_giro = new Giro();
  agregar(MAT_Traslacion(1.0,-3.1,2.5));
  agregar(MAT_Traslacion(-1.0,3.1,-5.0));
  agregar(MAT_Rotacion(0.0, {0,0,1}));
  agregar(inst_giro);
  inst_pinza1 = new GiroPinza();
  agregar(MAT_Rotacion(-10.0, {0,1,0}));
  agregar(inst_pinza1);
  agregar(MAT_Rotacion(180, 0, 0, 1));
  agregar(MAT_Rotacion(-10.0, {0,1,0}));
  inst_pinza2 = new GiroPinza();
  agregar(inst_pinza2);
  entradas.back().objeto->ponerNombre("Apoyo");
  entradas.back().objeto->ponerIdentificador(535353);
}

unsigned Apoyo::leerNumParametros() const {
	return 6;
}

void Apoyo::actualizarEstadoParametro(const unsigned iParam, const float tSec){
  if(iParam==0)
    *leerPtrMatriz(0)=MAT_Rotacion(-180.0*sin(2*M_PI*0.1*tSec), {0,1,0});
  if(iParam==1)
    *leerPtrMatriz(8)=MAT_Rotacion(15.0-15.0*sin(2*M_PI*0.1*tSec), {1,0,0});
  if(iParam==2)
    *leerPtrMatriz(11)=MAT_Rotacion(-15.0-15.0*sin(2*M_PI*0.1*tSec), {1,0,0});
  if(iParam==3)
    *leerPtrMatriz(15)=MAT_Rotacion(-90.0*sin(2*M_PI*0.1*tSec), {0,0,1});
  if(iParam==4)
    *leerPtrMatriz(17)=MAT_Rotacion(-10.0-15.0*sin(2*M_PI*0.1*tSec), {0,1,0});
  if(iParam==5)
    *leerPtrMatriz(20)=MAT_Rotacion(-2*(-10.0-15.0*sin(2*M_PI*0.1*tSec)), {0,1,0});
}

//***********BRAZO****************//

Brazo::Brazo(){
  Objeto3D *inst_inf1 = new Infinito();
  Objeto3D *inst_inf2 = new Infinito();
  Objeto3D *inst_cubo = new Cubo();
  agregar(MAT_Traslacion(0,0.0,-2.2));
	agregar(MAT_Escalado(0.7,0.7,0.7));
	agregar(inst_cubo);
	// FireBrick + dark
	inst_cubo->ponerColor({0.656863 , 0.137255 , 0.137255});
	agregar(MAT_Escalado(1.0/0.7,1.0/0.7,1.0/0.7));
	agregar(MAT_Traslacion(-0.7,0,1.0));
	agregar(MAT_Rotacion(90, 1, 0, 0));
	agregar(MAT_Rotacion(90, 0, 0, 1));
	agregar(MAT_Escalado(0.8,1.0,0.8));
	agregar(inst_inf1);
	agregar(MAT_Traslacion(0,-1.4,0));
	agregar(MAT_Rotacion(90, 0, 0, 1));
	agregar(MAT_Rotacion(90, 0, 0, 1));
	agregar(inst_inf2);
  entradas.back().objeto->ponerNombre("Brazo");
  entradas.back().objeto->ponerIdentificador(39428);
}

// PIEZAS EXTRA

Infinito::Infinito(){
  Objeto3D *inst_cubo = new Cubo();
  Objeto3D *inst_cil = new CilindroTornillos();
	agregar(MAT_Rotacion(180, 0, 1, 0));
	agregar(MAT_Traslacion(0,0.1,0));
	agregar(MAT_Escalado(0.9,0.1,0.4));
	agregar(inst_cubo);
	// SteelBlue
	inst_cubo->ponerColor({0.137255 , 0.419608 , 0.556863});
	agregar(MAT_Escalado(1.0/0.9,1.0/0.1,1.0/0.4));
	agregar(MAT_Traslacion(-1.5,0,0));
	agregar(MAT_Escalado(1.0,0.2,1.0));
	agregar(inst_cil);
	agregar(MAT_Escalado(1.0,1.0/0.2,1.0));
	agregar(MAT_Traslacion(3.0,0,0));
	agregar(MAT_Escalado(1.0,0.2,1.0));
	agregar(MAT_Rotacion(180, 0, 1, 0));
	agregar(inst_cil);
  entradas.back().objeto->ponerNombre("Infinito");
  entradas.back().objeto->ponerIdentificador(893299);
}

CilindroTornillos::CilindroTornillos(){
  Objeto3D *inst_cil = new CilindroTapas(2, 50);
  Objeto3D *inst_cil1 = new CilindroTapas(2, 50);
  Objeto3D *inst_cil2 = new CilindroTapas(2, 50);
  Objeto3D *inst_cono = new Cono(2, 50);
	agregar(inst_cil);
	// SteelBlue
	inst_cil->ponerColor({0.137255 , 0.419608 , 0.556863});
	agregar(MAT_Traslacion(-0.5,0.6,0.6));
	agregar(MAT_Escalado(0.1,0.4,0.1));
	agregar(inst_cil1);
	// DarkSlateGrey
	inst_cil1->ponerColor({0.184314 , 0.309804 , 0.309804});
	agregar(MAT_Escalado(1.0/0.1,1.0/0.1,1.0/0.1));
	agregar(MAT_Traslacion(0,0,-1.2));
	agregar(MAT_Escalado(0.1,0.1,0.1));
	agregar(inst_cil2);
	// DarkSlateGrey
	inst_cil2->ponerColor({0.184314 , 0.309804 , 0.309804});
	agregar(MAT_Escalado(1.0/0.1,1.0/0.1,1.0/0.1));
	agregar(MAT_Traslacion(0.5,-0.1,0.6));
	agregar(MAT_Escalado(0.3,0.3,0.3));
	agregar(inst_cono);
	// GreenForest
	inst_cono->ponerColor({0.137255 , 0.556863 , 0.137255});
  entradas.back().objeto->ponerNombre("CilindroTornillos");
  entradas.back().objeto->ponerIdentificador(93843923);
}

Giro::Giro(){
  Objeto3D *inst_esf = new Esfera(20, 50);
	agregar(MAT_Rotacion(90, 1, 0, 0));
	agregar(MAT_Escalado(0.7,1.0,0.7));
	agregar(inst_esf);
	// GreenForest
	inst_esf->ponerColor({0.137255 , 0.556863 , 0.137255});
  entradas.back().objeto->ponerNombre("Giro");
  entradas.back().objeto->ponerIdentificador(2323);
}

Pinza::Pinza(){
  Objeto3D *inst_cub = new Cubo();
	agregar(MAT_Rotacion(-20, 0, 1, 0));
	agregar(MAT_Traslacion(1.0,0,-0.8));
	agregar(MAT_Escalado(0.7,0.4,0.08));
	agregar(inst_cub);
	agregar(MAT_Escalado(1.0/0.7,1.0/0.4,1.0/0.08));
	agregar(MAT_Traslacion(0.6,0,0.7));
	agregar(MAT_Rotacion(89, 0, 1, 0));
	agregar(MAT_Escalado(0.7,0.4,0.08));
	agregar(inst_cub);
	// Bronce
	inst_cub->ponerColor({0.65 , 0.49 , 0.24});

  entradas.back().objeto->ponerNombre("Pinza");
  entradas.back().objeto->ponerIdentificador(9843);
}

GiroPinza::GiroPinza(){
  Objeto3D *inst_pinza = new Pinza();
	agregar(MAT_Rotacion(90, 1, 0, 0));
	agregar(MAT_Traslacion(-0.2,-1.0,0));
	agregar(MAT_Rotacion(90, 1, 0, 0));
	agregar(MAT_Escalado(0.8,0.8,0.8));
	agregar(inst_pinza);

  entradas.back().objeto->ponerNombre("GiroPinza");
  entradas.back().objeto->ponerIdentificador(129549);
}

// ****************************************************************************
