#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

// Macro untuk menghentikan program saat ada error
#define TERMINATE(...)              { \
                                        fprintf(stderr, "ERROR: "); \
                                        fprintf(stderr, __VA_ARGS__); \
                                        exit(1); \
                                    }

// Macro untuk memastikan suatu predikat terpenuhi
#define ASSERT(predicate, ...)      if (!(predicate)) TERMINATE(__VA_ARGS__);

// Macro untuk menentukan apakah suatu elemen ada di dalam suatu container atau tidak
#define EXIST(element, container)   ((container).find(element) != (container).end())

// Konstanta besar buffer string
#define MAX_BUFFER                  100

// Konstanta toleransi panjang string yang tidak dianggap sebagai kata pada TTS
#define WORD_THRESHOLD              1

// Nama kamus default
#define DEFAULT_DICTIONARY_FILENAME "kamus.txt"

/**
 * Tipe data yang merepresentasikan arah kata. Ada dua, yaitu mendatar (ACCROSS) dan menurun (DOWN).
 */
enum Direction {
    ACCROSS, DOWN
};

/**
 * Tipe data yang merepresentasikan koordinat.
 */
struct Coordinate {
    int row, col;

    Coordinate() : row(0), col(0) {}

    Coordinate(const Coordinate &other) : row(other.row), col(other.col) {}

    Coordinate(int row, int col) {
        this->row = row;
        this->col = col;
    }

    Coordinate operator+(Coordinate other) {
        return Coordinate(other.row + row, other.col + col);
    }

    void operator+=(Coordinate other) {
        row += other.row;
        col += other.col;
    }

    Coordinate operator-(Coordinate other) {
        return Coordinate(row - other.row, col - other.col);
    }

    void operator-=(Coordinate other) {
        row -= other.row;
        col -= other.col;
    }

    bool operator==(Coordinate other) {
        return row == other.row && col == other.col;
    }

    static Coordinate STEP[2];

    void advance(int direction) {
        switch (direction) {
            case ACCROSS:
            case DOWN:
                *this += Coordinate::STEP[direction];
                break;
        }
    }
};

// Konstruktor untuk konstanta koordinat
Coordinate Coordinate::STEP[2] = {Coordinate(0, 1), Coordinate(1, 0)};

bool operator<(Coordinate a, Coordinate b) {
    return a.row != b.row ? a.row < b.row : a.col < b.col;
}

/**
 * Tipe data yang merepresentasikan kata pada suatu Crossword
 */
struct CrosswordWord {
    string word;
    Direction direction;
    Coordinate start;

    CrosswordWord() {}

    CrosswordWord(const CrosswordWord &other) :
            word(other.word), direction(other.direction), start(other.start) {}

    CrosswordWord(string word, Direction direction, int row, int col) :
            word(word), direction(direction), start(row, col) {}

    static Direction getDirection(string direction) {
        if (direction == "MENDATAR") {
            return ACCROSS;
        } else if (direction == "MENURUN") {
            return DOWN;
        } else {
            // default value
            return ACCROSS;
        }
    }
};

/**
 * Mencari batasan pada suatu crossword grid.
 */
void getCrosswordGridBounds(map<Coordinate, char> &crosswordGrid,
        Coordinate* topLeft, Coordinate* bottomRight) {
    bool firstCharacter = true;

    for (map<Coordinate, char>::iterator positionIterator = crosswordGrid.begin();
            positionIterator != crosswordGrid.end(); ++positionIterator) {
        Coordinate position = positionIterator->first;
        // Memperbarui batas crossword: topLeft dan bottomRight
        if (firstCharacter) {
            firstCharacter = false;
            *topLeft = *bottomRight = position;
        } else {
            if (position.row < topLeft->row) {
                topLeft->row = position.row;
            }
            if (position.col < topLeft->col) {
                topLeft->col = position.col;
            }
            if (position.row > bottomRight->row) {
                bottomRight->row = position.row;
            }
            if (position.col > bottomRight->col) {
                bottomRight->col = position.col;
            }
        }
    }
}

