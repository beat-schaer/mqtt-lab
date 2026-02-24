project(chat-work)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    client.h
    client.cpp
    input_prompt.h
    main.cpp
    reader.h
    reader.cpp
)
