#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

#define TERMINATE(...)	fprintf(stderr, "ERROR: "); fprintf(stderr, __VA_ARGS__); exit(1)
#define EXIST(a,s) 		((s).find(a) != (s).end())

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
 * Fungsi yang mengubah karakter ASCII menjadi uppercase
 */
char toUppercase(char c) {
	if ('a' <= c && c <= 'z') {
		return c + 'A' - 'a';
	} else {
		return c;
	}
}

void toUppercaseWord(char* c) {
	for (; *c != 0; ++c) {
		*c = toUppercase(*c);
	}
}

char buffer[100], bufferDirection[100];

int main(int argc, char const *argv[])
{
	if (argc != 3) {
		TERMINATE("%s\n%s\n", "Cara kompilasi: g++ generator_checker.cpp -o generator_checker",
				"Cara pemakaian: ./generator_checker <file_input> <file_output>");
	}

	// Membuka berkas masukan dan keluaran
	FILE* fin = fopen(argv[1], "r");
	if (fin == NULL) {
		TERMINATE("Berkas masukan \"%s\" tidak ditemukan!\n", argv[1]);
	}

	FILE* fout = fopen(argv[2], "r");
	if (fin == NULL) {
		TERMINATE("Berkas keluaran \"%s\" tidak ditemukan!\n", argv[1]);
	}

	// Membaca seluruh kata pada berkas masukan dan memasukkannya ke inputWords
	set<string> inputWords;
	while (fscanf(fin, "%s", buffer) == 1) {
		toUppercaseWord(buffer);
		if (EXIST(buffer, inputWords)) {
			fclose(fin);
			TERMINATE("Kata %s muncul dua kali pada masukan!\n", buffer);
		}
		inputWords.insert(buffer);
	}
	fclose(fin);

	// Membaca seluruh kata, koordinat dan arah pada keluaran dan memasukkannya ke outputWords
	vector<CrosswordWord> outputWords;
	set<string> exsistedOutputWords;
	int row, col;
	while (fscanf(fout, "%s%d%d%s", buffer, &row, &col, bufferDirection) == 4) {
		toUppercaseWord(buffer);
		toUppercaseWord(bufferDirection);
		// Langsung keluar jika menemukan kata yang tidak ada pada masukan, atau muncul dua kali
		// pada keluaran
		if (!EXIST(buffer, inputWords)) {
			fclose(fout);
			TERMINATE("Kata %s tidak ditemukan pada masukan!\n", buffer);
		}
		if (EXIST(buffer, exsistedOutputWords)) {
			fclose(fout);
			TERMINATE("Kata %s muncul dua kali pada keluaran!\n", buffer);
		}
		outputWords.push_back(CrosswordWord(buffer, CrosswordWord::getDirection(bufferDirection),
				row, col));
		exsistedOutputWords.insert(buffer);
	}
	fclose(fout);

	// Langsung berhenti saat keluaran kosong
	if (outputWords.empty()) {
		TERMINATE("Tidak ada kata pada keluaran!\n");
	}

	// Memasukkan seluruh kata ke dalam grid yang direpresentasikan oleh map
	map<Coordinate, char> crosswordGrid;
	map<Coordinate, CrosswordWord> directionMapping[2];
	Coordinate topLeft, bottomRight;
	bool firstCharacter = true;
	for (vector<CrosswordWord>::iterator crosswordWord = outputWords.begin();
			crosswordWord != outputWords.end(); ++crosswordWord) {
		Coordinate position = crosswordWord->start;

		// Periksa apakah sudah ada kata pada koordinat dan arah yang sama
		if (EXIST(position, directionMapping[crosswordWord->direction])) {
			TERMINATE("Ada dua kata pada koordinat (%d, %d) %s\n", position.row, position.col,
					crosswordWord->direction == ACCROSS ? "mendatar" : "menurun");
		}
		directionMapping[crosswordWord->direction][position] = *crosswordWord;

		// Masukkan tiap karakter ke map
		for (unsigned int i = 0; i < crosswordWord->word.length(); ++i) {
			char ch = crosswordWord->word[i];
			if (EXIST(position, crosswordGrid)) {
				if (crosswordGrid[position] != ch) {
					TERMINATE("Terdapat kontradiksi pada koordinat (%d, %d)\n", position.row,
							position.col);
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

			position.advance(crosswordWord->direction);
		}
	}

	// Cetak grid
	printf("Teka-teki silang yang terbentuk:\n\n");
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

		// Kosong
		for (col = topLeft.col; col <= bottomRight.col; ++col) {
			if (EXIST(Coordinate(row, col), crosswordGrid) ||
					EXIST(Coordinate(row, col - 1), crosswordGrid)) {
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

		// Huruf
		for (col = topLeft.col; col <= bottomRight.col; ++col) {
			if (EXIST(Coordinate(row, col), crosswordGrid)) {
				printf("| %c ", toUppercase(crosswordGrid[Coordinate(row, col)]));
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

		// Kosong
		for (col = topLeft.col; col <= bottomRight.col; ++col) {
			if (EXIST(Coordinate(row, col), crosswordGrid) ||
					EXIST(Coordinate(row, col - 1), crosswordGrid)) {
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

		// Alas
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

			// Periksa kalau pada koordinat dan arah ini ada kata pada map
			Coordinate previous = start - Coordinate::STEP[direction];
			if (EXIST(start, directionMapping[direction])) {
				string originalWord = directionMapping[direction][start].word;
				if (word != originalWord) {
					TERMINATE("Pada koordinat (%d, %d) %s: Seharusnya \"%s\", tapi ditemukan \"%s\"\n",
							start.row, start.col, direction == ACCROSS ? "mendatar" : "menurun",
							originalWord.c_str(), word.c_str());
				}
				if (EXIST(previous, crosswordGrid)) {
					TERMINATE("Ditemukan huruf sebelum kata \"%s\", (%d, %d) %s\n",
							originalWord.c_str(), start.row, start.col,
							direction == ACCROSS ? "mendatar" : "menurun");
				}
			} else {
				if (!EXIST(previous, crosswordGrid) && word.length() > 1) {
					TERMINATE("Ditemukan kata \"%s\" yang tidak ada pada masukan pada (%d, %d) %s\n",
							word.c_str(), start.row, start.col,
							direction == ACCROSS ? "mendatar" : "menurun");
				}
			}
		}
	}

	// Setelah semua valid, keluarkan metrik penilaian
	printf("Teka-teki silang di atas valid!\n\n");

	// Keluarkan banyak kata yang berhasil dipakai
	printf("1. Kata yang terpakai                : %d dari %d\n", (int)outputWords.size(), (int)inputWords.size());

	// Keluarkan "keterhubungan" dalam bentuk banyak komponen graf yang ada
	int numComponent = 0;
	map<Coordinate, int> visited;
	for (map<Coordinate, char>::iterator startIterator = crosswordGrid.begin();
			startIterator != crosswordGrid.end(); ++startIterator) {
		if (visited[startIterator->first]) {
			continue;
		}
		vector<Coordinate> stack;
		stack.push_back(startIterator->first);
		visited[stack.back()] = 1;
		while (!stack.empty()) {
			Coordinate top = stack.back(), temp;
			stack.pop_back();

			temp = top - Coordinate::STEP[0];
			if (!visited[temp] && EXIST(temp, crosswordGrid)) {
				stack.push_back(temp);
				visited[temp] = 1;
			}
			temp = top + Coordinate::STEP[0];
			if (!visited[temp] && EXIST(temp, crosswordGrid)) {
				stack.push_back(temp);
				visited[temp] = 1;
			}
			temp = top - Coordinate::STEP[1];
			if (!visited[temp] && EXIST(temp, crosswordGrid)) {
				stack.push_back(temp);
				visited[temp] = 1;
			}
			temp = top + Coordinate::STEP[1];
			if (!visited[temp] && EXIST(temp, crosswordGrid)) {
				stack.push_back(temp);
				visited[temp] = 1;
			}
		}
		numComponent++;
	}
	printf("2. Komponen terhubung yang terbentuk : %d\n", numComponent);

	// Keluarkan ukuran teka-teki silang
	printf("3. Ukuran teka-teki silang           : %d baris x %d kolom = %d kotak\n",
		bottomRight.row - topLeft.row + 1, bottomRight.col - topLeft.col + 1,
		(bottomRight.row - topLeft.row + 1) * (bottomRight.col - topLeft.col + 1));
	printf("   Banyak kotak yang terpakai        : %d kotak\n", (int)crosswordGrid.size());

	return 0;
}