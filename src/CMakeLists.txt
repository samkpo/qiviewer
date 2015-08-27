ADD_DEFINITIONS(${QT_DEFINITIONS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/webp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/defines.h.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/defines.h)

#app ui's
file(GLOB_RECURSE app_UIS forms/*.ui)   
QT4_WRAP_UI(app_UIS_H ${app_UIS})

#resorces
QT4_ADD_RESOURCES(RESOURCE_FILES ../resources/qiviewer.qrc)
SET(DESKTOP_FILE ../resources/qiviewer.desktop)

#code files
file(GLOB_RECURSE CODE_FILES *.cpp *.c)

################################################################################
############################## Translations stuff ##############################
################################################################################
SET(LANGUAGES cs  el  es_AR  es_ES  pt ru_RU)
MACRO (QT4_WRAP_TS lupdate_outputs lrelease_outputs)
    FOREACH (it ${ARGN})
      SET(tsfile ${CMAKE_CURRENT_SOURCE_DIR}/translations/${it}.ts)
      ADD_CUSTOM_COMMAND(OUTPUT ${tsfile}
        COMMAND ${QT_LUPDATE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR} -ts ${tsfile}
      )
      
      SET(qmfile ${CMAKE_CURRENT_BINARY_DIR}/locale/${it}.qm)
      ADD_CUSTOM_COMMAND(OUTPUT ${qmfile}
        COMMAND ${QT_LRELEASE_EXECUTABLE} ${tsfile} -qm ${qmfile}
      )
      
      SET(${lupdate_outputs} ${${lupdate_outputs}} ${tsfile})
      SET(${lrelease_outputs} ${${lrelease_outputs}} ${qmfile})
    ENDFOREACH(it)
ENDMACRO (QT4_WRAP_TS)

QT4_WRAP_TS(lupdate_outputs lrelease_outputs
  ${LANGUAGES}
)
ADD_CUSTOM_TARGET(lupdate-qt4
  DEPENDS ${lupdate_outputs}
)
ADD_CUSTOM_TARGET(lrelease-qt4
  DEPENDS ${lrelease_outputs}
)

# SET(qm_files)
# foreach(a ${LANGUAGES})
#   set(qm_file "${CMAKE_CURRENT_BINARY_DIR}/locale/${a}.qm")
#   set(qm_files ${qm_file} ${qm_files})
#   message(${qm_file})
# endforeach(a ${LANGUAGES})

###############################################################################
################################## Libraries ##################################
###############################################################################

#create executalbe
add_executable(${CMAKE_PROJECT_NAME}
  ${CODE_FILES}
  ${RESOURCE_FILES}
  ${app_UIS_H}
  ${qm_files})
target_link_libraries(${CMAKE_PROJECT_NAME} ${QT_LIBRARIES})

#installation
INSTALL(TARGETS ${CMAKE_PROJECT_NAME}
        RUNTIME
        DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
INSTALL(FILES ${qm_files}
        DESTINATION ${PKGDATADIR}/locale
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

INSTALL(PROGRAMS ${DESKTOP_FILE} DESTINATION ${XDG_APPS_INSTALL_DIR} )

SET(DDM_PIXMAPS_DIR     "/usr/share/icons/")
INSTALL (FILES ../resources/qiviewer.png DESTINATION ${DDM_PIXMAPS_DIR})
