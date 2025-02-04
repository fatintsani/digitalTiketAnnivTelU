#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <map>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace std::chrono;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define BROWN "\033[38;5;94m"
#define GOLD "\033[38;5;220m"
#define SILVER "\033[38;5;250m"

int nomorPenontonTerakhir = 0;

struct Tiket
{
    string kategori;
    int harga;
    string fasilitas;
    int stok;
    int kuota_tiket;
};

struct Pesanan
{
    string kategori;
    int jumlah;
    int total_harga;
    string metode_pembayaran;
    string detail_pembayaran;
    int no_penonton;
    string nama;
    vector<string> kode_masuk;
    string waktu_pembelian;
};

struct Ulasan
{
    string kategori;
    string nama;
    string komentar;
    int penilaian;
};

struct Antrean
{
    string username;
    string kategori;
    int jumlah;
    string status;
};

vector<Antrean> daftarAntrean;
vector<Ulasan> listUlasan;
vector<Pesanan> laporanPesanan;

map<string, int> hargaKategori = {
    {"VIP", 1000000},
    {"Gold", 750000},
    {"Silver", 500000},
    {"Bronze", 250000}};

map<string, string> userRoles = {
    {"telkomuniversitynight24", "admin"},
    {"fatinmuflihutstsani", "user"},
    {"dindakhoirunnisabangun", "user"},
    {"muhammadvazrypalensky", "user"}};

string generateOTP()
{
    srand(time(0));
    string otp;
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    for (int i = 0; i < 6; i++)
    {
        otp += characters[rand() % characters.length()];
    }

    return otp;
}

string convertMonthToIndonesian(int month)
{
    const string months[] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni",
                             "Juli", "Agustus", "September", "Oktober", "November", "Desember"};
    if (month >= 1 && month <= 12)
    {
        return months[month - 1];
    }
    return "Invalid";
}

string getFormattedTimeWIB()
{
    auto now = system_clock::now();
    auto nowWIB = now + hours(7);
    time_t waktuWIB = system_clock::to_time_t(nowWIB);

    tm *localTime = localtime(&waktuWIB);

    stringstream formattedTime;
    formattedTime << put_time(localTime, "%H:%M:%S") << " | "
                  << localTime->tm_mday << " "
                  << convertMonthToIndonesian(localTime->tm_mon + 1) << " "
                  << (localTime->tm_year + 1900);

    return formattedTime.str();
}

bool verifyOTP(const string &expectedOTP)
{
    string inputOTP;
    int attempts = 3;

    while (attempts > 0)
    {
        cout << "Silahkan masukan OTP: ";
        cin >> inputOTP;

        if (inputOTP == expectedOTP)
        {
            cout << GREEN << "OTP berhasil diverifikasi!\n"
                 << RESET;
            return true;
        }
        else
        {
            attempts--;
            cout << RED << "OTP salah! Anda memiliki " << attempts << " percobaan lagi.\n"
                 << RESET;
        }
    }

    cout << RED << "Anda telah gagal memverifikasi OTP sebanyak 3 kali. Login gagal.\n"
         << RESET;
    return false;
}

bool loginSSO(string &username)
{
    string password;
    string otp;

    const string validUsername1 = "fatinmuflihutstsani";
    const string validPassword1 = "607022430003";

    const string validUsername2 = "dindakhoirunnisabangun";
    const string validPassword2 = "607022430001";

    const string validUsername3 = "muhammadvazrypalensky";
    const string validPassword3 = "607022400073";

    const string validUsername4 = "telkomuniversitynight24";
    const string validPassword4 = "telkomuniversitynight24";

    unordered_map<string, string> userRoles;
    userRoles[validUsername1] = "user";
    userRoles[validUsername2] = "user";
    userRoles[validUsername3] = "user";
    userRoles[validUsername4] = "admin";

    cout << BLUE << "\n=== LOGIN SSO ===\n"
         << RESET;
    cout << RED << "Welcome to Aplikasi Digital Tikets for Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;

    for (int attempts = 3; attempts > 0; attempts--)
    {
        cout << "Masukkan username: ";
        cin >> username;

        if (userRoles.find(username) == userRoles.end())
        {
            char guestOption;
            cout << YELLOW << "Username tidak ditemukan. Apakah Anda ingin login sebagai guest? (Y/N): " << RESET;
            cin >> guestOption;

            if (guestOption == 'Y' || guestOption == 'y')
            {
                cout << GREEN << "Login sebagai guest berhasil!\n"
                     << RESET;
                return true;
            }
            else
            {
                cout << RED << "Coba lagi.\n"
                     << RESET;
                if (attempts > 1)
                {
                    cout << YELLOW << "Anda memiliki " << attempts - 1 << " percobaan lagi.\n"
                         << RESET;
                }
                continue;
            }
        }

        cout << "Masukkan password: ";
        cin >> password;

        if ((username == validUsername1 && password == validPassword1) ||
            (username == validUsername2 && password == validPassword2) ||
            (username == validUsername3 && password == validPassword3) ||
            (username == validUsername4 && password == validPassword4))
        {
            otp = generateOTP();
            cout << YELLOW << "Kode OTP telah dikirim: " << otp << endl
                 << RESET;

            if (verifyOTP(otp))
            {
                cout << GREEN << "Berhasil login dengan role sebagai " << userRoles[username] << "!\n"
                     << RESET;
                return true;
            }
            else
            {
                cout << RED << "Login gagal karena verifikasi OTP salah.\n"
                     << RESET;
                return false;
            }
        }
        else
        {
            cout << RED << "Password salah untuk user " << username << ".\n"
                 << RESET;
        }

        if (attempts > 1)
        {
            cout << YELLOW << "Anda memiliki " << attempts - 1 << " percobaan lagi.\n"
                 << RESET;
        }
    }

    cout << RED << "Anda telah gagal login sebanyak 3 kali. Akses ditolak.\n"
         << RESET;
    return false;
}

bool konfirmasiLogout()
{
    char pilihan;
    do
    {
        cout << YELLOW << "\nApakah Anda yakin ingin keluar dari aplikasi? (Y/N): " << RESET;
        cin >> pilihan;
        if (pilihan == 'Y' || pilihan == 'y')
        {
            cout << GREEN << "\nAnda telah berhasil logout. Sampai jumpa!\n"
                 << RESET;
            return true;
        }
        else if (pilihan == 'N' || pilihan == 'n')
        {
            cout << BLUE << "\nLogout dibatalkan. Kembali ke menu utama.\n"
                 << RESET;
            return false;
        }
        else
        {
            cout << RED << "\nInput tidak valid. Silakan masukkan Y atau N.\n"
                 << RESET;
        }
    } while (true);
}

void tampilkanPromosi(const std::vector<Tiket> &tiketList)
{
    time_t now = time(0);
    tm *localTime = localtime(&now);
    int hari = localTime->tm_wday;

    if (hari == 5)
    {
        std::cout << GREEN << "\nPROMO JUMAT BERKAH! Diskon 15% untuk semua kategori tiket.\n"
                  << RESET;

        for (const auto &tiket : tiketList)
        {
            int hargaDiskon = tiket.harga * 0.85;
            std::cout << MAGENTA << "Kategori: " << tiket.kategori << "\n"
                      << RESET
                      << BLUE << "Harga Asli: Rp " << tiket.harga << "\n"
                      << RESET
                      << GREEN << "Harga Diskon: Rp " << hargaDiskon << "\n"
                      << RESET
                      << BLUE << "Fasilitas: " << tiket.fasilitas << "\n"
                      << RESET
                      << GREEN << "Stok Tersedia: " << tiket.stok << "\n"
                      << RESET
                      << "--------------------------------------\n";
        }
    }
    else
    {
        std::cout << BLUE << "\nSaat ini tidak ada promosi khusus.\n"
                  << RESET;
    }
}

void informasiKonser(const vector<Tiket> &tiketList)
{
    cout << BLUE << "\n=== INFORMASI KONSER ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    cout << "Guest Star : Noah dan Dewa19\n";
    cout << "Tanggal    : 17 Januari 2025\n";
    cout << "Waktu      : 19.00 WIB / 22.00 WIB\n";
    cout << "Lokasi     : Telkom University Convention Hall (TUCH)\n";
    cout << "Deskripsi  : Malam spektakuler untuk merayakan ulang tahun dengan\n"
         << "             penampilan dari Noah dan Dewa19. Jangan lewatkan momen ini!\n";

    cout << BLUE << "\nKategori Tiket dan Penjelasannya:\n"
         << RESET;
    cout << "1. " << MAGENTA << "VIP     " << RESET << ": Tempat duduk eksklusif di depan panggung, termasuk snack dan merchandise.\n";
    cout << "2. " << GOLD << "Gold    " << RESET << ": Tempat duduk premium dengan jarak dekat ke panggung.\n";
    cout << "3. " << SILVER << "Silver  " << RESET << ": Tempat duduk reguler dengan pemandangan panggung yang baik.\n";
    cout << "4. " << BROWN << "Bronze  " << RESET << ": Area berdiri dengan harga terjangkau.\n";

    cout << BLUE << "\nList Harga Tiket Konser:\n"
         << RESET;
    cout << "1. " << MAGENTA << "VIP     " << RESET << ": Rp. 1.000.000,00\n";
    cout << "2. " << GOLD << "Gold    " << RESET << ": Rp. 750.000,00\n";
    cout << "3. " << SILVER << "Silver  " << RESET << ": Rp. 500.000,00\n";
    cout << "4. " << BROWN << "Bronze  " << RESET << ": Rp. 250.000,00\n";
    cout << "--------------------------------------\n";

    tampilkanPromosi(tiketList);
}

