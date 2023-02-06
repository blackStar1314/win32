
file(GLOB_RECURSE SOURCES FOLLOW_SYMLINKS LIST_DIRECTORIES false CONFIGURE_DEPENDS
     *.h
     *.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_executable("SMBIOS-information")
target_sources("SMBIOS-information" PRIVATE ${SOURCES})