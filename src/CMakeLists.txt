project(MCA)
file(GLOB SOURCES widgets/*.cpp utils/*.cpp main.cpp)
file(GLOB HEADERS include/*.h)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Charts)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
)