void hitungCountdown()
{
    tm waktu_mulai = {};
    waktu_mulai.tm_year = 2025 - 1900;
    waktu_mulai.tm_mon = 01;
    waktu_mulai.tm_mday = 17;
    waktu_mulai.tm_hour = 19;
    waktu_mulai.tm_min = 0;
    waktu_mulai.tm_sec = 0;

    tm waktu_selesai = waktu_mulai;
    waktu_selesai.tm_hour = 22;

    time_t waktu_mulai_time = mktime(&waktu_mulai);
    time_t waktu_selesai_time = mktime(&waktu_selesai);
    time_t sekarang = time(0);

    if (sekarang > waktu_selesai_time)
    {
        cout << GREEN << "Acara sudah selesai, Terimakasih sampai jumpa tahun depan!.\n"
             << RESET;
        return;
    }

    if (sekarang >= waktu_mulai_time && sekarang <= waktu_selesai_time)
    {
        cout << MAGENTA << "Acara sedang berlangsung. Waktu acara: 19:00 WIB - 22:00 WIB.\n"
             << RESET;
        return;
    }

    double selisih = difftime(waktu_mulai_time, sekarang);
    int detik = static_cast<int>(selisih);

    int hari = detik / (60 * 60 * 24);
    detik %= (60 * 60 * 24);
    int jam = detik / (60 * 60);
    detik %= (60 * 60);
    int menit = detik / 60;
    detik %= 60;

    cout << BLUE << "\n=== COUNTDOWN KONSER ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    cout << "Tanggal    : 17 Januari 2025\n";
    cout << "Waktu      : 19.00 WIB / 22.00 WIB\n";
    cout << "Lokasi     : Telkom University Convention Hall (TUCH)\n";

    cout << RED << "\nCountdown menuju acara: " << hari << " hari " << jam << " jam "
         << menit << " menit " << detik << " detik\n"
         << RESET;
    cout << GREEN << "Acara akan dimulai pada 17 Januari 2025, pukul 19:00 WIB dan selesai pukul 22:00 WIB.\n"
         << RESET;
}

string generateKodeMasuk()
{
    string kodeMasuk = "#TELU";
    for (int i = 0; i < 4; i++)
    {
        kodeMasuk += to_string(rand() % 10);
    }
    return kodeMasuk;
}

void cetakTiket(Pesanan &pesanan)
{
    cout << BLUE << "\n=== DETAIL TIKET ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;

    for (size_t i = 0; i < pesanan.kode_masuk.size(); i++)
    {
        cout << "Tiket " << (i + 1) << "\n";
        cout << BLUE << "Nomor Penonton : " << pesanan.no_penonton + i << RESET << endl;
        cout << BLUE << "Nama           : " << pesanan.nama << RESET << endl;
        cout << YELLOW << "Kategori       : " << pesanan.kategori << RESET << endl;
        cout << GREEN << "Pembayaran     : Berhasil\n"
             << RESET;
        cout << MAGENTA << "Kode Masuk     : " << pesanan.kode_masuk[i] << RESET << endl;
        cout << "--------------------------------------\n";
    }
}

pair<string, string> pilihMetodePembayaran()
{
    int pilihan_metode, sub_pilihan;
    string metode, detail, nomor, cvv, nomorVA;

    cout << BLUE << "\n=== PILIH METODE PEMBAYARAN ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    cout << "1. Internet/Mobile Banking (Virtual Account)\n";
    cout << "2. E-Wallet\n";
    cout << "3. Transfer Bank\n";
    cout << "4. QRIS\n";
    cout << "5. Kartu Kredit/Debit\n";
    cout << "6. PayLater\n";
    cout << "Pilihan Anda: ";
    cin >> pilihan_metode;

    if (pilihan_metode < 1 || pilihan_metode > 6)
    {
        cout << RED << "Metode pembayaran tidak valid. Silakan coba lagi.\n"
             << RESET;
        return make_pair("Metode Pembayaran Tidak Valid", "-");
    }

    switch (pilihan_metode)
    {
    case 1:
        metode = "Internet/Mobile Banking";
        cout << "Pilih Aplikasi:\n";
        cout << "1. BCA Mobile\n";
        cout << "2. Livin by Mandiri\n";
        cout << "3. BRImo\n";
        cout << "4. BJBDigi\n";
        cout << "Pilihan Anda: ";
        cin >> sub_pilihan;

        if (sub_pilihan < 1 || sub_pilihan > 4)
        {
            cout << RED << "Bank tidak valid. Silakan coba lagi.\n"
                 << RESET;
            return make_pair("Internet/Mobile Banking", "Bank Tidak Valid");
        }

        if (sub_pilihan == 1)
        {
            detail = "Livin by Mandiri";
            nomorVA = "008" + to_string(rand() % 9000000000 + 1000000000);
        }
        else if (sub_pilihan == 2)
        {
            detail = "BCA Mobile";
            nomorVA = "014" + to_string(rand() % 9000000000 + 1000000000);
        }
        else if (sub_pilihan == 3)
        {
            detail = "BRImo";
            nomorVA = "002" + to_string(rand() % 9000000000 + 1000000000);
        }
        else
        {
            detail = "BJBDigi";
            nomorVA = "110" + to_string(rand() % 9000000000 + 1000000000);
        }

        detail += " - Nomor Virtual Account: " + nomorVA;

        detail = (sub_pilihan == 1) ? "BCA Mobile" : (sub_pilihan == 2) ? "Livin by Mandiri"
                                                 : (sub_pilihan == 3)   ? "BRImo"
                                                                        : "BJBDigi";

        cout << GREEN << "\nLangkah: Buka aplikasi mobile banking " << detail << " Anda, pilih menu transfer, dan masukkan nomor Virtual Account berikut : "
             << nomorVA
             << "\n"
             << RESET;
        break;

    case 2:
        metode = "E-Wallet";
        cout << "Pilih E-Wallet:\n";
        cout << "1. GoPay\n";
        cout << "2. OVO\n";
        cout << "3. Dana\n";
        cout << "4. ShopeePay\n";
        cout << "5. LinkAja\n";
        cout << "Pilihan Anda: ";
        cin >> sub_pilihan;

        if (sub_pilihan < 1 || sub_pilihan > 5)
        {
            cout << RED << "E-Wallet tidak valid. Silakan coba lagi.\n"
                 << RESET;
            return make_pair("E-Wallet", "E-Wallet Tidak Valid");
        }

        detail = (sub_pilihan == 1) ? "GoPay" : (sub_pilihan == 2) ? "OVO"
                                            : (sub_pilihan == 3)   ? "Dana"
                                            : (sub_pilihan == 4)   ? "ShopeePay"
                                                                   : "LinkAja";

        cout << "Masukkan nomor HP Anda (contoh: 081234567890): ";
        cin.ignore();
        getline(cin, nomor);
        if (nomor.length() != 12 || nomor.substr(0, 2) != "08")
        {
            cout << RED << "Nomor HP tidak valid. Silakan coba lagi.\n"
                 << RESET;
            return make_pair("E-Wallet", "Nomor HP Tidak Valid");
        }

        detail += " - No. HP: " + nomor;
        cout << GREEN << "Langkah: Buka aplikasi " << detail << ", pilih metode pembayaran, dan ikuti petunjuk untuk menyelesaikan transaksi.\n"
             << RESET;
        break;

    case 3:
        metode = "Transfer Bank";
        cout << "Pilih Bank:\n";
        cout << "1. Bank Mandiri\n";
        cout << "2. Bank BCA\n";
        cout << "3. Bank BRI\n";
        cout << "4. Bank BJB\n";
        cout << "Pilihan Anda: ";
        cin >> sub_pilihan;

        if (sub_pilihan < 1 || sub_pilihan > 4)
        {
            cout << RED << "Aplikasi tidak valid. Silakan coba lagi.\n"
                 << RESET;
            return make_pair("Transfer Bank", "Aplikasi Tidak Valid");
        }

        detail = (sub_pilihan == 1) ? "Bank Mandiri" : (sub_pilihan == 2) ? "Bank BCA"
                                                   : (sub_pilihan == 3)   ? "Bank BRI"
                                                                          : "Bank BJB";

        cout << GREEN << "Langkah: Periksa saldo kartu " << detail << ", Anda dan konfirmasi pembayaran sesuai instruksi di layar. \n"
             << RESET;
        break;

    case 4:
        metode = "QRIS";
        detail = "QRIS Universal";
        cout << GREEN << "Scan kode QR berikut dengan aplikasi pembayaran pilihan Anda:\n"
             << RESET;
        cout << "[QRIS CODE]\n ";
        cout << GREEN << "Langkah: Gunakan aplikasi pembayaran untuk memindai QR, lalu ikuti instruksi di layar.\n"
             << RESET;
        break;

    case 5:
        metode = "Kartu Kredit/Debit";
        cout << "Pilih Provider:\n";
        cout << "1. Visa\n";
        cout << "2. Mastercard\n";
        cout << "3. JCB\n";
        cout << "Pilihan Anda: ";
        cin >> sub_pilihan;

        if (sub_pilihan < 1 || sub_pilihan > 3)
        {
            cout << RED << "Provider tidak valid. Silakan coba lagi.\n"
                 << RESET;
            return make_pair("Kartu Kredit/Debit", "Provider Tidak Valid");
        }

        detail = (sub_pilihan == 1) ? "Visa" : (sub_pilihan == 2) ? "Mastercard"
                                                                  : "JCB";

        cout << "Masukkan nomor kartu (contoh: 1234-5678-9101-1121): ";
        cin.ignore();
        getline(cin, nomor);
        if (nomor.length() != 19 || !isdigit(nomor[0]))
        {
            cout << RED << "Nomor kartu tidak valid. Silakan coba lagi.\n"
                 << RESET;
            return make_pair("Kartu Kredit/Debit", "Nomor Kartu Tidak Valid");
        }

        detail += " - Kartu: " + nomor;
        cout << "Masukkan CVV (contoh: 123): ";
        getline(cin, cvv);
        if (cvv.length() != 3 || !isdigit(cvv[0]))
        {
            cout << RED << "CVV tidak valid. Silakan coba lagi.\n"
                 << RESET;
            return make_pair("Kartu Kredit/Debit", "CVV Tidak Valid");
        }

        detail += " - CVV: " + cvv;
        cout << GREEN << "Langkah: Periksa saldo kartu Anda " << detail << " dan konfirmasi pembayaran sesuai instruksi di layar.\n"
             << RESET;
        break;

    case 6:
        metode = "PayLater";
        cout << "Pilih PayLater:\n";
        cout << "1. GoPayLater\n";
        cout << "2. Shopee PayLater\n";
        cout << "3. Akulaku\n";
        cout << "Pilihan Anda: ";
        cin >> sub_pilihan;

        if (sub_pilihan < 1 || sub_pilihan > 3)
        {
            cout << RED << "PayLater tidak valid. Silakan coba lagi.\n"
                 << RESET;
            return make_pair("PayLater", "PayLater Tidak Valid");
        }

        detail = (sub_pilihan == 1) ? "GoPayLater" : (sub_pilihan == 2) ? "Shopee PayLater"
                                                                        : "Akulaku";

        cout << GREEN << "Langkah: Login ke aplikasi " << detail << ", pilih pembayaran PayLater, dan selesaikan transaksi sesuai instruksi.\n"
             << RESET;
        break;
    }

    cout << YELLOW << "\nApakah Anda yakin dengan pilihan metode pembayaran ini? (Y/N): " << RESET;
    char konfirmasi;
    cin >> konfirmasi;

    if (konfirmasi == 'N' || konfirmasi == 'n')
    {
        cout << RED << "Transaksi berhasil dibatalkan.\n"
             << RESET;
        return make_pair("Batal", "Kembali ke Menu Utama");
    }

    return make_pair(metode, detail);
}

