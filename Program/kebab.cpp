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

// ============================================================
// KONFIGURASI CSV
// ============================================================
const string FILE_AKUN     = "csv/akun.csv";
const string FILE_PEGAWAI  = "csv/pegawai.csv";
const string FILE_ABSENSI  = "csv/absensi.csv";

// ============================================================
// UTILITAS
// ============================================================
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
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d",
             1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(buffer);
}

string dapatkanWaktuSekarang() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[9];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",
             ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return string(buffer);
}

void prosesKeluar(int detik) {
    if (detik == 0) {
        cout << "Terima kasih telah menggunakan aplikasi ini!\n";
        return;
    }
    cout << "Aplikasi akan tertutup dalam " << detik << " detik...\n";
    prosesKeluar(detik - 1);
}

// Format angka ke Rupiah: 5000000 -> "Rp 5.000.000"
string formatRupiah(double nominal) {
    long long n = (long long)nominal;
    string s = to_string(n);
    int sisipan = (int)s.length() - 3;
    while (sisipan > 0) {
        s.insert(sisipan, ".");
        sisipan -= 3;
    }
    return "Rp " + s;
}

// Baca integer dengan validasi; lempar exception jika gagal
int bacaInt(const string &prompt) {
    int nilai;
    cout << prompt;
    if (!(cin >> nilai)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw runtime_error("Input harus berupa angka bulat!");
    }
    return nilai;
}

// Baca double dengan validasi
double bacaDouble(const string &prompt) {
    double nilai;
    cout << prompt;
    if (!(cin >> nilai)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw runtime_error("Input harus berupa angka!");
    }
    return nilai;
}

// Tunggu user tekan Enter sebelum lanjut
void jeda() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ============================================================
// CSV — AKUN
// ============================================================
void simpanAkun(Akun daftar_akun[], int total_akun) {
    ofstream file(FILE_AKUN);
    if (!file.is_open())
        throw runtime_error("Gagal membuka file akun untuk disimpan!");
    for (int i = 0; i < total_akun; i++) {
        file << daftar_akun[i].username << ","
             << daftar_akun[i].password << ","
             << daftar_akun[i].role     << "\n";
    }
    file.close();
}

void muatAkun(Akun daftar_akun[], int &total_akun) {
    ifstream file(FILE_AKUN);
    string line;
    total_akun = 0;
    while (getline(file, line) && total_akun < 100) {
        if (line.empty()) continue;
        stringstream ss(line);
        getline(ss, daftar_akun[total_akun].username, ',');
        getline(ss, daftar_akun[total_akun].password, ',');
        getline(ss, daftar_akun[total_akun].role,     ',');
        total_akun++;
    }
    file.close();

    // Buat akun admin default jika file kosong
    if (total_akun == 0) {
        daftar_akun[0] = {"admin", "admin123", "admin"};
        total_akun = 1;
        simpanAkun(daftar_akun, total_akun);
    }
}

// ============================================================
// CSV — PEGAWAI  (sekarang menyimpan kolom gaji)
// ============================================================
void simpanPegawai(Pegawai daftar_pegawai[], int total_pegawai) {
    ofstream file(FILE_PEGAWAI);
    if (!file.is_open())
        throw runtime_error("Gagal membuka file pegawai untuk disimpan!");
    for (int i = 0; i < total_pegawai; i++) {
        file << daftar_pegawai[i].username          << ","
             << daftar_pegawai[i].nama_pegawai       << ","
             << daftar_pegawai[i].umur               << ","
             << daftar_pegawai[i].jabatan            << ","
             << daftar_pegawai[i].lokasi.kota        << ","
             << daftar_pegawai[i].lokasi.jalan       << ","
             << fixed << setprecision(2)
             << daftar_pegawai[i].gaji               << ","   
             << daftar_pegawai[i].is_active          << "\n";
    }
    file.close();
}

