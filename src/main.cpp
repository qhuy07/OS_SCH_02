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
void runSRTF(vector<Process> p, stringstream &out) {
    ganttSRTF.clear();
    int n = p.size();
    if (n == 0) return;
    int currentTime = 0, completed = 0, contextSwitches = 0;
    string prevProcess = ""; // Lưu tên của Process trước

    while (completed < n) {
        int idx = -1, minRemaining = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].arrivalTime <= currentTime && p[i].remainingTime > 0 && p[i].remainingTime < minRemaining) {
                minRemaining = p[i].remainingTime;
                idx = i;
            }
        }
        if (idx != -1) {
            if (prevProcess != p[idx].id) {
                if (!prevProcess.empty()) contextSwitches++;
                prevProcess = p[idx].id;
            }
            if (ganttSRTF.empty() || ganttSRTF.back().id != p[idx].id)
                ganttSRTF.push_back({p[idx].id, currentTime, currentTime + 1});
            else
                ganttSRTF.back().end++;

            p[idx].remainingTime--;
            currentTime++;
            if (p[idx].remainingTime == 0) {
                completed++;
                p[idx].completionTime = currentTime;
                p[idx].turnaroundTime = p[idx].completionTime - p[idx].arrivalTime;
                p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;
            }
        } else {
            if (ganttSRTF.empty() || ganttSRTF.back().id != "IDLE")
                ganttSRTF.push_back({"IDLE", currentTime, currentTime + 1});
            else
                ganttSRTF.back().end++;
            currentTime++;
        }
    }

    float totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        totalWT += p[i].waitingTime;
        totalTAT += p[i].turnaroundTime;
    }
    float avgWT = totalWT / n, avgTAT = totalTAT / n;

    out << "  \tTHUAT TOAN SRTF (PREEMPTIVE)    \r\nID\tAT\tBT\tCT\tTAT\tWT\r\n";
    for (int i = 0; i < n; i++) {
        out << p[i].id << "\t" << p[i].arrivalTime << "\t" << p[i].burstTime << "\t" 
            << p[i].completionTime << "\t" << p[i].turnaroundTime << "\t" << p[i].waitingTime << "\r\n";
    }
    out << "Context Switches: " << contextSwitches << " | Avg WT: " << fixed << setprecision(2) << avgWT << " | Avg TAT: " << avgTAT << "\r\n\r\n";
    writeResultsToFile("output_srtf.csv", p, "SRTF", avgWT, avgTAT, contextSwitches);
}



void runSJF_NonPreemptive(vector<Process> p, stringstream &out) {
    ganttSJF.clear();
    int n = p.size();
    if (n == 0) return;
    int currentTime = 0, completed = 0, contextSwitches = 0;
    string prevProcess = "";

    while (completed < n) {
        int idx = -1, minBurst = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].arrivalTime <= currentTime && p[i].remainingTime > 0 && p[i].burstTime < minBurst) {
                minBurst = p[i].burstTime;
                idx = i;
            }
        }
        if (idx != -1) {
            if (!prevProcess.empty() && prevProcess != p[idx].id) contextSwitches++;
            prevProcess = p[idx].id;

            ganttSJF.push_back({p[idx].id, currentTime, currentTime + p[idx].burstTime});
            currentTime += p[idx].burstTime;
            p[idx].remainingTime = 0;
            completed++;
            p[idx].completionTime = currentTime;
            p[idx].turnaroundTime = p[idx].completionTime - p[idx].arrivalTime;
            p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;
        } else {
            if (ganttSJF.empty() || ganttSJF.back().id != "IDLE")
                ganttSJF.push_back({"IDLE", currentTime, currentTime + 1});
            else
                ganttSJF.back().end++;
            currentTime++;
        }
    }

    float totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        totalWT += p[i].waitingTime;
        totalTAT += p[i].turnaroundTime;
    }
    float avgWT = totalWT / n, avgTAT = totalTAT / n;

    out << "  \tTHUAT TOAN SJF (NON-PREEMPTIVE)    \r\nID\tAT\tBT\tCT\tTAT\tWT\r\n";
    for (int i = 0; i < n; i++) {
        out << p[i].id << "\t" << p[i].arrivalTime << "\t" << p[i].burstTime << "\t" 
            << p[i].completionTime << "\t" << p[i].turnaroundTime << "\t" << p[i].waitingTime << "\r\n";
    }
    out << "Context Switches: " << contextSwitches << " | Avg WT: " << fixed << setprecision(2) << avgWT << " | Avg TAT: " << avgTAT << "\r\n";
    writeResultsToFile("output_sjf_np.csv", p, "SJF_NonPreemptive", avgWT, avgTAT, contextSwitches);
}
