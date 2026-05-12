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

const string FILE_AKUN     = "csv/akun.csv";
const string FILE_PEGAWAI  = "csv/pegawai.csv";
const string FILE_ABSENSI  = "csv/absensi.csv";
const string FILE_LOG      = "csv/log_admin.txt"; 

void bersihkanLayar() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string dapatkanTanggalSekarang() {
    time_t now = time(0); tm *ltm = localtime(&now); char buffer[11];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(buffer);
}

string dapatkanWaktuSekarang() {
    time_t now = time(0); tm *ltm = localtime(&now); char buffer[9];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return string(buffer);
}

void prosesKeluar(int detik) {
    if (detik == 0) { cout << "Terima kasih telah menggunakan aplikasi ini!\n"; return; }
    cout << "Aplikasi akan tertutup dalam " << detik << " detik...\n";
    prosesKeluar(detik - 1);
}

string formatRupiah(double nominal) {
    long long n = (long long)nominal; string s = to_string(n); int sisipan = (int)s.length() - 3;
    while (sisipan > 0) { s.insert(sisipan, "."); sisipan -= 3; }
    return "Rp " + s;
}

int bacaInt(const string &prompt) {
    int nilai; cout << prompt;
    if (!(cin >> nilai)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); throw runtime_error("Input harus angka bulat!"); }
    return nilai;
}

double bacaDouble(const string &prompt) {
    double nilai; cout << prompt;
    if (!(cin >> nilai)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); throw runtime_error("Input harus angka!"); }
    return nilai;
}

void jeda() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
}

void simpanAkun(Akun daftar_akun[], int total_akun) {
    ofstream file(FILE_AKUN);
    for (int i = 0; i < total_akun; i++) file << daftar_akun[i].username << "," << daftar_akun[i].password << "," << daftar_akun[i].role << "\n";
    file.close();
}

void muatAkun(Akun daftar_akun[], int &total_akun) {
    ifstream file(FILE_AKUN); string line; total_akun = 0;
    while (getline(file, line) && total_akun < 100) {
        if (line.empty()) continue; stringstream ss(line);
        getline(ss, daftar_akun[total_akun].username, ','); getline(ss, daftar_akun[total_akun].password, ','); getline(ss, daftar_akun[total_akun].role, ',');
        total_akun++;
    }
    file.close();
    if (total_akun == 0) { daftar_akun[0] = {"admin", "admin123", "admin"}; total_akun = 1; simpanAkun(daftar_akun, total_akun); }
}

void simpanPegawai(Pegawai daftar_pegawai[], int total_pegawai) {
    ofstream file(FILE_PEGAWAI);
    for (int i = 0; i < total_pegawai; i++) {
        file << daftar_pegawai[i].username << "," << daftar_pegawai[i].nama_pegawai << "," << daftar_pegawai[i].umur << ","
             << daftar_pegawai[i].jabatan << "," << daftar_pegawai[i].lokasi.kota << "," << daftar_pegawai[i].lokasi.jalan << ","
             << fixed << setprecision(2) << daftar_pegawai[i].gaji << "," << daftar_pegawai[i].is_active << "\n";
    }
    file.close();
}

void muatPegawai(Pegawai daftar_pegawai[], int &total_pegawai) {
    ifstream file(FILE_PEGAWAI); string line, token; total_pegawai = 0;
    while (getline(file, line) && total_pegawai < 100) {
        if (line.empty()) continue; stringstream ss(line);
        getline(ss, daftar_pegawai[total_pegawai].username, ','); getline(ss, daftar_pegawai[total_pegawai].nama_pegawai, ',');
        getline(ss, token, ','); daftar_pegawai[total_pegawai].umur = stoi(token);
        getline(ss, daftar_pegawai[total_pegawai].jabatan, ','); getline(ss, daftar_pegawai[total_pegawai].lokasi.kota, ','); getline(ss, daftar_pegawai[total_pegawai].lokasi.jalan, ',');
        if (getline(ss, token, ',') && !token.empty()) { try { daftar_pegawai[total_pegawai].gaji = stod(token); } catch (...) { daftar_pegawai[total_pegawai].gaji = 0; } } else { daftar_pegawai[total_pegawai].gaji = 0; }
        getline(ss, token, ','); daftar_pegawai[total_pegawai].is_active = (token == "1");
        total_pegawai++;
    }
    file.close(); perbaikiPointer(daftar_pegawai, total_pegawai);
}

