project(iMCA)

set(CMAKE_AUTORCC ON)

file(GLOB SOURCES 
        widgets/*.cpp 
        utils/*.cpp 
        widgets/bar/*.cpp
        widgets/dialogs/*.cpp
        widgets/tab/*.cpp
        main.cpp
)
file(GLOB HEADERS include/*.h)

file(GLOB RESOURCES *.qrc)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Charts Qt6::Gui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
)