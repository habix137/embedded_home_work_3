#include <iostream>
#include <fcntl.h>      // shm_open
#include <sys/mman.h>   // mmap, PROT_*, MAP_*
#include <sys/stat.h>   // mode constants
#include <unistd.h>     // ftruncate, close
#include <cerrno>
#include <cstring>

#define SHM_SIZE 10
const char* SHM_NAME = "/my_shm";

int main() {
    // 1) Open or create the shared memory object
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        std::cerr << "shm_open error: " << std::strerror(errno) << "\n";
        return 1;
    }

    // 2) Resize it to SHM_SIZE bytes
    if (ftruncate(fd, SHM_SIZE) == -1) {
        std::cerr << "ftruncate error: " << std::strerror(errno) << "\n";
        close(fd);
        return 1;
    }

    // 3) Map it into memory (read/write)
    void* ptr = mmap(nullptr, SHM_SIZE, PROT_READ | PROT_WRITE,
                     MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "mmap error: " << std::strerror(errno) << "\n";
        close(fd);
        return 1;
    }

    // 4) Prompt user and write into the segment
    std::cout << "Enter up to " << SHM_SIZE << " characters: ";
    std::string input;
    std::getline(std::cin, input);

    size_t len = std::min(input.size(), (size_t)SHM_SIZE);
    std::memcpy(ptr, input.c_str(), len);
    if (len < SHM_SIZE) {
        // zero-pad the rest
        std::memset(static_cast<char*>(ptr) + len, 0, SHM_SIZE - len);
    }

    // 5) Clean up
    munmap(ptr, SHM_SIZE);
    close(fd);

    std::cout << "Wrote " << len << " bytes to " << SHM_NAME << "\n";
    return 0;
}
