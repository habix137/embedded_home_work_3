g++ -std=c++11 shm-writer.cpp -o shm-writer -lrt
g++ -std=c++11 shm-reader.cpp -o shm-reader -lrt
./shm-writer
# (enter up to 10 chars)

./shm-reader