void simpanAbsensi(Absensi daftar_absensi[], int total_absensi) {
    ofstream file(FILE_ABSENSI);
    for (int i = 0; i < total_absensi; i++) file << daftar_absensi[i].username << "," << daftar_absensi[i].tanggal << "," << daftar_absensi[i].waktu << "," << daftar_absensi[i].status << "\n";
    file.close();
}

void muatAbsensi(Absensi daftar_absensi[], int &total_absensi) {
    ifstream file(FILE_ABSENSI); string line; total_absensi = 0;
    while (getline(file, line) && total_absensi < 300) {
        if (line.empty()) continue; stringstream ss(line);
        getline(ss, daftar_absensi[total_absensi].username, ','); getline(ss, daftar_absensi[total_absensi].tanggal, ',');
        getline(ss, daftar_absensi[total_absensi].waktu, ','); getline(ss, daftar_absensi[total_absensi].status, ',');
        total_absensi++;
    }
    file.close();
}

void catatLogAdmin(string pesan) {
    ofstream file(FILE_LOG, ios::app);
    if (file.is_open()) {
        file << "[" << dapatkanTanggalSekarang() << " " << dapatkanWaktuSekarang() << "] " << pesan << "\n";
        file.close();
    }
}

void lihatLogAdmin() {
    bersihkanLayar();
    cout << "\n--- Catatan Log Aktivitas Admin ---\n";
    ifstream file(FILE_LOG);
    string line;
    bool ada = false;
    while (getline(file, line)) {
        cout << line << "\n";
        ada = true;
    }
    file.close();
    if (!ada) cout << "(Belum ada aktivitas yang terekam)\n";
    jeda();
}

void ajukanCutiUser(Absensi daftar_absensi[], int &total_absensi, string username) {
    bersihkanLayar(); cout << "\n--- Formulir Pengajuan Cuti ---\n";
    try {
        if (total_absensi >= 300) throw runtime_error("Data absensi penuh!");
        string tanggal;
        cout << "Masukkan Tanggal Cuti (YYYY-MM-DD): "; cin >> tanggal;
        
        daftar_absensi[total_absensi] = {username, tanggal, dapatkanWaktuSekarang(), "Pending Cuti"};
        total_absensi++;
        simpanAbsensi(daftar_absensi, total_absensi);
        cout << "\n[+] Pengajuan cuti berhasil dikirim ke Admin.\n";
    } catch (const exception &e) { cout << "\n[!] Peringatan: " << e.what() << "\n"; }
    jeda();
}

void kelolaCutiAdmin(Absensi daftar_absensi[], int total_absensi) {
    bersihkanLayar(); cout << "\n--- Persetujuan Cuti Pegawai ---\n";
    bool ada_pending = false;
    for (int i = 0; i < total_absensi; i++) {
        if (daftar_absensi[i].status == "Pending Cuti") {
            ada_pending = true;
            cout << "\nUsername: " << daftar_absensi[i].username << " | Tanggal Cuti: " << daftar_absensi[i].tanggal;
            cout << "\nSetujui cuti ini? (Y/N/Skip): ";
            char pilih; cin >> pilih;
            if (pilih == 'Y' || pilih == 'y') {
                daftar_absensi[i].status = "Cuti Approved";
                catatLogAdmin("Menyetujui cuti " + daftar_absensi[i].username + " untuk tgl " + daftar_absensi[i].tanggal);
                cout << "[+] Cuti disetujui.\n";
            } else if (pilih == 'N' || pilih == 'n') {
                daftar_absensi[i].status = "Cuti Rejected";
                catatLogAdmin("Menolak cuti " + daftar_absensi[i].username + " untuk tgl " + daftar_absensi[i].tanggal);
                cout << "[+] Cuti ditolak.\n";
            }
        }
    }
    if (!ada_pending) cout << "(Tidak ada pengajuan cuti yang pending)\n";
    else simpanAbsensi(daftar_absensi, total_absensi);
    jeda();
}

