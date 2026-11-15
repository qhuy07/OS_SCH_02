#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

struct Process {
    string id;
    int arrivalTime, burstTime, remainingTime;
    int completionTime, turnaroundTime, waitingTime;
};

struct GanttBlock {
    string id;
    int start, end;
};

HWND runButton, resultTextBox; // Nút bấm và khung kết quả thuật toán
vector<GanttBlock> ganttSRTF, ganttSJF; // Dữ liệu trong biểu đồ Gantt của 2 thuật toán
vector<HWND> ganttControls; // Lưu các ô biểu đồ để xóa đi vẽ lại khi đổi kích thước

vector<Process> readProcessesFromFile(string filename) {
    vector<Process> pList;
    ifstream file(filename); // Đọc dữ liệu từ file input.csv
    if (!file.is_open()) return pList;
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, atStr, btStr;
        getline(ss, idStr, ',');
        getline(ss, atStr, ',');
        getline(ss, btStr, ',');
        if (!idStr.empty() && !atStr.empty() && !btStr.empty()) {
            pList.push_back({idStr, stoi(atStr), stoi(btStr), stoi(btStr), 0, 0, 0});
        }
    }
    file.close();
    return pList;
}

void writeResultsToFile(string filename, const vector<Process>& p, string algorithmName, float avgWT, float avgTAT, int contextSwitches) {
    ofstream file(filename); // Ghi kết quả từ chương trình ra file
    if (!file.is_open()) return;
    file << "Algorithm," << algorithmName << "\n";
    file << "Process_ID,Arrival_Time,Burst_Time,Completion_Time,Turnaround_Time,Waiting_Time\n";
    for (const auto& proc : p) {
        file << proc.id << "," << proc.arrivalTime << "," << proc.burstTime << ","
             << proc.completionTime << "," << proc.turnaroundTime << "," << proc.waitingTime << "\n";
    }
    file << "Context_Switches," << contextSwitches << "\n";
    file << "Average_Waiting_Time," << fixed << setprecision(2) << avgWT << "\n";  // Định dạng số thập phân thời gian chờ trung bình có 2 chữ số
    file << "Average_Turnaround_Time," << fixed << setprecision(2) << avgTAT << "\n";
    file.close();
}
