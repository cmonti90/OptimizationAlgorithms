

set( target MetaOpt )

set( SOURCES
    Timer.cpp
    ThreadPool.cpp
    Semaphore.cpp
)

add_library( ${target} STATIC ${SOURCES} )

target_include_directories( ${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_options( ${target} PUBLIC -lpthread )


#####################################################
#####################################################


set( target main )

set( SOURCES
    main.cpp
)

add_executable( ${target} ${SOURCES} )

target_link_libraries( ${target} PUBLIC MetaOpt )
