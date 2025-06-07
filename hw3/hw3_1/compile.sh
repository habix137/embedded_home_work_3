g++ -std=c++17 \
    -Iinclude \
    src/main.cpp src/ssh_connection.cpp src/hardware_monitor.cpp \
    -o ssh_cpu \
    $(pkg-config --cflags --libs libssh)