double validasiKodeReferral(const string &kode)
{
    if (kode == "PANITIA")
    {
        return 1.0;
    }
    if (kode == "TELUANIV")
    {
        return 1.0;
    }
    if (kode == "DISKON10")
    {
        return 0.1;
    }
    if (kode == "DISKON20")
    {
        return 0.2;
    }
    return 0.0;
}

Pesanan prosesPesanan(vector<Tiket> &tiketList, int &nomorPenontonTerakhir, const string &username)
{
    Pesanan pesanan;
    pesanan.nama = username;
    int pilihan, jumlah, uang_dibayar, kembalian;

    pesanan.waktu_pembelian = getFormattedTimeWIB();

    if (pesanan.kategori != "Invalid" && !pesanan.kategori.empty() && pesanan.kategori != "Waitlist")
    {
        laporanPesanan.push_back(pesanan);
    }

    if (pesanan.kategori.empty())
    {
        pesanan.kategori = "Tidak Diketahui";
    }

    cout << BLUE << "\n=== INFORMASI TIKET ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    cout << BLUE << "Kategori Tiket:\n"
         << RESET;
    cout << "1. " << MAGENTA << "VIP     " << RESET << ": Tempat duduk eksklusif di depan panggung, termasuk snack dan merchandise.\n";
    cout << "2. " << GOLD << "Gold    " << RESET << ": Tempat duduk premium dengan jarak dekat ke panggung.\n";
    cout << "3. " << SILVER << "Silver  " << RESET << ": Tempat duduk reguler dengan pemandangan panggung yang baik.\n";
    cout << "4. " << BROWN << "Bronze  " << RESET << ": Area berdiri dengan harga terjangkau.\n";

    cout << BLUE << "\nList Harga Tiket Konser:\n"
         << RESET;
    cout << "1. " << MAGENTA << "VIP     " << RESET << ": Rp. 1.000.000,00\n";
    cout << "2. " << GOLD << "Gold    " << RESET << ": Rp. 750.000,00\n";
    cout << "3. " << SILVER << "Silver  " << RESET << ": Rp. 500.000,00\n";
    cout << "4. " << BROWN << "Bronze  " << RESET << ": Rp. 250.000,00\n";
    cout << "--------------------------------------\n";

    cout << GREEN << "\nPilih kategori tiket (1-" << tiketList.size() << "): " << RESET;
    cin >> pilihan;

    if (pilihan < 1 || pilihan > tiketList.size())
    {
        cout << RED << "Pilihan tidak valid!\n"
             << RESET;
        pesanan.kategori = "Invalid";
        return pesanan;
    }

    Tiket &tiketDipilih = tiketList[pilihan - 1];

    if (tiketDipilih.stok == 0)
    {
        cout << RED << "Tiket untuk kategori " << tiketDipilih.kategori << " habis.\n"
             << RESET;
        cout << YELLOW << "Masukkan jumlah tiket yang Anda inginkan: " << RESET;
        cin >> jumlah;

        daftarAntrean.push_back({username, tiketDipilih.kategori, jumlah});
        cout << GREEN << "Anda telah ditambahkan ke antrean untuk kategori " << tiketDipilih.kategori
             << " sebanyak " << jumlah << " tiket.\n"
             << RESET;

        pesanan.kategori = "Waitlist";
        return pesanan;
    }

    cout << BLUE << "Masukkan jumlah tiket: " << RESET;
    cin >> jumlah;

    if (jumlah > tiketList[pilihan - 1].stok || jumlah > tiketList[pilihan - 1].kuota_tiket)
    {
        cout << RED << "Maaf, tiket yang tersedia hanya " << tiketList[pilihan - 1].kuota_tiket << " tiket.\n"
             << RESET;
        pesanan.kategori = "Invalid";
        return pesanan;
    }

    pesanan.kategori = tiketList[pilihan - 1].kategori;
    pesanan.jumlah = jumlah;

    for (int i = 0; i < jumlah; i++)
    {
        string kode = generateKodeMasuk();
        pesanan.kode_masuk.push_back(kode);
    }

    int harga_awal = jumlah * tiketList[pilihan - 1].harga;
    double diskon = 0.0;

    if (jumlah >= 5)
    {
        diskon = 0.1;
        cout << GREEN << "Promo: Diskon 10% untuk pembelian 5 tiket atau lebih.\n"
             << RESET;
    }
    else if (pesanan.kategori == "VIP" && jumlah >= 3)
    {
        diskon = 0.2;
        cout << GREEN << "Promo: Diskon 20% untuk kategori VIP (min. 3 tiket).\n"
             << RESET;
    }

    cout << BLUE << "Masukkan kode referral (opsional): " << RESET;
    string kodeReferral;
    cin.ignore();
    getline(cin, kodeReferral);
    double diskonReferral = validasiKodeReferral(kodeReferral);

    if (diskonReferral > 0.0)
    {
        if (diskonReferral == 1.0)
        {
            cout << GREEN << "Kode referral berhasil digunakan. Tiket gratis!\n"
                 << RESET;
        }
        else
        {
            cout << GREEN << "Kode referral valid! Diskon tambahan " << diskonReferral * 100 << "%.\n"
                 << RESET;
        }
        diskon += diskonReferral;
        if (diskon > 1.0)
        {
            diskon = 1.0;
        }
    }
    else
    {
        cout << RED << "Kode referral tidak valid.\n"
             << RESET;
    }

    harga_awal = jumlah * tiketDipilih.harga;
    pesanan.total_harga = harga_awal * (1 - diskon);

    if (diskon == 1.0)
    {
        cout << GREEN << "Total harga setelah diskon: Gratis!\n"
             << RESET;
        pesanan.total_harga = 0;

        pesanan.kode_masuk.clear();

        srand(time(0));
        for (int i = 0; i < jumlah; i++)
        {
            int randomDigits = rand() % 9000 + 1000;
            string kode = "#ANIV" + to_string(randomDigits);
            pesanan.kode_masuk.push_back(kode);
        }

        tiketList[pilihan - 1].stok -= jumlah;
        tiketList[pilihan - 1].kuota_tiket -= jumlah;

        pesanan.no_penonton = nomorPenontonTerakhir + 1;
        nomorPenontonTerakhir += jumlah;

        pesanan.metode_pembayaran = "Referral Gratis";
        pesanan.detail_pembayaran = "Pembayaran menggunakan kode referral KHUSUS.";

        cetakTiket(pesanan);

        return pesanan;
    }
    else
    {
        cout << GREEN << "Total harga setelah diskon: Rp " << pesanan.total_harga << "\n"
             << RESET;
    }

    pair<string, string> metode_pembayaran = pilihMetodePembayaran();
    pesanan.metode_pembayaran = metode_pembayaran.first;
    pesanan.detail_pembayaran = metode_pembayaran.second;

    if (metode_pembayaran.first == "Batal")
    {
        cout << BLUE << "Kembali ke menu utama!\n"
             << RESET;
        return Pesanan{"Batal", 0, 0, "Batal", "Transaksi dibatalkan", 0, "", {}, ""};
    }

    if (pesanan.total_harga > 0)
    {
        cout << BLUE << "Masukkan uang yang dibayarkan: Rp " << RESET;
        cin >> uang_dibayar;

        if (uang_dibayar < pesanan.total_harga)
        {
            cout << RED << "Uang tidak cukup! Transaksi dibatalkan.\n"
                 << RESET;
            pesanan.kategori = "Invalid";
            return pesanan;
        }

        kembalian = uang_dibayar - pesanan.total_harga;
        cout << GREEN << "Pembayaran berhasil! Kembalian Anda: Rp " << kembalian << RESET << endl;
    }
    else
    {
        cout << GREEN << "Tiket Anda sudah dibayar penuh dengan kode referral.\n"
             << RESET;
    }

    tiketList[pilihan - 1].stok -= jumlah;
    tiketList[pilihan - 1].kuota_tiket -= jumlah;

    pesanan.no_penonton = nomorPenontonTerakhir + 1;
    nomorPenontonTerakhir += jumlah;

    cetakTiket(pesanan);
    return pesanan;
}

