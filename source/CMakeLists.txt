cmake_minimum_required(VERSION 3.5.1)

include(GNUInstallDirs)

include(CMakePrintHelpers)

file(GLOB LEXER "lexer/*.cpp")
file(GLOB PARSER "parser/*.cpp")

cmake_print_variables(LEXER)
cmake_print_variables(PARSER)
cmake_print_variables(PROJECT_NAME)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp ${LEXER} ${PARSER})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME}
 PUBLIC
  lexer
  parser
)

# find_package(SDL2 REQUIRED)
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(SDL2_ttf REQUIRED sdl2)

# get_filename_component(FONT_FILE "Peepo.ttf" REALPATH)
# target_compile_definitions(${BOIDS} PRIVATE FONT_FILE="${FONT_FILE}")

# include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} libcpphelpers)

# target_link_libraries(${BOIDS} ${SDL2_LIBRARIES})
# target_link_libraries(${BOIDS} SDL2_ttf)