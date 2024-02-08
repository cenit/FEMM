cmake_minimum_required(VERSION 3.10)
project(manual)

find_package(LATEX)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cflow.tex.in ${CMAKE_CURRENT_SOURCE_DIR}/cflow.tex @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/heatflow.tex.in ${CMAKE_CURRENT_SOURCE_DIR}/heatflow.tex @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/manual.tex.in ${CMAKE_CURRENT_SOURCE_DIR}/manual.tex @ONLY)

if(LATEX_COMPILER)
  add_custom_command(
    OUTPUT    ${CMAKE_CURRENT_BINARY_DIR}/manual.pdf
    COMMAND   ${LATEX_COMPILER}
    ARGS      -output-directory=${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_SOURCE_DIR}/manual.tex
    DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/manual.tex
    COMMENT   "pdflatex"
  )

add_custom_target(LaTeXDocument ALL echo
  DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/manual.pdf
)
endif()  # LATEX_COMPILER
