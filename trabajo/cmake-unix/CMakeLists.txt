## ----------------------------------------------------------------------------
## Prácticas de Informática Gráfica
##
## Archivo de configuración para compilar usando CMake en Linux o macOS
## última modificación: 20 Julio de 2019
## ----------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.5.1)
project (pracs_ig_exe)

## opciones para el compilador
set (opcs_warnings "-Wall -Wextra -Wno-unused-parameter -Wfatal-errors" )

## include de todo lo demás, en 'recursos/make' ...
include( ../../recursos/make/include-cmake-unix.txt)
