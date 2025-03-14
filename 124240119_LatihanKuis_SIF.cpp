#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

struct Mahasiswa {
    char nama[50];
    char nim[15];
    char jurusan[50];
    int tahunMasuk;
    float ipk;
};

void tambahMahasiswa() {
    system("cls");
    Mahasiswa mhs;
    FILE *file = fopen("MahasiswaData.dat", "ab");
    if (!file) {
        cout << "Gagal membuka file!" << endl;
        return;
    }
    
    cout << "Masukkan Nama: "; cin.ignore(); cin.getline(mhs.nama, 50);
    cout << "Masukkan NIM: "; cin >> mhs.nim;
    cout << "Masukkan Jurusan: "; cin.ignore(); cin.getline(mhs.jurusan, 50);
    cout << "Masukkan Tahun Masuk: "; cin >> mhs.tahunMasuk;
    mhs.ipk = 0.0;
    
    fwrite(&mhs, sizeof(Mahasiswa), 1, file);
    fclose(file);
    cout << "Mahasiswa berhasil ditambahkan!" << endl;
    system("pause");
}

void quickSort(Mahasiswa arr[], int low, int high) {
    if (low < high) {
        Mahasiswa pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (strcmp(arr[j].nim, pivot.nim) < 0) {
                i++;
                Mahasiswa temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        Mahasiswa temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        int pi = i + 1;
        
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void tampilkanMahasiswa() {
    system("cls");
    FILE *file = fopen("MahasiswaData.dat", "rb");
    if (!file) {
        cout << "Gagal membuka file!" << endl;
        return;
    }
    
    Mahasiswa data[100];
    int count = 0;
    while (fread(&data[count], sizeof(Mahasiswa), 1, file)) {
        count++;
    }
    fclose(file);
    
    quickSort(data, 0, count - 1);
    for (int i = 0; i < count; i++) {
        cout << "Nama: " << data[i].nama << "\nNIM: " << data[i].nim << "\nJurusan: " << data[i].jurusan
             << "\nTahun Masuk: " << data[i].tahunMasuk << "\nIPK: " << data[i].ipk << "\n";
    }
    system("pause");
}

int binarySearch(Mahasiswa arr[], int left, int right, const char* nim) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(arr[mid].nim, nim);
        if (cmp == 0) return mid;
        else if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void cariMahasiswaJurusan() {
    system("cls");
    char jurusanCari[50];
    cout << "Masukkan jurusan yang dicari: ";
    cin.ignore();
    cin.getline(jurusanCari, 50);

    FILE *file = fopen("MahasiswaData.dat", "rb");
    if (!file) {
        cout << "Gagal membuka file!" << endl;
        return;
    }

    Mahasiswa mhs;
    bool ditemukan = false;
    cout << "\n=== Mahasiswa dari Jurusan " << jurusanCari << " ===\n";
    while (fread(&mhs, sizeof(Mahasiswa), 1, file)) {
        if (strcmp(mhs.jurusan, jurusanCari) == 0) {
            cout << "Nama: " << mhs.nama << "\nNIM: " << mhs.nim
                 << "\nTahun Masuk: " << mhs.tahunMasuk << "\nIPK: " << mhs.ipk << "\n\n";
            ditemukan = true;
        }
    }
    fclose(file);

    if (!ditemukan) {
        cout << "Mahasiswa dengan jurusan " << jurusanCari << " tidak ditemukan!" << endl;
    }
    system("pause");
}

void cariMahasiswaNIM() {
    system("cls");
    char nimCari[15];
    cout << "Masukkan NIM yang dicari: ";
    cin >> nimCari;

    FILE *file = fopen("MahasiswaData.dat", "rb");
    if (!file) {
        cout << "Gagal membuka file!" << endl;
        return;
    }
    
    Mahasiswa data[100];
    int count = 0;
    while (fread(&data[count], sizeof(Mahasiswa), 1, file)) {
        count++;
    }
    fclose(file);
    
    quickSort(data, 0, count - 1);
    int result = binarySearch(data, 0, count - 1, nimCari);
    if (result != -1) {
        Mahasiswa mhs = data[result];
        cout << "Nama: " << mhs.nama << "\nNIM: " << mhs.nim << "\nJurusan: " << mhs.jurusan
             << "\nTahun Masuk: " << mhs.tahunMasuk << "\nIPK: " << mhs.ipk << "\n";
    } else {
        cout << "Mahasiswa dengan NIM " << nimCari << " tidak ditemukan!" << endl;
    }
    system("pause");
}

void perbaruiIPK() {
    system("cls");
    char nimCari[15];
    cout << "Masukkan NIM mahasiswa yang ingin diperbarui IPK-nya: ";
    cin >> nimCari;

    FILE *file = fopen("MahasiswaData.dat", "rb+");
    if (!file) {
        cout << "Gagal membuka file!" << endl;
        return;
    }

    Mahasiswa mhs;
    bool ditemukan = false;
    while (fread(&mhs, sizeof(Mahasiswa), 1, file)) {
        if (strcmp(mhs.nim, nimCari) == 0) {
            cout << "Masukkan IPK baru (0.0 - 4.0): ";
            float ipkBaru;
            cin >> ipkBaru;
            if (ipkBaru < 0.0 || ipkBaru > 4.0) {
                cout << "IPK tidak valid!" << endl;
                fclose(file);
                return;
            }
            mhs.ipk = ipkBaru;
            fseek(file, -sizeof(Mahasiswa), SEEK_CUR);
            fwrite(&mhs, sizeof(Mahasiswa), 1, file);
            cout << "IPK mahasiswa dengan NIM " << nimCari << " berhasil diperbarui!" << endl;
            ditemukan = true;
            break;
        }
    }
    fclose(file);

    if (!ditemukan) {
        cout << "Mahasiswa tidak ditemukan!" << endl;
    }
    system("pause");
}

// Menghapus mahasiswa berdasarkan NIM
void hapusMahasiswa() {
    system("cls");
    char nimCari[15];
    cout << "Masukkan NIM mahasiswa yang ingin dihapus: ";
    cin >> nimCari;

    FILE *file = fopen("MahasiswaData.dat", "rb");
    FILE *tempFile = fopen("Temp.dat", "wb");

    if (!file || !tempFile) {
        cout << "Gagal membuka file!" << endl;
        return;
    }

    Mahasiswa mhs;
    bool ditemukan = false;
    while (fread(&mhs, sizeof(Mahasiswa), 1, file)) {
        if (strcmp(mhs.nim, nimCari) == 0) {
            ditemukan = true;
        } else {
            fwrite(&mhs, sizeof(Mahasiswa), 1, tempFile);
        }
    }
    fclose(file);
    fclose(tempFile);

    remove("MahasiswaData.dat");
    rename("Temp.dat", "MahasiswaData.dat");

    if (ditemukan) {
        cout << "Mahasiswa dengan NIM " << nimCari << " berhasil dihapus!" << endl;
    } else {
        cout << "Mahasiswa tidak ditemukan!" << endl;
    }
    system("pause");
}

void keluarProgram() {
    system("cls");
    cout << "Terima kasih telah menggunakan SmartCampus++! Semoga sukses dalam studi Anda!" << endl;
    exit(0);
}

int main() {
    int pilihan;
    do {
        cout << "\n=== Menu SmartCampus++ ===\n";
        cout << "1. Tambah Mahasiswa\n";
        cout << "2. Tampilkan Semua Mahasiswa\n";
        cout << "3. Cari Mahasiswa Berdasarkan Jurusan\n";
        cout << "4. Cari Mahasiswa Berdasarkan NIM\n";
        cout << "5. Perbaharui IPK Mahasiswa\n";
        cout << "6. Hapus Data Mahasiswa\n";
        cout << "7. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;
        switch (pilihan) {
            case 1: tambahMahasiswa(); break;
            case 2: tampilkanMahasiswa(); break;
            case 3: cariMahasiswaJurusan(); break;
            case 4: cariMahasiswaNIM(); break;
            case 5: perbaruiIPK(); break;
            case 6: hapusMahasiswa(); break;
            case 7: keluarProgram(); break;
            default: cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 7);
    return 0;
}
