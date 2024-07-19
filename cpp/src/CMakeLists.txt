
set( SOURCES
    main.cpp
    Thread.cpp
)

add_executable(main ${SOURCES})

target_link_options(main PUBLIC -lpthread )