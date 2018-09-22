# compile main executable
add_executable(main.x main.cpp)
target_include_directories(
    main.x
    PRIVATE
        ${PROJECT_BINARY_DIR}
    )

# library that holds sources to test
add_library(playfair "")
target_sources(
    playfair
    PRIVATE
        playfair.cpp
        playfaircrack.cpp
        scoretext.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/playfair.h
        ${CMAKE_CURRENT_LIST_DIR}/playfaircrack.h
        ${CMAKE_CURRENT_LIST_DIR}/scoretext.h
    )
target_include_directories(
    playfair
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    )

# link library
target_link_libraries(main.x playfair)

# install binary
install(TARGETS main.x DESTINATION playfair/bin)
