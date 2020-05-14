# Edit following two lines to set component requirements (see docs)
idf_component_register(SRCS "src/input_manager.cpp" "src/input_manager_builder.cpp" "src/input_types.cpp"
                  INCLUDE_DIRS "include"
                  REQUIRES freertos)