void cetakSlipGaji(Pegawai daftar_pegawai[], int total_pegawai, Absensi daftar_absensi[], int total_absensi, string username) {
    bersihkanLayar();
    for (int i = 0; i < total_pegawai; i++) {
        if (daftar_pegawai[i].username == username) {
            int hadir = 0;
            for (int j = 0; j < total_absensi; j++) {
                if (daftar_absensi[j].username == username && daftar_absensi[j].status == "Hadir") hadir++;
            }
            
            double gaji_final = (hadir >= 20) ? daftar_pegawai[i].gaji : (daftar_pegawai[i].gaji / 20.0) * hadir;
            
            string nama_file = "SlipGaji_" + username + ".txt";
            ofstream file(nama_file);
            file << "======================================\n";
            file << "        SLIP GAJI KEBAB CENDANA       \n";
            file << "======================================\n";
            file << "Nama Pegawai : " << daftar_pegawai[i].nama_pegawai << "\n";
            file << "Jabatan      : " << *(daftar_pegawai[i].jabatan_ptr) << "\n";
            file << "Total Hadir  : " << hadir << " Hari\n";
            file << "--------------------------------------\n";
            file << "Gaji Pokok   : " << formatRupiah(daftar_pegawai[i].gaji) << "\n";
            file << "Gaji Diterima: " << formatRupiah(gaji_final) << "\n";
            file << "======================================\n";
            file.close();
            
            cout << "\n[+] Slip Gaji berhasil dicetak ke file '" << nama_file << "'!\n";
            jeda(); return;
        }
    }
    cout << "\n[!] Data pegawai tidak ditemukan.\n"; jeda();
}

string loginAkun(Akun daftar_akun[], int total_akun, string &role_output) {
    bersihkanLayar(); int sisa_kesempatan = 3; string input_nama, input_password;
    cout << "\n--- Login Sistem ---\n";
    while (sisa_kesempatan > 0) {
        cout << "Username : "; cin >> input_nama;
        cout << "Password : "; cin >> input_password;
        for (int i = 0; i < total_akun; i++) {
            if (input_nama == daftar_akun[i].username && input_password == daftar_akun[i].password) {
                role_output = daftar_akun[i].role;
                cout << "\n[+] Login Berhasil! Selamat datang, " << input_nama << ".\n";
                if (role_output == "admin") catatLogAdmin("Login sukses: " + input_nama);
                return input_nama;
            }
        }
        sisa_kesempatan--;
        if (sisa_kesempatan > 0) cout << "[!] Username atau Password salah! Sisa percobaan: " << sisa_kesempatan << "\n\n";
    }
    cout << "\n[X] Akses ditolak.\n"; jeda(); return "";
}

// [BARU] Fungsi dinamis untuk mengecek apakah hari ini pegawai tersebut cuti/izin/sakit
string cekStatusKerja(string username, bool is_active, Absensi daftar_absensi[], int total_absensi) {
    if (!is_active) return "Resign";
    
    string hari_ini = dapatkanTanggalSekarang();
    for (int i = 0; i < total_absensi; i++) {
        if (daftar_absensi[i].username == username && daftar_absensi[i].tanggal == hari_ini) {
            if (daftar_absensi[i].status == "Cuti Approved") return "Cuti";
            if (daftar_absensi[i].status == "Sakit") return "Sakit";
            if (daftar_absensi[i].status == "Izin") return "Izin";
            if (daftar_absensi[i].status == "Pending Cuti") return "Aktif (Tunggu Cuti)";
        }
    }
    return "Aktif";
}

void lihatPegawai(Pegawai daftar_pegawai[], int total_pegawai, bool filter_aktif, Absensi daftar_absensi[], int total_absensi) {
    cout << setfill('-') << setw(115) << "-" << "\n" << setfill(' ');
    cout << left << setw(15) << "Username" << setw(20) << "Nama" << setw(6)  << "Umur" << setw(20) << "Jabatan" << setw(20) << "Alamat" << setw(18) << "Gaji" << "Status\n";
    cout << setfill('-') << setw(115) << "-" << "\n" << setfill(' ');
    bool ada_data = false;
    for (int i = 0; i < total_pegawai; i++) {
        if (daftar_pegawai[i].is_active == filter_aktif) {
            ada_data = true; string alamat = daftar_pegawai[i].lokasi.kota + ", " + daftar_pegawai[i].lokasi.jalan;
            if (alamat.length() > 18) alamat = alamat.substr(0, 15) + "...";
            cout << left << setw(15) << daftar_pegawai[i].username << setw(20) << daftar_pegawai[i].nama_pegawai << setw(6)  << daftar_pegawai[i].umur
                 << setw(20) << *(daftar_pegawai[i].jabatan_ptr) << setw(20) << alamat << setw(18) << formatRupiah(daftar_pegawai[i].gaji)  
                 << cekStatusKerja(daftar_pegawai[i].username, daftar_pegawai[i].is_active, daftar_absensi, total_absensi) << "\n";
        }
    }
    if (!ada_data) cout << "  (Belum ada data pegawai di kategori ini)\n";
    cout << setfill('-') << setw(115) << "-" << "\n" << setfill(' ');
}