void tampilkanLaporan(const vector<Pesanan> &laporan)
{
    cout << BLUE << "\n=== LAPORAN PEMESANAN ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;

    for (const auto &pesanan : laporan)
    {

        if (pesanan.kategori.empty() || pesanan.kategori == "Waitlist")
        {
            continue;
        }

        cout << YELLOW << "Nama           : " << pesanan.nama << RESET << "\n"
             << MAGENTA << "Kategori       : " << pesanan.kategori << RESET << "\n"
             << BLUE << "Jumlah         : " << pesanan.jumlah << RESET << "\n"
             << GREEN << "Metode         : " << pesanan.metode_pembayaran << " (" << pesanan.detail_pembayaran << ")" << RESET << "\n"
             << RED << "Total Harga    : Rp " << pesanan.total_harga << RESET << "\n"
             << BLUE << "Waktu Pembelian: " << pesanan.waktu_pembelian << RESET << "\n"
             << "--------------------------------------\n";
    }
}

void tampilkanStatusAntrean(const string &username)
{
    bool ditemukan = false;
    cout << BLUE << "\n=== STATUS ANTREAN ANDA ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    for (const auto &antrean : daftarAntrean)
    {
        if (antrean.username == username)
        {
            string status;
            string warnaStatus;

            if (antrean.status.empty())
            {
                status = "Menunggu Konfirmasi";
                warnaStatus = YELLOW;
            }
            else if (antrean.status == "Disetujui")
            {
                status = "Disetujui";
                warnaStatus = GREEN;
            }
            else if (antrean.status == "Ditolak")
            {
                status = "Ditolak";
                warnaStatus = RED;
            }
            else
            {
                status = antrean.status;
                warnaStatus = RESET;
            }

            cout << "Kategori: " << MAGENTA << antrean.kategori << RESET
                 << ", Jumlah: " << BLUE << antrean.jumlah << RESET
                 << ", Status: " << warnaStatus << status << RESET << "\n";
            ditemukan = true;
        }
    }

    if (!ditemukan)
    {
        cout << RED << "Anda belum memiliki antrean.\n"
             << RESET;
    }
}

void cetakTiketJikaDiterima(const string &username)
{
    bool ditemukan = false;

    cout << BLUE << "\n=== CETAK TIKET ANTREAN ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;

    vector<int> antreanDisetujui;

    for (size_t i = 0; i < daftarAntrean.size(); ++i)
    {
        if (daftarAntrean[i].username == username && daftarAntrean[i].status == "Disetujui")
        {
            cout << i + 1 << ". Kategori: " << MAGENTA << daftarAntrean[i].kategori << RESET
                 << ", Jumlah: " << YELLOW << daftarAntrean[i].jumlah << RESET << "\n";
            antreanDisetujui.push_back(i);
            ditemukan = true;
        }
    }

    if (!ditemukan)
    {
        cout << RED << "\nTidak ada tiket yang disetujui untuk dicetak.\n"
             << RESET;
        return;
    }

    cout << GREEN << "\nPilih antrean yang ingin dicetak (1-" << antreanDisetujui.size() << "): " << RESET;
    int pilihan;
    cin >> pilihan;

    if (pilihan < 1 || pilihan > antreanDisetujui.size())
    {
        cout << RED << "Pilihan tidak valid!\n"
             << RESET;
        return;
    }

    Antrean &antreanDipilih = daftarAntrean[antreanDisetujui[pilihan - 1]];

    int hargaPerTiket = hargaKategori[antreanDipilih.kategori];
    int totalHarga = antreanDipilih.jumlah * hargaPerTiket;

    cout << GREEN << "\nTotal Harga: Rp " << totalHarga << RESET << "\n";

    pair<string, string> metodePembayaran = pilihMetodePembayaran();
    if (metodePembayaran.first == "Batal")
    {
        cout << RED << "Pembayaran dibatalkan.\n"
             << RESET;
        return;
    }

    cout << GREEN << "\nMetode pembayaran: " << metodePembayaran.first << RESET << "\n";
    cout << BLUE << "Detail pembayaran: " << metodePembayaran.second << RESET << "\n";

    if (!laporanPesanan.empty())
    {
        const Pesanan &tiketTerakhir = laporanPesanan.back();
        nomorPenontonTerakhir = tiketTerakhir.no_penonton + tiketTerakhir.jumlah;
    }

    Pesanan pesanan;
    pesanan.nama = username;
    pesanan.kategori = antreanDipilih.kategori;
    pesanan.jumlah = antreanDipilih.jumlah;
    pesanan.total_harga = totalHarga;
    pesanan.waktu_pembelian = getFormattedTimeWIB();
    pesanan.metode_pembayaran = metodePembayaran.first;
    pesanan.detail_pembayaran = metodePembayaran.second;

    pesanan.no_penonton = nomorPenontonTerakhir + 1;

    for (int i = 0; i < antreanDipilih.jumlah; ++i)
    {
        pesanan.kode_masuk.push_back(generateKodeMasuk());
    }

    nomorPenontonTerakhir = pesanan.no_penonton + pesanan.jumlah - 1;

    laporanPesanan.push_back(pesanan);

    cetakTiket(pesanan);

    cout << GREEN << "\nTiket berhasil dicetak untuk kategori " << antreanDipilih.kategori
         << " sebanyak " << antreanDipilih.jumlah << " tiket.\n"
         << RESET;

    daftarAntrean.erase(daftarAntrean.begin() + antreanDisetujui[pilihan - 1]);
}

void tampilkanAntrean()
{
    cout << BLUE << "\n=== DAFTAR ANTREAN ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;

    if (daftarAntrean.empty())
    {
        cout << RED << "Tidak ada antrean saat ini.\n"
             << RESET;
        return;
    }

    for (size_t i = 0; i < daftarAntrean.size(); ++i)
    {
        const auto &antrean = daftarAntrean[i];

        string warnaStatus;
        string status = antrean.status.empty() ? "Menunggu Konfirmasi" : antrean.status;

        if (status == "Menunggu Konfirmasi")
        {
            warnaStatus = YELLOW;
        }
        else if (status == "Disetujui")
        {
            warnaStatus = GREEN;
        }
        else if (status == "Ditolak")
        {
            warnaStatus = RED;
        }
        else
        {
            warnaStatus = RESET;
        }

        cout << i + 1 << ". Username: " << BLUE << antrean.username << RESET
             << ", Kategori: " << MAGENTA << antrean.kategori << RESET
             << ", Jumlah: " << YELLOW << antrean.jumlah << RESET
             << ", Status: " << warnaStatus << status << RESET << "\n";
    }
}

