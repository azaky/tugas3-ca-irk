# Tugas III Ca-IRK: Format File

## Crossword Solver

Program harus bisa menerima masukan berupa teka-teki silang, dan keluaran berupa teka-teki silang yang telah terisi. Kata-kata yang dibolehkan untuk dipakai dapat diperoleh dari suatu kamus.

### Format Kamus

Kamus berisi kata-kata, di mana tiap kata-kata pada baris yang berbeda. Masukan diakhiri oleh EOF (End-Of-File). Dijamin:

- Semua kata dijamin hanya terdiri atas alfabet kecil (a..z).
- Tidak ada dua kata yang sama yang muncul pada masukan.

Kamus dapat dilihat di [sini](sini). Note: kamus dapat sewaktu-waktu diubah. Jadi, pastikan program dapat memuat ulang kamus. Secara *default*, nama berkas yang digunakan adalah `kamus.txt`.

### Format Masukan

Masukan akan menjelaskan kata-kata yang ada pada teka-teki silang. Tiap kata dijelaskan sebagai berikut:

	<kata> <baris> <kolom> <arah>

Yang artinya, huruf pertama `<kata>` akan diletakkan pada koordinat `<baris>, <kolom>` dan memiliki arah sesuai dengan `<arah>`, di mana `<arah>` bisa merupakan `mendatar` atau `menurun`. Masukan diakhiri dengan EOF (End-Of-File). Dijamin:

- Semua huruf pada `<kata>` bisa berupa alfabet kecil (a..z) atau `.` (titik). Titik menandakan bahwa huruf tersebut belum diketahui. Contohnya, `k....t..i` bisa diisi dengan kata `komputasi`.
- Tidak ada huruf yang saling kontradiksi antara kata yang satu dengan yang lain.

**Contoh Masukan**

	.l.. 1 1 menurun
	k....t..i 1 4 menurun
	l...r..o.... 2 1 mendatar
	.......a 7 1 mendatar

Masukan di atas dapat digambarkan oleh teka-teki silang di bawah ini.

	.  k
	l...r..o....
	.  .
	.  .
       .
       t
    .......a
       .
       i

### Format Keluaran

Format keluaran mirip dengan format masukan. Format keluaran berisi tiap kata yang dapat mengisi teka-teki silang yang diberikan. Tiap kata dijelaskan sebagai berikut:

	<kata> <baris> <kolom> <arah>

Pada koordinat `<baris>, <kolom>` dan arah sesuai dengan `<arah>` akan diisi oleh `<kata>`. Keluaran harus memenuhi:

- Urutan dapat acak, yang penting seluruh koordinat dan arah terdapat pada masukan.
- Tidak ada huruf yang saling kontradiksi antara kata yang satu dengan yang lain.
- Kata harus ada pada kamus.

Aspek yang dinilai:

- Banyaknya kata yang dapat dijawab. **Masukan asisten memungkinkan untuk mengisi keseluruhan teka-teki silang dengan kata-kata pada kamus**.

**Contoh Keluaran**

	ilmu 1 1 menurun
	komputasi 1 4 menurun
	laboratorium 2 1 mendatar
	rekayasa 7 1 mendatar

Dengan asumsi keempat kata tersebut ada pada kamus.