void tambahPegawai(Pegawai daftar_pegawai[], int &total_pegawai, Akun daftar_akun[], int &total_akun) {
    bersihkanLayar(); cout << "\n--- Tambah Pegawai & Akun Baru ---\n";
    try {
        if (total_akun >= 100 || total_pegawai >= 100) throw runtime_error("Kapasitas penuh!");
        string new_username, new_password;
        cout << "Buat Username   : "; cin >> new_username;
        for (int i = 0; i < total_akun; i++) { if (daftar_akun[i].username == new_username) throw runtime_error("Username digunakan!"); }
        cout << "Buat Password   : "; cin >> new_password;
        if (new_password.length() < 4) throw runtime_error("Password min 4 karakter!");
        
        daftar_akun[total_akun] = {new_username, new_password, "user"}; total_akun++; simpanAkun(daftar_akun, total_akun);

        int idx = total_pegawai; daftar_pegawai[idx].username  = new_username; daftar_pegawai[idx].is_active = true;
        cout << "Nama Lengkap    : "; cin.ignore(); getline(cin, daftar_pegawai[idx].nama_pegawai);
        daftar_pegawai[idx].umur = bacaInt("Umur            : ");
        cout << "Jabatan         : "; getline(cin >> ws, daftar_pegawai[idx].jabatan);
        cout << "Kota Domisili   : "; getline(cin, daftar_pegawai[idx].lokasi.kota);
        cout << "Alamat Jalan    : "; getline(cin, daftar_pegawai[idx].lokasi.jalan);
        daftar_pegawai[idx].gaji = bacaDouble("Gaji (Rp)       : ");

        total_pegawai++; perbaikiPointer(daftar_pegawai, total_pegawai); simpanPegawai(daftar_pegawai, total_pegawai);
        catatLogAdmin("Menambahkan pegawai baru: " + new_username);
        cout << "\n[+] Data pegawai dan akun berhasil didaftarkan.\n";
    } catch (const exception &e) { cout << "\n[!] Proses Gagal: " << e.what() << "\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    jeda();
}

void ubahPegawaiAdmin(Pegawai daftar_pegawai[], int total_pegawai, Akun daftar_akun[], int total_akun, Absensi daftar_absensi[], int total_absensi) {
    bersihkanLayar(); lihatPegawai(daftar_pegawai, total_pegawai, true, daftar_absensi, total_absensi);
    try {
        string target; cout << "\nMasukkan Username Pegawai yang ingin diubah: "; cin >> target;
        int idx = -1;
        for (int i = 0; i < total_pegawai; i++) { if (daftar_pegawai[i].username == target && daftar_pegawai[i].is_active) { idx = i; break; } }
        if (idx == -1) throw runtime_error("Pegawai tidak ditemukan!");
        int pilihan = bacaInt("\n1. Edit Data\n2. Edit Gaji\n3. Reset Sandi\nPilih: ");

        if (pilihan == 1) {
            cout << "Jabatan Baru    : "; getline(cin >> ws, daftar_pegawai[idx].jabatan);
            cout << "Kota Baru       : "; getline(cin, daftar_pegawai[idx].lokasi.kota);
            cout << "Jalan Baru      : "; getline(cin, daftar_pegawai[idx].lokasi.jalan);
            perbaikiPointer(daftar_pegawai, total_pegawai); simpanPegawai(daftar_pegawai, total_pegawai);
            catatLogAdmin("Mengubah data profil pegawai: " + target);
            cout << "\n[+] Profil diperbarui.\n";
        } else if (pilihan == 2) {
            daftar_pegawai[idx].gaji = bacaDouble("Gaji Baru (Rp): ");
            simpanPegawai(daftar_pegawai, total_pegawai);
            catatLogAdmin("Mengubah gaji pegawai: " + target);
            cout << "\n[+] Gaji diperbarui.\n";
        } else if (pilihan == 3) {
            for (int j = 0; j < total_akun; j++) {
                if (daftar_akun[j].username == target) {
                    string pass_baru; cout << "Sandi Baru: "; cin >> pass_baru;
                    daftar_akun[j].password = pass_baru; simpanAkun(daftar_akun, total_akun);
                    catatLogAdmin("Mereset sandi pegawai: " + target);
                    cout << "\n[+] Sandi direset.\n"; break;
                }
            }
        }
    } catch (const exception &e) { cout << "\n[!] Error: " << e.what() << "\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    jeda();
}

void hapusPegawaiSoftDelete(Pegawai daftar_pegawai[], int total_pegawai, Absensi daftar_absensi[], int total_absensi) {
    bersihkanLayar(); lihatPegawai(daftar_pegawai, total_pegawai, true, daftar_absensi, total_absensi);
    try {
        string target; cout << "\nMasukkan Username Pegawai (Resign): "; cin >> target;
        for (int i = 0; i < total_pegawai; i++) {
            if (daftar_pegawai[i].username == target && daftar_pegawai[i].is_active) {
                daftar_pegawai[i].is_active = false; simpanPegawai(daftar_pegawai, total_pegawai);
                catatLogAdmin("Mencatat Resign pegawai: " + target);
                cout << "\n[+] Pegawai di-Resign.\n"; jeda(); return;
            }
        }
        throw runtime_error("Pegawai tidak ditemukan.");
    } catch (const exception &e) { cout << "\n[!] Error: " << e.what() << "\n"; cin.clear(); cin.ignore(10000, '\n'); }
    jeda();
}

void rekapAbsensiAdmin(Absensi daftar_absensi[], int total_absensi, Pegawai daftar_pegawai[], int total_pegawai) {
    bersihkanLayar(); int pilihan = bacaInt("\n--- Rekap Absensi ---\n1. Semua\n2. Per Username\n3. Laporan Kehadiran Pegawai\nPilih: ");
    if (pilihan == 1 || pilihan == 2) {
        string target = ""; if (pilihan == 2) { cout << "Username: "; cin >> target; }
        for (int i = 0; i < total_absensi; i++) {
            if (pilihan == 1 || daftar_absensi[i].username == target)
                cout << daftar_absensi[i].username << " | " << daftar_absensi[i].tanggal << " | " << daftar_absensi[i].status << "\n";
        }
    } else if (pilihan == 3) {
        for (int i = 0; i < total_pegawai; i++) {
            if (!daftar_pegawai[i].is_active) continue;
            int hadir = 0; for (int j = 0; j < total_absensi; j++) { if (daftar_absensi[j].username == daftar_pegawai[i].username && daftar_absensi[j].status == "Hadir") hadir++; }
            cout << daftar_pegawai[i].username << " | Total Hadir: " << hadir << "\n";
        }
    }
    jeda();
}

void laporanGaji(Pegawai daftar_pegawai[], int total_pegawai) {
    bersihkanLayar(); double total_gaji = 0; cout << "\n--- Laporan Gaji ---\n";
    for (int i = 0; i < total_pegawai; i++) {
        if (!daftar_pegawai[i].is_active) continue;
        cout << left << setw(15) << daftar_pegawai[i].username << " | " << formatRupiah(daftar_pegawai[i].gaji) << "\n";
        total_gaji += daftar_pegawai[i].gaji;
    }
    cout << "Total Beban: " << formatRupiah(total_gaji) << "\n"; jeda();
}

void inputAbsensi(Absensi daftar_absensi[], int &total_absensi, string username) {
    bersihkanLayar();
    try {
        string hari_ini = dapatkanTanggalSekarang();
        for (int i = 0; i < total_absensi; i++) { if (daftar_absensi[i].username == username && daftar_absensi[i].tanggal == hari_ini) throw runtime_error("Sudah absen/cuti hari ini!"); }
        int pilihan = bacaInt("1. Hadir\n2. Sakit\n3. Izin\nPilih: ");
        string status = (pilihan == 1) ? "Hadir" : (pilihan == 2) ? "Sakit" : "Izin";
        daftar_absensi[total_absensi] = {username, hari_ini, dapatkanWaktuSekarang(), status}; total_absensi++;
        simpanAbsensi(daftar_absensi, total_absensi); cout << "\n[+] Presensi " << status << " dicatat.\n";
    } catch (const exception &e) { cout << "\n[!] " << e.what() << "\n"; cin.clear(); cin.ignore(10000, '\n'); }
    jeda();
}

void lihatAbsensiUser(Absensi daftar_absensi[], int total_absensi, string username) {
    bersihkanLayar(); cout << "\n--- Riwayat Kehadiran ---\n";
    for (int i = 0; i < total_absensi; i++) {
        if (daftar_absensi[i].username == username) cout << daftar_absensi[i].tanggal << " | " << daftar_absensi[i].status << "\n";
    }
    jeda();
}

void batalkanAbsensi(Absensi daftar_absensi[], int &total_absensi, string username) {
    bersihkanLayar(); string hari_ini = dapatkanTanggalSekarang();
    for (int i = total_absensi - 1; i >= 0; i--) {
        if (daftar_absensi[i].username == username && daftar_absensi[i].tanggal == hari_ini) {
            for (int j = i; j < total_absensi - 1; j++) daftar_absensi[j] = daftar_absensi[j + 1];
            total_absensi--; simpanAbsensi(daftar_absensi, total_absensi);
            cout << "\n[+] Absensi hari ini dibatalkan.\n"; jeda(); return;
        }
    }
    cout << "\n[!] Tidak ada absensi hari ini.\n"; jeda();
}

void swapPegawai(Pegawai* a, Pegawai* b) {
    Pegawai temp = *a;
    *a = *b;
    *b = temp;
}

void mergeNamaDesc(Pegawai arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Pegawai* L = new Pegawai[n1];
    Pegawai* R = new Pegawai[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].nama_pegawai >= R[j].nama_pegawai) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    delete[] L; delete[] R;
}

void mergeSortNama(Pegawai arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortNama(arr, left, mid);
        mergeSortNama(arr, mid + 1, right);
        mergeNamaDesc(arr, left, mid, right);
    }
}

