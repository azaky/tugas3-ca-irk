#include <bits/stdc++.h>
using namespace std;

struct CrosswordWord {
	string word;
	enum Direction {
		accross, down
	} direction;
	int row, col;

	CrosswordWord(string word, Direction direction, int row, int col) {
		this->word = word;
		this->direction = direction;
		this->row = row;
		this->col = col;
	}

	Direction getDirection(string direction) {
		if (direction == "mendatar") {
			return accross;
		} else if (direction == "menurun") {
			return down;
		} else {
			// default value
			return accross;
		}
	}
};

char buffer[100], bufferDirection[100];

int int main(int argc, char const *argv[])
{
	if (argc != 3) {
		printf("Cara pemakaian: ./generator_checker <file_input> <file_output>\n");
		exit(1);
	}

	// Membuka berkas masukan dan keluaran
	FILE* fin = fopen(argv[1]);
	if (fin == NULL) {
		printf("Berkas masukan \"%s\" tidak ditemukan!\n", argv[1]);
		exit(1);
	}

	FILE* fout = fopen(argv[2]);
	if (fin == NULL) {
		printf("Berkas keluaran \"%s\" tidak ditemukan!\n", argv[1]);
		exit(1);
	}

	// Membaca seluruh kata pada berkas masukan dan memasukkannya ke vector inputWords
	vector<string> inputWords;
	while (scanf("%s", buffer) == 1) {
		inputWords.push_back(buffer);
	}

	// Membaca seluruh kata, koordinat dan arah pada keluaran dan memasukkannya ke vector outputWords
	vector<CrosswordWord> outputWords;
	while (scanf("%s%d%d%s", buffer, &row, &col, bufferDirection) == 1) {
		outputWords.push_back(CrosswordWord(buffer, getDirection(bufferDirection), row, col));
	}



	return 0;
}