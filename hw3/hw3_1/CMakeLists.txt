# ─────────── 1. Boiler-plate ───────────────────────────────────────────
cmake_minimum_required(VERSION 3.16)
project(remote_monitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─────────── 2. External libraries we depend on ────────────────────────
# libssh (via pkg-config, covers Debian/Ubuntu, Fedora, Arch …)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSSH REQUIRED libssh)     # gives us include dirs & libs

# {fmt} (modern CMake package ships with libfmt-dev on Ubuntu ≥22.04)
find_package(fmt REQUIRED)

# ─────────── 3. Library: sshconnection  (pure SSH wrapper) ─────────────
add_library(sshconnection STATIC
    src/ssh_connection.cpp
)
target_include_directories(sshconnection
    PUBLIC
        include
        ${LIBSSH_INCLUDE_DIRS}
)
target_link_libraries(sshconnection
    PUBLIC
        ${LIBSSH_LIBRARIES}         # usually just -lssh
)

# ─────────── 4. Library: hardware_monitor  (monitor logic) ─────────────
add_library(hardware_monitor STATIC
    src/hardware_monitor.cpp
)
target_include_directories(hardware_monitor
    PUBLIC
        include
)
target_link_libraries(hardware_monitor
    PUBLIC
        sshconnection               # needs exec over SSH
        fmt::fmt                    # uses fmt::print / fmt::format
)

# ─────────── 5. Final executable that ties everything together ─────────
add_executable(remote_monitor
    src/main.cpp
)
target_link_libraries(remote_monitor
    PRIVATE
        hardware_monitor
        fmt::fmt                    # make sure linker gets libfmt
)

# ─────────── 6. Optional: install step (cmake --install .) ─────────────
include(GNUInstallDirs)
install(TARGETS remote_monitor RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
