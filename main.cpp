#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

string chuanHoaLoai(const string& s) {
    string lower = s;
    for (size_t i = 0; i < lower.size(); ++i) {
        lower[i] = static_cast<char>(tolower(static_cast<unsigned char>(lower[i])));
    }
    if (lower == "car") return "Car";
    if (lower == "motorcycle") return "Motorcycle";
    if (lower == "bicycle") return "Bicycle";
    return "";
}

int docSoNguyen(const string& loiNhac) {
    int x;
    while (true) {
        cout << loiNhac;
        if (cin >> x) {
            return x;
        }
        cout << "Du lieu khong hop le! Vui long nhap so nguyen.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

class Vehicle {
private:
    int vehicleID;
    string vehicleType;
    int entryTime;
    int parkingDuration;
    double parkingFee;

public:
    Vehicle();
    Vehicle(int id, const string& type, int entry, int duration);

    int getVehicleID() const;
    string getVehicleType() const;
    int getEntryTime() const;
    int getParkingDuration() const;
    double getParkingFee() const;

    void setVehicleID(int id);
    void setVehicleType(const string& type);
    void setEntryTime(int entry);
    void setParkingDuration(int duration);

    void calculateFee();
    void display() const;
};

Vehicle::Vehicle()
    : vehicleID(0), vehicleType("Car"), entryTime(0), parkingDuration(1), parkingFee(0.0) {
    calculateFee();
}

Vehicle::Vehicle(int id, const string& type, int entry, int duration)
    : vehicleID(id), vehicleType(chuanHoaLoai(type)), entryTime(entry), parkingDuration(duration), parkingFee(0.0) {
    if (vehicleType.empty()) vehicleType = "Car";
    calculateFee();
}

int Vehicle::getVehicleID() const { return vehicleID; }
string Vehicle::getVehicleType() const { return vehicleType; }
int Vehicle::getEntryTime() const { return entryTime; }
int Vehicle::getParkingDuration() const { return parkingDuration; }
double Vehicle::getParkingFee() const { return parkingFee; }

void Vehicle::setVehicleID(int id) { vehicleID = id; }

void Vehicle::setVehicleType(const string& type) {
    string chuan = chuanHoaLoai(type);
    if (!chuan.empty()) {
        vehicleType = chuan;
        calculateFee();
    }
}

void Vehicle::setEntryTime(int entry) {
    if (entry >= 0 && entry <= 23) {
        entryTime = entry;
        calculateFee();
    }
}

void Vehicle::setParkingDuration(int duration) {
    if (duration >= 1 && duration <= 23) {
        parkingDuration = duration;
        calculateFee();
    }
}

void Vehicle::calculateFee() {
    string loai = chuanHoaLoai(vehicleType);
    double gia = (loai == "Car") ? 6.0 : (loai == "Motorcycle") ? 4.0 : 2.0;
    bool caoDiem = (entryTime >= 6 && entryTime <= 9) || (entryTime >= 15 && entryTime <= 18);
    double tong = 0.0;
    for (int h = 0; h < parkingDuration; ++h) {
        if (h < 2) tong += gia * (caoDiem ? 1.30 : 0.95);
        else       tong += gia * 0.90;
    }
    parkingFee = tong;
}

void Vehicle::display() const {
    cout << "  ID: " << vehicleID << " | " << vehicleType
         << " | Vao: " << entryTime << "h | Do: " << parkingDuration << "h"
         << " | Phi: $" << fixed << setprecision(2) << parkingFee << "\n";
}

class ParkingLot {
private:
    static const int MAX_VEHICLES = 3;
    int parkID;
    char size;
    Vehicle vehicles[MAX_VEHICLES];
    int currentVehicleCount;

    int getSizeCapacity() const;

public:
    ParkingLot();
    ParkingLot(int id, char lotSize);

    int getParkID() const;
    char getSize() const;
    int getCurrentVehicleCount() const;
    const Vehicle& getVehicleAt(int index) const;

    bool isFull() const;
    bool addVehicle(const Vehicle& v);
    bool removeVehicle(int vID);
    int findVehicleIndex(int vID) const;
    void displayLotInfo() const;
    void displayVehiclesByType(const string& type) const;
    void getTypeStats(const string& type, int& count, double& totalFee) const;
};

ParkingLot::ParkingLot() : parkID(0), size('S'), currentVehicleCount(0) {}

ParkingLot::ParkingLot(int id, char lotSize)
    : parkID(id), size(static_cast<char>(toupper(static_cast<unsigned char>(lotSize)))), currentVehicleCount(0) {
    if (size != 'S' && size != 'M' && size != 'L') {
        size = 'S';
    }
}

int ParkingLot::getSizeCapacity() const {
    if (size == 'S') return 1;
    if (size == 'M') return 2;
    return 3;
}

int ParkingLot::getParkID() const { return parkID; }
char ParkingLot::getSize() const { return size; }
int ParkingLot::getCurrentVehicleCount() const { return currentVehicleCount; }

const Vehicle& ParkingLot::getVehicleAt(int index) const {
    return vehicles[index];
}

bool ParkingLot::isFull() const {
    return currentVehicleCount >= getSizeCapacity();
}

bool ParkingLot::addVehicle(const Vehicle& v) {
    if (isFull() || currentVehicleCount >= MAX_VEHICLES) {
        return false;
    }
    vehicles[currentVehicleCount] = v;
    vehicles[currentVehicleCount].calculateFee();
    ++currentVehicleCount;
    return true;
}

int ParkingLot::findVehicleIndex(int vID) const {
    for (int i = 0; i < currentVehicleCount; ++i) {
        if (vehicles[i].getVehicleID() == vID) {
            return i;
        }
    }
    return -1;
}

bool ParkingLot::removeVehicle(int vID) {
    int idx = findVehicleIndex(vID);
    if (idx == -1) {
        return false;
    }
    for (int i = idx; i < currentVehicleCount - 1; ++i) {
        vehicles[i] = vehicles[i + 1];
    }
    --currentVehicleCount;
    return true;
}

void ParkingLot::displayLotInfo() const {
    cout << "Bai " << parkID << " [" << size << "] "
         << currentVehicleCount << "/" << getSizeCapacity() << " xe\n";
    for (int i = 0; i < currentVehicleCount; ++i) {
        vehicles[i].display();
    }
}

void ParkingLot::displayVehiclesByType(const string& type) const {
    string target = chuanHoaLoai(type);
    bool found = false;
    for (int i = 0; i < currentVehicleCount; ++i) {
        if (chuanHoaLoai(vehicles[i].getVehicleType()) == target) {
            if (!found) {
                cout << "Bai " << parkID << " [" << size << "]:\n";
                found = true;
            }
            vehicles[i].display();
        }
    }
}

void ParkingLot::getTypeStats(const string& type, int& count, double& totalFee) const {
    string target = chuanHoaLoai(type);
    count = 0;
    totalFee = 0.0;
    for (int i = 0; i < currentVehicleCount; ++i) {
        if (chuanHoaLoai(vehicles[i].getVehicleType()) == target) {
            ++count;
            totalFee += vehicles[i].getParkingFee();
        }
    }
}

class SystemInterface {
private:
    static const int MAX_LOTS = 10;
    ParkingLot parkingLots[MAX_LOTS];
    int currentLotCount;

    int findLotIndex(int pID) const;
    void removeLotAt(int index);

public:
    SystemInterface();
    bool isParkIDExists(int pID) const;
    bool isVehicleIDExists(int vID) const;
    int generateRandomID() const;
    void runMenu();
};

SystemInterface::SystemInterface() : currentLotCount(0) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

int SystemInterface::findLotIndex(int pID) const {
    for (int i = 0; i < currentLotCount; ++i) {
        if (parkingLots[i].getParkID() == pID) {
            return i;
        }
    }
    return -1;
}

bool SystemInterface::isParkIDExists(int pID) const {
    return findLotIndex(pID) != -1;
}

bool SystemInterface::isVehicleIDExists(int vID) const {
    for (int i = 0; i < currentLotCount; ++i) {
        if (parkingLots[i].findVehicleIndex(vID) != -1) {
            return true;
        }
    }
    return false;
}

int SystemInterface::generateRandomID() const {
    return 100 + (rand() % 9000);
}

void SystemInterface::removeLotAt(int index) {
    if (index < 0 || index >= currentLotCount) return;
    for (int i = index; i < currentLotCount - 1; ++i) {
        parkingLots[i] = parkingLots[i + 1];
    }
    --currentLotCount;
}

void SystemInterface::runMenu() {
    while (true) {
        cout << "\n===== QUAN LY BAI DO XE (GIAI DOAN 2) =====\n";
        cout << "1. Tao bai do xe moi\n";
        cout << "2. Them xe vao bai do\n";
        cout << "3. Xoa mot bai do xe\n";
        cout << "4. Lay xe ra khoi bai do\n";
        cout << "5. Hien thi danh sach tat ca cac bai do xe\n";
        cout << "6. Hien thi danh sach xe theo bai cu thể\n";
        cout << "7. Liet ke xe theo loai phuong tien\n";
        cout << "8. Tom tat phi do xe trung binh\n";
        cout << "9. Thoat chuong trinh\n";

        int chon = docSoNguyen("Chon: ");

        switch (chon) {
            case 1: {
                if (currentLotCount >= MAX_LOTS) {
                    cout << "Da dat suc chua toi da (" << MAX_LOTS << " bai do)!\n";
                    break;
                }

                int pID = docSoNguyen("Nhap ma bai (parkID): ");
                if (isParkIDExists(pID)) {
                    int newID;
                    do {
                        newID = generateRandomID();
                    } while (isParkIDExists(newID));
                    cout << "Canh bao: Ma bai " << pID << " da ton tai! Tu dong gan ID moi: " << newID << "\n";
                    pID = newID;
                }

                char kt;
                while (true) {
                    cout << "Nhap kich thuoc (S/M/L): ";
                    cin >> kt;
                    kt = static_cast<char>(toupper(static_cast<unsigned char>(kt)));
                    if (kt == 'S' || kt == 'M' || kt == 'L') break;
                    cout << "Kich thuoc khong hop le! Chi chap nhan S, M, L.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                parkingLots[currentLotCount] = ParkingLot(pID, kt);
                ++currentLotCount;
                cout << "Da tao bai do ID " << pID << " [" << kt << "] (" << currentLotCount << "/" << MAX_LOTS << ").\n";
                break;
            }
            case 2: {
                if (currentLotCount == 0) {
                    cout << "Chua co bai do xe nao trong he thong!\n";
                    break;
                }

                int vID = docSoNguyen("Nhap ma xe (vehicleID): ");
                if (isVehicleIDExists(vID)) {
                    int newID;
                    do {
                        newID = generateRandomID();
                    } while (isVehicleIDExists(newID));
                    cout << "Canh bao: Ma xe " << vID << " da ton tai! Tu dong gan ID moi: " << newID << "\n";
                    vID = newID;
                }

                string loai, loaiChuan;
                while (true) {
                    cout << "Nhap loai xe (Car/Motorcycle/Bicycle): ";
                    cin >> loai;
                    loaiChuan = chuanHoaLoai(loai);
                    if (!loaiChuan.empty()) break;
                    cout << "Loai xe khong hop le!\n";
                }

                int entry;
                while (true) {
                    entry = docSoNguyen("Nhap gio vao (0-23): ");
                    if (entry >= 0 && entry <= 23) break;
                    cout << "Gio vao phai trong khoang 0-23!\n";
                }

                int duration;
                while (true) {
                    duration = docSoNguyen("Nhap thoi luong do (1-23): ");
                    if (duration >= 1 && duration <= 23) break;
                    cout << "Thoi luong do phai trong khoang 1-23!\n";
                }

                Vehicle v(vID, loaiChuan, entry, duration);
                int targetLotIdx = -1;
                for (int i = 0; i < currentLotCount; ++i) {
                    if (!parkingLots[i].isFull()) {
                        targetLotIdx = i;
                        break;
                    }
                }

                if (targetLotIdx == -1) {
                    cout << "Canh bao: Tat ca cac bai do xe deu da day!\n";
                } else {
                    parkingLots[targetLotIdx].addVehicle(v);
                    cout << "Da xep xe ID " << vID << " vao bai " << parkingLots[targetLotIdx].getParkID()
                         << " | Phi: $" << fixed << setprecision(2) << v.getParkingFee() << "\n";
                }
                break;
            }
            case 3: {
                if (currentLotCount == 0) {
                    cout << "Khong co bai do xe nao de xoa!\n";
                    break;
                }
                int pID = docSoNguyen("Nhap ma bai can xoa: ");
                int idx = findLotIndex(pID);
                if (idx == -1) {
                    cout << "Khong tim thay bai do ID " << pID << "!\n";
                } else {
                    removeLotAt(idx);
                    cout << "Da xoa bai do ID " << pID << " va don mang.\n";
                }
                break;
            }
            case 4: {
                if (currentLotCount == 0) {
                    cout << "He thong chua co bai do xe nao!\n";
                    break;
                }
                int vID = docSoNguyen("Nhap ma xe can lay ra: ");
                bool removed = false;
                for (int i = 0; i < currentLotCount; ++i) {
                    if (parkingLots[i].removeVehicle(vID)) {
                        cout << "Da lay xe ID " << vID << " ra khoi bai " << parkingLots[i].getParkID() << ".\n";
                        removed = true;
                        break;
                    }
                }
                if (!removed) {
                    cout << "Khong tim thay xe ID " << vID << " trong he thong!\n";
                }
                break;
            }
            case 5: {
                if (currentLotCount == 0) {
                    cout << "Chua co bai do xe nao!\n";
                    break;
                }
                cout << "--- DANH SACH BAI DO XE (" << currentLotCount << "/" << MAX_LOTS << ") ---\n";
                for (int i = 0; i < currentLotCount; ++i) {
                    cout << "Bai " << parkingLots[i].getParkID()
                         << " | Kich thuoc: " << parkingLots[i].getSize()
                         << " | So xe: " << parkingLots[i].getCurrentVehicleCount() << "\n";
                }
                break;
            }
            case 6: {
                if (currentLotCount == 0) {
                    cout << "Chua co bai do xe nao!\n";
                    break;
                }
                int pID = docSoNguyen("Nhap ma bai can xem: ");
                int idx = findLotIndex(pID);
                if (idx == -1) {
                    cout << "Khong tim thay bai do ID " << pID << "!\n";
                } else {
                    parkingLots[idx].displayLotInfo();
                }
                break;
            }
            case 7: {
                if (currentLotCount == 0) {
                    cout << "Chua co bai do xe nao!\n";
                    break;
                }
                string loai, loaiChuan;
                while (true) {
                    cout << "Nhap loai xe can liet ke (Car/Motorcycle/Bicycle): ";
                    cin >> loai;
                    loaiChuan = chuanHoaLoai(loai);
                    if (!loaiChuan.empty()) break;
                    cout << "Loai xe khong hop le!\n";
                }

                bool foundAny = false;
                for (int i = 0; i < currentLotCount; ++i) {
                    int cnt = 0;
                    double sum = 0.0;
                    parkingLots[i].getTypeStats(loaiChuan, cnt, sum);
                    if (cnt > 0) {
                        parkingLots[i].displayVehiclesByType(loaiChuan);
                        foundAny = true;
                    }
                }
                if (!foundAny) {
                    cout << "Khong co xe nao thuoc loai " << loaiChuan << "!\n";
                }
                break;
            }
            case 8: {
                string loaiArr[3] = {"Car", "Motorcycle", "Bicycle"};

                cout << "--- PHAN TICH THEO TUNG BAI DO ---\n";
                if (currentLotCount == 0) {
                    cout << "Chua co bai do nao!\n";
                } else {
                    for (int i = 0; i < currentLotCount; ++i) {
                        cout << "Bai " << parkingLots[i].getParkID()
                             << " [" << parkingLots[i].getSize() << "]:\n";
                        for (int t = 0; t < 3; ++t) {
                            int cnt = 0;
                            double sum = 0.0;
                            parkingLots[i].getTypeStats(loaiArr[t], cnt, sum);
                            cout << "  " << loaiArr[t] << ": ";
                            if (cnt == 0) cout << "khong co xe\n";
                            else cout << cnt << " xe | TB: $" << fixed << setprecision(2) << sum / cnt << "\n";
                        }
                    }
                }

                cout << "--- TONG HOP TOAN HE THONG ---\n";
                for (int t = 0; t < 3; ++t) {
                    int totalCnt = 0;
                    double totalFee = 0.0;
                    for (int i = 0; i < currentLotCount; ++i) {
                        int cnt = 0;
                        double sum = 0.0;
                        parkingLots[i].getTypeStats(loaiArr[t], cnt, sum);
                        totalCnt += cnt;
                        totalFee += sum;
                    }
                    cout << loaiArr[t] << ": ";
                    if (totalCnt == 0) cout << "khong co xe\n";
                    else cout << totalCnt << " xe | TB: $" << fixed << setprecision(2) << totalFee / totalCnt << "\n";
                }
                break;
            }
            case 9:
                cout << "Thoat chuong trinh.\n";
                return;
            default:
                cout << "Lua chon khong hop le (1-9)!\n";
                break;
        }
    }
}

int main() {
    SystemInterface sys;
    sys.runMenu();
    return 0;
}