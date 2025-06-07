#include <iostream>
#include <fcntl.h>      // shm_open
#include <sys/mman.h>   // mmap, PROT_*, MAP_*
#include <unistd.h>     // close, munmap
#include <cerrno>
#include <cstring>

#define SHM_SIZE 10
const char* SHM_NAME = "/my_shm";

int main() {
    // 1) Open existing shared memory object (read-only)
    int fd = shm_open(SHM_NAME, O_RDONLY, 0);
    if (fd == -1) {
        std::cerr << "shm_open error: " << std::strerror(errno) << "\n";
        return 1;
    }

    // 2) Map it into memory (read-only)
    void* ptr = mmap(nullptr, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "mmap error: " << std::strerror(errno) << "\n";
        close(fd);
        return 1;
    }

    // 3) Read and display exactly SHM_SIZE bytes
    std::cout << "Read from " << SHM_NAME << ": '";
    std::cout.write(static_cast<char*>(ptr), SHM_SIZE);
    std::cout << "'\n";

    // 4) Clean up
    munmap(ptr, SHM_SIZE);
    close(fd);

    return 0;
}
