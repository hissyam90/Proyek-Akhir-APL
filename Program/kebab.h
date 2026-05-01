#ifndef KEBAB_H
#define KEBAB_H
#include <string>

using namespace std;

struct Akun {
    string username;
    string password;
    string role; 
};

struct Alamat {
    string kota;
    string jalan;
};

struct Pegawai {
    string username; 
    string nama_pegawai;
    int umur;
    string jabatan;
    Alamat lokasi;
    bool is_active; // true = aktif, false = resign
    string *jabatan_ptr;
};

struct Absensi {
    string username;
    string tanggal; // format YYYY-MM-DD
    string waktu;   // format HH:MM:SS
    string status;  // hadir/sakit/izin
};

// fungsi untuk memperbaiki pointer jabatan_ptr setelah sorting atau load data
void perbaikiPointer(Pegawai daftar_pegawai[], int n) {
    for (int i = 0; i < n; i++) {
        daftar_pegawai[i].jabatan_ptr = &daftar_pegawai[i].jabatan;
    }
}

#endif