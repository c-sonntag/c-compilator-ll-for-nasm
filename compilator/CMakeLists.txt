project(Compilateur)
cmake_minimum_required(VERSION 3.2)

file(GLOB LIB_FILES
 "util/*.c"
 "syntaxical/*.c"
 "semantique/*.c"
 "lexical/*.c"
 "generation/*.c"
 )

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options("-Wall")       ## Toute erreur
  #add_compile_options("-Wno-format") ## sauf format pour printf (Win32 have no %lu for size_t)

  set(CMAKE_C_COMPILER "gcc")        ## Force CMake to build only in C

  if(CMAKE_BUILD_TYPE STREQUAL "Release") # (not debug)
    add_compile_options("-O3")         ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")   ## Strip binary (no work on debug)
  endif()

endif()


file(GLOB_RECURSE  HEADER_FILES "*.h")  # Needs to add header into QtCreator project

set(FILES ${LIB_FILES} ${HEADER_FILES})

include_directories(".")

link_libraries("-lm")

add_executable("compilo" "compilo.c" ${FILES})
add_executable("dump_symb" "dump_symb.c" ${FILES})
