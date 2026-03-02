project(server-ex5)

find_package(Boost REQUIRED COMPONENTS json)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    client.h
    client.cpp
    generator.h
    generator.cpp
    main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
