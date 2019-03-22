#include <cassert>
#include <cmath>
#include "Parametro.hpp"
#include "Objeto3D.hpp"

// -----------------------------------------------------------------------------

// crear grado de libertad no acotado

Parametro::Parametro(
  const std::string &  p_descripcion,
  Matriz4f *           p_ptr_mat,
  TFuncionCMF          p_fun_calculo_matriz,
  bool                 p_acotado,
  float                p_c,
  float                p_s,
  float                p_f
): descripcion(p_descripcion), ptr_mat(p_ptr_mat), fun_calculo_matriz(p_fun_calculo_matriz),
   acotado(p_acotado), c(p_c), s(p_s), f(p_f)
{
  velocidad = v0;
  valor_norm = 0;
}

// -----------------------------------------------------------------------------

void Parametro::actualizar_matriz()
{
  *ptr_mat = fun_calculo_matriz(leer_valor_actual());
}

// -----------------------------------------------------------------------------

void Parametro::siguiente_cuadro()
{
  valor_norm += velocidad;
  actualizar_matriz();
}
// -----------------------------------------------------------------------------

void Parametro::reset()
{
  valor_norm = 0;
  velocidad = v0;
  actualizar_matriz();
}
// -----------------------------------------------------------------------------

void Parametro::incrementar()
{
  valor_norm += incremento;
  actualizar_matriz();
}

// -----------------------------------------------------------------------------

void Parametro::decrementar()
{
  valor_norm -= incremento;
  actualizar_matriz();
}

// -----------------------------------------------------------------------------

void Parametro::acelerar()
{
  velocidad += a;
}

// -----------------------------------------------------------------------------

void Parametro::decelerar()
{
  velocidad -= a;
  if (velocidad < 0) velocidad = 0;
}

// -----------------------------------------------------------------------------

float Parametro::leer_valor_actual()
{
  if (acotado)
    return c + s * sin(2*M_PI*f*valor_norm);
  else // no acotado
    return c + s * valor_norm;
}

// -----------------------------------------------------------------------------

float Parametro::leer_velocidad_actual()
{
  return velocidad;
}

// -----------------------------------------------------------------------------

std::string Parametro::leer_descripcion()
{
  return descripcion;
}

// -----------------------------------------------------------------------------