int partitionUmur(Pegawai arr[], int low, int high) {
    int pivot = arr[high].umur;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].umur < pivot) swapPegawai(&arr[++i], &arr[j]);
    }
    swapPegawai(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSortUmur(Pegawai arr[], int low, int high) {
    if (low < high) {
        int pi = partitionUmur(arr, low, high);
        quickSortUmur(arr, low, pi - 1);
        quickSortUmur(arr, pi + 1, high);
    }
}

void insertionSortJabatan(Pegawai arr[], int n) {
    for (int i = 1; i < n; i++) {
        Pegawai key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].jabatan > key.jabatan) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void sequentialSearchNama(Pegawai arr[], int total_pegawai, string target, Absensi daftar_absensi[], int total_absensi) {
    bersihkanLayar();
    cout << "\n--- Hasil Pencarian Berdasarkan Nama: " << target << " ---\n\n";
    bool ditemukan = false;
    for (int i = 0; i < total_pegawai; i++) {
        if (arr[i].nama_pegawai.find(target) != string::npos) {
            cout << " Nama Lengkap : " << arr[i].nama_pegawai << "\n"
                 << " Username     : " << arr[i].username << "\n"
                 << " Usia         : " << arr[i].umur << " Tahun\n"
                 << " Jabatan      : " << *(arr[i].jabatan_ptr) << "\n"
                 << " Pendapatan   : " << formatRupiah(arr[i].gaji) << "\n"
                 << " Status Peg.  : " << cekStatusKerja(arr[i].username, arr[i].is_active, daftar_absensi, total_absensi) << "\n"
                 << " ----------------------------------------\n";
            ditemukan = true;
        }
    }
    if (!ditemukan) cout << "[!] Data pegawai dengan nama mengandung kata '" << target << "' tidak ditemukan.\n";
    jeda();
}

void binarySearchUmur(Pegawai arr[], int total_pegawai, int target, Absensi daftar_absensi[], int total_absensi) {
    quickSortUmur(arr, 0, total_pegawai - 1);
    perbaikiPointer(arr, total_pegawai);
    
    int left = 0, right = total_pegawai - 1;
    bool ditemukan = false;
    bersihkanLayar();
    cout << "\n--- Hasil Pencarian Berdasarkan Umur: " << target << " Tahun ---\n\n";
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid].umur == target) {
            int temp = mid;
            while (temp >= 0 && arr[temp].umur == target) {
                cout << " > " << arr[temp].nama_pegawai << " (" << *(arr[temp].jabatan_ptr)
                     << ") | Status: " << cekStatusKerja(arr[temp].username, arr[temp].is_active, daftar_absensi, total_absensi) << "\n";
                temp--;
            }
            temp = mid + 1;
            while (temp < total_pegawai && arr[temp].umur == target) {
                cout << " > " << arr[temp].nama_pegawai << " (" << *(arr[temp].jabatan_ptr)
                     << ") | Status: " << cekStatusKerja(arr[temp].username, arr[temp].is_active, daftar_absensi, total_absensi) << "\n";
                temp++;
            }
            ditemukan = true;
            break;
        }
        if (arr[mid].umur < target) left = mid + 1;
        else right = mid - 1;
    }
    if (!ditemukan) cout << "[!] Tidak ditemukan pegawai yang berusia " << target << " tahun.\n";
    jeda();
}