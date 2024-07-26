
set( SOURCES
    main.cpp
    Timer.cpp
)

add_executable(main ${SOURCES})

target_link_options(main PUBLIC -lpthread )