# Tugas III Ca-IRK: Format File

## Crossword Generator

Program harus bisa menerima masukan berupa daftar kata-kata yang ada, dan mengeluarkan penempatan kata-kata tersebut dalam suatu teka-teki silang.

### Format Masukan

Masukan berisi kata-kata, di mana tiap kata-kata pada baris yang berbeda. Masukan diakhiri oleh EOF (End-Of-File). Dijamin:

- Semua kata dijamin hanya terdiri atas alfabet kecil (a..z).
- Tidak ada dua kata yang sama yang muncul pada masukan.

**Contoh Masukan**

	laboratorium
	ilmu
	rekayasa
	komputasi

### Format Keluaran

Keluaran akan menjelaskan penempatan kata-kata pada masukan dalam teka-teki silang. Tiap kata dijelaskan sebagai berikut:

	<kata> <baris> <kolom> <arah>

Yang artinya, huruf pertama `<kata>` akan diletakkan pada koordinat `<baris>, <kolom>` dan memiliki arah sesuai dengan `<arah>`, di mana `<arah>` bisa merupakan `mendatar` atau `menurun`.

Keluaran harus memenuhi:

1. Urutan kata dapat acak, yang penting tidak ada kata yang muncul dua kali.
2. Tidak ada huruf yang saling kontradiksi (lihat contoh untuk lebih jelasnya).
3. Teka-teki silang yang dihasilkan tidak menghasilkan kata lain (lihat contoh juga).

Aspek yang dinilai:

1. Banyak kata yang berhasil dimasukkan ke teka-teki silang. **Masukan yang diberikan asisten pasti memungkinkan untuk menggunakan semua kata yang ada**.
2. Keterhubungan antara kata yang satu dengan yang lainnya. Keluaran yang trivial adalah meletakkan tiap kata secara terpisah, misalnya seluruh kata pada baris ganjil yang berbeda dan mendatar. Meskipun demikan, **masukan yang diberikan asisten memungkinkan teka-teki silang yang dihasilkan terhubung satu sama lain**.
3. Ukuran dari teka-teki silang. Semakin kecil, semakin baik.

**Contoh Keluaran 1**

	ilmu 1 1 menurun
	komputasi 1 4 menurun
	laboratorium 2 1 mendatar
	rekayasa 7 1 mendatar

Keluaran di atas dapat digambarkan oleh teka-teki silang di bawah ini. Ukuran teka-teki silang ini adalah `9 baris * 12 kolom = 108 kotak`.

	i  k
	laboratorium
	m  m
	u  p
       u
       t
    rekayasa
       s
       i

**Contoh Keluaran 2**

	laboratorium 1 1 mendatar
	ilmu 2 1 mendatar
	rekayasa 3 1 mendatar
	komputasi 4 1 mendatar

Jika disusun, teka-teki silang yang dibentuk adalah sebagai berikut.

	laboratorium
	ilmu
	rekayasa
	komputasi

Terdapat kata-kata lain yang muncul di sana, contohnya adalah `lirk` yang muncul pada `1 1 menurun`, `at` yang muncul pada `3 6 menurun`, dan sebagainya. **Ini adalah contoh keluaran yang salah**.

**Contoh Keluaran 3**

	laboratorium 1 1 mendatar
	ilmu 1 1 menurun

Hanya ada dua kata di sini, **dan ini diperbolehkan**. Namun, terdapat ketidakkonsistenan pada huruf di baris 1 kolom 1, yaitu antara `l` dan `i`. Jadi, **ini juga contoh keluaran yang salah**.