void prosesAntrean()
{
    int pilihan;
    tampilkanAntrean();

    cout << BLUE << "\n=== ANTREAN CONFIRM ADMIN ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;

    if (daftarAntrean.empty())
    {
        cout << RED << "Tidak ada antrean untuk diproses.\n"
             << RESET;
        return;
    }

    cout << BLUE << "Pilih antrean yang akan diproses (1-" << daftarAntrean.size() << "): " << RESET;
    cin >> pilihan;

    if (pilihan < 1 || pilihan > daftarAntrean.size())
    {
        cout << RED << "Pilihan tidak valid.\n"
             << RESET;
        return;
    }

    Antrean &antreanDipilih = daftarAntrean[pilihan - 1];

    cout << YELLOW << "Proses antrean untuk " << antreanDipilih.username << ":\n"
         << RESET;
    cout << GREEN << "1. Terima\n"
         << RESET;
    cout << RED << "2. Tolak\n"
         << RESET;
    cout << YELLOW << "3. Tetap Menunggu\n"
         << RESET;
    cout << BLUE << "Pilihan: " << RESET;
    int aksi;
    cin >> aksi;

    switch (aksi)
    {
    case 1:
        antreanDipilih.status = "Disetujui";
        cout << GREEN << "Antrean telah disetujui.\n"
             << RESET;
        break;
    case 2:
        antreanDipilih.status = "Ditolak";
        cout << RED << "Antrean telah ditolak.\n"
             << RESET;
        break;
    case 3:
        cout << YELLOW << "Antrean tetap menunggu.\n"
             << RESET;
        break;
    default:
        cout << RED << "Pilihan tidak valid.\n"
             << RESET;
    }
}

void menuProsesAntreanAdmin()
{
    int pilihan;
    do
    {
        cout << BLUE << "\n=== MENU PROSES ANTREAN ===\n"
             << RESET;
        cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
             << RESET;
        cout << YELLOW << "1. Tampilkan Daftar Antrean\n"
             << RESET;
        cout << YELLOW << "2. Proses Antrean\n"
             << RESET;
        cout << YELLOW << "3. Kembali ke Menu Utama\n"
             << RESET;
        cout << GREEN << "Pilih opsi: " << RESET;
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            tampilkanAntrean();
            break;
        case 2:
            prosesAntrean();
            break;
        case 3:
            cout << GREEN << "Kembali ke menu utama.\n"
                 << RESET;
            break;
        default:
            cout << RED << "Pilihan tidak valid. Silakan coba lagi.\n"
                 << RESET;
        }
    } while (pilihan != 3);
}

void menuStatusAntreanUser(const string &username)
{
    int pilihan;
    do
    {
        cout << BLUE << "\n=== MENU STATUS ANTREAN ===\n"
             << RESET;
        cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
             << RESET;
        cout << YELLOW << "1. Tampilkan Status Antrean\n"
             << RESET;
        cout << YELLOW << "2. Cetak Tiket Jika Diterima\n"
             << RESET;
        cout << YELLOW << "3. Kembali ke Menu Utama\n"
             << RESET;
        cout << GREEN << "Pilih opsi: " << RESET;
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            tampilkanStatusAntrean(username);
            break;
        case 2:
            cetakTiketJikaDiterima(username);
            break;
        case 3:
            cout << GREEN << "Kembali ke menu utama.\n"
                 << RESET;
            break;
        default:
            cout << RED << "Pilihan tidak valid. Silakan coba lagi.\n"
                 << RESET;
        }
    } while (pilihan != 3);
}

void batalkanTiket(vector<Pesanan> &laporan, vector<Tiket> &tiketList)
{
    string nama, kategori;
    int nomor_penonton;
    bool ditemukan = false;

    cout << BLUE << "\n=== BATALKAN TIKET ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    cout << "Masukkan nama pemesan   : ";
    cin.ignore();
    getline(cin, nama);
    cout << "Masukkan kategori tiket : ";
    getline(cin, kategori);
    cout << "Masukkan nomor penonton : ";
    cin >> nomor_penonton;

    for (auto it = laporan.begin(); it != laporan.end(); ++it)
    {
        if (it->nama == nama && it->kategori == kategori &&
            nomor_penonton >= it->no_penonton &&
            nomor_penonton < it->no_penonton + it->jumlah)
        {
            int index = nomor_penonton - it->no_penonton;

            for (auto &tiket : tiketList)
            {
                if (tiket.kategori == kategori)
                {
                    tiket.stok++;
                    tiket.kuota_tiket++;
                    break;
                }
            }

            it->kode_masuk.erase(it->kode_masuk.begin() + index);
            it->jumlah--;

            if (it->jumlah == 0)
            {
                laporan.erase(it);
            }

            cout << GREEN << "Tiket berhasil dibatalkan.\n"
                 << RESET;
            ditemukan = true;
            break;
        }
    }

    if (!ditemukan)
    {
        cout << RED << "Tiket tidak ditemukan! Periksa kembali data Anda.\n"
             << RESET;
    }
}

void tiketMasuk(const vector<Pesanan> &laporan)
{
    int nomor_penonton;
    string nama, kategori, kode_masuk;
    bool tiket_valid = false;

    cout << BLUE << "\n=== VALIDASI TIKET MASUK ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;

    cout << "Masukkan nomor penonton: ";
    cin >> nomor_penonton;
    cin.ignore();
    cout << "Masukkan nama pemesan: ";
    getline(cin, nama);
    cout << "Masukkan kategori tiket (VIP, Gold, Silver, Bronze): ";
    getline(cin, kategori);
    cout << "Masukkan kode masuk tiket: ";
    getline(cin, kode_masuk);

    for (const auto &pesanan : laporan)
    {

        if (pesanan.nama == nama && pesanan.kategori == kategori)
        {

            for (size_t i = 0; i < pesanan.kode_masuk.size(); ++i)
            {
                int nomor_terhitung = pesanan.no_penonton + i;
                if (nomor_terhitung == nomor_penonton && pesanan.kode_masuk[i] == kode_masuk)
                {
                    tiket_valid = true;
                    break;
                }
            }
            if (tiket_valid)
                break;
        }
    }

    if (tiket_valid)
    {
        cout << GREEN << "\nTiket valid! Selamat menikmati konser.\n"
             << RESET;
    }
    else
    {
        cout << RED << "\nTiket tidak valid! Silakan periksa kembali informasi tiket Anda.\n"
             << RESET;
    }
}

void tampilkanStokTiket(const vector<Tiket> &tiketList)
{
    cout << BLUE << "\n=== STOK TIKET ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    for (const auto &tiket : tiketList)
    {
        string warnaKategori = RESET;

        if (tiket.kategori == "VIP")
        {
            warnaKategori = MAGENTA;
        }
        else if (tiket.kategori == "Gold")
        {
            warnaKategori = GOLD;
        }
        else if (tiket.kategori == "Silver")
        {
            warnaKategori = SILVER;
        }
        else if (tiket.kategori == "Bronze")
        {
            warnaKategori = BROWN;
        }

        cout << BLUE << "Kategori : " << warnaKategori << tiket.kategori << RESET << "\n"
             << YELLOW << "Stok     : " << tiket.stok << RESET << "\n"
             << "--------------------------------------\n";
    }
}

void tambahStokTiket(vector<Tiket> &tiketList)
{
    int pilihan, tambah;
    cout << BLUE << "\n===  TAMBAH STOK TIKET ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    cout << "\nPilih kategori tiket yang ingin ditambah stoknya:\n";

    for (int i = 0; i < tiketList.size(); i++)
    {
        string warnaKategori = RESET;

        if (tiketList[i].kategori == "VIP")
        {
            warnaKategori = MAGENTA;
        }
        else if (tiketList[i].kategori == "Gold")
        {
            warnaKategori = GOLD;
        }
        else if (tiketList[i].kategori == "Silver")
        {
            warnaKategori = SILVER;
        }
        else if (tiketList[i].kategori == "Bronze")
        {
            warnaKategori = BROWN;
        }

        cout << i + 1 << ". " << warnaKategori << tiketList[i].kategori << RESET << "\n";
    }

    cout << BLUE << "Pilihan: " << RESET;
    cin >> pilihan;

    if (pilihan < 1 || pilihan > tiketList.size())
    {
        cout << RED << "Kategori tidak valid!\n"
             << RESET;
        return;
    }

    cout << BLUE << "Masukkan jumlah stok yang ingin ditambah: " << RESET;
    cin >> tambah;

    tiketList[pilihan - 1].stok += tambah;
    tiketList[pilihan - 1].kuota_tiket += tambah;

    string warnaKategoriDipilih = RESET;
    if (tiketList[pilihan - 1].kategori == "VIP")
    {
        warnaKategoriDipilih = MAGENTA;
    }
    else if (tiketList[pilihan - 1].kategori == "Gold")
    {
        warnaKategoriDipilih = GOLD;
    }
    else if (tiketList[pilihan - 1].kategori == "Silver")
    {
        warnaKategoriDipilih = SILVER;
    }
    else if (tiketList[pilihan - 1].kategori == "Bronze")
    {
        warnaKategoriDipilih = BROWN;
    }

    cout << BLUE << "Masukkan jumlah stok yang ingin ditambah: " << RESET;
    cin >> tambah;

    tiketList[pilihan - 1].stok += tambah;
    tiketList[pilihan - 1].kuota_tiket += tambah;

    cout << GREEN << "Stok tiket untuk kategori " << warnaKategoriDipilih << tiketList[pilihan - 1].kategori << RESET
         << GREEN << " berhasil ditambah!\n"
         << RESET;
}

