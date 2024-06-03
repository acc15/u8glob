
function(copy_bin_to_target_dir bin_target target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            $<TARGET_FILE:${bin_target}> 
            $<TARGET_FILE_DIR:${target}>
    )
endfunction()

function(copy_bin_to_executables) 
    get_directory_property(dir_targets BUILDSYSTEM_TARGETS)
    foreach(target "${dir_targets}")
        get_target_property(target_type ${target} TYPE)
        if(target_type STREQUAL EXECUTABLE)
            get_target_property(target_libs ${target} LINK_LIBRARIES)
            foreach(bin_target "${ARGN}")
                if(${bin_target} IN_LIST target_libs)
                    copy_bin_to_target_dir(${bin_target} ${target})
                endif()                
            endforeach()
        endif()
    endforeach()
endfunction()
