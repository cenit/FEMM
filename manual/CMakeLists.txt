cmake_minimum_required(VERSION 3.10)
project(manual)

find_package(LATEX)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cflow.tex.in ${CMAKE_CURRENT_SOURCE_DIR}/cflow.tex @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/heatflow.tex.in ${CMAKE_CURRENT_SOURCE_DIR}/heatflow.tex @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/manual.tex.in ${CMAKE_CURRENT_SOURCE_DIR}/manual.tex @ONLY)

if(LATEX_COMPILER)
  add_custom_command(
    OUTPUT    ${CMAKE_CURRENT_BINARY_DIR}/manual.dvi
    COMMAND   ${LATEX_COMPILER}
    ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/manual.tex
    DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/manual.tex
    COMMENT   "Tex2dvi"
    POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/manual.dvi ${CMAKE_CURRENT_SOURCE_DIR}/.
  )

if(DVIPS_CONVERTER)
  add_custom_command(
    OUTPUT    ${CMAKE_CURRENT_SOURCE_DIR}/manual.ps
    COMMAND   ${DVIPS_CONVERTER}
    ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/manual.dvi
              -o ${CMAKE_CURRENT_SOURCE_DIR}/manual.ps
    DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/manual.dvi
    COMMENT   "dvi2ps"
 )

if(PS2PDF_CONVERTER)
  add_custom_command(
    OUTPUT    ${CMAKE_CURRENT_SOURCE_DIR}/manual.pdf
    COMMAND   ${PS2PDF_CONVERTER}
    ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/manual.ps
    DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/manual.ps
    COMMENT   "ps2pdf"
  )

add_custom_target(LaTeXDocument ALL echo
  DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/manual.pdf
)
endif()  # PS2PDF_CONVERTER
endif()  # DVIPS_CONVERTER
endif()  # LATEX_COMPILER
