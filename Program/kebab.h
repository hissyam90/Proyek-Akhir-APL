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
    double gaji;        
    bool is_active;      
    string *jabatan_ptr;
};

struct Absensi {
    string username;
    string tanggal; 
    string waktu;   
    string status;  
};

inline void perbaikiPointer(Pegawai daftar_pegawai[], int n) {
    for (int i = 0; i < n; i++) {
        daftar_pegawai[i].jabatan_ptr = &daftar_pegawai[i].jabatan;
    }
}

// utility
void bersihkanLayar();
string dapatkanTanggalSekarang();
string dapatkanWaktuSekarang();
void prosesKeluar(int detik);
string formatRupiah(double nominal);
int bacaInt(const string &prompt);
double bacaDouble(const string &prompt);
void jeda();

// file handling
void simpanAkun(Akun daftar_akun[], int total_akun);
void muatAkun(Akun daftar_akun[], int &total_akun);
void simpanPegawai(Pegawai daftar_pegawai[], int total_pegawai);
void muatPegawai(Pegawai daftar_pegawai[], int &total_pegawai);
void simpanAbsensi(Absensi daftar_absensi[], int total_absensi);
void muatAbsensi(Absensi daftar_absensi[], int &total_absensi);

// admin & user actions
void catatLogAdmin(string pesan);
void lihatLogAdmin();
void kelolaCutiAdmin(Absensi daftar_absensi[], int total_absensi);
void ajukanCutiUser(Absensi daftar_absensi[], int &total_absensi, string username);
void cetakSlipGaji(Pegawai daftar_pegawai[], int total_pegawai, Absensi daftar_absensi[], int total_absensi, string username);
string loginAkun(Akun daftar_akun[], int total_akun, string &role_output);
string cekStatusKerja(string username, bool is_active, Absensi daftar_absensi[], int total_absensi);
void lihatPegawai(Pegawai daftar_pegawai[], int total_pegawai, bool filter_aktif, Absensi daftar_absensi[], int total_absensi);
void tambahPegawai(Pegawai daftar_pegawai[], int &total_pegawai, Akun daftar_akun[], int &total_akun);
void ubahPegawaiAdmin(Pegawai daftar_pegawai[], int total_pegawai, Akun daftar_akun[], int total_akun, Absensi daftar_absensi[], int total_absensi);
void hapusPegawaiSoftDelete(Pegawai daftar_pegawai[], int total_pegawai, Absensi daftar_absensi[], int total_absensi);
void rekapAbsensiAdmin(Absensi daftar_absensi[], int total_absensi, Pegawai daftar_pegawai[], int total_pegawai);
void laporanGaji(Pegawai daftar_pegawai[], int total_pegawai);
void inputAbsensi(Absensi daftar_absensi[], int &total_absensi, string username);
void lihatAbsensiUser(Absensi daftar_absensi[], int total_absensi, string username);
void batalkanAbsensi(Absensi daftar_absensi[], int &total_absensi, string username);
void mergeSortNama(Pegawai arr[], int left, int right);
void quickSortUmur(Pegawai arr[], int low, int high);
void insertionSortJabatan(Pegawai arr[], int n);
void sequentialSearchNama(Pegawai arr[], int total_pegawai, string target, Absensi daftar_absensi[], int total_absensi);
void binarySearchUmur(Pegawai arr[], int total_pegawai, int target, Absensi daftar_absensi[], int total_absensi);

#endif