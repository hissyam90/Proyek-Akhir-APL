#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "kebab.h"

using namespace std;

int main() {
    Akun    daftar_akun[100];    int total_akun    = 0;
    Pegawai daftar_pegawai[100]; int total_pegawai = 0;
    Absensi daftar_absensi[300]; int total_absensi = 0;

    try {
        muatAkun(daftar_akun, total_akun);
        muatPegawai(daftar_pegawai, total_pegawai);
        muatAbsensi(daftar_absensi, total_absensi);
    } catch (const exception &e) {
        cout << "\n[CRITICAL ERROR] " << e.what() << "\n"; return 1;
    }

    bool aplikasi_berjalan = true;
    while (aplikasi_berjalan) {
        bersihkanLayar();
        cout << "\n+----------------------------------------+\n"
             << "| SISTEM MANAJEMEN PEGAWAI KEBAB CENDANA |\n"
             << "+----------------------------------------+\n"
             << " 1. Login Sistem\n 2. Keluar Aplikasi\n ----------------------------------------\n";
        
        try {
            int menu_awal = bacaInt(" Pilih menu: ");
            if (menu_awal == 1) {
                string current_role = "";
                string current_user = loginAkun(daftar_akun, total_akun, current_role);
                if (current_user.empty()) continue;

                bool sesi_login = true;
                while (sesi_login) {
                    bersihkanLayar();
                    cout << "\n+----------------------------------------+\n"
                         << "| DASHBOARD " << left << setw(29) << ("[" + current_role + "] " + current_user) << "|\n"
                         << "+----------------------------------------+\n";

                    try {
                        if (current_role == "admin") {
                            cout << " 1. Registrasi Pegawai & Akun\n 2. Lihat Daftar Pegawai\n 3. Edit Data / Gaji / Sandi\n 4. Proses Resign\n"
                                 << " 5. Rekap Absensi\n 6. Laporan Gaji\n 7. Urutkan & Cari Pegawai\n"
                                 << " 8. Kelola Pengajuan Cuti\n 9. Lihat Log Aktivitas Admin\n 10. Logout\n ----------------------------------------\n";
                            int menu_crud = bacaInt(" Navigasi Menu: ");

                            switch (menu_crud) {
                                case 1: tambahPegawai(daftar_pegawai, total_pegawai, daftar_akun, total_akun); break;
                                case 2: {
                                    bersihkanLayar();
                                    int pil_lihat = bacaInt(" 1. Lihat Pegawai Aktif\n 2. Lihat Pegawai Resign\n Pilih: ");
                                    if (pil_lihat == 1) {
                                        cout << "\n--- Tabel Data Pegawai Aktif ---\n";
                                        lihatPegawai(daftar_pegawai, total_pegawai, true, daftar_absensi, total_absensi);
                                    } else if (pil_lihat == 2) {
                                        cout << "\n--- Tabel Data Pegawai Resign ---\n";
                                        lihatPegawai(daftar_pegawai, total_pegawai, false, daftar_absensi, total_absensi);
                                    } else {
                                        cout << "[!] Pilihan tidak ada.\n";
                                    }
                                    jeda(); 
                                    break;
                                }
                                case 3: ubahPegawaiAdmin(daftar_pegawai, total_pegawai, daftar_akun, total_akun, daftar_absensi, total_absensi); break;
                                case 4: hapusPegawaiSoftDelete(daftar_pegawai, total_pegawai, daftar_absensi, total_absensi); break;
                                case 5: rekapAbsensiAdmin(daftar_absensi, total_absensi, daftar_pegawai, total_pegawai); break;  
                                case 6: laporanGaji(daftar_pegawai, total_pegawai); break;  
                                case 7: {
                                    bool menu_sort_search = true;
                                    while(menu_sort_search) {
                                        bersihkanLayar();
                                        cout << "\n--- Modul Pengurutan & Pencarian ---\n"
                                             << " 1. Urutkan Berdasarkan Nama (Z-A)\n 2. Urutkan Berdasarkan Usia (Termuda)\n 3. Urutkan Berdasarkan Jabatan (A-Z)\n"
                                             << " 4. Cari Pegawai (Nama)\n 5. Cari Pegawai (Usia)\n 6. Kembali\n ----------------------------------------\n";
                                        int pil = bacaInt(" Pilih opsi: ");
                                        if (pil == 1) { mergeSortNama(daftar_pegawai, 0, total_pegawai - 1); perbaikiPointer(daftar_pegawai, total_pegawai); bersihkanLayar(); cout << "\n--- Data Diurutkan Berdasarkan Nama (Menurun) ---\n"; lihatPegawai(daftar_pegawai, total_pegawai, true, daftar_absensi, total_absensi); jeda(); } 
                                        else if (pil == 2) { quickSortUmur(daftar_pegawai, 0, total_pegawai - 1); perbaikiPointer(daftar_pegawai, total_pegawai); bersihkanLayar(); cout << "\n--- Data Diurutkan Berdasarkan Usia (Menaik) ---\n"; lihatPegawai(daftar_pegawai, total_pegawai, true, daftar_absensi, total_absensi); jeda(); } 
                                        else if (pil == 3) { insertionSortJabatan(daftar_pegawai, total_pegawai); perbaikiPointer(daftar_pegawai, total_pegawai); bersihkanLayar(); cout << "\n--- Data Diurutkan Berdasarkan Jabatan (Menaik) ---\n"; lihatPegawai(daftar_pegawai, total_pegawai, true, daftar_absensi, total_absensi); jeda(); } 
                                        else if (pil == 4) { string cari; cout << " Masukkan nama target: "; cin.ignore(); getline(cin, cari); sequentialSearchNama(daftar_pegawai, total_pegawai, cari, daftar_absensi, total_absensi); } 
                                        else if (pil == 5) { int umur = bacaInt(" Masukkan angka usia target: "); binarySearchUmur(daftar_pegawai, total_pegawai, umur, daftar_absensi, total_absensi); } 
                                        else if (pil == 6) { menu_sort_search = false; }
                                    }
                                    break;
                                }
                                case 8: kelolaCutiAdmin(daftar_absensi, total_absensi); break;
                                case 9: lihatLogAdmin(); break;
                                case 10: sesi_login = false; break;
                                default: cout << "\n[!] Navigasi tidak dikenali.\n"; jeda();
                            }
                        } else if (current_role == "user") {
                            cout << " 1. Isi Presensi Kehadiran\n 2. Lihat Informasi Profil\n 3. Ajukan Cuti\n 4. Cetak Slip Gaji\n 5. Batalkan Presensi Hari Ini\n 6. Logout\n ----------------------------------------\n";
                            int menu_crud = bacaInt(" Navigasi Menu: ");

                            switch (menu_crud) {
                                case 1: inputAbsensi(daftar_absensi, total_absensi, current_user); break;
                                case 2:
                                    bersihkanLayar();
                                    cout << "\n--- Informasi Profil Karyawan ---\n";
                                    for (int i = 0; i < total_pegawai; i++) {
                                        if (daftar_pegawai[i].username == current_user) {
                                            cout << " Nama Lengkap    : " << daftar_pegawai[i].nama_pegawai << "\n"
                                                 << " Posisi/Jabatan  : " << *(daftar_pegawai[i].jabatan_ptr) << "\n"
                                                 << " Usia Saat Ini   : " << daftar_pegawai[i].umur << " Tahun\n"
                                                 << " Kota Domisili   : " << daftar_pegawai[i].lokasi.kota << "\n"
                                                 << " Gaji Pokok      : " << formatRupiah(daftar_pegawai[i].gaji) << "\n";
                                        }
                                    }
                                    jeda();
                                    break;
                                case 3: ajukanCutiUser(daftar_absensi, total_absensi, current_user); break;
                                case 4: cetakSlipGaji(daftar_pegawai, total_pegawai, daftar_absensi, total_absensi, current_user); break;
                                case 5: batalkanAbsensi(daftar_absensi, total_absensi, current_user); break;
                                case 6: sesi_login = false; break;
                                default: cout << "\n[!] Navigasi tidak dikenali.\n"; jeda();
                            }
                        }
                    } catch (const exception &e) { cout << "\n[!] Terjadi kesalahan internal: " << e.what() << "\n"; cin.clear(); cin.ignore(10000, '\n'); jeda(); }
                }
            } else if (menu_awal == 2) { cout << "\n"; prosesKeluar(3); aplikasi_berjalan = false; }
        } catch (...) { cin.clear(); cin.ignore(10000, '\n'); }
    }
    return 0;
}