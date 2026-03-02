project(client-ex5)

find_package(Boost REQUIRED COMPONENTS json)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    client.h
    client.cpp
    input_prompt.h
    limits_reader.h
    limits_reader.cpp
    main.cpp
    reader_base.h
    reader_base.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
