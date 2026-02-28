project(chat-ex4)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    client.h
    client.cpp
    input_prompt.h
    main.cpp
    reader_base.h
    reader_base.cpp
    publish_reader.h
    publish_reader.cpp
    subscribe_reader.h
    subscribe_reader.cpp
)
