#include <iostream>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <stdexcept>
#include <limits>
#include <fstream>
#include <sstream>
#include <ctime>
#include "kebab.h"

using namespace std;

// konfigurasi csv
const string FILE_AKUN = "csv/akun.csv";
const string FILE_PEGAWAI = "csv/pegawai.csv";
const string FILE_ABSENSI = "csv/absensi.csv";

void bersihkanLayar() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

string dapatkanTanggalSekarang() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(buffer);
}

string dapatkanWaktuSekarang() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[9];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return string(buffer);
}

void prosesKeluar(int detik) {
    if (detik == 0) {
        cout << "Terima kasih telah menggunakan aplikasi ini!\n";
        return;
    }
    cout << "Aplikasi akan tertutup dalam " << detik << "...\n";
    prosesKeluar(detik - 1);
}


// csv area
void simpanAkun(Akun daftar_akun[], int total_akun) {
    ofstream file(FILE_AKUN);
    for (int i = 0; i < total_akun; i++) {
        file << daftar_akun[i].username << "," << daftar_akun[i].password << "," << daftar_akun[i].role << "\n";
    }
    file.close();
}

void muatAkun(Akun daftar_akun[], int &total_akun) {
    ifstream file(FILE_AKUN);
    string line, token;
    total_akun = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, daftar_akun[total_akun].username, ',');
        getline(ss, daftar_akun[total_akun].password, ',');
        getline(ss, daftar_akun[total_akun].role, ',');
        total_akun++;
    }
    file.close();

    if (total_akun == 0) {
        daftar_akun[0].username = "admin";
        daftar_akun[0].password = "admin123";
        daftar_akun[0].role = "admin";
        total_akun++;
        simpanAkun(daftar_akun, total_akun);
    }
}

void simpanPegawai(Pegawai daftar_pegawai[], int total_pegawai) {
    ofstream file(FILE_PEGAWAI);
    for (int i = 0; i < total_pegawai; i++) {
        file << daftar_pegawai[i].username << "," 
             << daftar_pegawai[i].nama_pegawai << "," 
             << daftar_pegawai[i].umur << "," 
             << daftar_pegawai[i].jabatan << "," 
             << daftar_pegawai[i].lokasi.kota << "," 
             << daftar_pegawai[i].lokasi.jalan << "," 
             << daftar_pegawai[i].is_active << "\n";
    }
    file.close();
}

void muatPegawai(Pegawai daftar_pegawai[], int &total_pegawai) {
    ifstream file(FILE_PEGAWAI);
    string line, token;
    total_pegawai = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, daftar_pegawai[total_pegawai].username, ',');
        getline(ss, daftar_pegawai[total_pegawai].nama_pegawai, ',');
        
        getline(ss, token, ',');
        daftar_pegawai[total_pegawai].umur = stoi(token);
        
        getline(ss, daftar_pegawai[total_pegawai].jabatan, ',');
        getline(ss, daftar_pegawai[total_pegawai].lokasi.kota, ',');
        getline(ss, daftar_pegawai[total_pegawai].lokasi.jalan, ',');
        
        getline(ss, token, ',');
        daftar_pegawai[total_pegawai].is_active = (token == "1");
        
        total_pegawai++;
    }
    file.close();
    perbaikiPointer(daftar_pegawai, total_pegawai);
}

void simpanAbsensi(Absensi daftar_absensi[], int total_absensi) {
    ofstream file(FILE_ABSENSI);
    for (int i = 0; i < total_absensi; i++) {
        file << daftar_absensi[i].username << "," 
             << daftar_absensi[i].tanggal << "," 
             << daftar_absensi[i].waktu << "," 
             << daftar_absensi[i].status << "\n";
    }
    file.close();
}

void muatAbsensi(Absensi daftar_absensi[], int &total_absensi) {
    ifstream file(FILE_ABSENSI);
    string line;
    total_absensi = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, daftar_absensi[total_absensi].username, ',');
        getline(ss, daftar_absensi[total_absensi].tanggal, ',');
        getline(ss, daftar_absensi[total_absensi].waktu, ',');
        getline(ss, daftar_absensi[total_absensi].status, ',');
        total_absensi++;
    }
    file.close();
}


