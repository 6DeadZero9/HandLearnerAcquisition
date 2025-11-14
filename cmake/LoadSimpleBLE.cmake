function(load_simple_ble)
    include(FetchContent)

    FetchContent_Declare(
        simpleble
        SOURCE_SUBDIR simpleble
        GIT_REPOSITORY https://github.com/simpleble/simpleble.git
        GIT_TAG main
    )

    FetchContent_MakeAvailable(simpleble)
    FetchContent_GetProperties(simpleble)
endfunction()