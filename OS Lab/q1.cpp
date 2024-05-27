#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

void replaceWordInFile(const char *fileName, const char *wordToReplace, const char *replacementWord) {
    int fd = open(fileName, O_RDWR);
    if (fd == -1) {
        perror("open");
        return;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    char *map = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }

    for (size_t i = 0; i < size - strlen(wordToReplace); ++i) {
        if (memcmp(map + i, wordToReplace, strlen(wordToReplace)) == 0) {
            memcpy(map + i, replacementWord, strlen(replacementWord));
        }
    }

    if (munmap(map, size) == -1) {
        perror("munmap");
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <file> <wordToReplace> <replacementWord>\n";
        return 1;
    }

    replaceWordInFile(argv[1], argv[2], argv[3]);

    return 0;
}
