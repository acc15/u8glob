function(configure_install)
    
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    set(targets "${ARGN}")
    set(cmake_dir ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
    set(cmake_config_file ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake)
    set(cmake_version_file ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake)
    set(cmake_targets ${PROJECT_NAME}-targets)
    set(cmake_targets_file ${CMAKE_CURRENT_BINARY_DIR}/${cmake_targets}.cmake)
    set(namespace ${PROJECT_NAME}::)

    configure_package_config_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/config.cmake.in ${cmake_config_file} INSTALL_DESTINATION ${cmake_dir})
    write_basic_package_version_file(${cmake_version_file} VERSION ${PROJECT_VERSION} COMPATIBILITY ExactVersion)
    install(TARGETS "${targets}" EXPORT ${cmake_targets} LIBRARY FILE_SET HEADERS RUNTIME)
    install(EXPORT ${cmake_targets} DESTINATION ${cmake_dir} NAMESPACE ${namespace})
    install(FILES ${cmake_config_file} ${cmake_version_file} DESTINATION ${cmake_dir})
    export(EXPORT ${cmake_targets} FILE ${cmake_targets_file} NAMESPACE ${namespace})

    foreach(target "${targets}")
        generate_pkgconfig(${target})
    endforeach()

endfunction()

function(generate_pkgconfig target)
    
    set(pkgconfig_template ${CMAKE_CURRENT_BINARY_DIR}/${target}.pc.in)
    set(pkgconfig_dir ${CMAKE_INSTALL_LIBDIR}/pkgconfig)    

    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/config.pc.in ${pkgconfig_template} @ONLY)
    
    file(GENERATE 
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_LINKER_FILE_BASE_NAME:${target}>.pc
        INPUT ${pkgconfig_template}
        TARGET ${target}
    )

    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_LINKER_FILE_BASE_NAME:${target}>.pc
        DESTINATION ${pkgconfig_dir}
    )

endfunction()

function(copy_dll_to_target_dir dll_target target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            $<TARGET_FILE:${dll_target}> 
            $<TARGET_FILE_DIR:${target}>
    )
endfunction()

function(copy_dll_to_executables dll_target) 
    get_directory_property(dir_targets BUILDSYSTEM_TARGETS)
    foreach(target "${dir_targets}")
        get_target_property(target_type ${target} TYPE)
        if(target_type STREQUAL EXECUTABLE)
            get_target_property(target_libs ${target} LINK_LIBRARIES)
            if(${dll_target} IN_LIST target_libs)
                copy_dll_to_target_dir(${dll_target} ${target})
            endif()
        endif()
    endforeach()
endfunction()