void muatPegawai(Pegawai daftar_pegawai[], int &total_pegawai) {
    ifstream file(FILE_PEGAWAI);
    string line, token;
    total_pegawai = 0;
    while (getline(file, line) && total_pegawai < 100) {
        if (line.empty()) continue;
        stringstream ss(line);

        getline(ss, daftar_pegawai[total_pegawai].username,       ',');
        getline(ss, daftar_pegawai[total_pegawai].nama_pegawai,   ',');

        getline(ss, token, ',');
        daftar_pegawai[total_pegawai].umur = stoi(token);

        getline(ss, daftar_pegawai[total_pegawai].jabatan,        ',');
        getline(ss, daftar_pegawai[total_pegawai].lokasi.kota,    ',');
        getline(ss, daftar_pegawai[total_pegawai].lokasi.jalan,   ',');

        // [BARU] baca gaji — toleransi file lama yang belum punya kolom gaji
        if (getline(ss, token, ',') && !token.empty()) {
            try { daftar_pegawai[total_pegawai].gaji = stod(token); }
            catch (...) { daftar_pegawai[total_pegawai].gaji = 0; }
        } else {
            daftar_pegawai[total_pegawai].gaji = 0;
        }

        getline(ss, token, ',');
        daftar_pegawai[total_pegawai].is_active = (token == "1");

        total_pegawai++;
    }
    file.close();
    perbaikiPointer(daftar_pegawai, total_pegawai);
}

// ============================================================
// CSV — ABSENSI
// ============================================================
void simpanAbsensi(Absensi daftar_absensi[], int total_absensi) {
    ofstream file(FILE_ABSENSI);
    if (!file.is_open())
        throw runtime_error("Gagal membuka file absensi untuk disimpan!");
    for (int i = 0; i < total_absensi; i++) {
        file << daftar_absensi[i].username << ","
             << daftar_absensi[i].tanggal  << ","
             << daftar_absensi[i].waktu    << ","
             << daftar_absensi[i].status   << "\n";
    }
    file.close();
}

void muatAbsensi(Absensi daftar_absensi[], int &total_absensi) {
    ifstream file(FILE_ABSENSI);
    string line;
    total_absensi = 0;
    while (getline(file, line) && total_absensi < 300) {
        if (line.empty()) continue;
        stringstream ss(line);
        getline(ss, daftar_absensi[total_absensi].username, ',');
        getline(ss, daftar_absensi[total_absensi].tanggal,  ',');
        getline(ss, daftar_absensi[total_absensi].waktu,    ',');
        getline(ss, daftar_absensi[total_absensi].status,   ',');
        total_absensi++;
    }
    file.close();
}

// ============================================================
// AUTENTIKASI
// ============================================================
string loginAkun(Akun daftar_akun[], int total_akun, string &role_output) {
    bersihkanLayar();
    int sisa_kesempatan = 3;
    string input_nama, input_password;
    cout << "=== Login ===\n";

    while (sisa_kesempatan > 0) {
        cout << "Username : "; cin >> input_nama;
        cout << "Password : "; cin >> input_password;

        for (int i = 0; i < total_akun; i++) {
            if (input_nama == daftar_akun[i].username &&
                input_password == daftar_akun[i].password) {
                role_output = daftar_akun[i].role;
                cout << "[+] Login Berhasil! Selamat datang, " << input_nama << "!\n";
                return input_nama;
            }
        }
        sisa_kesempatan--;
        if (sisa_kesempatan > 0)
            cout << "[!] Username atau Password salah! Sisa kesempatan: "
                 << sisa_kesempatan << "\n\n";
    }
    cout << "\n[X] Gagal login 3 kali. Kembali ke menu utama.\n";
    jeda();
    return "";
}

