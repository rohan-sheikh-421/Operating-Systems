#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <thread>

void replaceIntegersWithSpaces(char *map, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (isdigit(map[i])) {
            map[i] = ' ';
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char *map = (char*)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    std::thread t1(replaceIntegersWithSpaces, map, 50);
    std::thread t2(replaceIntegersWithSpaces, map + 50, 50);

    t1.join();
    t2.join();

    if (munmap(map, 100) == -1) {
        perror("munmap");
    }

    close(fd);

    return 0;
}
