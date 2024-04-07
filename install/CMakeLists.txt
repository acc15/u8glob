include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(PKG_DIR lib/cmake/${PROJECT_NAME})

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY ExactVersion
)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets FILE_SET HEADERS)
install(EXPORT ${PROJECT_NAME}Targets FILE ${PROJECT_NAME}Targets.cmake DESTINATION ${PKG_DIR})
install(
    FILES 
        "${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" 
    DESTINATION 
        ${PKG_DIR}
)