// ============================================================
// TAMPILAN TABEL PEGAWAI
// ============================================================
void lihatPegawai(Pegawai daftar_pegawai[], int total_pegawai, bool filter_aktif) {
    cout << setfill('-') << setw(110) << "-" << "\n" << setfill(' ');
    cout << left
         << setw(15) << "Username"
         << setw(20) << "Nama"
         << setw(6)  << "Umur"
         << setw(20) << "Jabatan"
         << setw(25) << "Alamat"
         << setw(18) << "Gaji"         
         << "Status\n";
    cout << setfill('-') << setw(110) << "-" << "\n" << setfill(' ');

    bool ada_data = false;
    for (int i = 0; i < total_pegawai; i++) {
        if (daftar_pegawai[i].is_active == filter_aktif) {
            ada_data = true;
            string alamat = daftar_pegawai[i].lokasi.kota + ", " +
                            daftar_pegawai[i].lokasi.jalan;
            if (alamat.length() > 23) alamat = alamat.substr(0, 20) + "...";

            cout << left
                 << setw(15) << daftar_pegawai[i].username
                 << setw(20) << daftar_pegawai[i].nama_pegawai
                 << setw(6)  << daftar_pegawai[i].umur
                 << setw(20) << *(daftar_pegawai[i].jabatan_ptr)
                 << setw(25) << alamat
                 << setw(18) << formatRupiah(daftar_pegawai[i].gaji)  
                 << (filter_aktif ? "Aktif" : "Resign") << "\n";
        }
    }
    if (!ada_data) cout << "  (Belum ada data di kategori ini.)\n";
    cout << setfill('-') << setw(110) << "-" << "\n" << setfill(' ');
}

