function(load_raygui)
    include(FetchContent)

    FetchContent_Declare(
        raygui
        GIT_REPOSITORY https://github.com/raysan5/raygui
        GIT_TAG master
    )

    FetchContent_MakeAvailable(raygui)
endfunction()