/**
 * Mencetak crossword dengan format tertentu.
 */
void printCrossword(map<Coordinate, char> &crosswordGrid) {
    int row, col;

    // Mencari batasan grid
    Coordinate topLeft, bottomRight;
    getCrosswordGridBounds(crosswordGrid, &topLeft, &bottomRight);

    // Formatnya adalah sebagai berikut:
    // Atas         :  --- ---
    // Huruf        : | X | Y |
    // Alas/Atas    :  --- ---
    for (row = topLeft.row; row <= bottomRight.row; ++row) {
        // Atas
        if (row == topLeft.row) {
            for (col = topLeft.col; col <= bottomRight.col; ++col) {
                if (EXIST(Coordinate(row, col), crosswordGrid)) {
                    printf(" ---");
                } else {
                    printf("    ");
                }
            }
            printf(" \n");
        }

        // Huruf
        for (col = topLeft.col; col <= bottomRight.col; ++col) {
            if (EXIST(Coordinate(row, col), crosswordGrid)) {
                printf("| %c ", crosswordGrid[Coordinate(row, col)]);
            } else if (EXIST(Coordinate(row, col - 1), crosswordGrid)) {
                printf("|   ");
            } else {
                printf("    ");
            }
        }
        if (EXIST(Coordinate(row, bottomRight.col), crosswordGrid)) {
            printf("|\n");
        } else {
            printf(" \n");
        }

        // Alas/Atas
        for (col = topLeft.col; col <= bottomRight.col; ++col) {
            if (EXIST(Coordinate(row, col), crosswordGrid) ||
                    EXIST(Coordinate(row + 1, col), crosswordGrid)) {
                printf(" ---");
            } else {
                printf("    ");
            }
        }
        printf(" \n");
    }
    printf("\n");
}

/**
 * Mengubah karakter ASCII menjadi uppercase.
 */
char toUppercase(char c) {
    if ('a' <= c && c <= 'z') {
        return c + 'A' - 'a';
    } else {
        return c;
    }
}

/**
 * Mengubah string menjadi uppercase.
 */
void toUppercaseWord(char* c) {
    for (; *c != 0; ++c) {
        *c = toUppercase(*c);
    }
}

/**
 * Memeriksa apakah suatu string terdiri dari alfabet [a-zA-Z].
 */
bool consistsOfEnglishAlphabets(char* s) {
    for (; *s != 0; ++s) {
        if ((*s < 'a' || 'z' < *s) && (*s < 'A' || 'Z' < *s)) {
            return false;
        }
    }
    return true;
}

/**
 * Memeriksa apakah suatu string terdiri dari alfabet atau titik [a-zA-Z\.].
 */
bool consistsOfEnglishAlphabetsOrDots(char* s) {
    for (; *s != 0; ++s) {
        if ((*s < 'a' || 'z' < *s) && (*s < 'A' || 'Z' < *s) && *s != '.') {
            return false;
        }
    }
    return true;
}

char buffer[MAX_BUFFER], bufferDirection[MAX_BUFFER];