// ============================================================
// CRUD ADMIN — TAMBAH PEGAWAI
// ============================================================
void tambahPegawai(Pegawai daftar_pegawai[], int &total_pegawai,
                   Akun daftar_akun[], int &total_akun) {
    bersihkanLayar();
    cout << "=== Tambah Pegawai & Akun Baru ===\n";
    try {
        if (total_akun >= 100 || total_pegawai >= 100)
            throw runtime_error("Kapasitas data penuh (maks 100)!");

        string new_username, new_password;
        cout << "Buat Username Pegawai : "; cin >> new_username;

        // Cek duplikat username [BARU error handling]
        for (int i = 0; i < total_akun; i++) {
            if (daftar_akun[i].username == new_username)
                throw runtime_error("Username '" + new_username + "' sudah digunakan!");
        }

        cout << "Buat Password Pegawai : "; cin >> new_password;
        if (new_password.length() < 4)
            throw runtime_error("Password minimal 4 karakter!");

        // Simpan akun
        daftar_akun[total_akun] = {new_username, new_password, "user"};
        total_akun++;
        simpanAkun(daftar_akun, total_akun);

        // Simpan data pegawai
        int idx = total_pegawai;
        daftar_pegawai[idx].username  = new_username;
        daftar_pegawai[idx].is_active = true;

        cout << "Nama Pegawai : "; cin.ignore(); getline(cin, daftar_pegawai[idx].nama_pegawai);
        if (daftar_pegawai[idx].nama_pegawai.empty())
            throw runtime_error("Nama pegawai tidak boleh kosong!");

        daftar_pegawai[idx].umur = bacaInt("Umur         : ");
        if (daftar_pegawai[idx].umur <= 0 || daftar_pegawai[idx].umur > 80)
            throw runtime_error("Umur tidak valid (1-80)!");

        cout << "Jabatan      : "; getline(cin, daftar_pegawai[idx].jabatan);
        cout << "Kota         : "; getline(cin, daftar_pegawai[idx].lokasi.kota);
        cout << "Jalan        : "; getline(cin, daftar_pegawai[idx].lokasi.jalan);

        // [BARU] input gaji
        daftar_pegawai[idx].gaji = bacaDouble("Gaji (Rp)    : ");
        if (daftar_pegawai[idx].gaji < 0)
            throw runtime_error("Gaji tidak boleh negatif!");

        total_pegawai++;
        perbaikiPointer(daftar_pegawai, total_pegawai);
        simpanPegawai(daftar_pegawai, total_pegawai);
        cout << "[+] Data pegawai dan akun berhasil dibuat!\n";

    } catch (const exception &e) {
        cout << "\n[!] Gagal: " << e.what() << "\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    jeda();
}

// ============================================================
// CRUD ADMIN — UBAH PEGAWAI (termasuk ubah gaji) [BARU]
// ============================================================
void ubahPegawaiAdmin(Pegawai daftar_pegawai[], int total_pegawai,
                      Akun daftar_akun[], int total_akun) {
    bersihkanLayar();
    lihatPegawai(daftar_pegawai, total_pegawai, true);
    try {
        string target;
        cout << "\nMasukkan Username Pegawai yang akan diubah: "; cin >> target;

        int idx = -1;
        for (int i = 0; i < total_pegawai; i++) {
            if (daftar_pegawai[i].username == target && daftar_pegawai[i].is_active) {
                idx = i; break;
            }
        }
        if (idx == -1)
            throw runtime_error("Pegawai aktif dengan username tersebut tidak ditemukan!");

        cout << "\n1. Ubah Detail Pegawai\n"
             << "2. Ubah Gaji\n"
             << "3. Reset Password\n"
             << "Pilih: ";
        int pilihan = bacaInt("");

        if (pilihan == 1) {
            cout << "Jabatan Baru : "; cin.ignore(); getline(cin, daftar_pegawai[idx].jabatan);
            cout << "Kota Baru    : "; getline(cin, daftar_pegawai[idx].lokasi.kota);
            cout << "Jalan Baru   : "; getline(cin, daftar_pegawai[idx].lokasi.jalan);
            perbaikiPointer(daftar_pegawai, total_pegawai);
            simpanPegawai(daftar_pegawai, total_pegawai);
            cout << "[+] Profil pegawai diperbarui.\n";

        } else if (pilihan == 2) {
            // [BARU] ubah gaji
            cout << "Gaji saat ini : " << formatRupiah(daftar_pegawai[idx].gaji) << "\n";
            daftar_pegawai[idx].gaji = bacaDouble("Gaji Baru (Rp): ");
            if (daftar_pegawai[idx].gaji < 0)
                throw runtime_error("Gaji tidak boleh negatif!");
            simpanPegawai(daftar_pegawai, total_pegawai);
            cout << "[+] Gaji berhasil diperbarui menjadi "
                 << formatRupiah(daftar_pegawai[idx].gaji) << "\n";

        } else if (pilihan == 3) {
            bool ketemu = false;
            for (int j = 0; j < total_akun; j++) {
                if (daftar_akun[j].username == target) {
                    string pass_baru;
                    cout << "Password Baru: "; cin >> pass_baru;
                    if (pass_baru.length() < 4)
                        throw runtime_error("Password minimal 4 karakter!");
                    daftar_akun[j].password = pass_baru;
                    simpanAkun(daftar_akun, total_akun);
                    cout << "[+] Password berhasil direset.\n";
                    ketemu = true;
                    break;
                }
            }
            if (!ketemu)
                throw runtime_error("Akun tidak ditemukan untuk username tersebut!");
        } else {
            throw runtime_error("Pilihan tidak valid!");
        }

    } catch (const exception &e) {
        cout << "\n[!] Error: " << e.what() << "\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    jeda();
}

// ============================================================
// CRUD ADMIN — SOFT DELETE (RESIGN)
// ============================================================
void hapusPegawaiSoftDelete(Pegawai daftar_pegawai[], int total_pegawai) {
    bersihkanLayar();
    lihatPegawai(daftar_pegawai, total_pegawai, true);
    try {
        string target;
        cout << "\nMasukkan Username Pegawai yang akan di-Resign: "; cin >> target;

        for (int i = 0; i < total_pegawai; i++) {
            if (daftar_pegawai[i].username == target && daftar_pegawai[i].is_active) {
                daftar_pegawai[i].is_active = false;
                simpanPegawai(daftar_pegawai, total_pegawai);
                cout << "[+] Pegawai '" << target << "' berhasil dipindahkan ke daftar Resign.\n";
                jeda();
                return;
            }
        }
        throw runtime_error("Pegawai aktif dengan username tersebut tidak ditemukan!");

    } catch (const exception &e) {
        cout << "\n[!] Error: " << e.what() << "\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    jeda();
}

// ============================================================
// [BARU] ADMIN — REKAP ABSENSI LENGKAP
// ============================================================
void rekapAbsensiAdmin(Absensi daftar_absensi[], int total_absensi,
                       Pegawai daftar_pegawai[], int total_pegawai) {
    bersihkanLayar();
    cout << "=== Rekap Absensi ===\n";
    cout << "1. Lihat semua absensi\n"
         << "2. Filter berdasarkan username\n"
         << "3. Filter berdasarkan tanggal\n"
         << "4. Ringkasan per-pegawai (Hadir/Sakit/Izin)\n"
         << "Pilih: ";
    try {
        int pilihan = bacaInt("");

        auto cetakHeaderAbsensi = []() {
            cout << setfill('-') << setw(70) << "-" << "\n" << setfill(' ');
            cout << left << setw(15) << "Username"
                 << setw(15) << "Tanggal"
                 << setw(12) << "Waktu"
                 << "Status\n";
            cout << setfill('-') << setw(70) << "-" << "\n" << setfill(' ');
        };

        if (pilihan == 1) {
            // Semua absensi
            bersihkanLayar();
            cout << "=== Seluruh Data Absensi ===\n";
            if (total_absensi == 0) { cout << "Belum ada data absensi.\n"; jeda(); return; }
            cetakHeaderAbsensi();
            for (int i = 0; i < total_absensi; i++) {
                cout << left << setw(15) << daftar_absensi[i].username
                     << setw(15) << daftar_absensi[i].tanggal
                     << setw(12) << daftar_absensi[i].waktu
                     << daftar_absensi[i].status << "\n";
            }
            cout << setfill('-') << setw(70) << "-" << "\n" << setfill(' ');
            cout << "Total record: " << total_absensi << "\n";

        } else if (pilihan == 2) {
            // Filter username
            string target;
            cout << "Masukkan Username: "; cin >> target;
            bersihkanLayar();
            cout << "=== Absensi untuk: " << target << " ===\n";
            cetakHeaderAbsensi();
            int count = 0;
            for (int i = 0; i < total_absensi; i++) {
                if (daftar_absensi[i].username == target) {
                    cout << left << setw(15) << daftar_absensi[i].username
                         << setw(15) << daftar_absensi[i].tanggal
                         << setw(12) << daftar_absensi[i].waktu
                         << daftar_absensi[i].status << "\n";
                    count++;
                }
            }
            if (count == 0) cout << "  (Tidak ada data untuk username ini.)\n";
            cout << setfill('-') << setw(70) << "-" << "\n" << setfill(' ');
            cout << "Total: " << count << " record\n";

        } else if (pilihan == 3) {
            // Filter tanggal
            string target_tanggal;
            cout << "Masukkan Tanggal (YYYY-MM-DD): "; cin >> target_tanggal;
            bersihkanLayar();
            cout << "=== Absensi Tanggal: " << target_tanggal << " ===\n";
            cetakHeaderAbsensi();
            int count = 0;
            for (int i = 0; i < total_absensi; i++) {
                if (daftar_absensi[i].tanggal == target_tanggal) {
                    cout << left << setw(15) << daftar_absensi[i].username
                         << setw(15) << daftar_absensi[i].tanggal
                         << setw(12) << daftar_absensi[i].waktu
                         << daftar_absensi[i].status << "\n";
                    count++;
                }
            }
            if (count == 0) cout << "  (Tidak ada absensi pada tanggal ini.)\n";
            cout << setfill('-') << setw(70) << "-" << "\n" << setfill(' ');
            cout << "Total hadir pada tanggal ini: " << count << " orang\n";

        } else if (pilihan == 4) {
            // Ringkasan per-pegawai
            bersihkanLayar();
            cout << "=== Ringkasan Kehadiran Per Pegawai ===\n";
            cout << setfill('-') << setw(75) << "-" << "\n" << setfill(' ');
            cout << left << setw(15) << "Username"
                 << setw(20) << "Nama"
                 << setw(10) << "Hadir"
                 << setw(10) << "Sakit"
                 << setw(10) << "Izin"
                 << "Total\n";
            cout << setfill('-') << setw(75) << "-" << "\n" << setfill(' ');

            for (int i = 0; i < total_pegawai; i++) {
                if (!daftar_pegawai[i].is_active) continue; // skip yang resign
                int hadir = 0, sakit = 0, izin = 0;
                for (int j = 0; j < total_absensi; j++) {
                    if (daftar_absensi[j].username == daftar_pegawai[i].username) {
                        if (daftar_absensi[j].status == "Hadir") hadir++;
                        else if (daftar_absensi[j].status == "Sakit") sakit++;
                        else if (daftar_absensi[j].status == "Izin") izin++;
                    }
                }
                cout << left << setw(15) << daftar_pegawai[i].username
                     << setw(20) << daftar_pegawai[i].nama_pegawai
                     << setw(10) << hadir
                     << setw(10) << sakit
                     << setw(10) << izin
                     << (hadir + sakit + izin) << "\n";
            }
            cout << setfill('-') << setw(75) << "-" << "\n" << setfill(' ');

        } else {
            throw runtime_error("Pilihan tidak valid!");
        }

    } catch (const exception &e) {
        cout << "\n[!] Error: " << e.what() << "\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    jeda();
}

// ============================================================
// [BARU] ADMIN — LAPORAN GAJI
// ============================================================
void laporanGaji(Pegawai daftar_pegawai[], int total_pegawai) {
    bersihkanLayar();
    cout << "=== Laporan Gaji Pegawai Aktif ===\n";
    cout << setfill('-') << setw(70) << "-" << "\n" << setfill(' ');
    cout << left << setw(15) << "Username"
         << setw(25) << "Nama"
         << setw(20) << "Jabatan"
         << "Gaji\n";
    cout << setfill('-') << setw(70) << "-" << "\n" << setfill(' ');

    double total_gaji = 0;
    int count = 0;
    for (int i = 0; i < total_pegawai; i++) {
        if (!daftar_pegawai[i].is_active) continue;
        cout << left << setw(15) << daftar_pegawai[i].username
             << setw(25) << daftar_pegawai[i].nama_pegawai
             << setw(20) << *(daftar_pegawai[i].jabatan_ptr)
             << formatRupiah(daftar_pegawai[i].gaji) << "\n";
        total_gaji += daftar_pegawai[i].gaji;
        count++;
    }
    if (count == 0) cout << "  (Belum ada pegawai aktif.)\n";
    cout << setfill('-') << setw(70) << "-" << "\n" << setfill(' ');
    cout << right << setw(60) << "Total Penggajian : "
         << formatRupiah(total_gaji) << "\n";
    cout << right << setw(60) << "Jumlah Pegawai   : " << count << "\n";
    jeda();
}

// ============================================================
// USER — ABSENSI
// ============================================================
void inputAbsensi(Absensi daftar_absensi[], int &total_absensi, string username) {
    bersihkanLayar();
    cout << "=== Presensi Harian ===\n";
    try {
        if (total_absensi >= 300)
            throw runtime_error("Kapasitas data absensi penuh!");

        // [BARU] cek sudah absen hari ini
        string hari_ini = dapatkanTanggalSekarang();
        for (int i = 0; i < total_absensi; i++) {
            if (daftar_absensi[i].username == username &&
                daftar_absensi[i].tanggal  == hari_ini) {
                throw runtime_error("Anda sudah melakukan presensi hari ini ("
                                    + daftar_absensi[i].status + ")."
                                    " Gunakan menu Batalkan jika ingin mengubah.");
            }
        }

        cout << "1. Hadir\n2. Sakit\n3. Izin\n";
        int pilihan = bacaInt("Pilih: ");
        if (pilihan < 1 || pilihan > 3)
            throw runtime_error("Pilihan tidak valid (1-3)!");

        string status = (pilihan == 1) ? "Hadir" : (pilihan == 2) ? "Sakit" : "Izin";

        daftar_absensi[total_absensi] = {username, hari_ini, dapatkanWaktuSekarang(), status};
        total_absensi++;
        simpanAbsensi(daftar_absensi, total_absensi);
        cout << "[+] Absensi '" << status << "' tercatat pada "
             << hari_ini << " " << daftar_absensi[total_absensi-1].waktu << "\n";

    } catch (const exception &e) {
        cout << "\n[!] " << e.what() << "\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    jeda();
}

void lihatAbsensiUser(Absensi daftar_absensi[], int total_absensi, string username) {
    bersihkanLayar();
    cout << "=== Riwayat Kehadiran: " << username << " ===\n";
    cout << setfill('-') << setw(45) << "-" << "\n" << setfill(' ');
    cout << left << setw(15) << "Tanggal" << setw(12) << "Waktu" << "Status\n";
    cout << setfill('-') << setw(45) << "-" << "\n" << setfill(' ');

    int count = 0, hadir = 0, sakit = 0, izin = 0;
    for (int i = 0; i < total_absensi; i++) {
        if (daftar_absensi[i].username == username) {
            cout << left << setw(15) << daftar_absensi[i].tanggal
                 << setw(12) << daftar_absensi[i].waktu
                 << daftar_absensi[i].status << "\n";
            count++;
            if (daftar_absensi[i].status == "Hadir") hadir++;
            else if (daftar_absensi[i].status == "Sakit") sakit++;
            else izin++;
        }
    }
    if (count == 0) cout << "  (Belum ada riwayat absensi.)\n";
    cout << setfill('-') << setw(45) << "-" << "\n" << setfill(' ');
    cout << "Hadir: " << hadir << "  Sakit: " << sakit << "  Izin: " << izin
         << "  Total: " << count << "\n";
    jeda();
}

void batalkanAbsensi(Absensi daftar_absensi[], int &total_absensi, string username) {
    bersihkanLayar();
    try {
        string hari_ini = dapatkanTanggalSekarang();
        for (int i = total_absensi - 1; i >= 0; i--) {
            if (daftar_absensi[i].username == username &&
                daftar_absensi[i].tanggal  == hari_ini) {
                // geser array
                for (int j = i; j < total_absensi - 1; j++)
                    daftar_absensi[j] = daftar_absensi[j + 1];
                total_absensi--;
                simpanAbsensi(daftar_absensi, total_absensi);
                cout << "[+] Absensi hari ini berhasil dibatalkan.\n";
                jeda();
                return;
            }
        }
        throw runtime_error("Anda belum melakukan absensi hari ini.");
    } catch (const exception &e) {
        cout << "\n[!] " << e.what() << "\n";
    }
    jeda();
}

// ============================================================
// MAIN
// ============================================================
int main() {
    Akun    daftar_akun[100];    int total_akun    = 0;
    Pegawai daftar_pegawai[100]; int total_pegawai = 0;
    Absensi daftar_absensi[300]; int total_absensi = 0;

    try {
        muatAkun(daftar_akun, total_akun);
        muatPegawai(daftar_pegawai, total_pegawai);
        muatAbsensi(daftar_absensi, total_absensi);
    } catch (const exception &e) {
        cout << "[!] Gagal memuat data: " << e.what() << "\n";
        return 1;
    }

    bool aplikasi_berjalan = true;
    while (aplikasi_berjalan) {
        bersihkanLayar();
        cout << "==========================================\n";
        cout << "  Sistem Manajemen Pegawai Kebab Cendana  \n";
        cout << "==========================================\n";
        cout << "1. Login\n2. Keluar Aplikasi\nPilihan: ";

        try {
            int menu_awal = bacaInt("");

            if (menu_awal == 1) {
                string current_role = "";
                string current_user = loginAkun(daftar_akun, total_akun, current_role);

                if (current_user.empty()) continue;

                bool sesi_login = true;
                while (sesi_login) {
                    bersihkanLayar();
                    cout << "==========================================\n";
                    cout << "  Dashboard [" << left << setw(29) << (current_role + " : " + current_user) << "]\n";
                    cout << "==========================================\n";

                    try {
                        if (current_role == "admin") {
                            cout << "1. Tambah Pegawai Baru\n"
                                 << "2. Lihat Pegawai Aktif\n"
                                 << "3. Lihat Pegawai Resign\n"
                                 << "4. Ubah Detail / Reset Akun Pegawai\n"
                                 << "5. Proses Resign Pegawai\n"
                                 << "6. Rekap Absensi\n"          
                                 << "7. Laporan Gaji\n"           
                                 << "8. Logout\n"
                                 << "Pilihan: ";
                            int menu_crud = bacaInt("");

                            switch (menu_crud) {
                                case 1: tambahPegawai(daftar_pegawai, total_pegawai, daftar_akun, total_akun); break;
                                case 2:
                                    bersihkanLayar();
                                    cout << "=== Daftar Pegawai Aktif ===\n";
                                    lihatPegawai(daftar_pegawai, total_pegawai, true);
                                    jeda();
                                    break;
                                case 3:
                                    bersihkanLayar();
                                    cout << "=== Daftar Pegawai Resign ===\n";
                                    lihatPegawai(daftar_pegawai, total_pegawai, false);
                                    jeda();
                                    break;
                                case 4: ubahPegawaiAdmin(daftar_pegawai, total_pegawai, daftar_akun, total_akun); break;
                                case 5: hapusPegawaiSoftDelete(daftar_pegawai, total_pegawai); break;
                                case 6: rekapAbsensiAdmin(daftar_absensi, total_absensi, daftar_pegawai, total_pegawai); break;  
                                case 7: laporanGaji(daftar_pegawai, total_pegawai); break;   
                                case 8: sesi_login = false; break;
                                default: cout << "[!] Pilihan tidak valid!\n"; jeda();
                            }

                        } else if (current_role == "user") {
                            cout << "1. Presensi Hari Ini\n"
                                 << "2. Lihat Profil Saya\n"
                                 << "3. Cek Riwayat Presensi\n"
                                 << "4. Ubah Password\n"
                                 << "5. Batalkan Presensi Hari Ini\n"
                                 << "6. Logout\n"
                                 << "Pilihan: ";
                            int menu_crud = bacaInt("");

                            switch (menu_crud) {
                                case 1: inputAbsensi(daftar_absensi, total_absensi, current_user); break;
                                case 2:
                                    bersihkanLayar();
                                    cout << "=== Profil Saya ===\n";
                                    for (int i = 0; i < total_pegawai; i++) {
                                        if (daftar_pegawai[i].username == current_user) {
                                            cout << "Nama    : " << daftar_pegawai[i].nama_pegawai << "\n"
                                                 << "Jabatan : " << *(daftar_pegawai[i].jabatan_ptr) << "\n"
                                                 << "Umur    : " << daftar_pegawai[i].umur << "\n"
                                                 << "Kota    : " << daftar_pegawai[i].lokasi.kota << "\n"
                                                 << "Gaji    : " << formatRupiah(daftar_pegawai[i].gaji) << "\n"; 
                                        }
                                    }
                                    jeda();
                                    break;
                                case 3: lihatAbsensiUser(daftar_absensi, total_absensi, current_user); break;
                                case 4:
                                    try {
                                        string pass_baru;
                                        cout << "Masukkan Password Baru: "; cin >> pass_baru;
                                        if (pass_baru.length() < 4)
                                            throw runtime_error("Password minimal 4 karakter!");
                                        for (int i = 0; i < total_akun; i++) {
                                            if (daftar_akun[i].username == current_user) {
                                                daftar_akun[i].password = pass_baru;
                                                simpanAkun(daftar_akun, total_akun);
                                                cout << "[+] Password berhasil diubah!\n";
                                            }
                                        }
                                    } catch (const exception &e) {
                                        cout << "[!] " << e.what() << "\n";
                                    }
                                    jeda();
                                    break;
                                case 5: batalkanAbsensi(daftar_absensi, total_absensi, current_user); break;
                                case 6: sesi_login = false; break;
                                default: cout << "[!] Pilihan tidak valid!\n"; jeda();
                            }
                        }
                    } catch (const exception &e) {
                        cout << "\n[!] Error tidak terduga: " << e.what() << "\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        jeda();
                    }
                }

            } else if (menu_awal == 2) {
                prosesKeluar(3);
                aplikasi_berjalan = false;
            } else {
                cout << "[!] Pilihan tidak valid (1 atau 2)!\n";
                jeda();
            }

        } catch (const exception &e) {
            cout << "[!] Error: " << e.what() << "\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            jeda();
        }
    }
    return 0;
}