// autentikasi
string loginAkun(Akun daftar_akun[], int total_akun, string &role_output) {
    bersihkanLayar();
    int sisa_kesempatan = 3;
    string input_nama, input_password;
    cout << "--- Login ---\n";
    while (sisa_kesempatan > 0) {
        cout << "Masukkan Username : "; cin >> input_nama;
        cout << "Masukkan Password : "; cin >> input_password;
        
        for (int i = 0; i < total_akun; i++) {
            if (input_nama == daftar_akun[i].username && input_password == daftar_akun[i].password) {
                role_output = daftar_akun[i].role;
                cout << "[+] Login Berhasil!\n";
                return input_nama;
            }
        }
        sisa_kesempatan--;
        if (sisa_kesempatan > 0) cout << "[!] Username atau Password salah! Sisa: " << sisa_kesempatan << "\n\n";
    }
    cout << "\n[X] Gagal login 3 kali. Kembali ke menu utama.\n";
    return "";
}

// crud admin
void tambahPegawai(Pegawai daftar_pegawai[], int &total_pegawai, Akun daftar_akun[], int &total_akun) {
    bersihkanLayar();
    cout << "--- Tambah Pegawai & Akun Baru ---\n";
    try {
        string new_username, new_password;
        cout << "Buat Username Pegawai : "; cin >> new_username;
        cout << "Buat Password Pegawai : "; cin >> new_password;
        
        
        daftar_akun[total_akun].username = new_username;
        daftar_akun[total_akun].password = new_password;
        daftar_akun[total_akun].role = "user";
        total_akun++;
        simpanAkun(daftar_akun, total_akun);

        
        int indeks = total_pegawai;
        daftar_pegawai[indeks].username = new_username;
        daftar_pegawai[indeks].is_active = true;
        
        cout << "Nama Pegawai : "; cin.ignore(); getline(cin, daftar_pegawai[indeks].nama_pegawai);
        cout << "Umur         : ";
        if (!(cin >> daftar_pegawai[indeks].umur) || daftar_pegawai[indeks].umur <= 0) {
            throw runtime_error("Input umur tidak valid!");
        }
        cout << "Jabatan      : "; cin.ignore(); getline(cin, daftar_pegawai[indeks].jabatan);
        cout << "Kota         : "; getline(cin, daftar_pegawai[indeks].lokasi.kota);
        cout << "Jalan        : "; getline(cin, daftar_pegawai[indeks].lokasi.jalan);
        
        total_pegawai++;
        perbaikiPointer(daftar_pegawai, total_pegawai);
        simpanPegawai(daftar_pegawai, total_pegawai);
        cout << "[+] Data pegawai dan akun berhasil dibuat!\n";
    } catch (const exception& e) {
        cout << "\n[!] Gagal: " << e.what() << "\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void lihatPegawai(Pegawai daftar_pegawai[], int total_pegawai, bool filter_aktif) {
    cout << setfill('-') << setw(95) << "-" << endl;
    cout << setfill(' ');
    cout << left << setw(15) << "Username" 
         << left << setw(20) << "Nama" 
         << left << setw(8) << "Umur" 
         << left << setw(20) << "Jabatan" 
         << left << setw(20) << "Alamat" 
         << "Status\n";
    cout << setfill('-') << setw(95) << "-" << endl;
    cout << setfill(' ');
    
    bool ada_data = false;
    for (int i = 0; i < total_pegawai; i++) {
        if (daftar_pegawai[i].is_active == filter_aktif) {
            ada_data = true;
            cout << left << setw(15) << daftar_pegawai[i].username
                 << left << setw(20) << daftar_pegawai[i].nama_pegawai
                 << left << setw(8) << daftar_pegawai[i].umur
                 << left << setw(20) << *(daftar_pegawai[i].jabatan_ptr)
                 << daftar_pegawai[i].lokasi.kota << ", " << daftar_pegawai[i].lokasi.jalan << "\t"
                 << (filter_aktif ? "Aktif" : "Resign") << "\n";
        }
    }
    if (!ada_data) cout << "Belum ada data di kategori ini.\n";
    cout << setfill('-') << setw(95) << "-" << endl;
}

void ubahPegawaiAdmin(Pegawai daftar_pegawai[], int total_pegawai, Akun daftar_akun[], int total_akun) {
    bersihkanLayar();
    lihatPegawai(daftar_pegawai, total_pegawai, true);
    string target_username;
    cout << "\nMasukkan Username Pegawai yang akan diubah: "; cin >> target_username;
    
    bool found = false;
    for (int i = 0; i < total_pegawai; i++) {
        if (daftar_pegawai[i].username == target_username && daftar_pegawai[i].is_active) {
            found = true;
            int pilihan;
            cout << "1. Ubah Detail Pegawai\n2. Reset Password\nPilih: "; cin >> pilihan;
            
            if (pilihan == 1) {
                cout << "Jabatan Baru : "; cin.ignore(); getline(cin, daftar_pegawai[i].jabatan);
                cout << "Kota Baru    : "; getline(cin, daftar_pegawai[i].lokasi.kota);
                cout << "Jalan Baru   : "; getline(cin, daftar_pegawai[i].lokasi.jalan);
                simpanPegawai(daftar_pegawai, total_pegawai);
                cout << "[+] Profil diperbarui.\n";
            } else if (pilihan == 2) {
                for (int j = 0; j < total_akun; j++) {
                    if (daftar_akun[j].username == target_username) {
                        cout << "Password Baru: "; cin >> daftar_akun[j].password;
                        simpanAkun(daftar_akun, total_akun);
                        cout << "[+] Password direset.\n";
                        break;
                    }
                }
            }
            break;
        }
    }
    if (!found) cout << "[!] Pegawai aktif dengan username tersebut tidak ditemukan.\n";
}

void hapusPegawaiSoftDelete(Pegawai daftar_pegawai[], int total_pegawai) {
    bersihkanLayar();
    lihatPegawai(daftar_pegawai, total_pegawai, true);
    string target_username;
    cout << "\nMasukkan Username Pegawai yang akan di-Resign: "; cin >> target_username;
    
    for (int i = 0; i < total_pegawai; i++) {
        if (daftar_pegawai[i].username == target_username && daftar_pegawai[i].is_active) {
            daftar_pegawai[i].is_active = false;
            simpanPegawai(daftar_pegawai, total_pegawai);
            cout << "[+] Pegawai berhasil dipindahkan ke daftar Resign.\n";
            return;
        }
    }
    cout << "[!] Pegawai tidak ditemukan.\n";
}

// user area
void inputAbsensi(Absensi daftar_absensi[], int &total_absensi, string username) {
    bersihkanLayar();
    cout << "--- Presensi Harian ---\n";
    cout << "Pilih Status Kehadiran:\n1. Hadir\n2. Sakit\n3. Izin\nPilih: ";
    int pilihan; cin >> pilihan;
    
    string status = (pilihan == 1) ? "Hadir" : (pilihan == 2) ? "Sakit" : "Izin";
    
    daftar_absensi[total_absensi].username = username;
    daftar_absensi[total_absensi].tanggal = dapatkanTanggalSekarang();
    daftar_absensi[total_absensi].waktu = dapatkanWaktuSekarang();
    daftar_absensi[total_absensi].status = status;
    total_absensi++;
    
    simpanAbsensi(daftar_absensi, total_absensi);
    cout << "[+] Absensi tercatat pada " << daftar_absensi[total_absensi-1].tanggal << " " << daftar_absensi[total_absensi-1].waktu << "\n";
}

void lihatAbsensiUser(Absensi daftar_absensi[], int total_absensi, string username) {
    bersihkanLayar();
    cout << "--- Riwayat Kehadiran Anda ---\n";
    for (int i = 0; i < total_absensi; i++) {
        if (daftar_absensi[i].username == username) {
            cout << daftar_absensi[i].tanggal << " | " << daftar_absensi[i].waktu << " | " << daftar_absensi[i].status << "\n";
        }
    }
}

void batalkanAbsensi(Absensi daftar_absensi[], int &total_absensi, string username) {
    string hari_ini = dapatkanTanggalSekarang();
    for (int i = total_absensi - 1; i >= 0; i--) { 
        if (daftar_absensi[i].username == username && daftar_absensi[i].tanggal == hari_ini) {
            
            for (int j = i; j < total_absensi - 1; j++) {
                daftar_absensi[j] = daftar_absensi[j + 1];
            }
            total_absensi--;
            simpanAbsensi(daftar_absensi, total_absensi);
            cout << "[+] Input absensi hari ini berhasil dibatalkan.\n";
            return;
        }
    }
    cout << "[!] Anda belum melakukan absensi hari ini.\n";
}

// main area
int main() {
    Akun daftar_akun[100]; int total_akun = 0;
    Pegawai daftar_pegawai[100]; int total_pegawai = 0;
    Absensi daftar_absensi[300]; int total_absensi = 0;

    muatAkun(daftar_akun, total_akun);
    muatPegawai(daftar_pegawai, total_pegawai);
    muatAbsensi(daftar_absensi, total_absensi);

    int menu_awal;
    bool aplikasi_berjalan = true;

    while (aplikasi_berjalan) {
        bersihkanLayar();
        cout << "--- Sistem Manajemen Pegawai Kebab Cendana ---\n";
        cout << "1. Login\n";
        cout << "2. Keluar Aplikasi\n";
        cout << "Pilihan: ";
        
        try {
            if (!(cin >> menu_awal)) throw runtime_error("Input harus angka!");
            
            if (menu_awal == 1) {
                string current_role = "";
                string current_user = loginAkun(daftar_akun, total_akun, current_role);
                
                if (current_user != "") {
                    bool sesi_login = true;
                    int menu_crud;
                    
                    while (sesi_login) {
                        cout << "\n--- Dashboard (" << current_role << ") ---\n";
                        if (current_role == "admin") {
                            cout << "1. Tambah Pegawai Baru\n2. Lihat Pegawai Aktif\n3. Lihat Pegawai Resign\n";
                            cout << "4. Ubah Detail/Reset Akun Pegawai\n5. Proses Resign Pegawai\n6. Rekap Absensi Keseluruhan\n7. Logout\n";
                            cout << "Pilihan: "; cin >> menu_crud;
                            
                            switch (menu_crud) {
                                case 1: tambahPegawai(daftar_pegawai, total_pegawai, daftar_akun, total_akun); break;
                                case 2: bersihkanLayar(); cout << "--- Daftar Pegawai Aktif ---\n"; lihatPegawai(daftar_pegawai, total_pegawai, true); break;
                                case 3: bersihkanLayar(); cout << "--- Daftar Pegawai Resign ---\n"; lihatPegawai(daftar_pegawai, total_pegawai, false); break;
                                case 4: ubahPegawaiAdmin(daftar_pegawai, total_pegawai, daftar_akun, total_akun); break;
                                case 5: hapusPegawaiSoftDelete(daftar_pegawai, total_pegawai); break;
                                case 6: 
                                    bersihkanLayar(); cout << "--- Rekap Absensi Keseluruhan ---\n";
                                    for(int i=0; i<total_absensi; i++) cout << daftar_absensi[i].username << " | " << daftar_absensi[i].tanggal << " | " << daftar_absensi[i].status << "\n";
                                    break;
                                case 7: sesi_login = false; break;
                            }
                        } else if (current_role == "user") {
                            cout << "1. Presensi Hari Ini\n2. Lihat Profil Saya\n3. Cek Riwayat Presensi\n";
                            cout << "4. Ubah Password\n5. Batalkan Presensi Hari Ini\n6. Logout\n";
                            cout << "Pilihan: "; cin >> menu_crud;
                            
                            switch (menu_crud) {
                                case 1: inputAbsensi(daftar_absensi, total_absensi, current_user); break;
                                case 2: 
                                    bersihkanLayar(); cout << "--- Profil --- \n";
                                    for(int i=0; i<total_pegawai; i++) {
                                        if(daftar_pegawai[i].username == current_user) {
                                            cout << "Nama: " << daftar_pegawai[i].nama_pegawai << "\nJabatan: " << *(daftar_pegawai[i].jabatan_ptr) << "\nUmur: " << daftar_pegawai[i].umur << "\n";
                                        }
                                    }
                                    break;
                                case 3: lihatAbsensiUser(daftar_absensi, total_absensi, current_user); break;
                                case 4: 
                                    cout << "Masukkan Password Baru: "; 
                                    for(int i=0; i<total_akun; i++) {
                                        if(daftar_akun[i].username == current_user) { cin >> daftar_akun[i].password; simpanAkun(daftar_akun, total_akun); cout << "[+] Berhasil!\n"; }
                                    } break;
                                case 5: batalkanAbsensi(daftar_absensi, total_absensi, current_user); break;
                                case 6: sesi_login = false; break;
                            }
                        }
                    }
                }
            } else if (menu_awal == 2) {
                prosesKeluar(3);
                aplikasi_berjalan = false;
            } else {
                cout << "[!] Pilihan tidak valid!\n";
            }
        } catch (const exception& e) {
            cout << "[!] Error: " << e.what() << "\n";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return 0;
}