add_executable(Mew main.c)
target_link_libraries(Mew Ditto)
install(TARGETS Mew DESTINATION ${Ditto_BINARY_DIR}/bin)