void hapusStokTiket(vector<Tiket> &tiketList)
{
    int pilihan, hapusJumlah;
    cout << BLUE << "\n=== HAPUS STOK TIKET ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;

    for (int i = 0; i < tiketList.size(); i++)
    {
        string warnaKategori = RESET;

        if (tiketList[i].kategori == "VIP")
        {
            warnaKategori = MAGENTA;
        }
        else if (tiketList[i].kategori == "Gold")
        {
            warnaKategori = GOLD;
        }
        else if (tiketList[i].kategori == "Silver")
        {
            warnaKategori = SILVER;
        }
        else if (tiketList[i].kategori == "Bronze")
        {
            warnaKategori = BROWN;
        }

        cout << i + 1 << ". " << warnaKategori << tiketList[i].kategori << RESET << "\n";
        cout << "   Stok saat ini: " << tiketList[i].stok << " tiket\n";
    }

    cout << BLUE << "Pilih kategori tiket yang ingin dihapus stoknya (1-" << tiketList.size() << "): " << RESET;
    cin >> pilihan;

    if (pilihan < 1 || pilihan > tiketList.size())
    {
        cout << RED << "Kategori tidak valid!\n"
             << RESET;
        return;
    }

    cout << BLUE << "Masukkan jumlah tiket yang ingin dihapus: " << RESET;
    cin >> hapusJumlah;

    if (hapusJumlah > tiketList[pilihan - 1].stok)
    {
        cout << RED << "Jumlah yang dihapus melebihi stok yang tersedia!\n"
             << RESET;
        return;
    }

    tiketList[pilihan - 1].stok -= hapusJumlah;

    string warnaKategoriDipilih = RESET;
    if (tiketList[pilihan - 1].kategori == "VIP")
    {
        warnaKategoriDipilih = MAGENTA;
    }
    else if (tiketList[pilihan - 1].kategori == "Gold")
    {
        warnaKategoriDipilih = GOLD;
    }
    else if (tiketList[pilihan - 1].kategori == "Silver")
    {
        warnaKategoriDipilih = SILVER;
    }
    else if (tiketList[pilihan - 1].kategori == "Bronze")
    {
        warnaKategoriDipilih = BROWN;
    }

    cout << GREEN << "Stok tiket untuk kategori " << warnaKategoriDipilih << tiketList[pilihan - 1].kategori << RESET
         << GREEN << " berhasil dihapus!\n"
         << RESET;
}

void tambahUlasan(vector<Ulasan> &listUlasan, const string &username)
{
    Ulasan ulasan;
    cout << BLUE << "\n=== TAMBAH ULASAN ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    cout << "Kategori tiket (VIP, Gold, Silver, Bronze): ";
    cin.ignore();
    getline(cin, ulasan.kategori);
    cout << "Komentar: ";
    getline(cin, ulasan.komentar);
    cout << "Penilaian (1-5): ";
    cin >> ulasan.penilaian;

    if (ulasan.penilaian < 1 || ulasan.penilaian > 5)
    {
        cout << RED << "Penilaian tidak valid! Harus dalam rentang 1-5.\n"
             << RESET;
        return;
    }

    ulasan.nama = username;
    listUlasan.push_back(ulasan);
    cout << GREEN << "Ulasan berhasil ditambahkan!\n"
         << RESET;
}

void tampilkanUlasan(const vector<Ulasan> &listUlasan)
{
    cout << BLUE << "\n=== ULASAN PENGGUNA ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    if (listUlasan.empty())
    {
        cout << RED << "Belum ada ulasan.\n"
             << RESET;
        return;
    }

    for (const auto &ulasan : listUlasan)
    {
        cout << YELLOW << "Kategori  : " << ulasan.kategori << RESET << "\n"
             << BROWN << "Nama      : " << ulasan.nama << RESET << "\n"
             << MAGENTA << "Komentar  : " << ulasan.komentar << RESET << "\n"
             << GREEN << "Penilaian : " << ulasan.penilaian << "/5" << RESET << "\n"
             << "--------------------------------------\n";
    }
}

void tampilkanStatistik(const vector<Pesanan> &laporan, const vector<Tiket> &tiketList)
{
    int totalTiketTerjual = 0, totalPendapatan = 0;
    map<string, int> kategoriTiketTerjual;

    for (const auto &pesanan : laporan)
    {
        if (pesanan.kategori != "Waitlist")
        {
            totalTiketTerjual += pesanan.jumlah;
            totalPendapatan += pesanan.total_harga;
            kategoriTiketTerjual[pesanan.kategori] += pesanan.jumlah;
        }
    }

    cout << BLUE << "\n=== STATISTIK TIKET ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;

    if (laporan.empty())
    {
        cout << RED << "Belum ada pemesanan tiket.\n"
             << RESET;
        return;
    }

    cout << GREEN << "Total Tiket Terjual : " << totalTiketTerjual << RESET << "\n";
    cout << GREEN << "Total Pendapatan    : Rp " << totalPendapatan << RESET << "\n";

    cout << BLUE << "\nTiket Terjual per Kategori:\n"
         << RESET;
    for (const auto &tiket : tiketList)
    {
        string warnaKategori = RESET;

        if (tiket.kategori == "VIP")
        {
            warnaKategori = MAGENTA;
        }
        else if (tiket.kategori == "Gold")
        {
            warnaKategori = GOLD;
        }
        else if (tiket.kategori == "Silver")
        {
            warnaKategori = SILVER;
        }
        else if (tiket.kategori == "Bronze")
        {
            warnaKategori = BROWN;
        }

        cout << warnaKategori << tiket.kategori << RESET
             << ": " << kategoriTiketTerjual[tiket.kategori] << " tiket terjual\n";
    }

    auto maxKategori = max_element(kategoriTiketTerjual.begin(), kategoriTiketTerjual.end(),
                                   [](const auto &a, const auto &b)
                                   { return a.second < b.second; });
    if (maxKategori != kategoriTiketTerjual.end())
    {
        cout << YELLOW << "\nKategori Terpopuler : " << maxKategori->first
             << " (" << maxKategori->second << " tiket terjual)\n"
             << RESET;
    }

    cout << "--------------------------------------\n";
}

