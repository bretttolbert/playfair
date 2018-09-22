add_executable(
    unit_tests
    test_playfair.cpp
    test_playfaircrack.cpp
    test_scoretext.cpp
    )

target_link_libraries(
    unit_tests
    gtest_main
    playfair
    )

add_test(
  NAME
    unit
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
  )
