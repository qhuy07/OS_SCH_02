#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

// Cấu trúc lưu thông tin của 1 tiến trình
struct Process {
    string id;          // Mã tiến trình (P1, P2...)
    int arrivalTime;    // Thời gian đến (AT)
    int burstTime;      // Thời gian thực thi (BT)
    int remainingTime;  // Thời gian còn lại (Dùng cho SRTF)
    int completionTime; // Thời gian hoàn thành (CT)
    int turnaroundTime; // Thời gian lưu lại hệ thống (TAT = CT - AT)
    int waitingTime;    // Thời gian chờ (WT = TAT - BT)
};

// 1. Hàm đọc danh sách tiến trình từ file input.csv
vector<Process> readProcessesFromFile(string filename) {
    vector<Process> pList;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Loi: Khong the mo file " << filename << "!\n";
        return pList;
    }

    string line;
    // Bỏ qua dòng tiêu đề đầu tiên: Process_ID,Arrival_Time,Burst_Time
    getline(file, line); 

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, atStr, btStr;

        getline(ss, idStr, ',');
        getline(ss, atStr, ',');
        getline(ss, btStr, ',');

        if (!idStr.empty() && !atStr.empty() && !btStr.empty()) {
            string pid = idStr;
            int at = stoi(atStr);
            int bt = stoi(btStr);
            // Khởi tạo remainingTime ban đầu bằng burstTime
            pList.push_back({pid, at, bt, bt, 0, 0, 0});
        }
    }

    file.close();
    return pList;
}