void chatbot()
{
    map<string, string> faq = {
        {"kapan acara?", "Acara akan berlangsung pada 17 Januari 2025."},
        {"dimana acara?", "Acara akan berlangsung di Telkom University Convention Hall."},
        {"berapa harga tiket?", "Harga tiket mulai dari Rp 250.000 hingga Rp 1.000.000."},
        {"siapa guest star?", "Guest star untuk acara ini adalah Noah dan Dewa19."},
        {"apa fasilitas vip?", "Kategori VIP mencakup tempat duduk eksklusif, snack, dan merchandise."},
        {"apakah tiket masih tersedia?", "Silakan cek stok tiket pada menu utama untuk informasi terbaru."},
    };

    cout << BLUE << "\n=== CHATBOT (SERINA) ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    cout << YELLOW << "Tanyakan sesuatu (contoh: 'kapan acara?', 'dimana acara?', 'siapa guest star?'," << endl
         << "'apa fasilitas vip?', 'berapa harga tiket?', 'apakah tiket masih tersedia?')\n"
         << RESET;
    cout << "Ketik 'keluar' untuk kembali ke menu utama.\n";

    cin.ignore();
    string pertanyaan;
    do
    {
        cout << GREEN << "\nAnda: " << RESET;
        getline(cin, pertanyaan);

        if (pertanyaan == "keluar")
        {
            cout << BLUE << "Chatbot ditutup. Kembali ke menu utama.\n"
                 << RESET;
            break;
        }

        auto jawaban = faq.find(pertanyaan);
        if (jawaban != faq.end())
        {
            cout << MAGENTA << "Bot: " << jawaban->second << RESET << "\n";
        }
        else
        {
            cout << RED << "Bot: Maaf, saya tidak mengerti pertanyaan Anda.\n"
                 << RESET;
        }
    } while (true);
}

void cariTiket(const vector<Pesanan> &laporanPesanan)
{
    string nama, kodeMasuk;
    bool tiketDitemukan = false;

    cout << BLUE << "\n=== PENCARIAN TIKET ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;
    cout << "Masukkan Nama Pemesan: ";
    cin.ignore();
    getline(cin, nama);

    cout << "Masukkan Kode Masuk Tiket: ";
    getline(cin, kodeMasuk);

    for (const auto &pesanan : laporanPesanan)
    {
        for (size_t i = 0; i < pesanan.kode_masuk.size(); ++i)
        {

            if (pesanan.nama == nama && pesanan.kode_masuk[i] == kodeMasuk)
            {
                cout << GREEN << "\nTiket Ditemukan!\n"
                     << RESET;
                cout << BLUE << "Nomor Penonton : " << pesanan.no_penonton + i << RESET << "\n";
                cout << BLUE << "Nama           : " << pesanan.nama << RESET << "\n";
                cout << BLUE << "Kategori       : " << pesanan.kategori << RESET << "\n";
                cout << GREEN << "Pembayaran     : Berhasil\n"
                     << RESET;
                cout << MAGENTA << "Kode Masuk     : " << pesanan.kode_masuk[i] << RESET << endl;
                tiketDitemukan = true;
                break;
            }
        }
    }

    if (!tiketDitemukan)
    {
        cout << RED << "Tiket tidak ditemukan! Pastikan nama dan kode masuk benar.\n"
             << RESET;
    }
}

void exportToCSV(const vector<Pesanan> &laporan)
{
    ofstream file("laporan_pemesanan.csv", ios::app);
    if (!file.is_open())
    {
        cout << RED << "Gagal membuka file untuk ekspor!\n"
             << RESET;
        return;
    }

    if (file.tellp() == 0)
    {
        file << "Nama,Kategori,Jumlah,Metode Pembayaran,Detail Pembayaran,Total Harga,Waktu Pembelian\n";
    }

    for (const auto &pesanan : laporan)
    {
        if (pesanan.kategori != "Waitlist")
        {
            file << pesanan.nama << "," << pesanan.kategori << "," << pesanan.jumlah << ","
                 << pesanan.metode_pembayaran << "," << pesanan.detail_pembayaran << ","
                 << pesanan.total_harga << "," << pesanan.waktu_pembelian << "\n";
        }
    }

    file.close();
    cout << GREEN << "Laporan berhasil diekspor ke 'laporan_pemesanan.csv'.\n"
         << RESET;
}

void logAktivitas(const string &username, const string &aktivitas, bool isAdmin)
{
    ofstream logFile("log_aktivitas.txt", ios::app);
    if (logFile.is_open())
    {
        time_t sekarang = time(0);
        string waktu = ctime(&sekarang);
        waktu.pop_back();

        string role = isAdmin ? "Admin" : (userRoles[username] == "user" ? "User" : "Guest");

        logFile << "[" << waktu << "] " << role << ": " << username << " | Aktivitas: " << aktivitas << endl;
        logFile.close();
    }
    else
    {
        cout << RED << "Gagal membuka file log!\n"
             << RESET;
    }
}

void tampilkanLogAktivitas()
{
    ifstream logFile("log_aktivitas.txt");
    if (logFile.is_open())
    {
        string line;
        while (getline(logFile, line))
        {
            cout << line << endl;
        }
        logFile.close();
    }
    else
    {
        cout << RED << "Gagal membuka file log!\n"
             << RESET;
    }
}

void tampilkanMenuBantuan()
{
    cout << BLUE << "\n=== MENU BANTUAN ===\n"
         << RESET;
    cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung\n"
         << RESET;

    cout << YELLOW << "1. Informasi Konser:\n"
         << RESET;
    cout << "   Menampilkan detail lengkap tentang konser, termasuk tanggal pelaksanaan, waktu dimulai, lokasi acara, dan kategori tiket yang tersedia.\n";

    cout << YELLOW << "2. Countdown Waktu:\n"
         << RESET;
    cout << "   Menampilkan hitung mundur waktu secara real-time menuju dimulainya konser.\n";

    cout << YELLOW << "3. Pembelian Tiket:\n"
         << RESET;
    cout << "   Memungkinkan pengguna membeli tiket berdasarkan kategori yang diinginkan dengan langkah-langkah yang mudah.\n";

    cout << YELLOW << "4. Laporan Pemesanan:\n"
         << RESET;
    cout << "   Menampilkan ringkasan tiket yang sudah dipesan oleh pengguna, termasuk informasi tentang jumlah dan detail pemesanan.\n";

    cout << YELLOW << "5. Stok Tiket:\n"
         << RESET;
    cout << "   Menunjukkan ketersediaan stok tiket dalam setiap kategori sehingga pengguna dapat memantau sebelum membeli.\n";

    cout << YELLOW << "6. Lihat Tiket:\n"
         << RESET;
    cout << "   Memberikan akses kepada pengguna untuk melihat detail tiket yang telah berhasil dipesan.\n";

    cout << YELLOW << "7. Pencarian Tiket:\n"
         << RESET;
    cout << "   Memfasilitasi pencarian tiket menggunakan nama pemesan dan kode unik untuk mempermudah verifikasi.\n";

    cout << YELLOW << "8. Validasi Tiket:\n"
         << RESET;
    cout << "   Memastikan tiket yang dibawa oleh pengguna sah untuk digunakan pada saat masuk ke konser.\n";

    cout << YELLOW << "9. Pembatalan Tiket:\n"
         << RESET;
    cout << "   Memungkinkan pengguna membatalkan tiket yang sudah dipesan dengan syarat dan ketentuan yang berlaku.\n";

    cout << YELLOW << "10. Status Antrian:\n"
         << RESET;
    cout << "   Menampilkan antrean ketika stok habis dan memberikan status konfirmasi antrean.\n";

    cout << YELLOW << "11. Tambah Ulasan:\n"
         << RESET;
    cout << "   Memberikan fitur bagi pengguna untuk memberikan ulasan dan penilaian terhadap konser yang telah berlangsung.\n";

    cout << YELLOW << "12. Chatbot:\n"
         << RESET;
    cout << "   Menyediakan layanan tanya jawab informasi terkait konser melalui chatbot yang responsif.\n";

    cout << YELLOW << "13. Menu Bantuan:\n"
         << RESET;
    cout << "   Menampilkan informasi lengkap mengenai menu dan fungsi yang tersedia dalam aplikasi.\n";

    cout << YELLOW << "14. Pindah Account:\n"
         << RESET;
    cout << "   Memungkinkan pengguna berpindah akun tanpa harus keluar dari aplikasi terlebih dahulu.\n";

    cout << YELLOW << "15. Logout:\n"
         << RESET;
    cout << "   Mengakhiri sesi pengguna dan keluar dari aplikasi dengan aman.\n";

    cout << "--------------------------------------\n";
}

void resetProgram(vector<Tiket> &tiketList, vector<Pesanan> &laporanPesanan, vector<Ulasan> &listUlasan, vector<Antrean> &daftarAntrean, int &nomorPenontonTerakhir)
{

    tiketList = {
        {"VIP", 1000000, "Snack, Merchandise, Panggung Dekat", 50, 50},
        {"Gold", 750000, "Snack, Panggung Dekat", 100, 100},
        {"Silver", 500000, "Panggung Jauh, Pemandangan Bagus", 200, 200},
        {"Bronze", 250000, "Berdiri, Harga Terjangkau", 500, 500}};

    laporanPesanan.clear();

    listUlasan.clear();

    daftarAntrean.clear();

    nomorPenontonTerakhir = 0;

    cout << GREEN << "Program berhasil direset ke kondisi awal!\n"
         << RESET;
}

void tampilkanMenuAdmin()
{
    cout << GREEN << "\n=== SELAMAT DATANG ADMIN! ANDA MEMPUNYAI AKSES DATA ===\n"
         << RESET;
    cout << "1.  Statistik Pemesanan\n";
    cout << "2.  Laporan Pemesanan\n";
    cout << "3.  Stok Tiket\n";
    cout << "4.  Tambah Stok Tiket\n";
    cout << "5.  Hapus Stok Tike\n";
    cout << "6.  Antrian Tiket\n";
    cout << "7.  Lihat Ulasan\n";
    cout << "8.  Ekspor Laporan ke CSV\n";
    cout << "9.  Log Aktivitas\n";
    cout << "10. Reset Program\n";
    cout << "11. Pindah Account\n";
    cout << "12. Logout\n";
}

void tampilkanMenuUser()
{
    cout << GREEN << "\n=== SELAMAT DATANG USER! ===\n"
         << RESET;
    cout << "1.  Informasi Konser\n";
    cout << "2.  Countdown Waktu\n";
    cout << "3.  Pembelian Tiket\n";
    cout << "4.  Laporan Pemesanan\n";
    cout << "5.  Stok Tiket\n";
    cout << "6.  Lihat Tiket\n";
    cout << "7.  Pencarian Tiket\n";
    cout << "8.  Validasi Tiket\n";
    cout << "9.  Pembatalan Tiket\n";
    cout << "10. Status Atrean\n";
    cout << "11. Tambah Ulasan\n";
    cout << "12. Chatbot\n";
    cout << "13. Menu Bantuan\n";
    cout << "14. Pindah Account\n";
    cout << "15. Logout\n";
}

int main()
{
    vector<Tiket> tiketList = {
        {"VIP", 1000000, "Snack, Merchandise, Panggung Dekat", 50, 50},
        {"Gold", 750000, "Snack, Panggung Dekat", 100, 100},
        {"Silver", 500000, "Panggung Jauh, Pemandangan Bagus", 200, 200},
        {"Bronze", 250000, "Berdiri, Harga Terjangkau", 500, 500}};

    int pilihan;
    string username;
    bool isAdmin = false;
    bool keluar = false;

    if (loginSSO(username))
    {
        logAktivitas(username, "Login berhasil", isAdmin);

        if (userRoles[username] == "admin")
        {
            isAdmin = true;
        }

        while (!keluar)
        {
            cout << BLUE << "\n=== MENU UTAMA ===\n"
                 << RESET;
            cout << RED << "Digital Tikets for Anniversary Night Telkom University Bandung" << RESET;

            if (isAdmin)
            {
                tampilkanMenuAdmin();
            }
            else
            {
                tampilkanMenuUser();
            }

            cout << GREEN << "Pilihan: " << RESET;
            if (!(cin >> pilihan))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << RED << "Input tidak valid! Harap masukkan angka.\n"
                     << RESET;
                continue;
            }

            switch (pilihan)
            {
            case 1:
                if (isAdmin)
                {
                    tampilkanStatistik(laporanPesanan, tiketList);
                    logAktivitas(username, "Melihat statistik pemesanan", isAdmin);
                }
                else
                {
                    informasiKonser(tiketList);
                    logAktivitas(username, "Melihat informasi konser", isAdmin);
                }
                break;
            case 2:
                if (isAdmin)
                {
                    tampilkanLaporan(laporanPesanan);
                    logAktivitas(username, "Melihat laporan pemesanan", isAdmin);
                }
                else
                {
                    hitungCountdown();
                    logAktivitas(username, "Melihat countdown waktu acara", isAdmin);
                }
                break;
            case 3:
                if (isAdmin)
                {
                    tampilkanStokTiket(tiketList);
                    logAktivitas(username, "Melihat stok tiket", isAdmin);
                }
                else
                {
                    Pesanan pesanan = prosesPesanan(tiketList, nomorPenontonTerakhir, username);
                    if (pesanan.kategori != "Invalid")
                    {
                        laporanPesanan.push_back(pesanan);
                        logAktivitas(username, "Membeli tiket untuk kategori: " + pesanan.kategori, isAdmin);
                    }
                }
                break;
            case 4:
                if (isAdmin)
                {
                    tambahStokTiket(tiketList);
                    logAktivitas(username, "Menambah stok tiket", isAdmin);
                }
                else
                {
                    tampilkanLaporan(laporanPesanan);
                    logAktivitas(username, "Melihat laporan pemesanan", isAdmin);
                }
                break;
            case 5:
                if (isAdmin)
                {
                    hapusStokTiket(tiketList);
                    logAktivitas(username, "Menghapus stok tiket", isAdmin);
                }
                else
                {
                    tampilkanStokTiket(tiketList);
                    logAktivitas(username, "Melihat stok tiket", isAdmin);
                }
                break;
            case 6:
                if (isAdmin)
                {
                    menuProsesAntreanAdmin();
                    logAktivitas(username, "Memverifikasi proses antrean", isAdmin);
                }
                else
                {
                    if (!laporanPesanan.empty())
                    {
                        for (auto &pesanan : laporanPesanan)
                        {
                            cetakTiket(pesanan);
                            logAktivitas(username, "Mencetak tiket untuk kategori: " + pesanan.kategori, isAdmin);
                        }
                    }
                    else
                    {
                        cout << RED << "Tidak ada tiket yang dapat dicetak.\n"
                             << RESET;
                        logAktivitas(username, "Gagal mencetak tiket", isAdmin);
                    }
                }
                break;
            case 7:
                if (isAdmin)
                {
                    tampilkanUlasan(listUlasan);
                    logAktivitas(username, "Melihat ulasan pengguna", isAdmin);
                }
                else
                {
                    cariTiket(laporanPesanan);
                    logAktivitas(username, "Mencari tiket pada yang telah dicetak", isAdmin);
                }
                break;
            case 8:
                if (isAdmin)
                {
                    exportToCSV(laporanPesanan);
                    logAktivitas(username, "Mengekspor laporan pemesanan ke CSV", isAdmin);
                }
                else
                {
                    tiketMasuk(laporanPesanan);
                    logAktivitas(username, "Validasi tiket masuk konser", isAdmin);
                }
                break;
            case 9:
                if (isAdmin)
                {
                    tampilkanLogAktivitas();
                    logAktivitas(username, "Melihat log aktivitas", isAdmin);
                }
                else
                {
                    batalkanTiket(laporanPesanan, tiketList);
                    logAktivitas(username, "Membatalkan tiket", isAdmin);
                }
                break;
            case 10:
                if (isAdmin)
                {
                    resetProgram(tiketList, laporanPesanan, listUlasan, daftarAntrean, nomorPenontonTerakhir);
                    logAktivitas(username, "Mereset program ke kondisi awal", isAdmin);
                }
                else
                {
                    menuStatusAntreanUser(username);
                    break;
                    logAktivitas(username, "Melihat status antrean", isAdmin);
                }
                break;
            case 11:
                if (isAdmin)
                {

                    cout << BLUE << "Anda memilih untuk berpindah akun. Silakan login kembali.\n"
                         << RESET;
                    logAktivitas(username, "Memilih pindah akun", isAdmin);

                    username.clear();
                    isAdmin = false;

                    if (loginSSO(username))
                    {
                        logAktivitas(username, "Login berhasil setelah pindah akun", isAdmin);

                        if (userRoles[username] == "admin")
                        {
                            isAdmin = true;
                        }
                    }
                    else
                    {
                        cout << RED << "Gagal login setelah pindah akun. Program akan keluar.\n"
                             << RESET;
                        pilihan = isAdmin ? 12 : 15;
                        break;
                    }

                    continue;
                }
                else
                {
                    tambahUlasan(listUlasan, username);
                    logAktivitas(username, "Menambahkan ulasan", isAdmin);
                }
                break;
            case 12:
                if (isAdmin)
                {

                    if (konfirmasiLogout())
                    {
                        keluar = true;
                        logAktivitas(username, "Logout User berhasil", isAdmin);
                    }
                    break;
                }
                else
                {
                    chatbot();
                    logAktivitas(username, "Menggunakan chatbot serina", isAdmin);
                }
                break;
            case 13:
                if (isAdmin)
                {
                    cout << YELLOW << "Pilihan tidak valid! Silakan coba lagi.\n"
                         << RESET;
                    logAktivitas(username, "Memasukkan pilihan tidak valid", isAdmin);
                }
                else
                {
                    tampilkanMenuBantuan();
                    logAktivitas(username, "Melihat menu panduan", isAdmin);
                }
                break;
            case 14:
                if (isAdmin)
                {
                    cout << YELLOW << "Pilihan tidak valid! Silakan coba lagi.\n"
                         << RESET;
                    logAktivitas(username, "Memasukkan pilihan tidak valid", isAdmin);
                }
                else
                {

                    cout << BLUE << "Anda memilih untuk berpindah akun. Silakan login kembali.\n"
                         << RESET;
                    logAktivitas(username, "Memilih pindah akun", isAdmin);

                    username.clear();
                    isAdmin = false;

                    if (loginSSO(username))
                    {
                        logAktivitas(username, "Login berhasil setelah pindah akun", isAdmin);

                        if (userRoles[username] == "admin")
                        {
                            isAdmin = true;
                        }
                    }
                    else
                    {
                        cout << RED << "Gagal login setelah pindah akun. Program akan keluar.\n"
                             << RESET;
                        pilihan = isAdmin ? 12 : 15;
                        break;
                    }

                    continue;
                }
                break;
            case 15:
                if (isAdmin)
                {
                    cout << YELLOW << "Pilihan tidak valid! Silakan coba lagi.\n"
                         << RESET;
                    logAktivitas(username, "Memasukkan pilihan tidak valid", isAdmin);
                }
                else
                {

                    if (konfirmasiLogout())
                    {
                        keluar = true;
                        logAktivitas(username, "Logout User berhasil", isAdmin);
                    }
                }
                break;
            default:
                cout << RED << "Pilihan tidak valid! Silakan coba lagi.\n"
                     << RESET;
                logAktivitas(username, "Memasukkan pilihan tidak valid", isAdmin);
                break;

                if ((isAdmin && pilihan == 12) || (!isAdmin && pilihan == 15))
                {
                    break;
                }
            }
        }

        cout << GREEN << "\nProgram selesai. Terima kasih!\n"
             << RESET;
        return 0;
    }
}