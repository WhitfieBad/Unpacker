cmake_minimum_required(VERSION 3.20)
project(Unpacker C ASM)
SET(OBJ ${CMAKE_CURRENT_SOURCE_DIR}/data/data.o)

add_executable(Unpacker ${OBJ} src/entry.c src/entry.h src/zip.c src/zip.h src/miniz.h src/loader.c src/loader.h)