int main(int argc, char const *argv[])
{
    if (argc != 3 && argc != 4) {
        TERMINATE("%s\n%s\n%s\n%s\n", "Argumen tidak sesuai",
                "Cara kompilasi: g++ solver_checker.cpp -o solver_checker",
                "Cara pemakaian: ./solver_checker <file_input> <file_output> [<file_kamus>]",
                "                Jika <file_kamus> tidak ada, maka akan digunakan \"kamus.txt\".");
    }

    // Membuka berkas masukan dan keluaran
    FILE* fin = fopen(argv[1], "r");
    if (fin == NULL) {
        TERMINATE("Berkas masukan \"%s\" tidak ditemukan!\n", argv[1]);
    }

    FILE* fout = fopen(argv[2], "r");
    if (fout == NULL) {
        fclose(fin);
        TERMINATE("Berkas keluaran \"%s\" tidak ditemukan!\n", argv[2]);
    }

    if (argc == 4) {
        strcpy(buffer, argv[3]);
    } else {
        strcpy(buffer, DEFAULT_DICTIONARY_FILENAME);
    }
    FILE* fdict = fopen(buffer, "r");
    if (fdict == NULL) {
        fclose(fin);
        fclose(fout);
        TERMINATE("Berkas kamus \"%s\" tidak ditemukan!\n", buffer);
    }

    // Membaca seluruh kata pada kamus dan memasukkannya ke set dictionary
    set<string> dictionary;
    while (fscanf(fdict, "%s", buffer) == 1) {
        toUppercaseWord(buffer);

        // Memastikan format sesuai
        ASSERT(consistsOfEnglishAlphabets(buffer), "Kata \"%s\" pada kamus tidak valid!\n", buffer);

        // Memastikan kata tidak muncul dua kali
        ASSERT(!EXIST(buffer, dictionary), "Kata \"%s\" muncul dua kali pada kamus\n", buffer);

        dictionary.insert(buffer);
    }
    fclose(fdict);

    // Membaca seluruh kata, koordinat dan arah pada masukan dan memasukkannya ke inputWords
    vector<CrosswordWord> inputWords;
    int row, col;
    while (fscanf(fin, "%s%d%d%s", buffer, &row, &col, bufferDirection) == 4) {
        toUppercaseWord(buffer);
        toUppercaseWord(bufferDirection);

        // Memastikan format sesuai
        ASSERT(consistsOfEnglishAlphabetsOrDots(buffer), "Kata \"%s\" pada masukan tidak valid!\n",
                buffer);
        ASSERT(!(strcmp(bufferDirection, "MENDATAR") && strcmp(bufferDirection, "MENURUN")),
                "Arah \"%s\" tidak valid! (Seharusnya \"mendatar\" atau \"menurun\")\n",
                bufferDirection);

        inputWords.push_back(CrosswordWord(buffer, CrosswordWord::getDirection(bufferDirection),
                row, col));
    }
    fclose(fin);

    // Langsung berhenti saat masukan kosong
    ASSERT(!inputWords.empty(), "Tidak ada kata pada masukan!\n");

    // Membaca seluruh kata, koordinat dan arah pada keluaran dan memasukkannya ke outputWords
    vector<CrosswordWord> outputWords;
    while (fscanf(fout, "%s%d%d%s", buffer, &row, &col, bufferDirection) == 4) {
        toUppercaseWord(buffer);
        toUppercaseWord(bufferDirection);

        // Memastikan format sesuai
        ASSERT(consistsOfEnglishAlphabets(buffer), "Kata \"%s\" pada keluaran tidak valid!\n",
                buffer);
        ASSERT(!(strcmp(bufferDirection, "MENDATAR") && strcmp(bufferDirection, "MENURUN")),
                "Arah \"%s\" tidak valid! (Seharusnya \"mendatar\" atau \"menurun\")\n",
                bufferDirection);

        outputWords.push_back(CrosswordWord(buffer, CrosswordWord::getDirection(bufferDirection),
                row, col));
    }
    fclose(fout);

    // Langsung berhenti saat keluaran kosong
    ASSERT(!outputWords.empty(), "Tidak ada kata pada keluaran!\n");

    // Memasukkan seluruh kata ke dalam grid yang direpresentasikan oleh map sambil mendaftar
    // semua kontradiksi yang ada pada grid
    map<Coordinate, char> crosswordGrid;
    map<Coordinate, CrosswordWord> directionMapping[2];
    vector<Coordinate> contradictions;
    Coordinate topLeft, bottomRight;
    bool firstCharacter = true;

    for (vector<CrosswordWord>::iterator crosswordWord = inputWords.begin();
            crosswordWord != inputWords.end(); ++crosswordWord) {
        Coordinate position = crosswordWord->start;

        // Periksa apakah sudah ada kata pada koordinat dan arah yang sama
        ASSERT(!EXIST(position, directionMapping[crosswordWord->direction]),
                "Ada dua kata pada koordinat (%d, %d) %s\n", position.row, position.col,
                crosswordWord->direction == ACCROSS ? "mendatar" : "menurun");
        directionMapping[crosswordWord->direction][position] = *crosswordWord;

        // Masukkan tiap karakter ke map
        for (unsigned int i = 0; i < crosswordWord->word.length(); ++i) {
            char ch = crosswordWord->word[i];
            if (EXIST(position, crosswordGrid)) {
                if (crosswordGrid[position] != ch) {
                    crosswordGrid[position] = '?';
                    contradictions.push_back(position);
                }
            } else {
                crosswordGrid[position] = ch;
            }

            // Memperbarui batas crossword: topLeft dan bottomRight
            if (firstCharacter) {
                firstCharacter = false;
                topLeft = bottomRight = position;
            } else {
                if (position.row < topLeft.row) {
                    topLeft.row = position.row;
                }
                if (position.col < topLeft.col) {
                    topLeft.col = position.col;
                }
                if (position.row > bottomRight.row) {
                    bottomRight.row = position.row;
                }
                if (position.col > bottomRight.col) {
                    bottomRight.col = position.col;
                }
            }

            // Memajukan koordinat position sesuai dengan arah kata
            position.advance(crosswordWord->direction);
        }
    }

    // Cetak grid sebelum diisi
    printf("Teka-teki silang yang pada masukan:\n\n");
    printCrossword(crosswordGrid);

    // Jika ada kontradiksi, keluarkan daftarnya
    if (!contradictions.empty()) {
        string message = "Terdapat kontradiksi pada koordinat";
        for (int i = 0; i < contradictions.size(); ++i) {
            if (i == 0) {
                sprintf(buffer, " (%d, %d)", contradictions[i].row, contradictions[i].col);
            } else if (i + 1 < contradictions.size()) {
                sprintf(buffer, ", (%d, %d)", contradictions[i].row, contradictions[i].col);
            } else if (contradictions.size() == 2) {
                sprintf(buffer, " dan (%d, %d)", contradictions[i].row, contradictions[i].col);
            } else {
                sprintf(buffer, ", dan (%d, %d)", contradictions[i].row, contradictions[i].col);
            }
            message += buffer;
        }
        TERMINATE("%s\n", message.c_str());
    }

    // Final check: cari kata baru yang terbentuk, atau cari kata yang tidak valid
    for (map<Coordinate, char>::iterator startIterator = crosswordGrid.begin();
            startIterator != crosswordGrid.end(); ++startIterator) {
        Coordinate start = startIterator->first;
        for (int direction = 0; direction < 2; ++direction) {
            Coordinate position = start;
            string word;
            while (EXIST(position, crosswordGrid)) {
                word.push_back(crosswordGrid[position]);
                position.advance(direction);
            }

            Coordinate previous = start - Coordinate::STEP[direction];
            // Periksa kalau pada koordinat dan arah ini ada kata pada map
            if (EXIST(start, directionMapping[direction])) {
                string originalWord = directionMapping[direction][start].word;

                // Kata tidak cocok
                ASSERT(word == originalWord,
                        "Pada koordinat (%d, %d) %s: Seharusnya \"%s\", tapi ditemukan \"%s\"\n",
                        start.row, start.col, direction == ACCROSS ? "mendatar" : "menurun",
                        originalWord.c_str(), word.c_str());
                // Kata diawali oleh huruf lain
                ASSERT(!EXIST(previous, crosswordGrid),
                        "Ditemukan huruf sebelum kata \"%s\", (%d, %d) %s\n",
                        originalWord.c_str(), start.row, start.col,
                        direction == ACCROSS ? "mendatar" : "menurun");
            } else {
                // Periksa kalau ada kata baru yang terbentuk
                ASSERT(EXIST(previous, crosswordGrid) || word.length() <= WORD_THRESHOLD,
                        "Ditemukan kata \"%s\" yang tidak ada pada masukan pada (%d, %d) %s\n",
                        word.c_str(), start.row, start.col,
                        direction == ACCROSS ? "mendatar" : "menurun");
            }
        }
    }

    // Sekarang, isi teka-teki silang tersebut dengan kata pada keluaran
    // Tiap kali ada kegagalan, simpan di issues.
    map<int, string> issues;
    set<string> usedWords;
    map<Coordinate, bool> finished[2];
    int totalSuccess = 0;
    for (vector<CrosswordWord>::iterator crosswordWord = outputWords.begin();
            crosswordWord != outputWords.end(); ++crosswordWord) {
        int i = crosswordWord - outputWords.begin();
        Coordinate position = crosswordWord->start;

        // Kata tidak ada pada kamus
        if (!EXIST(crosswordWord->word, dictionary)) {
            sprintf(buffer, "Kata \"%s\" tidak ada pada kamus", crosswordWord->word.c_str());
            issues[i] = buffer;
            continue;
        }
        // Kata yang sama telah dipakai
        if (EXIST(crosswordWord->word, usedWords)) {
            sprintf(buffer, "Kata \"%s\" telah digunakan sebelumnya", crosswordWord->word.c_str());
            issues[i] = buffer;
            // issue ini diperbolehkan untuk lanjut
        }
        // Kata tidak ada pada directionMapping
        if (!EXIST(position, directionMapping[crosswordWord->direction])) {
            sprintf(buffer, "Tidak ada yang harus diisi pada (%d, %d) %s", position.row,
                    position.col, crosswordWord->direction == ACCROSS ? "mendatar" : "menurun");
            issues[i] = buffer;
            continue;
        }
        // Kata sudah diselesaikan sebelumnya
        if (finished[crosswordWord->direction][position]) {
            sprintf(buffer, "Kata pada (%d, %d) %s sudah diisi sebelumnya", position.row,
                    position.col, crosswordWord->direction == ACCROSS ? "mendatar" : "menurun");
            issues[i] = buffer;
            continue;
        }

        // Mencoba untuk mengisi kata pada posisi dan arah ini
        CrosswordWord unknown = directionMapping[crosswordWord->direction][position];
        bool success = unknown.word.length() == crosswordWord->word.length();
        for (int j = 0; j < unknown.word.length() && success; ++j) {
            if (unknown.word[j] != '.' && unknown.word[j] != crosswordWord->word[j]) {
                success = false;
            }
        }
        if (!success) {
            sprintf(buffer, "Kata \"%s\" tidak bisa diiisi pada (%d, %d) %s",
                    crosswordWord->word.c_str(), position.row, position.col,
                    crosswordWord->direction == ACCROSS ? "mendatar" : "menurun" );
            issues[i] = buffer;
            continue;
        }

        // Berhasil!
        finished[crosswordWord->direction][position] = true;
        totalSuccess++;
        for (int j = 0; j < unknown.word.length() && success; ++j) {
            crosswordGrid[position] = crosswordWord->word[j];
            position.advance(crosswordWord->direction);
        }
    }

    // Keluarkan hasilnya
    printf("Teka-teki silang setelah diisi:\n\n");
    printCrossword(crosswordGrid);

    printf("Kata yang berhasil diisi : %d dari %d\n", totalSuccess, (int)inputWords.size());
    if (!issues.empty()) {
        printf("Ditemukan %d masalah pada keluaran :\n", (int)issues.size());
        for (map<int, string>::iterator it = issues.begin(); it != issues.end(); ++it) {
            printf("%3d. %s\n", it->first + 1, it->second.c_str());
        }
    }

    return 0;
}
